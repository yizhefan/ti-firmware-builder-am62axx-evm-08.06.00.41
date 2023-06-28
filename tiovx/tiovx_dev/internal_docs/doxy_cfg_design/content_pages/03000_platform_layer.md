# Design Topic : Platform layer {#did_platform_layer}

[TOC]

# Requirements Addressed {#did_platform_topic_requirements}

- https://jira.itg.ti.com/browse/TIOVX-26
- https://jira.itg.ti.com/browse/TIOVX-27
- https://jira.itg.ti.com/browse/TIOVX-28
- https://jira.itg.ti.com/browse/TIOVX-29
- https://jira.itg.ti.com/browse/TIOVX-30
- https://jira.itg.ti.com/browse/TIOVX-31

# Introduction {#did_platform_topic_intro}

Purpose of the platform layer is to separate the OS and SoC independent
code of OpenVX from the OS and SoC specific code. This allows to quickly
port OpenVX to different OS, SoC.

This is achieved by defining a set of APIs which the framework will use.
The implementation of these APIs will be platform specific.

# Directory Structure {#did_platform_topic_dir_structure}

    source/platform/    # root folder of platform implementation
    ├── os              # OS specific but SoC indepedant API implementation
    │   ├── linux       # Linux pthread API based platform APIs
    │   └── win32       # Windows Win32 API based platform APIs
    ├── pc              # x86 PC platform implementation
    │   ├── common
    │   ├── linux
    │   └── windows
    └── vision_sdk      # Vision SDK platform implementation, works on TDA2x/3x/2Ex/2px
        ├── bios        # BIOS side platform APIs
        ├── common      # Linux/BIOS common platform APIs
        └── linux       # Linux side platform APIs

# Diagrams {#did_platform_topic_diagrams}

## Sequence Diagram {#did_platform_topic_sequence_diagram}
na

## Component Interaction {#did_platform_topic_component_interaction}

![](TIOpenVX-PlatformClassDiagram.jpg "TIOVX Platform Class Diagram")

Generated from TIOpenVX.uml using StarUML 1.0

# Resource usage {#did_platform_topic_resource_usage}

- When tivxInit is called, target threads/task would be created for that CPU
- When tivxInit is called, IPC will be initialzed and this could use IPC related
  resources like mailboxes/interrrupts/shared memory

# Error handling {#did_platform_topic_error_handling}

- All the platform APIs must return a error to the application in case of error
  and not assert within the APIs

# Interface {#did_platform_topic_interface}

## Platform modules {#did_platform_interface_modules}

The platform APIs are split into below modules

Module    | API file location               | Description
----------|---------------------------------|--------------------------------------------
event     | include/TI/tivx_event.h         | Abstraction for a object that framework can wait on. Similar to binary semaphore.
mutex     | include/TI/tivx_mutex.h         | Abstraction for a mutual exclusion lock.
task      | include/TI/tivx_task.h          | Abstraction for a task or a thread
queue     | include/TI/tivx_queue.h         | Abstraction for a thread safe, array based SW queue
memory    | include/TI/tivx_mem.h           | Abstraction for allocating from shared memory accesible to all CPUs. Typically a contigous memory allocator on SoCs
ipc       | source/include/tivx_ipc.h       | Abstraction to send/receive a IPC messages from one CPU to another CPU
platform  | source/include/tivx_platform.h  | Misc platform specific APIs like getting current time
init      | include/TI/tivx.h               | Init APIs for platform specific init

## Event APIs {#did_platform_interface_event}

The event APIs are used by a thread to wait or pend on a event object.
When the required event occurs, typically in another thread, that
thread calls a event post API to unblock the blocked thread.

NOTE: Events are not queued, i.e if event post is called multiple times
before the thread that is blocked on the event is scheduled, it results
only a single event wait getting unblocked.

See include/TI/tivx_event.h for detailed interface specification

### SysBIOS
It is recommended to implement event using a SysBIOS binary semaphore.
It is recommended to use the PDK OSAL API instead of using SysBIOS API directly.

### Linux
It is recommended to implement event using pthread_mutex and pthread_cond APIs


## Mutex APIs {#did_platform_interface_mutex}

