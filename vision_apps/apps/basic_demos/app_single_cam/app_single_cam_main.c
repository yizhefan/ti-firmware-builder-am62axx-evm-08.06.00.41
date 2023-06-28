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

#include "app_single_cam_main.h"
#include <utils/iss/include/app_iss.h>
#include "app_test.h"

#ifdef A72
#if defined(LINUX)
/*ITT server is supported only in target mode and only on Linux*/
#include <itt_server.h>
#endif
#endif

static char availableSensorNames[ISS_SENSORS_MAX_SUPPORTED_SENSOR][ISS_SENSORS_MAX_NAME];
static vx_uint8 num_sensors_found;
static IssSensor_CreateParams sensorParams;

#define NUM_CAPT_CHANNELS   (4u)

#ifdef _APP_DEBUG_
static char *app_get_test_file_path()
{
    char *tivxPlatformGetEnv(char *env_var);

    #if defined(SYSBIOS)
    return tivxPlatformGetEnv("VX_TEST_DATA_PATH");
    #else
    return getenv("VX_TEST_DATA_PATH");
    #endif
}
#endif //_APP_DEBUG_

/*
 * Utility API used to add a graph parameter from a node, node parameter index
 */
void add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index)
{
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);

    vxAddParameterToGraph(graph, parameter);
    vxReleaseParameter(&parameter);
}

vx_status app_init(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    char* sensor_list[ISS_SENSORS_MAX_SUPPORTED_SENSOR];
    vx_uint8 count = 0;
    char ch = 0xFF;
    vx_uint8 selectedSensor = 0xFF;
    vx_uint8 detectedSensors[ISS_SENSORS_MAX_CHANNEL];
#ifdef A72
#if defined(LINUX)
/*ITT server is supported only in target mode and only on Linux*/
    status = itt_server_init((void*)obj, (void*)save_debug_images, (void*)appSingleCamUpdateVpacDcc);
    if(status != 0)
    {
        printf("Warning : Failed to initialize ITT server. Live tuning will not work \n");
    }
#endif
#endif

    for(count=0;count<ISS_SENSORS_MAX_CHANNEL;count++)
    {
        detectedSensors[count] = 0xFF;
    }

    for(count=0;count<ISS_SENSORS_MAX_SUPPORTED_SENSOR;count++)
    {
        sensor_list[count] = NULL;
    }

    obj->stop_task = 0;
    obj->stop_task_done = 0;
    obj->selectedCam = 0xFF;

    if(status == VX_SUCCESS)
    {
        obj->context = vxCreateContext();
        status = vxGetStatus((vx_reference) obj->context);
    }

    if(status == VX_SUCCESS)
    {
        tivxHwaLoadKernels(obj->context);
        tivxImagingLoadKernels(obj->context);
        APP_PRINTF("tivxImagingLoadKernels done\n");
    }

    /*memset(availableSensorNames, 0, ISS_SENSORS_MAX_SUPPORTED_SENSOR*ISS_SENSORS_MAX_NAME);*/
    for(count=0;count<ISS_SENSORS_MAX_SUPPORTED_SENSOR;count++)
    {
        availableSensorNames[count][0] = '\0';
        sensor_list[count] = availableSensorNames[count];
    }

    if(status == VX_SUCCESS)
    {
        status = appEnumerateImageSensor(sensor_list, &num_sensors_found);
    }

    if(obj->is_interactive)
    {
        selectedSensor = 0xFF;
        obj->selectedCam = 0xFF;
        while(obj->selectedCam == 0xFF)
        {
            printf("Select camera port index 0-%d : ", ISS_SENSORS_MAX_CHANNEL-1);
            ch = getchar();
            obj->selectedCam = ch - '0';

            if(obj->selectedCam >= ISS_SENSORS_MAX_CHANNEL)
            {
                printf("Invalid entry %c. Please choose between 0 and %d \n", ch, ISS_SENSORS_MAX_CHANNEL-1);
                obj->selectedCam = 0xFF;
            }

            while ((obj->selectedCam != 0xFF) && (selectedSensor > (num_sensors_found-1)))
            {
                printf("%d registered sensor drivers\n", num_sensors_found);
                for(count=0;count<num_sensors_found;count++)
                {
                    printf("%c : %s \n", count+'a', sensor_list[count]);
                }

                printf("Select a sensor above or press '0' to autodetect the sensor : ");
                ch = getchar();
                if(ch == '0')
                {
                    uint8_t num_sensors_detected = 0;
                    uint8_t channel_mask = (1<<obj->selectedCam);

                    status = appDetectImageSensor(detectedSensors, &num_sensors_detected, channel_mask);
                    if(0 == status)
                    {
                        selectedSensor = detectedSensors[obj->selectedCam];
                        if(selectedSensor > ISS_SENSORS_MAX_SUPPORTED_SENSOR)
                        {
                            printf("No sensor detected at port %d. Please select another port \n", obj->selectedCam);
                            obj->selectedCam = 0xFF;
                            selectedSensor = 0xFF;
                        }
                    }
                    else
                    {
                        printf("sensor detection at port %d returned error . Please try again \n", obj->selectedCam);
                        obj->selectedCam = 0xFF;
                        selectedSensor = 0xFF;
                    }
                }
                else
                {
                    selectedSensor = ch - 'a';
                    if(selectedSensor > (num_sensors_found-1))
                    {
                        printf("Invalid selection %c. Try again \n", ch);
                    }
                }
            }
        }

        obj->sensor_name = sensor_list[selectedSensor];
        printf("Sensor selected : %s\n", obj->sensor_name);

        ch = 0xFF;
        fflush (stdin);
        while ((ch != '0') && (ch != '1'))
        {
            fflush (stdin);
            printf ("LDC Selection Yes(1)/No(0) : ");
            ch = getchar();
        }

        obj->ldc_enable = ch - '0';
    }
    else
    {
        selectedSensor = obj->sensor_sel;
        if(selectedSensor > (num_sensors_found-1))
        {
            printf("Invalid sensor selection %d \n", selectedSensor);
            return VX_FAILURE;
        }
    }

    obj->sensor_wdr_mode = 0;

    obj->table_width = LDC_TABLE_WIDTH;
    obj->table_height = LDC_TABLE_HEIGHT;
    obj->ds_factor = LDC_DS_FACTOR;

    /* Display initialization */
    memset(&obj->display_params, 0, sizeof(tivx_display_params_t));
    obj->display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
    obj->display_params.pipeId = 2;
    obj->display_params.outHeight = 1080;
    obj->display_params.outWidth = 1920;
    obj->display_params.posX = 0;
    obj->display_params.posY = 0;

    obj->scaler_enable = vx_false_e;

    appPerfPointSetName(&obj->total_perf , "TOTAL");

    return status;
}

