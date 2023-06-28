/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <app_mem_map.h>
#include APP_CFG_FILE
#include <app.h>
#include <stdio.h>
#include <string.h>
#include <ti/csl/csl_types.h>

/* Vision_apps utils header files */
#include <utils/mem/include/app_mem.h>
#include <utils/ipc/include/app_ipc.h>
#include <utils/remote_service/include/app_remote_service.h>
#include <utils/console_io/include/app_log.h>
#include <utils/console_io/include/app_cli.h>
#include <utils/misc/include/app_misc.h>
#include <utils/hwa/include/app_hwa.h>
#include <utils/iss/include/app_iss.h>
#include <utils/udma/include/app_udma.h>
#include <utils/dss/include/app_dss_defaults.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/sciclient/include/app_sciclient.h>
#include <utils/sciserver/include/app_sciserver.h>
#include <utils/sensors/include/app_sensors.h>
#include <utils/ethfw/include/app_ethfw.h>

/* TIOVX header files */
#include <TI/tivx.h>

/* Vision_apps custom kernel header files */
#include <TI/tivx_img_proc.h>
#include <TI/tivx_fileio.h>
#include <TI/tivx_srv.h>

/* Imaging header files */
#include <TI/j7_imaging_aewb.h>

/* PDK header files */
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

app_log_shared_mem_t g_app_log_shared_mem
__attribute__ ((section(".bss:app_log_mem")))
__attribute__ ((aligned(4096)))
        ;

uint8_t g_tiovx_obj_desc_mem[TIOVX_OBJ_DESC_SHARED_MEM_SIZE]
__attribute__ ((section(".bss:tiovx_obj_desc_mem")))
__attribute__ ((aligned(4096)))
        ;

uint8_t g_ipc_vring_mem[IPC_VRING_MEM_SIZE]
__attribute__ ((section(".bss:ipc_vring_mem")))
__attribute__ ((aligned(4096)))
        ;

uint8_t g_ddr_local_mem[DDR_HEAP_MEM_SIZE]
__attribute__ ((section(".bss:ddr_local_mem")))
__attribute__ ((aligned(4096)))
        ;

#ifdef L1_MEM_SIZE
uint8_t g_l1_mem[L1_MEM_SIZE]
__attribute__ ((section(".bss:l1mem")))
__attribute__ ((aligned(4096)))
        ;
#endif

#ifdef L2_MEM_SIZE
uint8_t g_l2_mem[L2_MEM_SIZE]
__attribute__ ((section(".bss:l2mem")))
__attribute__ ((aligned(4096)))
        ;
#endif

#ifdef L3_MEM_SIZE
uint8_t g_l3_mem[L3_MEM_SIZE]
__attribute__ ((section(".bss:l3mem")))
__attribute__ ((aligned(4096)))
        ;
#endif

#ifdef DDR_SCRATCH_SIZE
uint8_t g_ddr_scratch_mem[DDR_SCRATCH_SIZE]
__attribute__ ((section(".bss:ddr_scratch_mem")))
__attribute__ ((aligned(4096)))
        ;
#endif

#ifdef DDR_HEAP_NON_CACHE_MEM_SIZE
uint8_t g_ddr_non_cache_mem[DDR_HEAP_NON_CACHE_MEM_SIZE]
__attribute__ ((section(".bss:ddr_non_cache_mem")))
__attribute__ ((aligned(4096)))
        ;
#endif

#ifdef DDR_SCRATCH_NON_CACHE_SIZE
uint8_t g_ddr_scratch_non_cache_mem[DDR_SCRATCH_NON_CACHE_SIZE]
__attribute__ ((section(".bss:ddr_scratch_non_cache_mem")))
__attribute__ ((aligned(4096)))
        ;
#endif

static void appRegisterOpenVXTargetKernels();
static void appUnRegisterOpenVXTargetKernels();
void appRtosTestRegister();
void appRtosTestUnRegister();

