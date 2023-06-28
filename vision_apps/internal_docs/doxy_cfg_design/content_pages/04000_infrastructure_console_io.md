# Infrastructure : Console IO

[TOC]

# Requirements Addressed {#did_infrastructure_console_io_requirements}

- https://jira.itg.ti.com/browse/ADASVISION-1800

# Introduction {#did_infrastructure_console_io_intro}

## Purpose {#did_infrastructure_console_io_purpose}

This design topic describes the integration of user input and output
via a command line like interface.


## Short Application Description {#did_infrastructure_console_io_short_desc}

The below features are integrated as part of console IO
- Logging via a printf API
  - Here stdio printf API is redirected to a log writer API such that
    all prints for different CPUs get logged to a shared memory
  - A log reader thread then reads these string and outputs to the
    console device
  - Console device will be UART
  -	This logging infrastructure will not affect the system characteristic or real time performance
    as this is implemented as delayed print mechanism
- Command line interface
  - Here a command line interface is provided to input read from UART
  - Based on keywords entered by the user, specific pre-registered functions wil be invoked
  - In, RTOS systems, this will act as a command line interface to trigger applications/demos
  - In, Linux systems, standard linux command line interface will be used and this module will be disabled.

NOTE: The CLI module is not a general purpose shell and is not meant to replace one.
This module allows users to invoke handlers associated with a keyword (command).
Typically handlers will run some OpenVX app or they will query the system and print some
useful information, ex, SoC system load, memory consumption etc.

## Input and Output format {#did_infrastructure_console_io_io_format}
na

# Directory Structure {#did_infrastructure_console_io_dir_structure}

    utils/console_io/
    ├── include
    │   ├── app_cli.h           # CLI user callbable APIs
    │   ├── app_log.h           # Log reader/writer user callbable APIs
    │   └── app_uart.h          # UART IO user callbable APIs
    └── src
        ├── app_cli.c           # command line interface implementation
        ├── app_log_reader.c    # logger implementation to read from shared memory and output to device
        ├── app_log_writer.c    # logger implementation to write to shared memory
        └── app_uart.c          # UART IO implementation using UART LLD from PDK

# Diagrams {#did_infrastructure_console_io_diagrams}

## Sequence Diagram {#did_infrastructure_console_io_sequence_diagram}
na

## Component Interaction {#did_infrastructure_console_io_component_interaction}

### Log reader and writer
![Log reader and writer](app_cons_io_log_rdwr.png)

### Command line interface
![CLI Task](app_cons_io_cli_task.png)

# Resource usage {#did_infrastructure_console_io_resource_usage}

## Log reader and writer

- This module will take as input a pointer to shared memory where log writers will
  log their prints and log reader will from this area
  - There can be only one log reader in a system
  - There will one log writer per CPU in a system
- Log reader will run as low priority thread, priority configurable by user.
  Default will be lowest priority
- Log reader will run periodically at user configured polling interval
  Default will be 1ms
- In RTOS systems, stdio printf will redirect to the Log writer API

## Command line interface

- No task will be created for CLI by the "console_io" module.
- User will run "appCliHandleCmd(&is_exit)"
  in a loop until "is_exit" returns as true
  - This allows users/demos to nest calling of "app_cli_execute" to show sub-menu/commands
    while a demo is running

## UART
- UART module will take UART instance ID as parameter from user and create the UART LLD

# Error handling {#did_infrastructure_console_io_error_handling}

## Log reader and writer

- In log writer, if space is not available in shared memory to write the sring, then the
  caller will not block, it will drop the string and return
- In log reader, if no string is available for reading from shared memory and writing to console,
  log reader will sleep for user configured time and try again

## Command line interface

- CLI will call handlers for registered commands, if a unknown command is encoutered,
  CLI will output "Unsupport command [<command name>]" to console and show the CLI prompt again

# Interface {#did_infrastructure_console_io_interface}

## UART {#did_infrastructure_console_io_interface_uart}

### Data structures {#did_infrastructure_console_io_interface_uart_ds}