vx_status app_deinit(AppObj *obj)
{
    vx_status status = VX_FAILURE;
    tivxHwaUnLoadKernels(obj->context);
    APP_PRINTF("tivxHwaUnLoadKernels done\n");

    tivxImagingUnLoadKernels(obj->context);
    APP_PRINTF("tivxImagingUnLoadKernels done\n");

    status = vxReleaseContext(&obj->context);
    if(VX_SUCCESS == status)
    {
        APP_PRINTF("vxReleaseContext done\n");
    }
    else
    {
        printf("Error: vxReleaseContext returned 0x%x \n", status);
    }
    return status;
}

/*
 * Graph,
 *           viss_config
 *               |
 *               v
 * input_img -> VISS -----> LDC -----> output_img
 *
 */

vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    int32_t sensor_init_status = -1;
    obj->configuration = NULL;
    obj->raw = NULL;
    obj->y12 = NULL;
    obj->uv12_c1 = NULL;
    obj->y8_r8_c2 = NULL;
    obj->uv8_g8_c3 = NULL;
    obj->s8_b8_c4 = NULL;
    obj->histogram = NULL;
    obj->h3a_aew_af = NULL;
    obj->display_image = NULL;

    unsigned int image_width = obj->width_in;
    unsigned int image_height = obj->height_in;

    tivx_raw_image raw_image = 0;
    vx_user_data_object capture_config;
    vx_uint8 num_capture_frames = 1;
    tivx_capture_params_t local_capture_config;
    uint32_t buf_id;
    const vx_char capture_user_data_object_name[] = "tivx_capture_params_t";
    uint32_t sensor_features_enabled = 0;
    uint32_t sensor_features_supported = 0;
    uint32_t sensor_wdr_enabled = 0;
    uint32_t sensor_exp_control_enabled = 0;
    uint32_t sensor_gain_control_enabled = 0;

    vx_bool yuv_cam_input = vx_false_e;
    vx_image viss_out_image = NULL;
    vx_image ldc_in_image = NULL;
    vx_image capt_yuv_image = NULL;

    uint8_t channel_mask = (1<<obj->selectedCam);

    vx_uint32 params_list_depth = 1;
    if(obj->test_mode == 1)
    {
        params_list_depth = 2;
    }
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[params_list_depth];

    printf("Querying %s \n", obj->sensor_name);
    memset(&sensorParams, 0, sizeof(sensorParams));
    status = appQueryImageSensor(obj->sensor_name, &sensorParams);
    if(VX_SUCCESS != status)
    {
        printf("appQueryImageSensor returned %d\n", status);
        return status;
    }

    if(sensorParams.sensorInfo.raw_params.format[0].pixel_container == VX_DF_IMAGE_UYVY)
    {
        yuv_cam_input = vx_true_e;
        printf("YUV Input selected. VISS and AEWB nodes will be bypassed. \n");
    }


    /*
    Check for supported sensor features.
    It is upto the application to decide which features should be enabled.
    This demo app enables WDR, DCC and 2A if the sensor supports it.
    */
    sensor_features_supported = sensorParams.sensorInfo.features;

    if(vx_false_e == yuv_cam_input)
    {
        if(ISS_SENSOR_FEATURE_COMB_COMP_WDR_MODE == (sensor_features_supported & ISS_SENSOR_FEATURE_COMB_COMP_WDR_MODE))
        {
            APP_PRINTF("WDR mode is supported \n");
            sensor_features_enabled |= ISS_SENSOR_FEATURE_COMB_COMP_WDR_MODE;
            sensor_wdr_enabled = 1;
            obj->sensor_wdr_mode = 1;
        }else
        {
            APP_PRINTF("WDR mode is not supported. Defaulting to linear \n");
            sensor_features_enabled |= ISS_SENSOR_FEATURE_LINEAR_MODE;
            sensor_wdr_enabled = 0;
            obj->sensor_wdr_mode = 0;
        }

        if(ISS_SENSOR_FEATURE_MANUAL_EXPOSURE == (sensor_features_supported & ISS_SENSOR_FEATURE_MANUAL_EXPOSURE))
        {
            APP_PRINTF("Expsoure control is supported \n");
            sensor_features_enabled |= ISS_SENSOR_FEATURE_MANUAL_EXPOSURE;
            sensor_exp_control_enabled = 1;
        }

        if(ISS_SENSOR_FEATURE_MANUAL_GAIN == (sensor_features_supported & ISS_SENSOR_FEATURE_MANUAL_GAIN))
        {
            APP_PRINTF("Gain control is supported \n");
            sensor_features_enabled |= ISS_SENSOR_FEATURE_MANUAL_GAIN;
            sensor_gain_control_enabled = 1;
        }

        if(ISS_SENSOR_FEATURE_CFG_UC1 == (sensor_features_supported & ISS_SENSOR_FEATURE_CFG_UC1))
        {
            APP_PRINTF("CMS Usecase is supported \n");
            sensor_features_enabled |= ISS_SENSOR_FEATURE_CFG_UC1;
        }

        switch(sensorParams.sensorInfo.aewbMode)
        {
            case ALGORITHMS_ISS_AEWB_MODE_NONE:
                obj->aewb_cfg.ae_mode = ALGORITHMS_ISS_AE_DISABLED;
                obj->aewb_cfg.awb_mode = ALGORITHMS_ISS_AWB_DISABLED;
                break;
            case ALGORITHMS_ISS_AEWB_MODE_AWB:
                obj->aewb_cfg.ae_mode = ALGORITHMS_ISS_AE_DISABLED;
                obj->aewb_cfg.awb_mode = ALGORITHMS_ISS_AWB_AUTO;
                break;
            case ALGORITHMS_ISS_AEWB_MODE_AE:
                obj->aewb_cfg.ae_mode = ALGORITHMS_ISS_AE_AUTO;
                obj->aewb_cfg.awb_mode = ALGORITHMS_ISS_AWB_DISABLED;
                break;
            case ALGORITHMS_ISS_AEWB_MODE_AEWB:
                obj->aewb_cfg.ae_mode = ALGORITHMS_ISS_AE_AUTO;
                obj->aewb_cfg.awb_mode = ALGORITHMS_ISS_AWB_AUTO;
                break;
        }
        if(obj->aewb_cfg.ae_mode == ALGORITHMS_ISS_AE_DISABLED)
        {
            if(sensor_exp_control_enabled || sensor_gain_control_enabled )
            {
                obj->aewb_cfg.ae_mode = ALGORITHMS_ISS_AE_MANUAL;
            }
        }

        APP_PRINTF("obj->aewb_cfg.ae_mode = %d\n", obj->aewb_cfg.ae_mode);
        APP_PRINTF("obj->aewb_cfg.awb_mode = %d\n", obj->aewb_cfg.awb_mode);

    }

    if(ISS_SENSOR_FEATURE_DCC_SUPPORTED == (sensor_features_supported & ISS_SENSOR_FEATURE_DCC_SUPPORTED))
    {
        sensor_features_enabled |= ISS_SENSOR_FEATURE_DCC_SUPPORTED;
        APP_PRINTF("Sensor DCC is enabled \n");
    }else
    {
        APP_PRINTF("Sensor DCC is NOT enabled \n");
    }

    APP_PRINTF("Sensor width = %d\n", sensorParams.sensorInfo.raw_params.width);
    APP_PRINTF("Sensor height = %d\n", sensorParams.sensorInfo.raw_params.height);
    APP_PRINTF("Sensor DCC ID = %d\n", sensorParams.dccId);
    APP_PRINTF("Sensor Supported Features = 0x%x\n", sensor_features_supported);
    APP_PRINTF("Sensor Enabled Features = 0x%x\n", sensor_features_enabled);
    sensor_init_status = appInitImageSensor(obj->sensor_name, sensor_features_enabled, channel_mask);/*Mask = 1 for camera # 0*/
    if(0 != sensor_init_status)
    {
        /* Not returning failure because application may be waiting for
            error/test frame */
        printf("Error initializing sensor %s \n", obj->sensor_name);
    }

    image_width     = sensorParams.sensorInfo.raw_params.width;
    image_height    = sensorParams.sensorInfo.raw_params.height;
    obj->cam_dcc_id = sensorParams.dccId;
    obj->width_in = image_width;
    obj->height_in = image_height;

