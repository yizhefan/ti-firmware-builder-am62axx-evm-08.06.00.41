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
#include <TI/tivx_srv.h>

VX_API_ENTRY vx_node VX_API_CALL tivxPointDetectNode(vx_graph graph,
                                      vx_user_data_object  in_configuration,
                                      vx_user_data_object  in_ldclut,
                                      vx_image             in,
                                      vx_user_data_object  out_configuration,
                                      vx_image             buf_bwluma_frame)
{
    vx_reference prms[] = {
            (vx_reference)in_configuration,
            (vx_reference)in_ldclut,
            (vx_reference)in,
            (vx_reference)out_configuration,
            (vx_reference)buf_bwluma_frame
    };
    vx_node node = tivxCreateNodeByKernelName(graph,
                                           TIVX_KERNEL_POINT_DETECT_NAME,
                                           prms,
                                           dimof(prms));
    return node;
}



VX_API_ENTRY vx_node VX_API_CALL tivxPoseEstimationNode(vx_graph graph,
                                      vx_user_data_object  in_configuration,
                                      vx_user_data_object  in_ldclut,
                                      vx_object_array      in_corner_points,
                                      vx_user_data_object  out_calmat)
{
    vx_reference prms[] = {
            (vx_reference)in_configuration,
            (vx_reference)in_ldclut,
            (vx_reference)in_corner_points,
            (vx_reference)out_calmat
    };
    vx_node node = tivxCreateNodeByKernelName(graph,
                                           TIVX_KERNEL_POSE_ESTIMATION_NAME,
                                           prms,
                                           dimof(prms));
    return node;
}

VX_API_ENTRY vx_node VX_API_CALL tivxGenerate3DbowlNode(vx_graph graph,
                                      vx_user_data_object  in_configuration,
                                      vx_user_data_object  in_calmat,
                                      vx_user_data_object  in_offset,
                                      vx_array             out_lut3dxyz,
                                      vx_user_data_object  out_calmat_scaled)
{
    vx_reference prms[] = {
            (vx_reference)in_configuration,
            (vx_reference)in_calmat,
            (vx_reference)in_offset,
            (vx_reference)out_lut3dxyz,
            (vx_reference)out_calmat_scaled
    };
    vx_node node = tivxCreateNodeByKernelName(graph,
                                           TIVX_KERNEL_GENERATE_3DBOWL_NAME,
                                           prms,
                                           dimof(prms));
    return node;
}

VX_API_ENTRY vx_node VX_API_CALL tivxGenerateGpulutNode(vx_graph graph,
                                      vx_user_data_object  in_configuration,
                                      vx_user_data_object  in_ldclut,
                                      vx_user_data_object  in_calmat_scaled,
                                      vx_array             in_lut3dxyz,
                                      vx_array             out_gpulut3d)
{
    vx_reference prms[] = {
            (vx_reference)in_configuration,
            (vx_reference)in_ldclut,
            (vx_reference)in_calmat_scaled,
            (vx_reference)in_lut3dxyz,
            (vx_reference)out_gpulut3d
    };
    vx_node node = tivxCreateNodeByKernelName(graph,
                                           TIVX_KERNEL_GENERATE_GPULUT_NAME,
                                           prms,
                                           dimof(prms));
    return node;
}

#if defined(LINUX) || defined(x86_64) || defined(QNX)
VX_API_ENTRY vx_node VX_API_CALL tivxGlSrvNode(vx_graph graph,
                                      vx_user_data_object  configuration,
                                      vx_object_array      input,
                                      vx_object_array      srv_views,
                                      vx_array             galign_lut,
                                      vx_image             output)
{
    vx_reference prms[] = {
            (vx_reference)configuration,
            (vx_reference)input,
            (vx_reference)srv_views,
            (vx_reference)galign_lut,
            (vx_reference)output
    };
    vx_node node = tivxCreateNodeByKernelName(graph,
                                           TIVX_KERNEL_GL_SRV_NAME,
                                           prms,
                                           dimof(prms));
    return node;
}
#endif