The mutex APIs are used by threads when they want to execute a piece of code
which needs to be thread safe, i.e mutual exclusion needs to be maintain
of that peice of code.

See include/TI/tivx_mutex.h for detailed interface specification

### SysBIOS
It is recommended to use SysBIOS binary semaphore to implement a mutex.
It is recommended to use the PDK OSAL API instead of using SysBIOS API directly.

### Linux
It is recommended to implement mutex using pthread_mutex APIs


## Task APIs {#did_platform_interface_task}

The Task APIs are used to create threads or tasks. Multiple
threads can execute independantly in a system.

See include/TI/tivx_task.h for detailed interface specification

### SysBIOS
It is recommended to use SysBIOS "task" to implement a task.
It is recommended to use the PDK OSAL API instead of using SysBIOS API directly.

### Linux
It is recommended to implement tasks using pthread APIs

## Queue APIs {#did_platform_interface_queue}

SW queues are used heavily within TIOVX implementation.
It is required to make these
queues blocking for data to be available and also needs to be
thread safe. The mechanism of implementing a queue could be different
on different OS hence its part of the platform layer.

Features,
- The data memory to be used by the queue is provided by the user.
- The queue element type is always uint32_t.
- User can select to block on dequeue or block on queue
- User can peek into the queue without dequeing
- User can check if queue is empty
- User can use queue in non-blocking mode as well

See include/TI/tivx_queue.h for detailed interface specification

### SysBIOS
It is recommended to use SysBIOS "semaphore" for locks and blocking with SW logic to implement the queue.
It is recommended to use the PDK OSAL API instead of using SysBIOS API directly.

### Linux
It is recommended to implement quue using pthread_mutex for locks, pthread_cond for blocking

## Memory APIs {#did_platform_interface_memory}

The memory APIs are used to allcoate memory for data objects.
The data objects could be used by targets running on different CPUs and
address spaces, hence this memory allocation on a SoC needs to be from a
shared memory location. Since each target CPU could have different
address space, ex, virtual memory in case of Linux, there needs to be
some means of translation address from the shared memory space to
local host or target memory space. The memory API provides such APIs as well.

Features,
- APIs to alloc/free memory from different heap pools
- APIS to translate pointer between shared memory, host memory and target memory
- APIs to make the memory coherent (cache ops) at local CPU (map/unmap APIs)
- APIs to return heap information

See include/TI/tivx_mem.h for detailed interface specification

### SysBIOS (TDA2x/3x)
In SysBIOS in TDA2x/3x systems with Vision SDK the memory alloc is done as below
- Vision SDK provides a API called Utils_memAlloc/Free
- Internally Vision SDK does below
  - When a CPU calls the API to get memory it sends a message to IPU1 Core0 CPU
    - IPU1 Core0 allocates the memory and sends it back to the requesting CPU
- No address translation is required since all CPUs are mapped to have the
  same address space

### Linux (TDA2x/3x)
In Linux in TDA2x/3x system with Vision SDK the memory alloc is done as below
- Vision SDK provides a API called OSA_memAllocSR/FreeSR
- Internally Vision SDK does below
  - When a CPU calls the API to get memory it sends a message to IPU1 Core0 CPU
    - IPU1 Core0 allocates the memory and sends it back to the requesting CPU
- Address translation is done to convert the physical memory pointer to
  Linux user space virtual address

### SysBIOS (TDA4x)
In SysBIOS TDA4x systems with Vision Apps the memory alloc is done as below
- Vision Apps will provide a API called "appMemAlloc/Free"
- Internally Vision Apps does below
  - Alloc memory from a SysBIOS heap on the same CPU
  - This pointer will be valid across all CPUs
- No address translation is required since all CPUs are mapped to have the
  same address space

### Linux (TDA4x)
In Linux in TDA4x system with Vision Apps the memory alloc is done as below
- Vision Apps provides a API called "appMemAlloc/Free"
- Internally Vision Apps does below
  - Vision Apps uses either "ion" or "cmem" APIs to allocate memory
- Address translation is done to convert the virtual memory pointer to
  physical memory pointer which can be passed to the target CPUs


