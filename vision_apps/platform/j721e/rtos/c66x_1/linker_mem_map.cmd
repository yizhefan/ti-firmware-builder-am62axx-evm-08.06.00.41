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
    /* L2 for C66x_1 [ size 224.00 KB ] */
    L2RAM_C66x_1             ( RWIX ) : ORIGIN = 0x00800000 , LENGTH = 0x00038000
    /* DDR for C66x_1 for Linux resource table [ size 1024 B ] */
    DDR_C66x_1_RESOURCE_TABLE ( RWIX ) : ORIGIN = 0xA8100000 , LENGTH = 0x00000400
    /* DDR for C66x_1 for boot section [ size 1024 B ] */
    DDR_C66x_1_BOOT          ( RWIX ) : ORIGIN = 0xA8200000 , LENGTH = 0x00000400
    /* DDR for C66x_1 for code/data [ size 14.00 MB ] */
    DDR_C66x_1               ( RWIX ) : ORIGIN = 0xA8200400 , LENGTH = 0x00DFFC00
    /* DDR for C66x_1 for Linux IPC [ size 1024.00 KB ] */
    DDR_C66x_1_IPC           ( RWIX ) : ORIGIN = 0xA9000000 , LENGTH = 0x00100000
    /* Memory for IPC Vring's. MUST be non-cached or cache-coherent [ size 32.00 MB ] */
    IPC_VRING_MEM                     : ORIGIN = 0xAA000000 , LENGTH = 0x02000000
    /* Memory for remote core logging [ size 256.00 KB ] */
    APP_LOG_MEM                       : ORIGIN = 0xAC000000 , LENGTH = 0x00040000
    /* Memory for TI OpenVX shared memory. MUST be non-cached or cache-coherent [ size 63.75 MB ] */
    TIOVX_OBJ_DESC_MEM                : ORIGIN = 0xAC040000 , LENGTH = 0x03FC0000
    /* Memory for shared memory buffers in DDR [ size 512.00 MB ] */
    DDR_SHARED_MEM                    : ORIGIN = 0xB8000000 , LENGTH = 0x20000000
    /* DDR for c66x_1 for local heap [ size 16.00 MB ] */
    DDR_C66X_1_LOCAL_HEAP    ( RWIX ) : ORIGIN = 0xDC000000 , LENGTH = 0x01000000
    /* DDR for c66x_1 for Scratch Memory [ size 48.00 MB ] */
    DDR_C66X_1_SCRATCH       ( RWIX ) : ORIGIN = 0xDD000000 , LENGTH = 0x03000000
}