/*
Assuming same dataformat for all exposures.
This may not be true for staggered HDR. WIll be handled later
    for(count = 0;count<raw_params.num_exposures;count++)
    {
        memcpy(&(raw_params.format[count]), &(sensorProperties.sensorInfo.dataFormat), sizeof(tivx_raw_image_format_t));
    }
*/

/*
Sensor driver does not support metadata yet.
*/

    APP_PRINTF("Creating graph \n");

    obj->graph = vxCreateGraph(obj->context);
    if(status == VX_SUCCESS)
    {
        status = vxGetStatus((vx_reference) obj->graph);
    }
    APP_ASSERT(vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_VPAC_VISS1));

    APP_PRINTF("Initializing params for capture node \n");

    /* Setting to num buf of capture node */
    obj->num_cap_buf = NUM_BUFS;

    if(vx_false_e == yuv_cam_input)
    {
        raw_image = tivxCreateRawImage(obj->context, &sensorParams.sensorInfo.raw_params);

        /* allocate Input and Output refs, multiple refs created to allow pipelining of graph */
        for(buf_id=0; buf_id<obj->num_cap_buf; buf_id++)
        {
            if(status == VX_SUCCESS)
            {
                obj->cap_frames[buf_id] = vxCreateObjectArray(obj->context, (vx_reference)raw_image, num_capture_frames);
                status = vxGetStatus((vx_reference) obj->cap_frames[buf_id]);
            }
        }
    }
    else
    {
        capt_yuv_image = vxCreateImage(
                                obj->context,
                                sensorParams.sensorInfo.raw_params.width,
                                sensorParams.sensorInfo.raw_params.height,
                                VX_DF_IMAGE_UYVY
                         );

        /* allocate Input and Output refs, multiple refs created to allow pipelining of graph */
        for(buf_id=0; buf_id<obj->num_cap_buf; buf_id++)
        {
            if(status == VX_SUCCESS)
            {
                obj->cap_frames[buf_id] = vxCreateObjectArray(obj->context, (vx_reference)capt_yuv_image, num_capture_frames);
                status = vxGetStatus((vx_reference) obj->cap_frames[buf_id]);
            }
        }
    }

    /* Config initialization */
    tivx_capture_params_init(&local_capture_config);

    local_capture_config.timeout = 33;
    local_capture_config.timeoutInitial = 500;

    local_capture_config.numInst  = 2U;/* Configure both instances */
    local_capture_config.numCh = 1U;/* Single cam. Only 1 channel enabled */
    {
        vx_uint8 ch, id, lane, q;
        for(id = 0; id < local_capture_config.numInst; id++)
        {
            local_capture_config.instId[id]                       = id;
            local_capture_config.instCfg[id].enableCsiv2p0Support = (uint32_t)vx_true_e;
            local_capture_config.instCfg[id].numDataLanes         = sensorParams.sensorInfo.numDataLanes;
            local_capture_config.instCfg[id].laneBandSpeed        = sensorParams.sensorInfo.csi_laneBandSpeed;

            for (lane = 0; lane < local_capture_config.instCfg[id].numDataLanes; lane++)
            {
                local_capture_config.instCfg[id].dataLanesMap[lane] = lane + 1;
            }
            for (q = 0; q < NUM_CAPT_CHANNELS; q++)
            {
                ch = (NUM_CAPT_CHANNELS-1)* id + q;
                local_capture_config.chVcNum[ch]   = q;
                local_capture_config.chInstMap[ch] = id;
            }
        }
    }

    local_capture_config.chInstMap[0] = obj->selectedCam/NUM_CAPT_CHANNELS;
    local_capture_config.chVcNum[0]   = obj->selectedCam%NUM_CAPT_CHANNELS;

    capture_config = vxCreateUserDataObject(obj->context, capture_user_data_object_name, sizeof(tivx_capture_params_t), &local_capture_config);
    APP_PRINTF("capture_config = 0x%p \n", capture_config);

    APP_PRINTF("Creating capture node \n");
    obj->capture_node = tivxCaptureNode(obj->graph, capture_config, obj->cap_frames[0]);
    APP_PRINTF("obj->capture_node = 0x%p \n", obj->capture_node);

    if(status == VX_SUCCESS)
    {
        status = vxReleaseUserDataObject(&capture_config);
    }
    if(status == VX_SUCCESS)
    {
        status = vxSetNodeTarget(obj->capture_node, VX_TARGET_STRING, TIVX_TARGET_CAPTURE2);
    }

    if(vx_false_e == yuv_cam_input)
    {
        obj->raw = (tivx_raw_image)vxGetObjectArrayItem(obj->cap_frames[0], 0);
        if(status == VX_SUCCESS)
        {
            status = tivxReleaseRawImage(&raw_image);
        }
#ifdef _APP_DEBUG_


        obj->fs_test_raw_image = tivxCreateRawImage(obj->context, &(sensorParams.sensorInfo.raw_params));

        if (NULL != obj->fs_test_raw_image)
        {
            if(status == VX_SUCCESS)
            {
                status = read_test_image_raw(NULL, obj->fs_test_raw_image, obj->test_mode);
            }
            else
            {
                status = tivxReleaseRawImage(&obj->fs_test_raw_image);
                obj->fs_test_raw_image = NULL;
            }
        }
#endif //_APP_DEBUG_

        status = app_create_viss(obj, sensor_wdr_enabled);
        if(VX_SUCCESS == status)
        {
        vxSetNodeTarget(obj->node_viss, VX_TARGET_STRING, TIVX_TARGET_VPAC_VISS1);
        tivxSetNodeParameterNumBufByIndex(obj->node_viss, 6u, obj->num_cap_buf);
        }
        else
        {
            printf("app_create_viss failed \n");
            return -1;
        }

        status = app_create_aewb(obj, sensor_wdr_enabled);
        if(VX_SUCCESS != status)
        {
            printf("app_create_aewb failed \n");
            return -1;
        }
        viss_out_image = obj->y8_r8_c2;
        ldc_in_image = viss_out_image;
    }
    else
    {
        obj->capt_yuv_image = (vx_image)vxGetObjectArrayItem(obj->cap_frames[0], 0);
        ldc_in_image = obj->capt_yuv_image;
        vxReleaseImage(&capt_yuv_image);
    }

    if (obj->ldc_enable)
    {
        printf ("Enabling LDC \n");
        status = app_create_ldc(obj, ldc_in_image);

        if(status == VX_SUCCESS)
        {
            status = vxSetNodeTarget(obj->node_ldc, VX_TARGET_STRING, TIVX_TARGET_VPAC_LDC1);
        }
        else
        {
            printf("app_create_ldc returned error \n");
            return status;
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->node_ldc, 7u, obj->num_cap_buf);
        }

        /*Check if resizing is needed for display*/
        if((obj->table_width >= obj->display_params.outWidth) && (obj->table_height >= obj->display_params.outHeight))
        {
            vx_uint16 scaler_out_w, scaler_out_h;
            obj->scaler_enable = vx_true_e;
            appIssGetResizeParams(obj->table_width, obj->table_height, obj->display_params.outWidth, obj->display_params.outHeight, &scaler_out_w, &scaler_out_h);
            obj->scaler_out_img = vxCreateImage(obj->context, scaler_out_w, scaler_out_h, VX_DF_IMAGE_NV12);
            obj->scalerNode = tivxVpacMscScaleNode(obj->graph, obj->ldc_out, obj->scaler_out_img, NULL, NULL, NULL, NULL);
            if(status == VX_SUCCESS)
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->scalerNode, 1u, obj->num_cap_buf);
            }
            obj->display_params.outHeight = scaler_out_h;
            obj->display_params.outWidth = scaler_out_w;
            obj->display_image = obj->scaler_out_img;
        }else /*No resize needed*/
        {
            obj->scaler_enable = vx_false_e;
            obj->display_image = obj->ldc_out;

            /* MSC can only downsize. If ldc resolution is lower,
               display resolution must be set accordingly
            */
            obj->display_params.outWidth = obj->table_width;
            obj->display_params.outHeight = obj->table_height;
        }
    }
    else /*ldc_enable*/
    {
        if(NULL != obj->capt_yuv_image)
        {
            /*MSC does not support YUV422 input*/
            obj->scaler_enable = vx_false_e;
        }
        else
        {
            if ((image_width >= obj->display_params.outWidth) && (image_height >= obj->display_params.outHeight))
            {
                obj->scaler_enable = vx_true_e;
            }
            else
            {
                obj->scaler_enable = vx_false_e;
                /* MSC can only downsize. If viss resolution is lower,
                   display resolution must be set accordingly
                */
                obj->display_params.outWidth = image_width;
                obj->display_params.outHeight = image_height;
            }
        }

        if(vx_true_e == obj->scaler_enable)
        {
            vx_uint16 scaler_out_w, scaler_out_h;
            appIssGetResizeParams(image_width, image_height, obj->display_params.outWidth, obj->display_params.outHeight, &scaler_out_w, &scaler_out_h);
            obj->scaler_out_img = vxCreateImage(obj->context, scaler_out_w, scaler_out_h, VX_DF_IMAGE_NV12);
            obj->scalerNode = tivxVpacMscScaleNode(obj->graph, ldc_in_image, obj->scaler_out_img, NULL, NULL, NULL, NULL);
            if(status == VX_SUCCESS)
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->scalerNode, 1u, obj->num_cap_buf);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetNodeTarget(obj->scalerNode, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
            }
            obj->display_params.outHeight = scaler_out_h;
            obj->display_params.outWidth = scaler_out_w;
            obj->display_image = obj->scaler_out_img;
        }
        else
        {
            obj->display_image = ldc_in_image;
        }
    }

    if(NULL == obj->display_image)
    {
        printf("Error : Display input is uninitialized \n");
        return VX_FAILURE;
    }
    else
    {
        obj->display_params.posX = (1920U - obj->display_params.outWidth)/2;
        obj->display_params.posY = (1080U - obj->display_params.outHeight)/2;
        obj->display_param_obj = vxCreateUserDataObject(obj->context, "tivx_display_params_t", sizeof(tivx_display_params_t), &obj->display_params);
        obj->displayNode = tivxDisplayNode(obj->graph, obj->display_param_obj, obj->display_image);
    }

    if(status == VX_SUCCESS)
    {
        status = vxSetNodeTarget(obj->displayNode, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
        APP_PRINTF("Display Set Target done\n");
    }
    int graph_parameter_num = 0;

    /* input @ node index 1, becomes graph parameter 0 */
    add_graph_parameter_by_node_index(obj->graph, obj->capture_node, 1);

    /* set graph schedule config such that graph parameter @ index 0 is enqueuable */
    graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
    graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = obj->num_cap_buf;
    graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&(obj->cap_frames[0]);
    graph_parameter_num++;

    if(obj->test_mode == 1)
    {
        add_graph_parameter_by_node_index(obj->graph, obj->displayNode, 1);

        /* set graph schedule config such that graph parameter @ index 0 is enqueuable */
        graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = 1;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&(obj->display_image);
        graph_parameter_num++;
    }

    if(status == VX_SUCCESS)
    {
        status = tivxSetGraphPipelineDepth(obj->graph, obj->num_cap_buf);
    }

    /* Schedule mode auto is used, here we dont need to call vxScheduleGraph
     * Graph gets scheduled automatically as refs are enqueued to it
     */
    if(status == VX_SUCCESS)
    {
        status = vxSetGraphScheduleConfig(obj->graph,
                        VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                        params_list_depth,
                        graph_parameters_queue_params_list
                        );
    }
    APP_PRINTF("vxSetGraphScheduleConfig done\n");

    if(status == VX_SUCCESS)
    {
        status = vxVerifyGraph(obj->graph);
    }

    if(vx_true_e == obj->scaler_enable)
    {
        tivx_vpac_msc_coefficients_t sc_coeffs;
        vx_reference refs[1];

        printf("Scaler is enabled\n");

        tivx_vpac_msc_coefficients_params_init(&sc_coeffs, VX_INTERPOLATION_BILINEAR);

        obj->sc_coeff_obj = vxCreateUserDataObject(obj->context, "tivx_vpac_msc_coefficients_t", sizeof(tivx_vpac_msc_coefficients_t), NULL);
        if(status == VX_SUCCESS)
        {
            status = vxCopyUserDataObject(obj->sc_coeff_obj, 0, sizeof(tivx_vpac_msc_coefficients_t), &sc_coeffs, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        }
        refs[0] = (vx_reference)obj->sc_coeff_obj;
        if(status == VX_SUCCESS)
        {
            status = tivxNodeSendCommand(obj->scalerNode, 0u, TIVX_VPAC_MSC_CMD_SET_COEFF, refs, 1u);
        }
    }
    else
    {
        printf("Scaler is disabled\n");
    }

    if(status == VX_SUCCESS)
    {
        status = tivxExportGraphToDot(obj->graph, ".", "single_cam_graph");
    }

#ifdef _APP_DEBUG_
    if(vx_false_e == yuv_cam_input)
    {
        if( (NULL != obj->fs_test_raw_image) && (NULL != obj->capture_node) && (status == VX_SUCCESS))
        {
            status = app_send_test_frame(obj->capture_node, obj->fs_test_raw_image);
        }
    }
#endif //_APP_DEBUG_

    APP_PRINTF("app_create_graph exiting\n");
    return status;
}

vx_status app_delete_graph(AppObj *obj)
{
    uint32_t buf_id;
    vx_status status = VX_SUCCESS;

    if(NULL != obj->capture_node)
    {
        APP_PRINTF("releasing capture node\n");
        status |= vxReleaseNode(&obj->capture_node);
    }

    if(NULL != obj->node_viss)
    {
        APP_PRINTF("releasing node_viss\n");
        status |= vxReleaseNode(&obj->node_viss);
    }

    if(NULL != obj->node_aewb)
    {
        APP_PRINTF("releasing node_aewb\n");
        status |= vxReleaseNode(&obj->node_aewb);
    }

    if(NULL != obj->displayNode)
    {
        APP_PRINTF("releasing displayNode\n");
        status |= vxReleaseNode(&obj->displayNode);
    }

    status |= tivxReleaseRawImage(&obj->raw);
    APP_PRINTF("releasing raw image done\n");

    for(buf_id=0; buf_id<obj->num_cap_buf; buf_id++)
    {
       if(NULL != obj->cap_frames[buf_id])
       {
        APP_PRINTF("releasing cap_frame # %d\n", buf_id);
        status |= vxReleaseObjectArray(&(obj->cap_frames[buf_id]));
       }
    }

    for(buf_id=0; buf_id<obj->num_viss_out_buf; buf_id++)
    {
      if(NULL != obj->viss_out_luma[buf_id])
      {
        APP_PRINTF("releasing y8 buffer # %d\n", buf_id);
        status |= vxReleaseImage(&(obj->viss_out_luma[buf_id]));
      }
    }

    if(NULL != obj->capt_yuv_image)
    {
        APP_PRINTF("releasing capt_yuv_image\n");
        status |= vxReleaseImage(&obj->capt_yuv_image);
    }


    if(NULL != obj->y12)
    {
        APP_PRINTF("releasing y12\n");
        status |= vxReleaseImage(&obj->y12);
    }

    if(NULL != obj->uv12_c1)
    {
        APP_PRINTF("releasing uv12_c1\n");
        status |= vxReleaseImage(&obj->uv12_c1);
    }

    if(NULL != obj->s8_b8_c4)
    {
        APP_PRINTF("releasing s8_b8_c4\n");
        status |= vxReleaseImage(&obj->s8_b8_c4);
    }

    if(NULL != obj->y8_r8_c2)
    {
        APP_PRINTF("releasing y8_r8_c2\n");
        status |= vxReleaseImage(&obj->y8_r8_c2);
    }

    if(NULL != obj->uv8_g8_c3)
    {
        APP_PRINTF("releasing uv8_g8_c3\n");
        status |= vxReleaseImage(&obj->uv8_g8_c3);
    }

    if(NULL != obj->histogram)
    {
        APP_PRINTF("releasing histogram\n");
        status |= vxReleaseDistribution(&obj->histogram);
    }

    if(NULL != obj->configuration)
    {
        APP_PRINTF("releasing configuration\n");
        status |= vxReleaseUserDataObject(&obj->configuration);

    }

    if (NULL != obj->ae_awb_result)
    {
        status |= vxReleaseUserDataObject(&obj->ae_awb_result);
        APP_PRINTF("releasing ae_awb_result done\n");
    }

    if(NULL != obj->h3a_aew_af)
    {
        APP_PRINTF("releasing h3a_aew_af\n");
        status |= vxReleaseUserDataObject(&obj->h3a_aew_af);
    }

    if(NULL != obj->aewb_config)
    {
        APP_PRINTF("releasing aewb_config\n");
        status |= vxReleaseUserDataObject(&obj->aewb_config);
    }

    if(NULL != obj->dcc_param_viss)
    {
        APP_PRINTF("releasing VISS DCC Data Object\n");
        status |= vxReleaseUserDataObject(&obj->dcc_param_viss);
    }

    if(NULL != obj->display_param_obj)
    {
        APP_PRINTF("releasing Display Param Data Object\n");
        status |= vxReleaseUserDataObject(&obj->display_param_obj);
    }

    if(NULL != obj->dcc_param_2a)
    {
        APP_PRINTF("releasing 2A DCC Data Object\n");
        status |= vxReleaseUserDataObject(&obj->dcc_param_2a);
    }

    if(NULL != obj->dcc_param_ldc)
    {
        APP_PRINTF("releasing LDC DCC Data Object\n");
        status |= vxReleaseUserDataObject(&obj->dcc_param_ldc);
    }

    if (obj->ldc_enable)
    {
        if (NULL != obj->mesh_img)
        {
            APP_PRINTF("releasing LDC Mesh Image \n");
            status |= vxReleaseImage(&obj->mesh_img);
        }

        if (NULL != obj->ldc_out)
        {
            APP_PRINTF("releasing LDC Output Image \n");
            status |= vxReleaseImage(&obj->ldc_out);
        }

        if (NULL != obj->mesh_params_obj)
        {
            APP_PRINTF("releasing LDC Mesh Parameters Object\n");
            status |= vxReleaseUserDataObject(&obj->mesh_params_obj);
        }

        if (NULL != obj->ldc_param_obj)
        {
            APP_PRINTF("releasing LDC Parameters Object\n");
            status |= vxReleaseUserDataObject(&obj->ldc_param_obj);
        }

        if (NULL != obj->region_params_obj)
        {
            APP_PRINTF("releasing LDC Region Parameters Object\n");
            status |= vxReleaseUserDataObject(&obj->region_params_obj);
        }

        if(NULL != obj->node_ldc)
        {
            APP_PRINTF("releasing LDC Node \n");
            status |= vxReleaseNode(&obj->node_ldc);
        }
    }
    if(vx_true_e == obj->scaler_enable)
    {
        if (NULL != obj->scaler_out_img)
        {
            APP_PRINTF("releasing Scaler Output Image \n");
            status |= vxReleaseImage(&obj->scaler_out_img);
        }

        if(NULL != obj->scalerNode)
        {
            APP_PRINTF("releasing Scaler Node \n");
            status |= vxReleaseNode(&obj->scalerNode);
        }

        if (NULL != obj->sc_coeff_obj)
        {
            APP_PRINTF("release Scalar coefficient data object \n");
            status |= vxReleaseUserDataObject(&obj->sc_coeff_obj);
        }
    }

#ifdef _APP_DEBUG_
    if(NULL != obj->fs_test_raw_image)
    {
        APP_PRINTF("releasing test raw image buffer # %d\n", buf_id);
        status |= tivxReleaseRawImage(&obj->fs_test_raw_image);
    }
#endif

    APP_PRINTF("releasing graph\n");
    status |= vxReleaseGraph(&obj->graph);
    APP_PRINTF("releasing graph done\n");
    return status;
}

vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_uint32 i;
    vx_uint32 frm_loop_cnt;

    uint32_t buf_id;
    uint32_t num_refs_capture;
    vx_object_array out_capture_frames;
    int graph_parameter_num = 0;

    uint8_t channel_mask = (1<<obj->selectedCam);

    if(NULL == obj->sensor_name)
    {
        printf("sensor name is NULL \n");
        return VX_FAILURE;
    }
    status = appStartImageSensor(obj->sensor_name, channel_mask);
    if(status < 0)
    {
        printf("Failed to start sensor %s \n", obj->sensor_name);
        if (NULL != obj->fs_test_raw_image)
        {
            printf("Defaulting to file test mode \n");
            status = 0;
        }
    }

    graph_parameter_num = 0;
    for(buf_id=0; buf_id<obj->num_cap_buf; buf_id++)
    {
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, 0, (vx_reference*)&(obj->cap_frames[buf_id]), 1);
        }
        /* in order for the graph to finish execution, the
            display still needs to be enqueued 4 times for testing */
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, 1, (vx_reference*)&(obj->display_image), 1);
        }
    }

    /*
        The application reads and  processes the same image "frm_loop_cnt" times
        The output may change because on VISS, parameters are updated every frame based on AEWB results
        AEWB result is avaialble after 1 frame and is applied after 2 frames
        Therefore, first 2 output images will have wrong colors
    */
    frm_loop_cnt = obj->num_frames_to_run;
    frm_loop_cnt += obj->num_cap_buf;

    if(obj->is_interactive)
    {
        /* in interactive mode loop for ever */
        frm_loop_cnt  = 0xFFFFFFFF;
    }

