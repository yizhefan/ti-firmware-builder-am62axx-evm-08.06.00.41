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

#include <TI/tivx.h>
#include <TI/tivx_img_proc.h>
#include "tivx_img_proc_kernels_priv.h"
#include "tivx_kernels_host_utils.h"

static vx_status VX_CALLBACK publishKernels(vx_context context);
static vx_status VX_CALLBACK unPublishKernels(vx_context context);

static uint32_t gIsImgProcKernelsLoad = 0u;

vx_status tivxAddKernelODPostProc(vx_context context);
vx_status tivxAddKernelImgPreProc(vx_context context);
vx_status tivxAddKernelOCPreProc(vx_context context);
vx_status tivxAddKernelOCPostProc(vx_context context);
vx_status tivxAddKernelDofPlaneSep(vx_context context);
vx_status tivxAddKernelPoseViz(vx_context context);
vx_status tivxAddKernelVisualLocalization(vx_context context);
vx_status tivxAddKernelDrawKeypointDetections(vx_context context);
vx_status tivxAddKernelDrawBoxDetections(vx_context context);
vx_status tivxAddKernelImgHist(vx_context context);
vx_status tivxAddKernelSFM(vx_context context);
vx_status tivxAddKernelDLPreProc(vx_context context);
vx_status tivxAddKernelDLColorBlend(vx_context context);
vx_status tivxAddKernelDLDrawBox(vx_context context);
vx_status tivxAddKernelDLColorConvert(vx_context context);

vx_status tivxRemoveKernelODPostProc(vx_context context);
vx_status tivxRemoveKernelImgPreProc(vx_context context);
vx_status tivxRemoveKernelOCPreProc(vx_context context);
vx_status tivxRemoveKernelOCPostProc(vx_context context);
vx_status tivxRemoveKernelDofPlaneSep(vx_context context);
vx_status tivxRemoveKernelPoseViz(vx_context context);
vx_status tivxRemoveKernelVisualLocalization(vx_context context);
vx_status tivxRemoveKernelDrawKeypointDetections(vx_context context);
vx_status tivxRemoveKernelDrawBoxDetections(vx_context context);
vx_status tivxRemoveKernelImgHist(vx_context context);
vx_status tivxRemoveKernelSFM(vx_context context);
vx_status tivxRemoveKernelDLPreProc(vx_context context);
vx_status tivxRemoveKernelDLColorBlend(vx_context context);
vx_status tivxRemoveKernelDLDrawBox(vx_context context);
vx_status tivxRemoveKernelDLColorConvert(vx_context context);

static Tivx_Host_Kernel_List  gTivx_host_kernel_list[] = {
    {&tivxAddKernelImgPreProc, &tivxRemoveKernelImgPreProc},
    {&tivxAddKernelOCPreProc, &tivxRemoveKernelOCPreProc},
    {&tivxAddKernelOCPostProc, &tivxRemoveKernelOCPostProc},
    {&tivxAddKernelODPostProc, &tivxRemoveKernelODPostProc},
    {&tivxAddKernelDofPlaneSep, &tivxRemoveKernelDofPlaneSep},
    {&tivxAddKernelPoseViz, &tivxRemoveKernelPoseViz},
    {&tivxAddKernelVisualLocalization, &tivxRemoveKernelVisualLocalization},
    {&tivxAddKernelDrawKeypointDetections, &tivxRemoveKernelDrawKeypointDetections},
    {&tivxAddKernelDrawBoxDetections, &tivxRemoveKernelDrawBoxDetections},
    {&tivxAddKernelImgHist, &tivxRemoveKernelImgHist},
    {&tivxAddKernelSFM, &tivxRemoveKernelSFM},
    {&tivxAddKernelDLPreProc, &tivxRemoveKernelDLPreProc},
    {&tivxAddKernelDLColorBlend, &tivxRemoveKernelDLColorBlend},
    {&tivxAddKernelDLDrawBox, &tivxRemoveKernelDLDrawBox},
    {&tivxAddKernelDLColorConvert, &tivxRemoveKernelDLColorConvert}
};

static vx_status VX_CALLBACK publishKernels(vx_context context)
{
    return tivxPublishKernels(context, gTivx_host_kernel_list, dimof(gTivx_host_kernel_list));
}

static vx_status VX_CALLBACK unPublishKernels(vx_context context)
{
    return tivxUnPublishKernels(context, gTivx_host_kernel_list, dimof(gTivx_host_kernel_list));
}

void tivxRegisterImgProcKernels(void)
{
    tivxRegisterModule(TIVX_MODULE_NAME_IMG_PROC, publishKernels, unPublishKernels);
}

void tivxUnRegisterImgProcKernels(void)
{
    tivxUnRegisterModule(TIVX_MODULE_NAME_IMG_PROC);
}

void tivxImgProcLoadKernels(vx_context context)
{
    if ((0 == gIsImgProcKernelsLoad) && (NULL != context))
    {
        void tivxSetSelfCpuId(vx_enum cpu_id);

        tivxRegisterImgProcKernels();
        vxLoadKernels(context, TIVX_MODULE_NAME_IMG_PROC);

        #if defined(LINUX) || defined(x86_64) || defined(QNX)
        #ifdef x86_64
        /* These three lines only work on PC emulation mode ...
         * this will need to be updated when moving to target */

        tivxSetSelfCpuId(TIVX_CPU_ID_DSP1);
        tivxRegisterImgProcTargetC66Kernels();

        #if defined (SOC_J721E)
        tivxSetSelfCpuId(TIVX_CPU_ID_DSP2);
        tivxRegisterImgProcTargetC66Kernels();
        #endif

        tivxSetSelfCpuId(TIVX_CPU_ID_DSP_C7_1);
        tivxRegisterImgProcTargetC71Kernels();

        #if defined(SOC_AM62A)
        tivxSetSelfCpuId(TIVX_CPU_ID_MCU1_0);
        #else
        tivxSetSelfCpuId(TIVX_CPU_ID_MCU2_0);
        #endif
        tivxRegisterImgProcTargetR5FKernels();

        tivxSetSelfCpuId(TIVX_CPU_ID_A72_0);
        #endif
        tivxRegisterImgProcTargetA72Kernels();
        #endif
        gIsImgProcKernelsLoad = 1U;
    }
}

void tivxImgProcUnLoadKernels(vx_context context)
{
    if ((1u == gIsImgProcKernelsLoad) && (NULL != context))
    {
        vxUnloadKernels(context, TIVX_MODULE_NAME_IMG_PROC);
        tivxUnRegisterImgProcKernels();

        #if defined(LINUX) || defined(x86_64) || defined(QNX)
        #ifdef x86_64
        /* These three lines only work on PC emulation mode ...
         * this will need to be updated when moving to target */
        tivxUnRegisterImgProcTargetC66Kernels();
        tivxUnRegisterImgProcTargetC71Kernels();
        #endif
        tivxUnRegisterImgProcTargetA72Kernels();
        #endif

        gIsImgProcKernelsLoad = 0U;
    }
}
