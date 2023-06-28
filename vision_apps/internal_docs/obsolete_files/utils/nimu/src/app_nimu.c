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

#include <utils/console_io/include/app_log.h>
#include <stdio.h>
#include <string.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/utils/Load.h>

/* CSL Header files */
#include <ti/csl/soc.h>

/* NDK headers */
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/stkmain.h>

/* OSAL Header files */
#include <ti/osal/osal.h>

/* BOARD Header files */
#include <ti/board/board.h>

/* EMAC Driver Header File. */
#include <ti/drv/emac/emac_drv.h>
#include <ti/drv/emac/src/v5/emac_drv_v5.h>
#include <ti/drv/emac/soc/emac_soc_v5.h>


#define NIMU_MAX_TABLE_ENTRIES   9U
#define NIMU_NUM_PORTS 1U
#define NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT 2
#define NIMU_TEST_MAX_CHANS_PER_PORT 2

#define NIMU_RX_GOOD_FRAMES_ADDR  (*((uint32_t *)0x4603A000))
#define NIMU_RX_BCAST_FRAMES_ADDR (*((uint32_t *)0x4603A200))
#define NIMU_TX_GOOD_FRAMES_ADDR  (*((uint32_t *)0x4603A034))
#define NIMU_TX_BCAST_FRAMES_ADDR (*((uint32_t *)0x4603A234))

/* brief Number of ring entries - we can prime this much memcpy operations */
#define NIMU_TEST_APP_RING_ENTRIES      (128U)
/* Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define NIMU_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
 /* Total ring memory */
#define NIMU_TEST_APP_RING_MEM_SIZE     (NIMU_TEST_APP_RING_ENTRIES * \
                                          NIMU_TEST_APP_RING_ENTRY_SIZE)

/* UDMA host mode buffer descriptor memory size.    *  Make it multiple of 128 byte alignment */
#define NIMU_TEST_APP_DESC_SIZE         (sizeof(CSL_UdmapCppi5HMPD) + (128U - sizeof(CSL_UdmapCppi5HMPD)))


/* UDMA driver objects
 */