#ifdef A72
#if defined(LINUX)

    appDccUpdatefromFS(obj->sensor_name, obj->sensor_wdr_mode,
                        obj->node_aewb, 0,
                        obj->node_viss, 0,
                        obj->node_ldc, 0,
                        obj->context);
#endif
#endif

    for(i=0; i<frm_loop_cnt; i++)
    {
        vx_image test_image;
        appPerfPointBegin(&obj->total_perf);
        graph_parameter_num = 0;
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, graph_parameter_num, (vx_reference*)&out_capture_frames, 1, &num_refs_capture);
        }
        graph_parameter_num++;
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, 1, (vx_reference*)&test_image, 1, &num_refs_capture);
        }
        if((obj->test_mode == 1) && (i > TEST_BUFFER) && (status == VX_SUCCESS))
        {
            vx_uint32 actual_checksum = 0;
            if(app_test_check_image(test_image, checksums_expected[obj->sensor_sel][0], &actual_checksum) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            populate_gatherer(obj->sensor_sel, 0, actual_checksum);
        }
        APP_PRINTF(" i %d...\n", i);
        graph_parameter_num = 0;
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, 1, (vx_reference*)&test_image, 1);
        }
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, graph_parameter_num, (vx_reference*)&out_capture_frames, 1);
        }
        graph_parameter_num++;

        appPerfPointEnd(&obj->total_perf);

        if((obj->stop_task) || (status != VX_SUCCESS))
        {
            break;
        }
    }

    if(status == VX_SUCCESS)
    {
        status = vxWaitGraph(obj->graph);
    }