#ifdef ENABLE_UART
void appLogDeviceWrite(char *string, uint32_t max_size)
{
    UART_puts(string, max_size);
}
#endif

int32_t appInit()
{
    int32_t status = 0;
    app_mem_init_prm_t mem_init_prm;
    app_log_init_prm_t log_init_prm;
    app_ipc_init_prm_t ipc_init_prm;

    app_mem_heap_prm_t *heap_prm;
    uint32_t host_os_type;

    void *ipc_resource_table = NULL;

    /* Init and start GTC timer */
    status = appLogGlobalTimeInit();
    APP_ASSERT_SUCCESS(status);

    /* appGetIpcResourceTable() returns NULL in RTOS only mode and returns a valid resource table
     * in Linux+RTOS mode
     */
    ipc_resource_table = appGetIpcResourceTable();

    host_os_type = appGetHostOSType();

    appMemInitPrmSetDefault(&mem_init_prm);
    appLogInitPrmSetDefault(&log_init_prm);
    appIpcInitPrmSetDefault(&ipc_init_prm);

    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_DDR];
    heap_prm->base = g_ddr_local_mem;
    strncpy(heap_prm->name, "DDR_LOCAL_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = DDR_HEAP_MEM_SIZE;
    heap_prm->flags = APP_MEM_HEAP_FLAGS_IS_SHARED;

    #ifdef L1_MEM_SIZE
    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_L1];
    heap_prm->base = g_l1_mem;
    strncpy(heap_prm->name, "L1_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = L1_MEM_SIZE;
    heap_prm->flags = APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE;
    #endif
    #ifdef L2_MEM_SIZE
    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_L2];
    heap_prm->base = g_l2_mem;
    strncpy(heap_prm->name, "L2_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = L2_MEM_SIZE;
    heap_prm->flags = APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE;
    #endif
    #ifdef L3_MEM_SIZE
    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_L3];
    heap_prm->base = g_l3_mem;
    strncpy(heap_prm->name, "L3_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = L3_MEM_SIZE;
    #ifdef CPU_mcu2_0
    heap_prm->flags = 0; /* when CPU is mcu2-0 use it as normal heap */
    #else
    heap_prm->flags = APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE;
    #endif
    #endif
    #ifdef DDR_SCRATCH_SIZE
    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_DDR_SCRATCH];
    heap_prm->base = g_ddr_scratch_mem;
    strncpy(heap_prm->name, "DDR_SCRATCH_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = DDR_SCRATCH_SIZE;
    heap_prm->flags = APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE;
    #endif
    #ifdef DDR_HEAP_NON_CACHE_MEM_SIZE
    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_DDR_NON_CACHE];
    heap_prm->base = g_ddr_non_cache_mem;
    strncpy(heap_prm->name, "DDR_NON_CACHE_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = DDR_HEAP_NON_CACHE_MEM_SIZE;
    heap_prm->flags = APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE;
    #endif

    #ifdef DDR_SCRATCH_NON_CACHE_SIZE
    heap_prm = &mem_init_prm.heap_info[APP_MEM_HEAP_DDR_NON_CACHE_SCRATCH];
    heap_prm->base = g_ddr_scratch_non_cache_mem;
    strncpy(heap_prm->name, "DDR_SCRATCH_NON_CACHE_MEM", APP_MEM_HEAP_NAME_MAX);
    heap_prm->size = DDR_SCRATCH_NON_CACHE_SIZE;
    heap_prm->flags = APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE;
    #endif

    #ifdef ENABLE_IPC
    /* appGetIpcResourceTable() returns NULL in RTOS only mode and returns a valid resource table
     */
    ipc_init_prm.ipc_resource_tbl = ipc_resource_table;
    if((host_os_type == APP_HOST_TYPE_LINUX))
    {
        ipc_init_prm.enable_tiovx_ipc_announce = 1;
    }
    else
    {
        ipc_init_prm.enable_tiovx_ipc_announce = 0;
    }
    ipc_init_prm.num_cpus = 0;
    #ifdef ENABLE_IPC_MPU1_0
    ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MPU1_0;
    ipc_init_prm.num_cpus++;
    log_init_prm.log_rd_cpu_enable[APP_IPC_CPU_MPU1_0] = 1;
    #endif
    #ifdef ENABLE_IPC_MCU1_0
    ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU1_0;
    ipc_init_prm.num_cpus++;
    log_init_prm.log_rd_cpu_enable[APP_IPC_CPU_MCU1_0] = 1;
    #endif
    #ifdef ENABLE_IPC_C7x_1
    ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_C7x_1;
    ipc_init_prm.num_cpus++;
    log_init_prm.log_rd_cpu_enable[APP_IPC_CPU_C7x_1] = 1;
    #endif
    ipc_init_prm.tiovx_obj_desc_mem = (void*)g_tiovx_obj_desc_mem;
    ipc_init_prm.tiovx_obj_desc_mem_size = TIOVX_OBJ_DESC_SHARED_MEM_SIZE;
    ipc_init_prm.tiovx_log_rt_mem   = (void*)TIOVX_LOG_RT_MEM_ADDR;
    ipc_init_prm.tiovx_log_rt_mem_size   = TIOVX_LOG_RT_MEM_SIZE;
    ipc_init_prm.ipc_vring_mem = g_ipc_vring_mem;
    ipc_init_prm.ipc_vring_mem_size = IPC_VRING_MEM_SIZE;
    #ifdef CPU_mpu1
    ipc_init_prm.self_cpu_id = APP_IPC_CPU_MPU1_0;
    #endif
    #ifdef CPU_mcu1_0
    ipc_init_prm.self_cpu_id = APP_IPC_CPU_MCU1_0;
    #endif
    #ifdef CPU_c7x_1
    ipc_init_prm.self_cpu_id = APP_IPC_CPU_C7x_1;
    #endif
    #endif

    log_init_prm.shared_mem = &g_app_log_shared_mem;
    log_init_prm.self_cpu_index = ipc_init_prm.self_cpu_id;
    #ifdef CPU_mpu1
    strncpy(log_init_prm.self_cpu_name, "MPU1" , APP_LOG_MAX_CPU_NAME);
    #endif
    #ifdef CPU_mcu1_0
    strncpy(log_init_prm.self_cpu_name, "MCU1_0" , APP_LOG_MAX_CPU_NAME);
    #endif
    #ifdef CPU_c7x_1
    strncpy(log_init_prm.self_cpu_name, "C7x_1" , APP_LOG_MAX_CPU_NAME);
    #endif
    #ifdef ENABLE_UART
    log_init_prm.log_rd_max_cpus = APP_IPC_CPU_MAX;
    log_init_prm.device_write = appLogDeviceWrite;
    #endif

    appPerfStatsInit();

    #ifdef ENABLE_UART
    status = appLogRdInit(&log_init_prm);
    APP_ASSERT_SUCCESS(status);
    #endif

    status = appLogWrInit(&log_init_prm);
    APP_ASSERT_SUCCESS(status);

    #ifdef ENABLE_PRINTF_REDIRECT
    status = appLogCioInit();
    APP_ASSERT_SUCCESS(status);
    #endif

    appUtilsPrintCpuHz();

    appLogPrintf("APP: Init ... !!!\n");

    #ifdef ENABLE_UART
    {
        app_cli_init_prm_t cli_init_prm;

        appCliInitPrmSetDefault(&cli_init_prm);
        status = appCliInit(&cli_init_prm);
        APP_ASSERT_SUCCESS(status);
    }
    #endif

    #ifdef ENABLE_SCICLIENT
    status = appSciclientInit();
    APP_ASSERT_SUCCESS(status);
    #endif

    #ifdef ENABLE_UDMA
    status = appUdmaInit();
    APP_ASSERT_SUCCESS(status);
    #endif

    status = appMemInit(&mem_init_prm);
    APP_ASSERT_SUCCESS(status);

    #ifdef ENABLE_IPC
    status = appIpcInit(&ipc_init_prm);
    APP_ASSERT_SUCCESS(status);

	{
        uint32_t sync_cpu_id_list[APP_LOG_MAX_CPUS];
        uint32_t i, self_cpu_id, master_cpu_id, num_sync_cpus;

        if((host_os_type == APP_HOST_TYPE_LINUX) || (host_os_type == APP_HOST_TYPE_QNX))
        {
            #if defined(SOC_AM62A)
            master_cpu_id = APP_IPC_CPU_MCU1_0;
            #else
            master_cpu_id = APP_IPC_CPU_MCU2_0;
            #endif
        }
        else
        {
            master_cpu_id = APP_IPC_CPU_MPU1_0;
        }
        self_cpu_id = ipc_init_prm.self_cpu_id;
        num_sync_cpus = 0;
        for(i=0; i<ipc_init_prm.num_cpus; i++)
        {
            if(i<APP_LOG_MAX_CPUS)
            {
                if((host_os_type == APP_HOST_TYPE_LINUX) || (host_os_type == APP_HOST_TYPE_QNX))
                {
                    /* dont sync with MPU1 running linux/qnx since that is taken care by the kernel */
                    if(ipc_init_prm.enabled_cpu_id_list[i]!=APP_IPC_CPU_MPU1_0)
                    {
                        sync_cpu_id_list[num_sync_cpus] = ipc_init_prm.enabled_cpu_id_list[i];
                        num_sync_cpus++;
                    }
                }
                else
                {
                    sync_cpu_id_list[num_sync_cpus] = ipc_init_prm.enabled_cpu_id_list[i];
                    num_sync_cpus++;
                }
            }
        }
        appLogPrintf("APP: Syncing with %d CPUs ... !!!\n", num_sync_cpus);
        appLogCpuSyncInit(master_cpu_id, self_cpu_id, sync_cpu_id_list, num_sync_cpus);
        appLogPrintf("APP: Syncing with %d CPUs ... Done !!!\n", num_sync_cpus);
    }

    {
        app_remote_service_init_prms_t init_prms;

        appRemoteServiceInitSetDefault(&init_prms);
        appRemoteServiceInit(&init_prms);
        appRtosTestRegister();
        appPerfStatsRemoteServiceInit();
    }
    #endif

    #ifdef ENABLE_FVID2
    status = appFvid2Init();
    APP_ASSERT_SUCCESS(status);
    #endif

    #ifdef ENABLE_I2C
    appI2cInit();
    #endif

    #ifdef ENABLE_VHWA_VPAC
    status = appVhwaVpacInit(0u);
    APP_ASSERT_SUCCESS(status);
    #endif

    #ifdef ENABLE_TIOVX
    tivxInit();
    #ifdef ENABLE_TIOVX_HOST
    tivxHostInit();
    #endif
    #endif
    appRegisterOpenVXTargetKernels();

    #ifdef ENABLE_VHWA_VPAC
    status = appVissRemoteServiceInit();
    APP_ASSERT_SUCCESS(status);
    #endif

    #ifdef ENABLE_UDMA_COPY
    status = appUdmaCopyInit();
    APP_ASSERT_SUCCESS(status);
    #endif

    #ifdef CPU_mcu1_0
    #if defined(FREERTOS) || defined(SAFERTOS)
    status = appIpcCreateTraceBufFlushTask();
    #endif
    #endif

    appLogPrintf("APP: Init ... Done !!!\n");

    return status;
}