~~~C

typedef struct {

    uint32_t uart_instance_id; /* 0..n, UART instance ID to use */

} app_uart_init_prm_t

~~~

### Functions {#did_infrastructure_console_io_interface_uart_funcs}

~~~C

/* init and create UART driver */
int32_t appUartInit(app_uart_init_prm_t *prm);

/* deinit UART driver */
int32_t appUartDeInit();

/* write string to UART device, string may or may not be terminated by new line char */
int32_t appUartWriteString(char *str, uint32_t max_size);

/* read string from UART device, string will be terminated by newline char, new line char will not be returned */
int32_t appUartReadString(char *str, uint32_t max_size, uint32_t *string_size);

~~~

## Log reader and writer {#did_infrastructure_console_io_interface_log_rd_wr}

### Data structures {#did_infrastructure_console_io_interface_log_rd_wr_ds}

~~~C

#define APP_LOG_MAX_CPUS        (16u)
#define APP_LOG_MAX_CPU_NAME    ( 8u)
#define APP_LOG_MEM_SIZE        (4*1024u)  /* log memory for one CPU */
#define APP_LOG_AREA_VALID_FLAG (0x1357231u)

/* callback to write string to console device */
typedef int (*app_log_device_send_string_f)(char *string, uint32_t max_size);

/* shared memory structure for a specific CPU */
typedef struct {

    /* init by reader to 0 */
    uint32_t log_rd_idx;

    /* init by writer to 0 */
    uint32_t log_wr_idx;

    /* init by writer to APP_LOG_AREA_VALID_FLAG.
       reader will ignore this CPU shared mem log
       until the writer sets this
       to APP_LOG_AREA_VALID_FLAG */
    uint32_t log_area_is_valid;

    /* init by writer to CPU name, used by reader to add a prefix when writing to console device */
    uint8_t  log_cpu_name[APP_LOG_MAX_CPU_NAME];

    /* memory into which logs are written by this CPU */
    uint8_t  log_mem[APP_LOG_MEM_SIZE];

} app_log_cpu_shared_mem_t;

/* shared memory structure for all CPUs, used by reader and writer CPUs */
typedef struct {

    app_log_cpu_shared_mem_t cpu_shared_mem[APP_LOG_MAX_CPUS]

} app_log_shared_mem_t;

/* init parameters to use for appLogInit */
typedef struct {

    app_log_shared_mem_t *shared_mem;       /* shared memory to use for logging, all CPUs must point to the same shared memory */
    uint32_t self_cpu_index;                /* index into shared memory area for self CPU to use to use when writing. Two CPUs must not use the same CPU index */
    uint8_t  self_cpu_name[APP_LOG_MAX_CPU_NAME]; /* self CPU name */
    uin32_t  log_rd_enable;                 /* 1: enable log reader, NOTE: there can be only 1 log reader, else only log writer is enabled */
    uint32_t log_rd_task_pri;               /* task priority for log reader */
    uint32_t log_rd_poll_interval_in_msecs; /* polling interval for log reader in msecs */
    uint32_t log_rd_max_cpus;               /* maximum CPUs that log into the shared memory */
    app_log_device_send_string_f device_write; /* write a string to a device specific function, by default this will be set to appUartWriteString() */

} app_log_init_prm_t;
~~~

### Functions {#did_infrastructure_console_io_interface_log_rd_wr_funcs}

~~~C
/* Initialize app_log_init_prm_t with default parameters
 * always call this function before calling appLogInit
 */
void appLogInitPrmSetDefault(app_log_init_prm_t *prms);

/* Init Log reader and log writer,
   This function also does init to redirect printf to appLogPrintf for RTOS systems
*/
int32_t appLogInit(app_log_init_prm_t *prms);

/* De-init log reader and log writer */
int32_t appLogDeInit();

/* Write a string to shared memory
 * user can use this API or the stdio printf API
 */
int32_t appLogPrintf(char *string, ...);
~~~

## Command line interface {#did_infrastructure_console_io_interface_cli}