/* Dequeue buf for pipe down */
#if 0
    for(buf_id=0; buf_id<obj->num_cap_buf-2; buf_id++)
    {
        APP_PRINTF(" Dequeuing capture # %d...\n", buf_id);
        graph_parameter_num = 0;
        vxGraphParameterDequeueDoneRef(obj->graph, graph_parameter_num, (vx_reference*)&out_capture_frames, 1, &num_refs_capture);
        graph_parameter_num++;
    }
#endif
    if(status == VX_SUCCESS)
    {
        status = appStopImageSensor(obj->sensor_name, channel_mask);
    }
    return status;
}

static void app_run_task(void *app_var)
{
    AppObj *obj = (AppObj *)app_var;

    appPerfStatsCpuLoadResetAll();

    app_run_graph(obj);

    obj->stop_task_done = 1;
}

static int32_t app_run_task_create(AppObj *obj)
{
    tivx_task_create_params_t params;
    int32_t status;

    tivxTaskSetDefaultCreateParams(&params);
    params.task_main = app_run_task;
    params.app_var = obj;

    obj->stop_task_done = 0;
    obj->stop_task = 0;

    status = tivxTaskCreate(&obj->task, &params);

    return status;
}

static void app_run_task_delete(AppObj *obj)
{
    while(obj->stop_task_done==0)
    {
         tivxTaskWaitMsecs(100);
    }

    tivxTaskDelete(&obj->task);
}