## IPC APIs {#did_platform_interface_ipc}

IPC APIs allows TIOVX framework to send messages from host to target
or target to target

CPU ID is a arbitrary number 4b number decided by the platform layer.
Every SoC could have different CPU IDs.

IPC payload is 24b value as shown below

Obj Desc ID | Destination CPU ID | Destination Target Instance ID
------------|--------------------|-------------------------------
16b         | 4b                 | 4b

Features,
- APIs to register a handler to invoke when a message is received.
  - The message is 32b value which passed to the TIOVX framework
    via the callback
- APIs to send a message, 32b value from any CPU to any CPU
- APIs to get self CPU ID
- APIs to check if a given target is enabled in the system or not

See source/include/tivx_ipc.h for detailed interface specification

Additional APIs related to IPC platform API but not listed in above file are given below

API                   | Description
----------------------|--------------------------------------------------
tivxGetSelfCpuId()    | returns CPU ID of current or self CPU
tivxIsTargetEnabled() | Given a target name, tell if it is enabled or not

### SysBIOS (TDA2x/3x)
- IPC is done using IPC 3.x Notify APIs

### Linux (TDA2x/3x)
- IPC is done using Linux RPMessage APIs (RPMessage proto driver in linux kernel)

### SysBIOS (TDA4x)
- IPC is done using PDK IPC LLD RPMessage APIs

### Linux (TDA4x)
- IPC is done using Linux RPMessage APIs (RPMessage proto driver in linux kernel)

## Platform APIs {#did_platform_interface_platform}

Platform APIs consists of additional APIs whose implementation would be
platform specific.

Features,
- APIs to get target ID from target name, target name from target ID.
- APIs to lock a SoC wide lock using HW mechnisms like spinlocks
- APIs to get current time in usecs
- APIs to print a string
- APIs to activate/deactivate
  - Used to set CPU in a state such that it can execute target kernels
    - Only used in EVE/ARP32 CPU in TDA2x to enabled/disable EDMA clock
- APIs to create/delete targets
- APIs to init/deinit platform
- APIs to get pointer to shared memory object descriptor and its information

Target ID is combination of CPU ID and target instance as shown below

 CPU ID | Target Instance ID
--------|--------------------
 4b     | 4b

A SoC has multiple CPUs (upto 16).
And each CPU could have multiple target threads (upto 16).
During platform init, targets thread for the given CPU are created and target IDs/names
registered so that they cna be queried later by the framework.

For the system lock/unlock APIs, it is recommended
to take a OS semaphore or mutex before taking a HW spinlock.

See source/include/tivx_platform.h for detailed interface specification

## Init APIs {#did_platform_interface_init}

These APIs are used to init the target side and host side of the TIOVX framework.
Any builtin host and target kernels are alos registered during init

In SysBIOS, tivxInit is called during
BIOS executable init by the integration application.

In Linux application, user needs to call only tivxInit() after main()
to start using OpenVX APIs

The list of init/deinit APIs are listed below

API                               | Description
----------------------------------|---------------------------------------
tivxInit() / tivxDeInit()         | APIs to init or deinit host and target side framework

The typical sequence of init steps are listed below
1. Set default debug zone state as enable/disable
2. Init resource logging module
3. Platform init
4. Target init
5. Host init, if current CPU supports HOST APIs
6. Register target side builtin kernels, if any
7. Register host side builtin kernels, if any
8. Object Descriptor init
9. Create target threads

At this point kernels can be scheduled on targets using OpenVX APIs


# Design Analysis and Resolution (DAR) {#did_platform_topic_dar}

## Design Decision : xyz {#did_platform_topic_dar_01}


### Design Criteria: xyz {#did_platform_topic_dar_01_criteria}


### Design Alternative: xyz {#did_platform_topic_dar_01_alt_01}


### Design Alternative: xyz {#did_platform_topic_dar_01_alt_02}


### Final Decision {#did_platform_topic_dar_01_decision}

# Revision History {#did_rev_history}

TIOVX Version     | Date          | Author             | Description
------------------|---------------|--------------------|------------
\ref TIOVX_1_00  | 24 Oct  2019  | Kedar C            | First draft