### Data structures {#did_infrastructure_console_io_interface_cli_ds}

~~~C

#define APP_CLI_MAX_PROMPT_NAME     (16u)

/* command handler callback */
typdef int32_t (*app_cli_cmd_handler_f)(int argc, char *argv[]);

/* callback to write string to console device */
typedef int (*app_cli_device_send_string_f)(char *string, uint32_t max_size);

/* callback to read string from console device */
typedef int (*app_cli_device_get_string_f)(char *string, uint32_t max_size, uint32_t *string_size);

/* CLI init parameters */
typedef struct {

    app_cli_device_send_string_f device_write; /* device specific callback to write string to device, by default appUartWriteString() is used */
    app_cli_device_get_string_f device_read;  /* device specific callback to read string from device, by default appUartReadString() is used */
    char cli_prompt_name[APP_CLI_MAX_PROMPT_NAME]   /* CLI prompt name, by default "ti" will be used */
} app_cli_init_prm_t;
~~~

### Functions {#did_infrastructure_console_io_interface_cli_funcs}

~~~C

/* set default parameters for CLI */
void appCliInitPrmSetDefault(app_cli_init_prm_t *);

/* init CLI */
int32_t appCliInit(app_cli_init_prm_t *);

/* de-init CLI */
int32_t appCliDeInit();

/* Register a handler for system command 'cmd'
 * A system command is a command that can be invoked at point during application invokation
 * ex, while a application use-case is running
 */
int32_t appCliRegisterSystemCmd(char *cmd, app_cli_cmd_handler_f *cmd_handler);

/* Register a handler for application command 'cmd'
 * Once a application command is invoke, additional application commands
 * cannot be invoked until this application finishes execution.
 * System commands and "Application sub-commands" can be invoked while
 * a application is running.
 * An application handler should register application specific sub-commands
 * which can be invoked while a application runs
 */
int32_t appCliRegisterAppCmd(char *cmd, app_cli_cmd_handler_f *cmd_handler);

/* Register a handler for application sub-commands 'cmd'
 * This are commands specific to the currently running application
 * ex, handler to stop current application would be different for different
 * applications.
 *
 * This commands can be invoked only after a application command is executed
 */
int32_t appCliRegisterAppSubCmd(char *cmd, app_cli_cmd_handler_f *cmd_handler);

/* Unregisters all application sub-cmds, when a application command is executed
 * CLI unregisteres all previous registered app sub-cmds
 * applications should register any sub-cmds they wish to handle while the application
 * runs exeplicitly
 */
int32_t appCliUnRegisterAllAppSubCmds();

/* waits to read input from user and invokes handler based on
   the command that is entered.

   After handling one command, the function returns.
   If unsupport command is entered, function returns.

   User should call this function in a loop to get the effect of a CLI.

   If user enters 'exit', is_exit flag is set to 1
   User can use this flag to break out of the loop.
 */
int32_t appCliHandleCmd(uint32_t *is_exit);

~~~

## Example API calls and CLI interaction {#did_infrastructure_console_io_interface_ex}

### Example init sequence {#did_infrastructure_console_io_interface_init_ex}

#### Host CPU init and main {#did_infrastructure_console_io_interface_init_host_ex}

~~~C

#pragma DATA_SECTION(gAppLogSharedMem, .bss:nonCacheSharedMem)
#pragma DATA_ALIGN(gAppLogSharedMem, 1024)
app_log_shared_mem_t gAppLogSharedMem;

app_uart_init_prm_t gAppUartPrm;
app_cli_init_prm_t gAppCliPrm;
app_log_init_prm_t gAppLogPrm;

void myHostInit() {

    appUartInitPrmSetDefault(&gAppUartPrm);
    appCliInitPrmSetDefault(&gAppCliPrm);
    appLogInitPrmSetDefault(&gAppLogPrm);

    gAppUartPrm.uart_instance_id = 0;
    gAppLogPrm.shared_mem = &gAppLogSharedMem;
    gAppLogPrm.self_cpu_index = Ipc_mp_self();
    strcpy(gAppLogPrm.self_cpu_name, Ipc_mp_getName(Ipc_mp_self()));
    gAppLogPrm.log_rd_enable = 1;
    gAppLogPrm.log_rd_max_cpus = Ipc_mp_getNumProcessors();

    appUartInit(&gAppUartPrm);
    appCliInit(&gAppCliPrm);
    appLogInit(&gAppLogPrm);
}

