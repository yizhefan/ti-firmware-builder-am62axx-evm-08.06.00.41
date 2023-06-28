/*
 *
 * Copyright (c) 2022 Texas Instruments Incorporated
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

#include <edgeai_tiovx_img_proc.h>
#include <tivx_dl_color_blend_armv8_host.h>
#include <tivx_dl_draw_box_armv8_host.h>

VX_API_ENTRY vx_node VX_API_CALL tivxDLPreProcArmv8Node(vx_graph             graph,
                                                        vx_user_data_object  config,
                                                        vx_image             input_image,
                                                        vx_tensor            output_tensor)
{
    vx_reference prms[] = {
            (vx_reference)config,
            (vx_reference)input_image,
            (vx_reference)output_tensor
    };

    vx_node node = tivxCreateNodeByKernelName(graph,
                                                TIVX_KERNEL_DL_PRE_PROC_ARMV8_NAME,
                                                prms,
                                                dimof(prms));

    return(node);
}

VX_API_ENTRY vx_node VX_API_CALL tivxDLColorBlendArmv8Node(vx_graph             graph,
                                                           vx_user_data_object  config,
                                                           vx_image             input_image,
                                                           vx_tensor            input_tensor,
                                                           vx_image             output_image)
{
    vx_reference prms[] = {
            (vx_reference)config,
            (vx_reference)input_image,
            (vx_reference)input_tensor,
            (vx_reference)output_image
    };

    vx_node node = tivxCreateNodeByKernelName(graph,
                                             TIVX_KERNEL_DL_COLOR_BLEND_ARMV8_NAME,
                                             prms,
                                             dimof(prms));

    return(node);
}

VX_API_ENTRY vx_node VX_API_CALL tivxDLDrawBoxArmv8Node(vx_graph             graph,
                                                        vx_user_data_object  config,
                                                        vx_image             input_image,
                                                        vx_tensor            input_tensor,
                                                        vx_image             output_image)
{
    vx_reference prms[] = {
            (vx_reference)config,
            (vx_reference)input_image,
            (vx_reference)input_tensor,
            (vx_reference)output_image
    };

    vx_node node = tivxCreateNodeByKernelName(graph,
                                             TIVX_KERNEL_DL_DRAW_BOX_ARMV8_NAME,
                                             prms,
                                             dimof(prms));

    return(node);
}

VX_API_ENTRY vx_node VX_API_CALL tivxDLColorConvertArmv8Node(vx_graph  graph,
                                                             vx_image  input_image,
                                                             vx_image  output_image)
{
    vx_reference prms[] = {
            (vx_reference)input_image,
            (vx_reference)output_image
    };

    vx_node node = tivxCreateNodeByKernelName(graph,
                                             TIVX_KERNEL_DL_COLOR_CONVERT_ARMV8_NAME,
                                             prms,
                                             dimof(prms));

    return(node);
}