static char menu[] = {
    "\n"
    "\n =========================="
    "\n Demo : Single Camera w/ 2A"
    "\n =========================="
    "\n"
    "\n p: Print performance statistics"
    "\n"
#ifdef _APP_DEBUG_
    "\n s: Save Sensor RAW, VISS Output and H3A output images to File System"
    "\n"
#endif
    "\n e: Export performance statistics"
#ifdef A72
#if defined(LINUX)
    "\n"
    "\n u: Update DCC from File System"
    "\n"
    "\n"
#endif
#endif
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};

static vx_status app_run_graph_interactive(AppObj *obj)
{
    vx_status status;
    uint32_t done = 0;
    char ch;
    FILE *fp;
    app_perf_point_t *perf_arr[1];
    uint8_t channel_mask = (1<<obj->selectedCam);

    status = app_run_task_create(obj);
    if(status!=0)
    {
        printf("ERROR: Unable to create task\n");
    }
    else
    {
        appPerfStatsResetAll();
        while(!done && (status == VX_SUCCESS))
        {
            printf(menu);
            ch = getchar();
            printf("\n");

            switch(ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    status = tivx_utils_graph_perf_print(obj->graph);
                    appPerfPointPrint(&obj->total_perf);
                    printf("\n");
                    appPerfPointPrintFPS(&obj->total_perf);
                    appPerfPointReset(&obj->total_perf);
                    printf("\n");
                    break;
#ifdef _APP_DEBUG_
                case 's':
                    save_debug_images(obj);
                    break;
#endif
                case 'e':
                    perf_arr[0] = &obj->total_perf;
                    fp = appPerfStatsExportOpenFile(".", "basic_demos_app_single_cam");
                    if (NULL != fp)
                    {
                        appPerfStatsExportAll(fp, perf_arr, 1);
                        status = tivx_utils_graph_perf_export(fp, obj->graph);
                        appPerfStatsExportCloseFile(fp);
                        appPerfStatsResetAll();
                    }
                    else
                    {
                        printf("fp is null\n");
                    }
                    break;
#ifdef A72
#if defined(LINUX)
                case 'u':
                    appDccUpdatefromFS(obj->sensor_name, obj->sensor_wdr_mode,
                        obj->node_aewb, 0,
                        obj->node_viss, 0,
                        obj->node_ldc, 0,
                        obj->context);
                    break;
#endif
#endif

                case 'x':
                    obj->stop_task = 1;
                    done = 1;
                    break;

                default:
                    printf("Unsupported command %c\n", ch);
                    break;

            }
        }
        app_run_task_delete(obj);
    }
    if(status == VX_SUCCESS)
    {
        status = appStopImageSensor(obj->sensor_name, channel_mask);
    }
    return status;
}