void myHostMain() {

    uint32_t is_exit = 0;

    myHostInit();
    while(!is_exit) {
        appCliHandleCmd(&is_exit);
    }
    myHostExit();
}

void myHostDeInit() {
    appCliDeInit();
    appLogDeInit();
    appUartDeInit();
}

#### Remote CPU init and main {#did_infrastructure_console_io_interface_init_remote_ex}

~~~C

#pragma DATA_SECTION(gAppLogSharedMem, .bss:nonCacheSharedMem)
#pragma DATA_ALIGN(gAppLogSharedMem, 1024)
app_log_shared_mem_t gAppLogSharedMem;

app_log_init_prm_t gAppLogPrm;

void myRemoteInit() {

    appLogInitPrmSetDefault(&gAppLogPrm);

    gAppLogPrm.shared_mem = &gAppLogSharedMem;
    gAppLogPrm.self_cpu_index = Ipc_mp_self();
    strcpy(gAppLogPrm.self_cpu_name, Ipc_mp_getName(Ipc_mp_self()));

    appLogInit(&gAppLogPrm);
}

void myRemoteMain() {

    myRemoteInit();
    while(1);
    myRemoteExit();
}

void myRemoteDeInit() {
    appLogDeInit();
}

~~~

### Example CLI interaction {#did_infrastructure_console_io_interface_cli_ex}

    System started, starting CLI !!!

    cli:/>
    cli:/> help

    Supported system commands,
      help                - print this help
      info                - show device and binary information
      exit                - exit current application

    Supported application commands,
      capture_display     - run capture+display app
      surround_view       - run surround view application

    cli:/> capture_display --cfg app_capture_display.cfg

    Starting appilcation "capture_display"

    cli:/capture_display> help

    Supported system commands,
      help                - print this help
      info                - show device and binary information
      exit                - exit current application

    Supported application sub-commands,
      pause               - pause display
      profile             - show application profile information

    cli:/capture_display> exit

    Exiting appilcation "capture_display"

    cli:/> xyz
    Unsupported command, [xyz]

    cli:/> exit

    System shutdown !!!


# Design Analysis and Resolution (DAR) {#did_infrastructure_console_io_dar}

## Design Decision : Init Sequence {#did_infrastructure_console_io_dar_01}

Decide how to init the UART, CLI and logger modules.

### Design Criteria: Init Sequence {#did_infrastructure_console_io_dar_01_criteria}

As part of init sequence, system integrator needs to make below decisions
- which UART instance to use for IO ?
- where will the shared memory be located in the memory map ?
- which API to use to read and write strings ?
- how to run the CLI - in a spearate thread or in the main thread ?

These questions are dependant on the customer use-case, memory map, board setup.
Goal is to not creep in these specific details within console_io implementation.

### Design Alternative: Init sequence inside the module {#did_infrastructure_console_io_dar_01_alt_01}

- Here we have a appConsoleInit() and that does everything internally.

Advantages
- Simpler for application when all of above parameters are fixed

Disadvantages
- The implementation in console_io would become dependant on board (UART), CPU type (host or remote CPU), memory map (shared memory)

### Design Alternative: Init sequence inside the application {#did_infrastructure_console_io_dar_01_alt_02}

- Here we have a init for each sub-module with all board, use-case, memory-map specific
  parameters taken as input.

Advantages
- Implementation independant of board, use-case, memory-map.

Disadvantages
- The app init sequence is more eloborate. A wrapper can be put at app level to make the init
  sequence simpler

### Final Decision {#did_infrastructure_console_io_dar_01_decision}

Use "Init sequence inside the application" to keep console_io independant of board and use-case.

