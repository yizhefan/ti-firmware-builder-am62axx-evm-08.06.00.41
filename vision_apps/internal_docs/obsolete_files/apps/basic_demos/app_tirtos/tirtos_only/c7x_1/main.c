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

#include <app.h>
#include <utils/console_io/include/app_log.h>
#include <utils/misc/include/app_misc.h>
#include <stdio.h>
#include <string.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/family/c7x/Hwi.h>
#include <app_mem_map.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_clec.h>

static Void appMain(UArg arg0, UArg arg1)
{
    appInit();
    appRun();
    #if 1
    while(1)
    {
        appLogWaitMsecs(100u);
    }
    #else
    appDeInit();
    #endif
}

void StartupEmulatorWaitFxn (void)
{
    volatile uint32_t enableDebug = 0;
    do
    {
    }while (enableDebug);
}

/* To set C71 timer interrupts */
void appTimerInterruptInit(void)
{
    CSL_ClecEventConfig   cfgClec;
    CSL_CLEC_EVTRegs     *clecBaseAddr = (CSL_CLEC_EVTRegs*)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;

    uint32_t input         = 1249; /* Used for Timer Interrupt */
    uint32_t corepackEvent = 15;

    /* Configure CLEC */
    cfgClec.secureClaimEnable = FALSE;
    cfgClec.evtSendEnable     = TRUE;
    cfgClec.rtMap             = CSL_CLEC_RTMAP_CPU_ALL;
    cfgClec.extEvtNum         = 0;
    cfgClec.c7xEvtNum         = corepackEvent;
    CSL_clecConfigEvent(clecBaseAddr, input, &cfgClec);
    Hwi_setPriority(corepackEvent, 1);
}

/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 16KB
 * - AND stack assigned for task context is "size - 8KB"
 *       - 8KB chunk for the stack area is used for interrupt handling in this task context
 */
static uint8_t gTskStackMain[16*1024]
__attribute__ ((section(".bss:taskStackSection")))
__attribute__ ((aligned(8192)))
    ;

int main(void)
{
    Task_Params tskParams;
    Error_Block eb;
    Task_Handle task;


    /* This is for debug purpose - see the description of function header */
    StartupEmulatorWaitFxn();

    /* move C7x to non-secure mode, this is needed so that coherency
     * between A72 (which runs in non-secure mode) and C7x can be effective
     *
     * For this we need to init CLEC to allow non-secure to program it
     * after the switch
     * And then we switch C7x to non-secure mode
     */
    appC7xClecInitForNonSecAccess();
    appC7xSecSupv2NonSecSupv();

    appTimerInterruptInit();

    Error_init(&eb);
    Task_Params_init(&tskParams);

    tskParams.arg0 = (UArg) NULL;
    tskParams.arg1 = (UArg) NULL;
    tskParams.priority = 8u;
    tskParams.stack = gTskStackMain;
    tskParams.stackSize = sizeof (gTskStackMain);
    task = Task_create(appMain, &tskParams, &eb);
    if(NULL == task)
    {
        BIOS_exit(0);
    }
    BIOS_start();

    return 0;
}

#include <ti/sysbios/family/c7x/Mmu.h>

void appMmuInit(void)
{
    Bool            retVal;
    Mmu_MapAttrs    attrs;

    Mmu_initMapAttrs(&attrs);

    attrs.attrIndx = Mmu_AttrIndx_MAIR0;

    retVal = Mmu_map(0x00000000, 0x00000000, 0x20000000, &attrs);
    if(retVal==FALSE)
    {
        goto mmu_exit;
    }

    retVal = Mmu_map(0x20000000, 0x20000000, 0x20000000, &attrs);
    if(retVal==FALSE)
    {
        goto mmu_exit;
    }

    retVal = Mmu_map(0x40000000, 0x40000000, 0x20000000, &attrs);
    if(retVal==FALSE)
    {
        goto mmu_exit;
    }

    retVal = Mmu_map(0x60000000, 0x60000000, 0x10000000, &attrs);
    if(retVal==FALSE)
    {
        goto mmu_exit;
    }

    retVal = Mmu_map(0x70000000, 0x70000000, 0x10000000, &attrs);
    if(retVal==FALSE)
    {
        goto mmu_exit;
    }

    Mmu_initMapAttrs(&attrs);

    attrs.attrIndx = Mmu_AttrIndx_MAIR4;

    retVal = Mmu_map(NON_CACHED_MEM_ADDR, NON_CACHED_MEM_ADDR, NON_CACHED_MEM_SIZE, &attrs); /* ddr            */
    if(retVal == FALSE)
    {
        goto mmu_exit;
    }

    Mmu_initMapAttrs(&attrs);

    attrs.attrIndx = Mmu_AttrIndx_MAIR7;

    retVal = Mmu_map(MSMC_C7x_1_ADDR, MSMC_C7x_1_ADDR, MSMC_C7x_1_SIZE, &attrs); /* msmc        */
    if(retVal == FALSE)
    {
        goto mmu_exit;
    }

    Mmu_initMapAttrs(&attrs);
    attrs.attrIndx = Mmu_AttrIndx_MAIR7;

    retVal = Mmu_map(DDR_C7x_1_BOOT_ADDR, DDR_C7x_1_BOOT_ADDR, DDR_C7x_1_BOOT_SIZE, &attrs); /* ddr            */
    if(retVal == FALSE)
    {
        goto mmu_exit;
    }

    retVal = Mmu_map(DDR_C7x_1_ADDR, DDR_C7x_1_ADDR, DDR_C7x_1_SIZE, &attrs); /* ddr            */
    if(retVal == FALSE)
    {
        goto mmu_exit;
    }

    Mmu_initMapAttrs(&attrs);
    attrs.attrIndx = Mmu_AttrIndx_MAIR7;

    retVal = Mmu_map(DDR_SHARED_MEM_ADDR, DDR_SHARED_MEM_ADDR, DDR_SHARED_MEM_SIZE, &attrs); /* ddr            */
    if(retVal == FALSE)
    {
        goto mmu_exit;
    }

mmu_exit:
    if(retVal == FALSE)
    {
         printf(" ERROR: MMU init failed (status = %d) !!!", retVal);
    }



    return;
}

void *appGetIpcResourceTable()
{
    return NULL;
}