static void app_show_usage(int argc, char* argv[])
{
    printf("\n");
    printf(" Single Camera Demo - (c) Texas Instruments 2019\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

#ifdef A72
#if defined(LINUX)
int appSingleCamUpdateVpacDcc(AppObj *obj, uint8_t* dcc_buf, uint32_t dcc_buf_size)
{
    int32_t status = 0;

    status = appUpdateVpacDcc(dcc_buf, dcc_buf_size, obj->context,
                obj->node_viss, 0,
                obj->node_aewb, 0,
                obj->node_ldc, 0
             );

    return status;
}
#endif
#endif

#ifdef _APP_DEBUG_
int save_debug_images(AppObj *obj)
{
    int num_bytes_io = 0;
    static int file_index = 0;
    char raw_image_fname[MAX_FNAME];
    char yuv_image_fname[MAX_FNAME];
    char h3a_image_fname[MAX_FNAME];
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = app_get_test_file_path();
    struct stat s;

    if(NULL == test_data_path)
    {
        printf("Test data path is NULL. Defaulting to current folder \n");
        test_data_path = failsafe_test_data_path;
    }

    if (stat(test_data_path, &s))
    {
        printf("Test data path %s does not exist. Defaulting to current folder \n", test_data_path);
        test_data_path = failsafe_test_data_path;
    }

    if(NULL == obj->capt_yuv_image)
    {
        snprintf(raw_image_fname, MAX_FNAME, "%s/%s_%04d.raw", test_data_path, "img", file_index);
        printf("RAW file name %s \n", raw_image_fname);
        num_bytes_io = write_output_image_raw(raw_image_fname, obj->raw);
        if(num_bytes_io < 0)
        {
            printf("Error writing to RAW file \n");
            return VX_FAILURE;
        }

        snprintf(yuv_image_fname, MAX_FNAME, "%s/%s_%04d.yuv", test_data_path, "img_viss", file_index);
        printf("YUV file name %s \n", yuv_image_fname);
        num_bytes_io = write_output_image_nv12_8bit(yuv_image_fname, obj->y8_r8_c2);
        if(num_bytes_io < 0)
        {
            printf("Error writing to VISS NV12 file \n");
            return VX_FAILURE;
        }

        snprintf(h3a_image_fname, MAX_FNAME, "%s/%s_%04d.bin", test_data_path, "h3a", file_index);
        printf("H3A file name %s \n", h3a_image_fname);
        num_bytes_io = write_h3a_image(h3a_image_fname, obj->h3a_aew_af);
        if(num_bytes_io < 0)
        {
            printf("Error writing to H3A file \n");
            return VX_FAILURE;
        }

    }
    else
    {
        vx_image cap_yuv;
        snprintf(raw_image_fname, MAX_FNAME, "%s/%s_%04d.yuv", test_data_path, "cap", file_index);
        printf("YUV file name %s \n", raw_image_fname);
        cap_yuv = (vx_image)vxGetObjectArrayItem(obj->cap_frames[0], 0);
        num_bytes_io = write_output_image_yuv422_8bit(raw_image_fname, cap_yuv);
        if(num_bytes_io < 0)
        {
            printf("Error writing to YUV file \n");
            return VX_FAILURE;
        }
    }

    if(obj->scaler_enable)
    {
        snprintf(yuv_image_fname, MAX_FNAME, "%s/%s_%04d.yuv", test_data_path, "img_msc", file_index);
        printf("YUV file name %s \n", yuv_image_fname);
        num_bytes_io = write_output_image_nv12_8bit(yuv_image_fname, obj->scaler_out_img);
        if(num_bytes_io < 0)
        {
            printf("Error writing to MSC NV12 file \n");
            return VX_FAILURE;
        }
    }

    if(obj->ldc_enable)
    {
        snprintf(yuv_image_fname, MAX_FNAME, "%s/%s_%04d.yuv", test_data_path, "img_ldc", file_index);
        printf("YUV file name %s \n", yuv_image_fname);
        num_bytes_io = write_output_image_nv12_8bit(yuv_image_fname, obj->ldc_out);
        if(num_bytes_io < 0)
        {
            printf("Error writing to LDC NV12 file \n");
            return VX_FAILURE;
        }
    }

    file_index++;
    return (file_index-1);
}
#endif //_APP_DEBUG_

static void app_parse_cfg_file(AppObj *obj, char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    char line_str[1024];
    char *token;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]. Switching to interactive mode\n", cfg_file_name);
        obj->is_interactive = 1;
    }
    else
    {
        while(fgets(line_str, sizeof(line_str), fp)!=NULL)
        {
            char s[]=" \t";

            if (strchr(line_str, '#'))
            {
                continue;
            }

            /* get the first token */
            token = strtok(line_str, s);
            if (NULL != token)
            {
                if(strcmp(token, "sensor_index")==0)
                {
                    token = strtok(NULL, s);
                    if (NULL != token)
                    {
                        obj->sensor_sel = atoi(token);
                        printf("sensor_selection = [%d]\n", obj->sensor_sel);
                    }
                }
                else
                if(strcmp(token, "ldc_enable")==0)
                {
                    token = strtok(NULL, s);
                    if (NULL != token)
                    {
                        obj->ldc_enable = atoi(token);
                        printf("ldc_enable = [%d]\n", obj->ldc_enable);
                    }
                }
                else
                if(strcmp(token, "num_frames_to_run")==0)
                {
                    token = strtok(NULL, s);
                    if (NULL != token)
                    {
                        obj->num_frames_to_run = atoi(token);
                        printf("num_frames_to_run = [%d]\n", obj->num_frames_to_run);
                    }
                }
                else
                if(strcmp(token, "is_interactive")==0)
                {
                    token = strtok(NULL, s);
                    if (NULL != token)
                    {
                        obj->is_interactive = atoi(token);
                        printf("is_interactive = [%d]\n", obj->is_interactive);
                    }
                }
                else
                {
                    APP_PRINTF("Invalid token [%s]\n", token);
                }
            }
        }

        fclose(fp);
    }

    if(obj->width_in<128)
        obj->width_in = 128;
    if(obj->height_in<128)
        obj->height_in = 128;
    if(obj->width_out<128)
        obj->width_out = 128;
    if(obj->height_out<128)
        obj->height_out = 128;

}

