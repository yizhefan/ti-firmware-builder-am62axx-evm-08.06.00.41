/*
 *
 * Copyright (c) 2017 Texas Instruments Incorporated
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

#ifndef _TIVX_KERNEL_EGO_LOCALIZATION_
#define _TIVX_KERNEL_EGO_LOCALIZATION_

#ifdef __cplusplus
extern "C" {
#endif

#define TIVX_KERNEL_VISUAL_LOCALIZATION_CONFIGURATION_IDX (0U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_VOXEL_INFO_IDX (1U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_INPUT_START_IDX (TIVX_KERNEL_VISUAL_LOCALIZATION_VOXEL_INFO_IDX)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_MAP_PTS_IDX (2U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_MAP_DESC_IDX (3U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_FEAT_PT_IDX (4U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_FEAT_DESC_IDX (5U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_FILTER_IDX (6U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_R2R_TABLE_IDX (7U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_TIDL_OUT_ARGS_IDX (8U)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_MAX_INPUT (TIVX_KERNEL_VISUAL_LOCALIZATION_TIDL_OUT_ARGS_IDX - TIVX_KERNEL_VISUAL_LOCALIZATION_INPUT_START_IDX + 1)

#define TIVX_KERNEL_VISUAL_LOCALIZATION_OUTPUT_POSE_IDX (TIVX_KERNEL_VISUAL_LOCALIZATION_TIDL_OUT_ARGS_IDX +  1)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_OUTPUT_START_IDX (TIVX_KERNEL_VISUAL_LOCALIZATION_OUTPUT_POSE_IDX)
#define TIVX_KERNEL_VISUAL_LOCALIZATION_MAX_OUTPUT (TIVX_KERNEL_VISUAL_LOCALIZATION_OUTPUT_POSE_IDX - TIVX_KERNEL_VISUAL_LOCALIZATION_OUTPUT_START_IDX + 1)

#define TIVX_KERNEL_VISUAL_LOCALIZATION_MAX_PARAMS (TIVX_KERNEL_VISUAL_LOCALIZATION_OUTPUT_POSE_IDX + 1)

void tivxRegisterPixelVizKernels();
void tivxUnRegisterPixelVizKernels();

#ifdef __cplusplus
}
#endif

#endif /* _TIVX_KERNEL_VISUAL_LOCALIZATION_ */