struct Udma_ChObj       gUdmaTxChObj[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT];
struct Udma_ChObj       gUdmaRxChObj[NIMU_NUM_PORTS];
struct Udma_EventObj    gUdmaRxCqEventObj[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT];
struct Udma_ChObj       gUdmaRxCfgPsiChObj[NIMU_NUM_PORTS];
struct Udma_EventObj    gUdmaRxCfgPsiCqEventObj[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT];
static uint8_t gUdmapDescRamTx[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT][NIMU_TEST_APP_RING_ENTRIES * NIMU_TEST_APP_DESC_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmapDescRamRx[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT][NIMU_TEST_APP_RING_ENTRIES * NIMU_TEST_APP_DESC_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmapDescRamRxCfgPsi[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT][NIMU_TEST_APP_RING_ENTRIES * NIMU_TEST_APP_DESC_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT][NIMU_TEST_APP_RING_MEM_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gTxCompRingMem[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT][NIMU_TEST_MAX_CHANS_PER_PORT][NIMU_TEST_APP_RING_MEM_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
struct Udma_FlowObj gUdmaFlowHnd[NIMU_NUM_PORTS] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
struct Udma_RingObj gUdmaRxRingHnd[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
struct Udma_RingObj gUdmaRxCompRingHnd[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
struct Udma_RingObj gUdmaRxRingHndCfgPsi[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
struct Udma_RingObj gUdmaRxCompRingHndCfgPsi[NIMU_NUM_PORTS][NIMU_TEST_MAX_CHANS_PER_PORT] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gRxRingMem[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT][NIMU_TEST_APP_RING_MEM_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gRxCompRingMem[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT][NIMU_TEST_APP_RING_MEM_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gRxRingMemCfgPsi[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT][NIMU_TEST_APP_RING_MEM_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gRxCompRingMemCfgPsi[NIMU_NUM_PORTS][NIMU_TEST_MAX_SUB_RX_CHANS_PER_PORT][NIMU_TEST_APP_RING_MEM_SIZE] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));
struct Udma_FlowObj gUdmaFlowHndCfgPsi[NIMU_NUM_PORTS] __attribute__ ((aligned(UDMA_CACHELINE_ALIGNMENT)));



#define NIMU_EMAC_PORT_CPSW 6U

NIMU_DEVICE_TABLE_ENTRY NIMUDeviceTable[NIMU_MAX_TABLE_ENTRIES];

extern int32_t (*NimuEmacInitFxn[NIMU_MAX_TABLE_ENTRIES])(STKEVENT_Handle hEvent);

extern char *LocalIPAddr;




/*
 *  ======== appNimuInitEmac========
 */
void appNimuInitEmac(uint32_t portNum, uint32_t index)
{
    EMAC_HwAttrs_V5 emacCfg;
    EMAC_socGetInitCfg(0, &emacCfg);
    int32_t chanNum = 0;
    int32_t subChanNum = 0;


    for (chanNum = 0; chanNum < emacCfg.portCfg[portNum].nTxChans; chanNum++)
    {
        emacCfg.portCfg[portNum].txChannel[chanNum].chHandle = (void *)&gUdmaTxChObj[index][chanNum];
        emacCfg.portCfg[portNum].txChannel[chanNum].freeRingMem= (void*)&gTxRingMem[index][chanNum][0];
        emacCfg.portCfg[portNum].txChannel[chanNum].compRingMem= (void*)&gTxCompRingMem[index][chanNum][0];
        emacCfg.portCfg[portNum].txChannel[chanNum].hPdMem = (void*)&gUdmapDescRamTx[index][chanNum][0];
    }
    emacCfg.portCfg[portNum].rxChannel.chHandle = (void *)&gUdmaRxChObj[index];
    emacCfg.portCfg[portNum].rxChannel.flowHandle= (void *)&gUdmaFlowHnd[index];

    for (subChanNum = 0; subChanNum < emacCfg.portCfg[portNum].rxChannel.nsubChan; subChanNum++)
    {
        emacCfg.portCfg[portNum].rxChannel.subChan[subChanNum].freeRingMem[0] = (void*)&gRxRingMem[index][subChanNum][0];
        emacCfg.portCfg[portNum].rxChannel.subChan[subChanNum].freeRingHandle[0] = (void*)&gUdmaRxRingHnd[index][subChanNum];
        emacCfg.portCfg[portNum].rxChannel.subChan[subChanNum].compRingMem= (void*)&gRxCompRingMem[index][subChanNum][0];
        emacCfg.portCfg[portNum].rxChannel.subChan[subChanNum].compRingHandle= (void*)&gUdmaRxCompRingHnd[index][subChanNum];
        emacCfg.portCfg[portNum].rxChannel.subChan[subChanNum].hPdMem[0] = (void*)&gUdmapDescRamRx[index][subChanNum][0];
        emacCfg.portCfg[portNum].rxChannel.subChan[subChanNum].eventHandle = (void *)&gUdmaRxCqEventObj[index][subChanNum];
    }
    emacCfg.portCfg[portNum].rxChannelCfgOverPSI.chHandle = (void *)&gUdmaRxCfgPsiChObj[index];
    emacCfg.portCfg[portNum].rxChannelCfgOverPSI.flowHandle= (void *)&gUdmaFlowHndCfgPsi[index];

    for (subChanNum = 0; subChanNum < emacCfg.portCfg[portNum].rxChannelCfgOverPSI.nsubChan; subChanNum++)
    {
        emacCfg.portCfg[portNum].rxChannelCfgOverPSI.subChan[subChanNum].freeRingMem[0] = (void*)&gRxRingMemCfgPsi[index][subChanNum][0];
        emacCfg.portCfg[portNum].rxChannelCfgOverPSI.subChan[subChanNum].freeRingHandle[0] = (void*)&gUdmaRxRingHndCfgPsi[index][subChanNum];
        emacCfg.portCfg[portNum].rxChannelCfgOverPSI.subChan[subChanNum].compRingMem= (void*)&gRxCompRingMemCfgPsi[index][subChanNum][0];
        emacCfg.portCfg[portNum].rxChannelCfgOverPSI.subChan[subChanNum].compRingHandle= (void*)&gUdmaRxCompRingHndCfgPsi[index][subChanNum];
        emacCfg.portCfg[portNum].rxChannelCfgOverPSI.subChan[subChanNum].hPdMem[0] = (void*)&gUdmapDescRamRxCfgPsi[index][subChanNum][0];
        emacCfg.portCfg[portNum].rxChannelCfgOverPSI.subChan[subChanNum].eventHandle = (void *)&gUdmaRxCfgPsiCqEventObj[index][subChanNum];
    }


    EMAC_socSetInitCfg(0, &emacCfg);

}

void appNimuGetIpAddr(char* ipAddrStr)
{
    strcpy(ipAddrStr, LocalIPAddr);
}

void appNimuGetStats(void)
{
    EMAC_STATISTICS_T cpswStats;

    memset(&cpswStats, 0, sizeof(EMAC_STATISTICS_T));

    cpswStats.RxGoodFrames = NIMU_RX_GOOD_FRAMES_ADDR;
    cpswStats.RxBCastFrames = NIMU_RX_BCAST_FRAMES_ADDR;

    cpswStats.TxGoodFrames = NIMU_TX_GOOD_FRAMES_ADDR;
    cpswStats.TxBCastFrames =NIMU_TX_BCAST_FRAMES_ADDR;

    appLogPrintf("**********CPSW stats for PORT NUMBER 6**********\n");
    appLogPrintf("-----------------------------------------------------------------------------------------------------------------\n");
    appLogPrintf(" RX |     Good:      %12u | Bcast:       %12u | Mcast:     %12u | Oct:          %12u |\n",
                    cpswStats.RxGoodFrames,
                    cpswStats.RxBCastFrames,
                    cpswStats.RxMCastFrames,
                    cpswStats.RxOctets);
    appLogPrintf(" TX |     Good:      %12u | Bcast:       %12u | Mcast:     %12u | Oct           %12u |\n",
                    cpswStats.TxGoodFrames,
                    cpswStats.TxBCastFrames,
                    cpswStats.TxMCastFrames,
                    cpswStats.TxOctets);
    appLogPrintf("---------------------------------------------------------------------------------------------------------------\n");

}

int32_t appNimuInit()
{
    uint32_t nimuDeviceIndex = 0U;

    appNimuInitEmac(NIMU_EMAC_PORT_CPSW, nimuDeviceIndex);

    NIMUDeviceTable[nimuDeviceIndex++].init =  NimuEmacInitFxn[NIMU_EMAC_PORT_CPSW];
    NIMUDeviceTable[nimuDeviceIndex].init =  NULL;

    return 0;
}
