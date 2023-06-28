/*
 *
 * Copyright (c) 2019 Texas Instruments Incorporated
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
#include <utils/console_io/include/app_log.h>
#include <utils/remote_service/include/app_remote_service.h>
#include <utils/mem/include/app_mem.h>
#include <utils/ipc/include/app_ipc.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/misc/include/app_misc.h>

#if defined(SYSBIOS) || defined(FREERTOS) || defined(SAFERTOS)
/* define this to enable load test */
#define ENABLE_LOAD_TEST
#endif

/* #define APP_REMOTE_SERVICE_TEST_DEBUG */
#define APP_REMOTE_SERVICE_TEST_NAME  "com.ti.remote_service_test"

#define APP_REMOTE_SERVICE_TEST_CMD_0   (0x1234)
#define APP_REMOTE_SERVICE_TEST_CMD_1   (0x5678)
#define APP_REMOTE_SERVICE_TEST_CMD_2   (0x0002)

#define APP_REMOTE_SERVICE_TEST_CMD_LOAD_TEST_START  (0x0003)
#define APP_REMOTE_SERVICE_TEST_CMD_LOAD_TEST_STOP   (0x0004)

#ifdef ENABLE_LOAD_TEST

#include <ti/osal/TaskP.h>
#include <ti/osal/SemaphoreP.h>

#define APP_REMOTE_SERVICE_LOAD_TEST_MAX_TASK_NAME  (64u)

/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 16KB
 * - AND stack assigned for task context is "size - 8KB"
 *       - 8KB chunk for the stack area is used for interrupt handling in this task context
 */
#define APP_REMOTE_SERVICE_LOAD_TEST_TASK_STACK_SIZE   (32*1024u)
#define APP_REMOTE_SERVICE_LOAD_TEST_TASK_PRI          (10u)

#if defined(R5F) && defined(SAFERTOS)
#define APP_REMOTE_SERVICE_LOAD_TEST_TASK_ALIGNMENT    APP_REMOTE_SERVICE_LOAD_TEST_TASK_STACK_SIZE
#else
#define APP_REMOTE_SERVICE_LOAD_TEST_TASK_ALIGNMENT    (8192u)
#endif

static uint8_t g_app_remote_service_rx_task_stack[APP_REMOTE_SERVICE_LOAD_TEST_TASK_STACK_SIZE]
__attribute__ ((section(".bss:taskStackSection")))
__attribute__ ((aligned(APP_REMOTE_SERVICE_LOAD_TEST_TASK_ALIGNMENT)))
    ;


typedef struct {

    TaskP_Handle task_handle;
    SemaphoreP_Handle start;
    volatile uint32_t stop;
    char task_name[APP_REMOTE_SERVICE_LOAD_TEST_MAX_TASK_NAME];
    uint32_t ld;
} app_remote_service_load_test_obj_t;

static app_remote_service_load_test_obj_t g_app_remote_service_load_test_obj;

void appRemoteServiceLoadTestTaskLoad(uint32_t ld)
{
    volatile int i = 0, j =0, a = 1, count;
    uint32_t cpu_id = appIpcGetSelfCpuId();

    #if defined(SOC_AM62A)
    if (ld > 50 && (cpu_id == APP_IPC_CPU_MCU1_0))
        ld += 5;
    #else
    if (ld > 50 && (cpu_id == APP_IPC_CPU_MCU1_0 || cpu_id == APP_IPC_CPU_MCU2_0))
        ld += 5;
    #endif

    #if defined(SOC_J721E)
    if (ld > 95 && (cpu_id == APP_IPC_CPU_C6x_1 || cpu_id == APP_IPC_CPU_C6x_2))
        ld = 95;
    #endif

    if (cpu_id == APP_IPC_CPU_C7x_1)
        ld = (ld / 2) + (ld / 20);

    #if defined(SOC_J721S2) || defined(SOC_J784S4)
    if (cpu_id == APP_IPC_CPU_C7x_2)
        ld = (ld / 2) + (ld / 20);
    #endif

    #if defined(SOC_J784S4)
    if (cpu_id == APP_IPC_CPU_C7x_3 || cpu_id == APP_IPC_CPU_C7x_4)
        ld = (ld / 2) + (ld / 20);
    #endif

    while(i++ < ld * 1000)
    {
        a++;
    }
}

