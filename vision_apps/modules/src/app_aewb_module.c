/*
 *
 * Copyright (c) 2020 Texas Instruments Incorporated
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

#include "app_aewb_module.h"

static vx_status configure_dcc(vx_context context, AEWBObj *aewbObj, SensorObj *sensorObj)
{
    vx_status status = VX_SUCCESS;

    if(sensorObj->sensor_dcc_enabled)
    {
        int32_t dcc_buff_size;
        uint8_t * dcc_buf;
        vx_map_id dcc_buf_map_id;

        dcc_buff_size = appIssGetDCCSize2A(sensorObj->sensor_name, sensorObj->sensor_wdr_enabled);
        if(dcc_buff_size < 0)
        {
            printf("[AEWB-MODULE] Invalid DCC size for 2A! \n");
            return VX_FAILURE;
        }
        aewbObj->dcc_config = vxCreateUserDataObject(context, "dcc_2a", dcc_buff_size, NULL);
        status = vxGetStatus((vx_reference)aewbObj->dcc_config);

        if(status == VX_SUCCESS)
        {
            vxSetReferenceName((vx_reference)aewbObj->dcc_config, "aewb_node_dcc_config");

            vxMapUserDataObject(
                aewbObj->dcc_config,
                0,
                dcc_buff_size,
                &dcc_buf_map_id,
                (void **)&dcc_buf,
                VX_WRITE_ONLY,
                VX_MEMORY_TYPE_HOST,
                0
            );
            status = appIssGetDCCBuff2A(sensorObj->sensor_name, sensorObj->sensor_wdr_enabled,  dcc_buf, dcc_buff_size);
            if(status != VX_SUCCESS)
            {
                printf("[AEWB-MODULE] Error getting 2A DCC buffer! \n");
                return VX_FAILURE;
            }
            vxUnmapUserDataObject(aewbObj->dcc_config, dcc_buf_map_id);
        }
        else
        {
            printf("[AEWB-MODULE] Unable to create DCC config object!\n");
        }
    }
    else
    {
        aewbObj->dcc_config = NULL;
        printf("[AEWB-MODULE] Sensor DCC is disabled \n");
    }

    return status;
}

static vx_status configure_aewb(vx_context context, AEWBObj *aewbObj, SensorObj *sensorObj, uint32_t starting_channel, uint32_t num_cameras_enabled)
{
    vx_status status = VX_SUCCESS;
    vx_int32 ch;
    vx_int32 ch_mask;

    aewbObj->params.sensor_dcc_id       = sensorObj->sensorParams.dccId;
    aewbObj->params.sensor_img_format   = 0;
    aewbObj->params.sensor_img_phase    = 3;

    if(sensorObj->sensor_exp_control_enabled || sensorObj->sensor_gain_control_enabled )
    {
        aewbObj->params.ae_mode = ALGORITHMS_ISS_AE_AUTO;
    }
    else
    {
        aewbObj->params.ae_mode = ALGORITHMS_ISS_AE_DISABLED;
    }
    aewbObj->params.awb_mode = ALGORITHMS_ISS_AWB_AUTO;

    aewbObj->params.awb_num_skip_frames = 9;
    aewbObj->params.ae_num_skip_frames  = 9;
    aewbObj->params.channel_id          = starting_channel;

    vx_user_data_object config = vxCreateUserDataObject(context, "tivx_aewb_config_t", sizeof(tivx_aewb_config_t), &aewbObj->params);
    status = vxGetStatus((vx_reference)config);
    if(status == VX_SUCCESS)
    {
        aewbObj->config_arr = vxCreateObjectArray(context, (vx_reference)config, num_cameras_enabled);
        status = vxGetStatus((vx_reference)aewbObj->config_arr);
        if(status != VX_SUCCESS)
        {
            printf("[AEWB_MODULE] Unable to create AEWB config object array! \n");
        }
        vxReleaseUserDataObject(&config);

        if(status == VX_SUCCESS)
        {
            vx_uint32 array_obj_index = 0;
            vxSetReferenceName((vx_reference)aewbObj->config_arr, "aewb_node_config_arr");

            ch = 0;
            ch_mask = sensorObj->ch_mask >> starting_channel;
            while(ch_mask > 0)
            {
                if(ch_mask & 0x1)
                {
                    vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem(aewbObj->config_arr, array_obj_index);
                    array_obj_index++;
                    aewbObj->params.channel_id = ch + starting_channel;
                    vxCopyUserDataObject(config, 0, sizeof(tivx_aewb_config_t), &aewbObj->params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
                    vxReleaseUserDataObject(&config);
                }
                ch++;
                ch_mask = ch_mask >> 1;
                if (ch >= num_cameras_enabled)
                {
                    break;
                }
            }
        }
    }
    else
    {
        printf("[AEWB_MODULE] Unable to create AEWB config object! \n");
    }

    return status;
}

static vx_status create_histogram(vx_context context, AEWBObj *aewbObj, SensorObj *sensorObj, uint32_t num_cameras_enabled)
{
    vx_status status = VX_SUCCESS;

    vx_distribution histogram = vxCreateDistribution(context, 256, 0, 256);
    status = vxGetStatus((vx_reference)histogram);
    if(status == VX_SUCCESS)
    {
        aewbObj->histogram_arr = vxCreateObjectArray(context, (vx_reference)histogram, num_cameras_enabled);
        status = vxGetStatus((vx_reference)aewbObj->histogram_arr);
        if(status != VX_SUCCESS)
        {
            printf("[AEWB_MODULE] Unable to create Histogram object array! \n");
        }
        else
        {
            vxSetReferenceName((vx_reference)aewbObj->histogram_arr, "aewb_node_histogram_arr");
        }
        vxReleaseDistribution(&histogram);
    }
    else
    {
        printf("[AEWB_MODULE] Unable to create Histogram object! \n");
    }

    return status;
}

static vx_status create_aewb_output(vx_context context, AEWBObj *aewbObj, SensorObj *sensorObj, uint32_t num_cameras_enabled)
{
    vx_status status = VX_SUCCESS;

    vx_user_data_object aewb_output =  vxCreateUserDataObject(context, "tivx_ae_awb_params_t", sizeof(tivx_ae_awb_params_t), NULL);
    status = vxGetStatus((vx_reference)aewb_output);
    if(status == VX_SUCCESS)
    {
        aewbObj->aewb_output_arr = vxCreateObjectArray(context, (vx_reference)aewb_output, num_cameras_enabled);
        status = vxGetStatus((vx_reference)aewbObj->aewb_output_arr);
        if(status != VX_SUCCESS)
        {
            printf("[AEWB_MODULE] Unable to create AEWB output object array! \n");
        }
        else
        {
            vxSetReferenceName((vx_reference)aewbObj->aewb_output_arr, "aewb_node_aewb_output_arr");
        }
        vxReleaseUserDataObject(&aewb_output);
    }
    else
    {
        printf("[AEWB_MODULE] Unable to create AEWB output object! \n");
    }

    return status;
}

vx_status app_init_aewb(vx_context context, AEWBObj *aewbObj, SensorObj *sensorObj, char *objName, uint32_t starting_channel, uint32_t num_cameras_enabled)
{
    vx_status status = VX_SUCCESS;

    status = configure_dcc(context, aewbObj, sensorObj);

    if(status == VX_SUCCESS)
    {
        status = configure_aewb(context, aewbObj, sensorObj, starting_channel, num_cameras_enabled);
    }

    if(status == VX_SUCCESS)
    {
        status = create_histogram(context, aewbObj, sensorObj, num_cameras_enabled);
    }

    if(status == VX_SUCCESS)
    {
        status = create_aewb_output(context, aewbObj, sensorObj, num_cameras_enabled);
    }

    return (status);
}

void app_deinit_aewb(AEWBObj *aewbObj)
{
    vxReleaseObjectArray(&aewbObj->histogram_arr);
    vxReleaseObjectArray(&aewbObj->config_arr);
    vxReleaseObjectArray(&aewbObj->aewb_output_arr);

    if(aewbObj->dcc_config != NULL)
    {
        vxReleaseUserDataObject(&aewbObj->dcc_config);
    }
}

void app_delete_aewb(AEWBObj *aewbObj)
{
    if(aewbObj->node != NULL)
    {
        vxReleaseNode(&aewbObj->node);
    }
}

vx_status app_create_graph_aewb(vx_graph graph, AEWBObj *aewbObj, vx_object_array h3a_stats_arr)
{
    vx_status status = VX_SUCCESS;

    vx_user_data_object aewb_output = (vx_user_data_object)vxGetObjectArrayItem(aewbObj->aewb_output_arr, 0);
    vx_user_data_object h3a_stats   = (vx_user_data_object)vxGetObjectArrayItem(h3a_stats_arr, 0);

    vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem(aewbObj->config_arr, 0);
    vx_distribution histogram  = (vx_distribution)vxGetObjectArrayItem(aewbObj->histogram_arr, 0);

    aewbObj->node = tivxAewbNode(graph,
                                 config,
                                 histogram,
                                 h3a_stats,
                                 NULL,
                                 aewb_output,
                                 aewbObj->dcc_config);

    vxReleaseUserDataObject(&aewb_output);
    vxReleaseUserDataObject(&h3a_stats);
    vxReleaseUserDataObject(&config);
    vxReleaseDistribution(&histogram);

    status = vxGetStatus((vx_reference)aewbObj->node);
    if(status != VX_SUCCESS)
    {
        printf("[AEWB_MODULE] Unable to create AEWB node! \n");
        return status;
    }

    vxSetReferenceName((vx_reference)aewbObj->node, "aewb_node");
    vxSetNodeTarget(aewbObj->node, VX_TARGET_STRING, TIVX_TARGET_MCU2_0);

    vx_bool replicate[] = { vx_true_e, vx_true_e, vx_true_e, vx_false_e, vx_true_e, vx_false_e};
    vxReplicateNode(graph, aewbObj->node, replicate, 6);


    return status;
}