void appDeInit()
{
    appLogPrintf("APP: Deinit ... !!!\n");

    appUnRegisterOpenVXTargetKernels();
    #ifdef ENABLE_UDMA_COPY
    appUdmaCopyDeinit();
    #endif
    #ifdef ENABLE_TIOVX
    #ifdef ENABLE_TIOVX_HOST
    tivxHostDeInit();
    #endif
    tivxDeInit();
    #endif
    #ifdef ENABLE_VHWA_VPAC
    appVhwaVpacDeInit();
    #endif
    #ifdef ENABLE_FVID2
    appFvid2DeInit();
    #endif
    #ifdef ENABLE_IPC
    appPerfStatsDeInit();
    appRtosTestUnRegister();
    appRemoteServiceDeInit();
    appIpcDeInit();
    #endif
    appMemDeInit();
    #ifdef ENABLE_PRINTF_REDIRECT
    appLogCioDeInit();
    #endif
    appLogWrDeInit();
    #ifdef ENABLE_UART
    appLogRdDeInit();
    appCliDeInit();
    #endif
    #ifdef ENABLE_UDMA
    appUdmaDeInit();
    #endif
    #ifdef ENABLE_SCICLIENT
    appSciclientDeInit();
    #endif

    #ifdef ENABLE_I2C
    appI2cDeInit();
    #endif

    #if defined(ENABLE_VHWA_VPAC)
    appVissRemoteServiceDeInit();
    #endif

    /* De-init GTC timer */
    appLogGlobalTimeDeInit();

    /* Unregister remote service for SL2 reallocation.
     * Can add more conditions if needed.
     */
    #ifdef ENABLE_VHWA_DMPAC
    appVhwaRemoteServiceDeInit();
    #endif

    appLogPrintf("APP: Deinit ... Done !!!\n");
}