static void appRemoteServiceLoadTestTaskMain(void *arg0, void *arg1)
{
    app_remote_service_load_test_obj_t *obj = &g_app_remote_service_load_test_obj;
    uint32_t cpu_id = appIpcGetSelfCpuId();

    appUtilsTaskInit();

    while(1)
    {
        SemaphoreP_pend(obj->start, SemaphoreP_WAIT_FOREVER);

        while(1)
        {
            appRemoteServiceLoadTestTaskLoad(obj->ld);
            appLogWaitMsecs(1);
            if(obj->stop)
            {
                break;
            }
        }
        obj->stop = 0;
    }
}

static int32_t appRemoteServiceTestLoadTestInit()
{
    app_remote_service_load_test_obj_t *obj = &g_app_remote_service_load_test_obj;
    TaskP_Params task_prms;
    SemaphoreP_Params semParams;
    int32_t status = 0;

    TaskP_Params_init(&task_prms);

    task_prms.stacksize = APP_REMOTE_SERVICE_LOAD_TEST_TASK_STACK_SIZE;
    task_prms.stack = g_app_remote_service_rx_task_stack;
    task_prms.priority = APP_REMOTE_SERVICE_LOAD_TEST_TASK_PRI;
    task_prms.arg0 = NULL;
    task_prms.arg1 = NULL;
    task_prms.name = (const char*)&obj->task_name[0];

    strncpy(obj->task_name, "LOAD_TEST", APP_REMOTE_SERVICE_LOAD_TEST_MAX_TASK_NAME);
    obj->task_name[APP_REMOTE_SERVICE_LOAD_TEST_MAX_TASK_NAME-1] = 0;

    obj->stop = 0;

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    obj->start = SemaphoreP_create(0U, &semParams);
    if(obj->start==NULL)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: Unable to create tx semaphore\n");
        appLogPrintf("  Check for memory leak, or may need to increase\n");
        appLogPrintf("  the value of OSAL_TIRTOS_MAX_SEMAPHOREP_PER_SOC\n");
        appLogPrintf("  in pdk/packages/ti/osal/soc/<>/osal_soc.h \n");

        status = -1;
    }

    obj->task_handle = (void*)TaskP_create(
                            &appRemoteServiceLoadTestTaskMain,
                            &task_prms);
    if(obj->task_handle==NULL)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Unable to create task \n");
        status = -1;
    }
    else
    {
        appPerfStatsRegisterTask(obj->task_handle, obj->task_name);
    }
    return status;
}

static void appRemoteServiceTestLoadTestStart(uint32_t load)
{
    app_remote_service_load_test_obj_t *obj = &g_app_remote_service_load_test_obj;

    obj->ld = load;
    obj->stop = 0;

    SemaphoreP_post(obj->start);
}

static void appRemoteServiceTestLoadTestStop()
{
    app_remote_service_load_test_obj_t *obj = &g_app_remote_service_load_test_obj;

    obj->stop = 1;

    while(obj->stop != 0)
    {
        appLogWaitMsecs(1);
    }
}

#endif