vx_status app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[])
{
    vx_bool set_test_mode = vx_false_e;
    vx_int8 sensor_override = 0xFF;
    app_set_cfg_default(obj);

    int i;
    if(argc==1)
    {
        app_show_usage(argc, argv);
        printf("Defaulting to interactive mode \n");
        obj->is_interactive = 1;
        return VX_SUCCESS;
    }

    for(i=0; i<argc; i++)
    {
        if(strcmp(argv[i], "--cfg")==0)
        {
            i++;
            if(i>=argc)
            {
                app_show_usage(argc, argv);
            }
            app_parse_cfg_file(obj, argv[i]);
        }
        else
        if(strcmp(argv[i], "--help")==0)
        {
            app_show_usage(argc, argv);
            return VX_FAILURE;
        }
        else
        if(strcmp(argv[i], "--test")==0)
        {
            set_test_mode = vx_true_e;
        }
        else
        if(strcmp(argv[i], "--sensor")==0)
        {
            // check to see if there is another argument following --sensor
            if (argc > i+1)
            {
                sensor_override = atoi(argv[i+1]);
                // increment i again to avoid this arg
                i++;
            }
        }
    }

    if(set_test_mode == vx_true_e)
    {
        obj->test_mode = 1;
        obj->is_interactive = 0;
        obj->num_frames_to_run = NUM_FRAMES;
        if (sensor_override != 0xFF)
        {
            obj->sensor_sel = sensor_override;
        }
    }
    return VX_SUCCESS;
}



#ifdef _APP_DEBUG_
vx_int32 write_output_image_nv12(char * file_name, vx_image out_nv12)
{
    FILE * fp = fopen(file_name, "wb");
    if(!fp)
    {
        APP_PRINTF("Unable to open file %s\n", file_name);
        return -1;
    }
    vx_uint32 len1 = write_output_image_fp(fp, out_nv12);
    fclose(fp);
    APP_PRINTF("%d bytes written to %s\n", len1, file_name);
    return len1;
}
#endif


AppObj gAppObj;

int app_single_cam_main(int argc, char* argv[])
{
    AppObj *obj = &gAppObj;
    vx_status status = VX_FAILURE;
    status = app_parse_cmd_line_args(obj, argc, argv);
    if(VX_SUCCESS == status)
    {
        status = app_init(obj);
        if(VX_SUCCESS == status)
        {
            APP_PRINTF("app_init done\n");
            /* Not checking status because application may be waiting for
                error/test frame */
            app_create_graph(obj);
            if(VX_SUCCESS == status)
            {
                APP_PRINTF("app_create_graph done\n");
                if(obj->is_interactive)
                {
                    status = app_run_graph_interactive(obj);
                }
                else
                {
                    status = app_run_graph(obj);
                }
                if(VX_SUCCESS == status)
                {
                    APP_PRINTF("app_run_graph done\n");
                    status = app_delete_graph(obj);
                    if(VX_SUCCESS == status)
                    {
                        APP_PRINTF("app_delete_graph done\n");
                    }
                    else
                    {
                        printf("Error : app_delete_graph returned 0x%x \n", status);
                    }
                }
                else
                {
                    printf("Error : app_run_graph_xx returned 0x%x \n", status);
                }
            }
            else
            {
                printf("Error : app_create_graph returned 0x%x is_interactive =%d  \n", status, obj->is_interactive);
            }
        }
        else
        {
            printf("Error : app_init returned 0x%x \n", status);
        }
        status = app_deinit(obj);
        if(VX_SUCCESS == status)
        {
            APP_PRINTF("app_deinit done\n");
        }
        else
        {
            printf("Error : app_deinit returned 0x%x \n", status);
        }
        appDeInitImageSensor(obj->sensor_name);
    }
    else
    {
        printf("Error: app_parse_cmd_line_args returned 0x%x \n", status);
    }
    if(obj->test_mode == 1)
    {
        if((test_result == vx_false_e) || (status == VX_FAILURE))
        {
            printf("\n\nTEST FAILED\n\n");
            print_new_checksum_structs();
            status = (status == VX_SUCCESS) ? VX_FAILURE : status;
        }
        else
        {
            printf("\n\nTEST PASSED\n\n");
        }
    }
    return status;
}

vx_status app_send_test_frame(vx_node cap_node, tivx_raw_image raw_img)
{
    vx_status status = VX_SUCCESS;

    status = tivxCaptureRegisterErrorFrame(cap_node, (vx_reference)raw_img);

    return status;
}

