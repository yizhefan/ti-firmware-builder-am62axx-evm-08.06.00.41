/* 
 * This file is AUTO GENERATED by PyTI_PSDK_RTOS tool. 
 * It is NOT recommended to manually edit this file 
 */ 
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
 *        No reverse engineering, decompilation, or disassembly of this software is 
 * permitted with respect to any software provided in binary form. 
 * 
 *        any redistribution and use are licensed by TI for use only with TI Devices. 
 * 
 *        Nothing shall obligate TI to provide you with source code for the software 
 * licensed and provided to you in object code. 
 * 
 * If software source code is provided to you, modification and redistribution of the 
 * source code are permitted provided that the following conditions are met: 
 * 
 *        any redistribution and use of the source code, including any resulting derivative 
 * works, are licensed by TI for use only with TI Devices. 
 * 
 *        any redistribution and use of any object code compiled from the source code 
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

MEMORY
{
    /* R5F_TCMA_VECS [ size 64 B ] */
    R5F_TCMA_VECS            (    X ) : ORIGIN = 0x00000000 , LENGTH = 0x00000040
    /* R5F_TCMA [ size 31.94 KB ] */
    R5F_TCMA                 (    X ) : ORIGIN = 0x00000040 , LENGTH = 0x00007FC0
    /* Main OCRAM for MCU2_1 [ size 256.00 KB ] */
    MAIN_OCRAM_MCU2_1        ( RWIX ) : ORIGIN = 0x03640000 , LENGTH = 0x00040000
    /* R5F_TCMB0 [ size 32.00 KB ] */
    R5F_TCMB0                ( RWIX ) : ORIGIN = 0x41010000 , LENGTH = 0x00008000
    /* DDR for MCU2_1 for Linux IPC [ size 1024.00 KB ] */
    DDR_MCU2_1_IPC           ( RWIX ) : ORIGIN = 0xA4000000 , LENGTH = 0x00100000
    /* DDR for MCU2_1 for Linux resource table [ size 1024 B ] */
    DDR_MCU2_1_RESOURCE_TABLE ( RWIX ) : ORIGIN = 0xA4100000 , LENGTH = 0x00000400
    /* DDR for MCU2_1 for code/data [ size 31.00 MB ] */
    DDR_MCU2_1               ( RWIX ) : ORIGIN = 0xA4100400 , LENGTH = 0x01EFFC00
    /* Memory for IPC Vring's. MUST be non-cached or cache-coherent [ size 32.00 MB ] */
    IPC_VRING_MEM                     : ORIGIN = 0xAA000000 , LENGTH = 0x02000000
    /* Memory for remote core logging [ size 256.00 KB ] */
    APP_LOG_MEM                       : ORIGIN = 0xAC000000 , LENGTH = 0x00040000
    /* Memory for TI OpenVX shared memory. MUST be non-cached or cache-coherent [ size 63.75 MB ] */
    TIOVX_OBJ_DESC_MEM                : ORIGIN = 0xAC040000 , LENGTH = 0x03FC0000
    /* Memory for shared memory buffers in DDR [ size 512.00 MB ] */
    DDR_SHARED_MEM                    : ORIGIN = 0xB8000000 , LENGTH = 0x20000000
    /* DDR for MCU2_1 for local heap [ size 16.00 MB ] */
    DDR_MCU2_1_LOCAL_HEAP    ( RWIX ) : ORIGIN = 0xDA000000 , LENGTH = 0x01000000
    /* Inter-core ethernet shared desc queues. MUST be non-cached or cache-coherent [ size  8.00 MB ] */
    INTERCORE_ETH_DESC_MEM            : ORIGIN = 0xE4000000 , LENGTH = 0x00800000
    /* Inter-core ethernet shared data buffers. MUST be non-cached or cache-coherent [ size 24.00 MB ] */
    INTERCORE_ETH_DATA_MEM            : ORIGIN = 0xE4800000 , LENGTH = 0x01800000
}