int32_t appRemoteServiceTestHandler(char *service_name, uint32_t cmd, void *prm, uint32_t prm_size, uint32_t flags)
{
    uint32_t *value;

    #ifdef APP_REMOTE_SERVICE_TEST_DEBUG
    appLogPrintf("REMOTE_SERVICE_TEST: %s service received 0x%08x command and parameters of size %d bytes with %08x flags\n",
        service_name,
        cmd,
        prm_size,
        flags
        );
    #endif
    if(cmd==APP_REMOTE_SERVICE_TEST_CMD_0)
    {
        if(prm!=NULL)
        {
            value = (uint32_t*)prm;

            *value = *value + 1;
        }
    }
    else
    if(cmd==APP_REMOTE_SERVICE_TEST_CMD_1)
    {
        /* prm is a physical pointer, it will be written with a value by the sender
         * we just print the value here and also echo back the same + 1 */
        if(prm!=NULL && prm_size == sizeof(uint32_t))
        {
            uintptr_t addr = (uintptr_t)(*(volatile uint32_t*)prm);

            appMemCacheInv((void*)addr, 256);

            uint32_t value = *(volatile uint32_t*)addr;

            #ifdef APP_REMOTE_SERVICE_TEST_DEBUG
            appLogPrintf("REMOTE_SERVICE_TEST: 0x%08x = %d\n",
                (uint32_t)addr,
                (uint32_t)value
                );
            #endif

            *(volatile uint32_t*)addr = value+1;

            appMemCacheWbInv((void*)addr, 256);

            #ifdef R5F
            asm("  dsb");
            #endif
        }
    }
    else
    if(cmd==APP_REMOTE_SERVICE_TEST_CMD_2)
    {
        uint64_t local_time, global_time;
        uint32_t delay;

        delay = 1;
        if(prm!=NULL)
        {
            delay = *(uint32_t*)prm;
        }

        local_time = appLogGetLocalTimeInUsec();
        global_time = appLogGetGlobalTimeInUsec();

        appLogWaitMsecs(delay);

        local_time = appLogGetLocalTimeInUsec() - local_time;
        global_time = appLogGetGlobalTimeInUsec() - global_time;

        appLogPrintf("REMOTE_SERVICE_TEST: delay = %d ms, elasped time local = %d ms, elapsed time global = %d ms\n",
            delay,
            (uint32_t)(local_time/1000),
            (uint32_t)(global_time/1000)
            );
    }
    #ifdef ENABLE_LOAD_TEST
    else
    if(cmd==APP_REMOTE_SERVICE_TEST_CMD_LOAD_TEST_START)
    {
        appRemoteServiceTestLoadTestStart(*(volatile uint32_t*)prm);
    }
    if(cmd==APP_REMOTE_SERVICE_TEST_CMD_LOAD_TEST_STOP)
    {
        appRemoteServiceTestLoadTestStop();
    }
    #endif
    return 0;
}

int32_t appRemoteServiceTestInit()
{
    int32_t status = 0;

    #ifdef ENABLE_LOAD_TEST
    status = appRemoteServiceTestLoadTestInit();
    if(status!=0)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Unable to create load test task/semaphore\n");
    }
    #endif

    status = appRemoteServiceRegister(APP_REMOTE_SERVICE_TEST_NAME, appRemoteServiceTestHandler);
    if(status!=0)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Unable to register remote service test handler\n");
    }
    return status;
}

int32_t appRemoteServiceTestDeInit()
{
    int32_t status = 0;

    status = appRemoteServiceUnRegister(APP_REMOTE_SERVICE_TEST_NAME);
    if(status!=0)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Unable to register remote service test handler\n");
    }
    return status;
}

int32_t appRemoteServiceTestRunCmd0(uint32_t cpu_id)
{
    int32_t status = 0;
    volatile uint32_t value = 0;
    uint32_t i, count = 10;
    uint32_t test_pass = 1;

    appLogPrintf("REMOTE_SERVICE_TEST: Running test for CPU %s !!!\n",
        appIpcGetCpuName(cpu_id)
        );

    for(i=0; i<count; i++)
    {
        status = appRemoteServiceRun(cpu_id, APP_REMOTE_SERVICE_TEST_NAME, APP_REMOTE_SERVICE_TEST_CMD_0, (void*)&value, sizeof(uint32_t), 0);
        if(status!=0 || value != (i+1))
        {
            test_pass = 0;
            status = -1;
            break;
        }
    }
    if(test_pass)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: Test passed !!!\n");
    }
    else
    {
        appLogPrintf("REMOTE_SERVICE_TEST: Test failed @ iteration %d !!!\n", i);
    }
    return status;
}

