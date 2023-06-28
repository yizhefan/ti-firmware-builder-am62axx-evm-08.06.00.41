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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <utils/mem/include/app_mem.h>
#include <utils/console_io/include/app_log.h>
#include <utils/ipc/include/app_ipc.h>
#include <utils/remote_service/include/app_remote_service.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <stdint.h>
#include <TI/tivx.h>

#include <app_mem_map.h>
#include <app_cfg.h>

#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

/* Mutex for controlling access to Init/De-Init. */
static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

/* Counter for tracking the {init, de-init} calls. This is also used to
 * guarantee a single init/de-init operation.
 */
static uint32_t gInitCount = 0U;

static int32_t appCommonInitLocal();
static int32_t appCommonDeInitLocal();

int32_t appCommonInit()
{
    int32_t status = 0;

    pthread_mutex_lock(&gMutex);

    if (gInitCount == 0U)
    {
        status = appCommonInitLocal();
    }

    gInitCount++;

    pthread_mutex_unlock(&gMutex);

    return status;
}

int32_t appCommonDeInit()
{
    int32_t status = 0;

    pthread_mutex_lock(&gMutex);

    if (gInitCount != 0U)
    {
        gInitCount--;

        if (gInitCount == 0U)
        {
            status = appCommonDeInitLocal();
        }
    }
    else
    {
        status = -1;
    }

    pthread_mutex_unlock(&gMutex);

    return status;
}

static int32_t appCommonInitLocal()
{
    int32_t status = 0;
    app_log_init_prm_t log_init_prm;
    app_ipc_init_prm_t ipc_init_prm;
    app_remote_service_init_prms_t remote_service_init_prm;

    printf("APP: Init QNX ... !!!\n");

    status = appLogGlobalTimeInit();
    if(status!=0)
    {
        printf("APP: ERROR: Global timer init failed !!!\n");
    }

    if(status==0)
    {
        appLogInitPrmSetDefault(&log_init_prm);

        log_init_prm.shared_mem = (app_log_shared_mem_t *)APP_LOG_MEM_ADDR;
        log_init_prm.self_cpu_index = APP_IPC_CPU_MPU1_0;
        strncpy(log_init_prm.self_cpu_name, "MPU1_0", APP_LOG_MAX_CPU_NAME);
   
        status = appLogWrInit(&log_init_prm);
        if(status!=0)
        {
            printf("APP: ERROR: Log writer init failed !!!\n");
        }
    }
    if(status==0)
    {
        status = appMemInit(NULL);
        if(status!=0)
        {
            printf("APP: ERROR: Memory init failed !!!\n");
        }
    }
    if(status==0)
    {
        appIpcInitPrmSetDefault(&ipc_init_prm);
        appRemoteServiceInitSetDefault(&remote_service_init_prm);

        #ifdef ENABLE_IPC
        ipc_init_prm.num_cpus = 0;
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MPU1_0;
        ipc_init_prm.num_cpus++;
        #ifdef ENABLE_IPC_MCU1_0
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU1_0;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_MCU1_1
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU1_1;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_MCU2_0
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU2_0;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_MCU2_1
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU2_1;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_MCU3_0
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU3_0;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_MCU3_1
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_MCU3_1;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_C6x_1
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_C6x_1;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_C6x_2
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_C6x_2;
        ipc_init_prm.num_cpus++;
        #endif
        #ifdef ENABLE_IPC_C7x_1
        ipc_init_prm.enabled_cpu_id_list[ipc_init_prm.num_cpus] = APP_IPC_CPU_C7x_1;
        ipc_init_prm.num_cpus++;
        #endif

        ipc_init_prm.self_cpu_id = APP_IPC_CPU_MPU1_0;

        /* Adding so vring memory not NULL */
        ipc_init_prm.ipc_vring_mem =  (void *)  IPC_VRING_MEM_ADDR;
        ipc_init_prm.ipc_vring_mem_size = IPC_VRING_MEM_SIZE;

        status = appIpcInit(&ipc_init_prm);
        if(status!=0)
        {
            printf("APP: ERROR: IPC init failed !!!\n");
        }
        status = appRemoteServiceInit(&remote_service_init_prm);
        if(status!=0)
        {
            printf("APP: ERROR: Remote service init failed !!!\n");
        }
        status = appPerfStatsInit();
        if(status!=0)
        {
            printf("APP: ERROR: Perf stats init failed !!!\n");
        }
        status = appPerfStatsRemoteServiceInit();
        if(status!=0)
        {
            printf("APP: ERROR: Perf stats remote service init failed !!!\n");
        }
        #endif

        appLogPrintGtcFreq();
    }
    printf("APP: Init ... Done !!!\n");
    return status;
}

static int32_t appCommonDeInitLocal()
{
    int32_t status = 0;

    printf("APP: Deinit ... !!!\n");

    appRemoteServiceDeInit();
    appIpcDeInit();
    appLogWrDeInit();
    appMemDeInit();

    /* De-init GTC timer */
    status = appLogGlobalTimeDeInit();

    printf("APP: Deinit ... Done !!!\n");

    return status;
}