static void appRegisterOpenVXTargetKernels()
{
    #ifdef ENABLE_TIOVX
    appLogPrintf("APP: OpenVX Target kernel init ... !!!\n");
        #ifdef ENABLE_VHWA_VPAC
        tivxRegisterHwaTargetVpacMscKernels();
        tivxRegisterHwaTargetVpacLdcKernels();
        tivxRegisterHwaTargetVpacVissKernels();
        #endif
        #ifdef C7504
        {
            void app_c7x_target_kernel_img_add_register(void);

            app_c7x_target_kernel_img_add_register();
        }
        tivxRegisterTIDLTargetKernels();
        tivxRegisterTVMTargetKernels();
        #if 0
        tivxRegisterImgProcTargetC71Kernels();
        #endif
        #endif
        #ifdef ENABLE_VHWA_VPAC
        tivxRegisterImgProcTargetR5FKernels();
        #endif
    appLogPrintf("APP: OpenVX Target kernel init ... Done !!!\n");
    #endif
}

static void appUnRegisterOpenVXTargetKernels()
{
    #ifdef ENABLE_TIOVX
    appLogPrintf("APP: OpenVX Target kernel deinit ... !!!\n");
        #ifdef ENABLE_VHWA_VPAC
        tivxUnRegisterHwaTargetVpacMscKernels();
        tivxUnRegisterHwaTargetVpacLdcKernels();
        tivxUnRegisterHwaTargetVpacVissKernels();
        #endif
        #ifdef C71
        {
            void app_c7x_target_kernel_img_add_unregister(void);

            app_c7x_target_kernel_img_add_unregister();
        }
        tivxUnRegisterTIDLTargetKernels();
        tivxUnRegisterTVMTargetKernels();
        #if 0
        tivxUnRegisterImgProcTargetC71Kernels();
        #endif
        #endif
        #ifdef ENABLE_VHWA_VPAC
        {
            void tivxUnRegisterSampleTargetR5FKernels();

            tivxUnRegisterSampleTargetR5FKernels();
        }
        tivxUnRegisterImgProcTargetR5FKernels();
        #endif
    appLogPrintf("APP: OpenVX Target kernel deinit ... Done !!!\n");
    #endif
}

void appIdleLoop(void)
{
    #if defined(__C7504__)
   __asm(" IDLE");
   #endif
}