int32_t appRemoteServiceTestRunCmd1(uint32_t cpu_id)
{
    int32_t status = 0;
    uint32_t value = 0;
    uint32_t i, count = 10;
    uint32_t test_pass = 1;
    uint32_t mem_size = 1024;
    uint64_t phys_ptr;
    void *virt_ptr;

    virt_ptr = appMemAlloc(APP_MEM_HEAP_DDR, mem_size, 32);
    if(NULL == virt_ptr)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: Test failed!!!\n");
        return (-1);
    }

    phys_ptr = appMemGetVirt2PhyBufPtr((uint64_t)(uintptr_t)virt_ptr, APP_MEM_HEAP_DDR);

    /* assume 32b physical pointer */
    value = (uint32_t)phys_ptr;

    appLogPrintf("REMOTE_SERVICE_TEST: Running test @ 0x%08x of %d bytes size for CPU %s !!!\n",
        value,
        mem_size,
        appIpcGetCpuName(cpu_id)
        );

    for(i=0; i<count; i++)
    {
        volatile uint32_t before, after;

        before = i*10;

        *(volatile uint32_t*)virt_ptr = before;

        appMemCacheWbInv((void*)virt_ptr, 256);

        #ifdef A72
        asm("    DSB SY");
        #endif

        status = appRemoteServiceRun(cpu_id, APP_REMOTE_SERVICE_TEST_NAME, APP_REMOTE_SERVICE_TEST_CMD_1, &value, sizeof(uint32_t), 0);

        appMemCacheInv((void*)virt_ptr, 256);

        #ifdef A72
        asm("    DSB SY");
        #endif

        after = *(volatile uint32_t*)virt_ptr;

        #ifdef APP_REMOTE_SERVICE_TEST_DEBUG
        appLogPrintf("REMOTE_SERVICE_TEST: 0x%08x = %d\n",
            phys_ptr,
            after
            );
        #endif

        #if 1
        if(after != (before+1))
        {
            test_pass = 0;
            break;
        }
        #endif
    }

    appMemFree(APP_MEM_HEAP_DDR, virt_ptr, mem_size);
    if(test_pass)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: Test passed !!!\n");
    }
    else
    {
        appLogPrintf("REMOTE_SERVICE_TEST: Test failed @ iteration %d !!!\n", i);
    }
    return status;
}

int32_t appRemoteServiceTestRunTimerTest(uint32_t cpu_id)
{
    int32_t status = 0;
    uint32_t value = 10*1000;

    appLogPrintf("REMOTE_SERVICE_TEST: Running timer test of %d msecs for CPU %s !!!\n",
        value,
        appIpcGetCpuName(cpu_id)
        );

    status = appRemoteServiceRun(cpu_id, APP_REMOTE_SERVICE_TEST_NAME, APP_REMOTE_SERVICE_TEST_CMD_2, &value, sizeof(uint32_t), 0);
    if(status != 0)
    {
        appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Timer test !!!\n");
    }

    appLogPrintf("REMOTE_SERVICE_TEST: Running timer test of %d msecs for CPU %s ... DONE !!!\n",
        value,
        appIpcGetCpuName(cpu_id)
        );

    return status;
}

int32_t appRemoteServiceTestRunLoadTestStart(uint32_t cpu_id, uint32_t load)
{
    int32_t status = 0;
    volatile uint32_t tl = load;

    if(appIpcIsCpuEnabled(cpu_id))
    {

        appLogPrintf("REMOTE_SERVICE_TEST: Started load test for CPU %s load is %d !!!\n",
           appIpcGetCpuName(cpu_id), load);

        status = appRemoteServiceRun(cpu_id, APP_REMOTE_SERVICE_TEST_NAME, APP_REMOTE_SERVICE_TEST_CMD_LOAD_TEST_START, (void *)&tl, sizeof(uint32_t), 0);
        if(status != 0)
        {
            appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Load Test !!!\n");
        }
    }
    return status;
}

int32_t appRemoteServiceTestRunLoadTestStop(uint32_t cpu_id)
{
    int32_t status = 0;

    if(appIpcIsCpuEnabled(cpu_id))
    {
        status = appRemoteServiceRun(cpu_id, APP_REMOTE_SERVICE_TEST_NAME, APP_REMOTE_SERVICE_TEST_CMD_LOAD_TEST_STOP, NULL, 0, 0);
        if(status != 0)
        {
            appLogPrintf("REMOTE_SERVICE_TEST: ERROR: Load Test !!!\n");
        }

        appLogPrintf("REMOTE_SERVICE_TEST: Stopped load test for CPU %s !!!\n",
            appIpcGetCpuName(cpu_id)
            );

    }
    return status;
}

int32_t appRemoteServiceTestRun(uint32_t cpu_id)
{
    appRemoteServiceTestRunCmd0(cpu_id);
    appRemoteServiceTestRunCmd1(cpu_id);
    appRemoteServiceTestRunTimerTest(cpu_id);

    return 0;
}
