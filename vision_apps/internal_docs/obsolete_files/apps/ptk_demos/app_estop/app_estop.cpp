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
 * *       any redistribution and use ar./apps/ptk_demos/app_dof_sfm_fisheye/config/app.cfge licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in appCntxtect code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any appCntxtect code compiled from the source code
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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "app_estop_main.h"
#include "app_estop.h"

#include <app_ptk_demo_common.h>
#include <app_ptk_demo_disparity.h>

#define APP_ESTOP_NAME    "app_estop"

#define DISTANCE_LINE_COLOR 1

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)<(b))?(b):(a))

void ESTOP_APP_setPcParams(ESTOP_APP_Context *appCntxt);
void ESTOP_APP_setOgParams(ESTOP_APP_Context *appCntxt);


#if !defined(PC)
static void ESTOP_APP_drawGraphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    return;
}
#endif

void ESTOP_APP_parseCfgFile(ESTOP_APP_Context *appCntxt, const char *cfg_file_name)
{
    FILE  * fp = fopen(cfg_file_name, "r");
    char  * pParamStr;
    char  * pValueStr;
    char  * pSLine;
    char  * basePath;
    char    paramSt[ESTOP_APP_MAX_LINE_LEN];
    char    valueSt[ESTOP_APP_MAX_LINE_LEN];
    char    sLine[ESTOP_APP_MAX_LINE_LEN];

    // set default parameters
    appCntxt->display_option       = 0;
    appCntxt->width                = 1280;
    appCntxt->height               = 720;
    appCntxt->tensor_width         = 768;
    appCntxt->tensor_height        = 432;
    appCntxt->numClasses           = 20;
    appCntxt->preProcMean[0]       = 128;
    appCntxt->preProcMean[1]       = 128;
    appCntxt->preProcMean[2]       = 128;
    appCntxt->preProcScale[0]      = 0.015625;
    appCntxt->preProcScale[1]      = 0.015625;
    appCntxt->preProcScale[2]      = 0.015625;
    appCntxt->enablePostProcNode   = 1;
    appCntxt->is_interactive       = 0;
    appCntxt->inputFormat          = PTK_IMG_FORMAT_UYVY;

    appCntxt->sdeAlgoType          = 0;
    appCntxt->confidence_threshold = 0;
    appCntxt->numLayers            = 2;
    appCntxt->ppMedianFilterEnable = 0;

    appCntxt->exportGraph          = 0;
    appCntxt->rtLogEnable          = 0;

    appCntxt->enableSpatialObjMerge                  = 1;
    appCntxt->enableTemporalObjMerge                 = 1;
    appCntxt->enableTemporalObjSmoothing             = 0;
    appCntxt->objectDistanceMode                     = 0;
    appCntxt->boundingBoxType                        = 1;

    appCntxt->sde_params.median_filter_enable        = 1;
    appCntxt->sde_params.reduced_range_search_enable = 0;
    appCntxt->sde_params.disparity_min               = 0;
    appCntxt->sde_params.disparity_max               = 1;
    appCntxt->sde_params.threshold_left_right        = 3;
    appCntxt->sde_params.texture_filter_enable       = 0;
    appCntxt->sde_params.threshold_texture           = 0;
    appCntxt->sde_params.aggregation_penalty_p1      = 32;
    appCntxt->sde_params.aggregation_penalty_p2      = 197;

    appCntxt->sde_params.confidence_score_map[0]     = 0;
    appCntxt->sde_params.confidence_score_map[1]     = 4;
    appCntxt->sde_params.confidence_score_map[2]     = 9;
    appCntxt->sde_params.confidence_score_map[3]     = 18;
    appCntxt->sde_params.confidence_score_map[4]     = 28;
    appCntxt->sde_params.confidence_score_map[5]     = 43;
    appCntxt->sde_params.confidence_score_map[6]     = 109;
    appCntxt->sde_params.confidence_score_map[7]     = 127;

    appCntxt->pipelineDepth                          = ESTOP_APP_MAX_PIPELINE_DEPTH;

    if (fp == NULL)
    {
        PTK_printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(0);
    }

    basePath = getenv("APP_STEREO_DATA_PATH");
    if (basePath == NULL)
    {
        PTK_printf("Please define APP_STEREO_DATA_PATH environment variable.\n");
        exit(-1);
    }

    pParamStr  = paramSt;
    pValueStr  = valueSt;
    pSLine     = sLine;

    while (1)
    {
        pSLine = fgets(pSLine, ESTOP_APP_MAX_LINE_LEN, fp);

        if( pSLine == NULL )
        {
            break;
        }

        if (strchr(pSLine, '#'))
        {
            continue;
        }

        pParamStr[0] = '\0';
        pValueStr[0] = '\0';
        
        sscanf(pSLine,"%128s %128s", pParamStr, pValueStr);

        if (pParamStr[0] == '\0' || pValueStr[0] == '\0')
        {
            continue;
        }

        /* get the first token */
        /* get the first token */
        if (strcmp(pParamStr, "left_img_file_path") == 0)
        {
            snprintf(appCntxt->left_img_file_path, ESTOP_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        if (strcmp(pParamStr, "right_img_file_path") == 0)
        {
            snprintf(appCntxt->right_img_file_path, ESTOP_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "output_file_path") == 0)
        {
            snprintf(appCntxt->output_file_path, ESTOP_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        if (strcmp(pParamStr, "left_LUT_file_name") == 0)
        {
            snprintf(appCntxt->left_LUT_file_name, ESTOP_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        if (strcmp(pParamStr, "right_LUT_file_name") == 0)
        {
            snprintf(appCntxt->right_LUT_file_name, ESTOP_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "dlr_model_file_path") == 0)
        {
            snprintf(appCntxt->dlrModelPath, ESTOP_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "input_format") == 0)
        {
            appCntxt->inputFormat = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "start_seq") == 0)
        {
            appCntxt->start_fileno = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "end_seq") == 0)
        {
            appCntxt->end_fileno = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "width") == 0)
        {
            appCntxt->width = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "height") == 0)
        {
            appCntxt->height = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "tensor_width") == 0)
        {
            appCntxt->tensor_width = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "tensor_height") == 0)
        {
            appCntxt->tensor_height = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "num_classes") == 0)
        {
            appCntxt->numClasses = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "pre_proc_mean") == 0)
        {
            sscanf(pValueStr, "%f,%f,%f",
                   &appCntxt->preProcMean[0],
                   &appCntxt->preProcMean[1],
                   &appCntxt->preProcMean[2]);
        }
        else if (strcmp(pParamStr, "pre_proc_scale") == 0)
        {
            sscanf(pValueStr, "%f,%f,%f",
                   &appCntxt->preProcScale[0],
                   &appCntxt->preProcScale[1],
                   &appCntxt->preProcScale[2]);
        }
        else if (strcmp(pParamStr, "enable_post_proc") == 0)
        {
            appCntxt->enablePostProcNode = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "sde_algo_type") == 0)
        {
            appCntxt->sdeAlgoType = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "num_layers") == 0)
        {
            appCntxt->numLayers = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "pp_median_filter_enable") == 0)
        {
            appCntxt->ppMedianFilterEnable = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "disparity_min") == 0)
        {
            appCntxt->sde_params.disparity_min = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "disparity_max") == 0)
        {
            appCntxt->sde_params.disparity_max = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "sde_confidence_threshold") == 0)
        {
            appCntxt->confidence_threshold = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "threshold_left_right")==0)
        {
            appCntxt->sde_params.threshold_left_right = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "texture_filter_enable")==0)
        {
            appCntxt->sde_params.texture_filter_enable = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "threshold_texture")==0)
        {
            appCntxt->sde_params.threshold_texture = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "aggregation_penalty_p1")==0)
        {
            appCntxt->sde_params.aggregation_penalty_p1 = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "aggregation_penalty_p2")==0)
        {
            appCntxt->sde_params.aggregation_penalty_p2 = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "median_filter_enable")==0)
        {
            appCntxt->sde_params.median_filter_enable = atoi(pValueStr);
        }
        else if(strcmp(pParamStr, "reduced_range_search_enable")==0)
        {
            appCntxt->sde_params.reduced_range_search_enable = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "distortion_center_x") == 0)
        {
            appCntxt->distCenterX = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "distortion_center_y") == 0)
        {
            appCntxt->distCenterY = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "focal_length") == 0)
        {
            appCntxt->focalLength = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "camera_roll") == 0)
        {
            appCntxt->camRoll = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "camera_pitch") == 0)
        {
            appCntxt->camPitch = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "camera_yaw") == 0)
        {
            appCntxt->camYaw = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "camera_height") == 0)
        {
            appCntxt->camHeight = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "stereo_baseline") == 0)
        {
            appCntxt->baseline = atof(pValueStr);
        }
        else if (strcmp(pParamStr, "grid_x_size") == 0)
        {
            appCntxt->xGridSize = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "grid_y_size") == 0)
        {
            appCntxt->yGridSize = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "min_x_range") == 0)
        {
            appCntxt->xMinRange = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "max_x_range") == 0)
        {
            appCntxt->xMaxRange = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "min_y_range") == 0)
        {
            appCntxt->yMinRange = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "max_y_range") == 0)
        {
            appCntxt->yMaxRange = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "min_pixel_count_grid") == 0)
        {
            appCntxt->thCnt = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "min_pixel_count_object") == 0)
        {
            appCntxt->thObjCnt = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "max_object_to_detect") == 0)
        {
            appCntxt->maxNumObject = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "num_neighbor_grid") == 0)
        {
            appCntxt->cNeighNum = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "enable_spatial_obj_merge") == 0)
        {
            appCntxt->enableSpatialObjMerge = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "enable_temporal_obj_merge") == 0)
        {
            appCntxt->enableTemporalObjMerge = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "enable_temporal_obj_smoothing") == 0)
        {
            appCntxt->enableTemporalObjSmoothing = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "object_distance_mode") == 0)
        {
            appCntxt->objectDistanceMode = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "bounding_box_type") == 0)
        {
            appCntxt->boundingBoxType = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "display_option") == 0)
        {
            appCntxt->display_option = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "is_interactive") == 0)
        {
            appCntxt->is_interactive = atoi(pValueStr);
        }        
        else if (strcmp(pParamStr, "exportGraph") == 0)
        {
            appCntxt->exportGraph = (uint8_t)atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "rtLogEnable") == 0)
        {
            appCntxt->rtLogEnable = (uint8_t)atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "pipeline_depth") == 0)
        {
            appCntxt->pipelineDepth  = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "disp_merge_deploy_core") == 0)
        {
            appCntxt->mlSdeCreateParams.dispMergeNodeCore = app_common_get_coreName(pValueStr);
        }
        else if (strcmp(pParamStr, "hole_filling_deploy_core") == 0)
        {
            appCntxt->mlSdeCreateParams.holeFillingNodeCore = app_common_get_coreName(pValueStr);
        }
        else if (strcmp(pParamStr, "pc_deploy_core") == 0)
        {
            appCntxt->ssDetectCreateParams.pcNodeCore = app_common_get_coreName(pValueStr);
        }
        else if (strcmp(pParamStr, "og_deploy_core") == 0)
        {
            appCntxt->ssDetectCreateParams.ogNodeCore = app_common_get_coreName(pValueStr);
        }
    }

    fclose(fp);

    appCntxt->xGridNum = ceil((appCntxt->xMaxRange - appCntxt->xMinRange) / appCntxt->xGridSize);
    appCntxt->yGridNum = ceil((appCntxt->yMaxRange - appCntxt->yMinRange) / appCntxt->yGridSize);

    if (appCntxt->width < 128)
    {
        appCntxt->width = 128;
    }
    if (appCntxt->height < 128)
    {
        appCntxt->height = 128;
    }
    if (appCntxt->end_fileno < appCntxt->start_fileno)
    {
        appCntxt->end_fileno = appCntxt->start_fileno;
    }

    if (appCntxt->pipelineDepth > ESTOP_APP_MAX_PIPELINE_DEPTH)
    {
        PTK_printf("Pipeline depth is larger than maximum pipeline depth allowed. Clipped..\n");
        appCntxt->pipelineDepth = ESTOP_APP_MAX_PIPELINE_DEPTH;
    }

    if (appCntxt->inputFormat != PTK_IMG_FORMAT_UYVY)
    {
        PTK_printf("Input format should be YUV_UYVY...\n");
        exit(0);
    }

    // when multi-layer SDE is used
    if (appCntxt->sdeAlgoType == 1)
    {
        int8_t  factor = (appCntxt->numLayers - 1) * 2;
        int32_t w, i;

        if (appCntxt->height % factor != 0 || appCntxt->width % factor != 0)
        {
            PTK_printf("Improper stereo image resolution...\n");
            exit(0);
        }

        for (i = 1; i < appCntxt->numLayers; i++)
        {
            w = appCntxt->width  / (2*i);

            if (w % 16 != 0)
            {
                PTK_printf("Improper image width is not multiple of 16...\n");
                exit(0);
            }
        }
    }

    appCntxt->renderPeriod = 0;

    return;

} /* ESTOP_APP_parseCfgFile */


void ESTOP_APP_setLDCCreateParams(ESTOP_APP_Context *appCntxt)
{
    SDELDCAPPLIB_createParams * createParams = &appCntxt->sdeLdcCreateParams;

    createParams->leftLutFileName  = appCntxt->left_LUT_file_name;
    createParams->rightLutFileName = appCntxt->right_LUT_file_name;

    createParams->width            = appCntxt->width;
    createParams->height           = appCntxt->height;
    createParams->inputFormat      = appCntxt->inputFormat;
    createParams->pipelineDepth    = appCntxt->pipelineDepth;
}

void ESTOP_APP_setSLSdeCreateParams(ESTOP_APP_Context *appCntxt)
{
    SL_SDEAPPLIB_createParams * createParams = &appCntxt->slSdeCreateParams;
    createParams->sdeCfg = appCntxt->sde_params;

     if (appCntxt->sde_params.disparity_min == 0)
    {
        createParams->minDisparity = 0;
    }
    else if (appCntxt->sde_params.disparity_min == 1)
    {
        createParams->minDisparity = -3;
    }

    if (appCntxt->sde_params.disparity_max == 0)
    {
        createParams->maxDisparity = 63;
    }
    else if (appCntxt->sde_params.disparity_max == 1)
    {
        createParams->maxDisparity = 127;
    }
    else if (appCntxt->sde_params.disparity_max == 2)
    {
        createParams->maxDisparity = 191;
    }

    createParams->width              = appCntxt->width;
    createParams->height             = appCntxt->height;
    createParams->inputFormat        = appCntxt->inputFormat;
    createParams->pipelineDepth      = appCntxt->pipelineDepth;
}

void ESTOP_APP_setMLSdeCreateParams(ESTOP_APP_Context *appCntxt)
{
    ML_SDEAPPLIB_createParams * createParams = &appCntxt->mlSdeCreateParams;
    createParams->sdeCfg = appCntxt->sde_params;

    if (appCntxt->sde_params.disparity_min == 0)
    {
        createParams->minDisparity = 0;
    }
    else if (appCntxt->sde_params.disparity_min == 1)
    {
        createParams->minDisparity = -3;
    }

    if (appCntxt->sde_params.disparity_max == 0)
    {
        createParams->maxDisparity = 63;
    }
    else if (appCntxt->sde_params.disparity_max == 1)
    {
        createParams->maxDisparity = 127;
    }
    else if (appCntxt->sde_params.disparity_max == 2)
    {
        createParams->maxDisparity = 191;
    }

    createParams->inputFormat        = appCntxt->inputFormat;
    createParams->numLayers          = appCntxt->numLayers;
    createParams->enableMedianFilter = appCntxt->ppMedianFilterEnable;
    createParams->width              = appCntxt->width;
    createParams->height             = appCntxt->height;
    createParams->pipelineDepth      = appCntxt->pipelineDepth;
}

void ESTOP_APP_setSSDetectCreateParams(ESTOP_APP_Context *appCntxt)
{
    SS_DETECT_APPLIB_createParams * createParams = &appCntxt->ssDetectCreateParams;

    createParams->width            = appCntxt->width;
    createParams->height           = appCntxt->height;
    createParams->tensorWidth      = appCntxt->tensor_width;
    createParams->tensorHeight     = appCntxt->tensor_height;

    createParams->inputFormat      = appCntxt->inputFormat;
    createParams->exportGraph      = appCntxt->exportGraph;
    createParams->rtLogEnable      = appCntxt->rtLogEnable;
    createParams->vxEvtAppValBase  = 0;

    ESTOP_APP_setPcParams(appCntxt);
    ESTOP_APP_setOgParams(appCntxt);
}


void ESTOP_APP_setPcParams(ESTOP_APP_Context *appCntxt)
{
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.width        = appCntxt->width;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.height       = appCntxt->height;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.tensorWidth  = appCntxt->tensor_width;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.tensorHeight = appCntxt->tensor_height;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.dsFactor     = 4;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.confidenceTh = appCntxt->confidence_threshold;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.dsWidth      =
        appCntxt->ssDetectCreateParams.pcCfg.cfgParams.width / appCntxt->ssDetectCreateParams.pcCfg.cfgParams.dsFactor;
    appCntxt->ssDetectCreateParams.pcCfg.cfgParams.dsHeight     =
        appCntxt->ssDetectCreateParams.pcCfg.cfgParams.height / appCntxt->ssDetectCreateParams.pcCfg.cfgParams.dsFactor;

    appCntxt->ssDetectCreateParams.pcCfg.camParams.camHeight    = appCntxt->camHeight;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.camRoll      = appCntxt->camRoll;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.camPitch     = appCntxt->camPitch;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.camYaw       = appCntxt->camYaw;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.sinPitch     = sin(appCntxt->camPitch);
    appCntxt->ssDetectCreateParams.pcCfg.camParams.cosPitch     = cos(appCntxt->camPitch);
    appCntxt->ssDetectCreateParams.pcCfg.camParams.baseline     = appCntxt->baseline;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.dcx          = appCntxt->distCenterX;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.dcy          = appCntxt->distCenterY;
    appCntxt->ssDetectCreateParams.pcCfg.camParams.f            = appCntxt->focalLength;
}


void ESTOP_APP_setOgParams(ESTOP_APP_Context *appCntxt)
{
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.width                      = appCntxt->width;
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.height                     = appCntxt->height;
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.enableSpatialObjMerge      = appCntxt->enableSpatialObjMerge;
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.enableTemporalObjMerge     = appCntxt->enableTemporalObjMerge;
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.enableTemporalObjSmoothing = appCntxt->enableTemporalObjSmoothing;
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.objectDistanceMode         = appCntxt->objectDistanceMode;

    appCntxt->ssDetectCreateParams.ogCfg.camParams.camHeight    = appCntxt->camHeight;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.camRoll      = appCntxt->camRoll;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.camPitch     = appCntxt->camPitch;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.camYaw       = appCntxt->camYaw;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.sinPitch     = sin(appCntxt->camPitch);
    appCntxt->ssDetectCreateParams.ogCfg.camParams.cosPitch     = cos(appCntxt->camPitch);
    appCntxt->ssDetectCreateParams.ogCfg.camParams.baseline     = appCntxt->baseline;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.dcx          = appCntxt->distCenterX;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.dcy          = appCntxt->distCenterY;
    appCntxt->ssDetectCreateParams.ogCfg.camParams.f            = appCntxt->focalLength;

    appCntxt->ssDetectCreateParams.ogCfg.ogParams.xGridSize     = appCntxt->xGridSize;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.yGridSize     = appCntxt->yGridSize;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.xMinRange     = appCntxt->xMinRange;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.xMaxRange     = appCntxt->xMaxRange;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.yMinRange     = appCntxt->yMinRange;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.yMaxRange     = appCntxt->yMaxRange;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.xGridNum      = appCntxt->xGridNum;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.yGridNum      = appCntxt->yGridNum;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.thCnt         = appCntxt->thCnt;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.thObjCnt      = appCntxt->thObjCnt;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.maxNumObject  = appCntxt->maxNumObject;
    appCntxt->ssDetectCreateParams.ogCfg.ogParams.cNeighNum     = appCntxt->cNeighNum;
}

void ESTOP_APP_setAllParams(ESTOP_APP_Context *appCntxt)
{
    appCntxt->vxEvtAppValBase = 0;

    /* LDC params */
    ESTOP_APP_setLDCCreateParams(appCntxt);

    /* SDE params */
    if (appCntxt->sdeAlgoType == 0)
    {
        ESTOP_APP_setSLSdeCreateParams(appCntxt);
    }
    else if (appCntxt->sdeAlgoType == 1)
    {
        ESTOP_APP_setMLSdeCreateParams(appCntxt);
    }

    ESTOP_APP_setSSDetectCreateParams(appCntxt);
}


int32_t ESTOP_APP_createDraw(ESTOP_APP_Context *appCntxt)
{
    int32_t status;
    Draw2D_BufInfo sBufInfo;

    appCntxt->pDisplayBuf565 = (uint16_t *)tivxMemAlloc(appCntxt->width * appCntxt->height * sizeof(uint16_t), TIVX_MEM_EXTERNAL);
    PTK_assert(NULL != appCntxt->pDisplayBuf565);

    Draw2D_create(&appCntxt->pHndl);

    sBufInfo.bufWidth = appCntxt->width;
    sBufInfo.bufHeight = appCntxt->height;
    sBufInfo.bufPitch[0] = appCntxt->width * 2;
    sBufInfo.dataFormat = DRAW2D_DF_BGR16_565;
    sBufInfo.transperentColor = 1;
    sBufInfo.transperentColorFormat = DRAW2D_DF_BGR16_565;
    sBufInfo.bufAddr[0] = (uint8_t *)appCntxt->pDisplayBuf565;

    status = Draw2D_setBufInfo(appCntxt->pHndl, &sBufInfo);

    return status;
}

void ESTOP_APP_init(ESTOP_APP_Context *appCntxt)
{
    int32_t             status;
    vx_status           vxStatus = VX_SUCCESS;

    // Create the DLR Model handle
    CM_DLRCreateParams  params;

    params.modelPath = appCntxt->dlrModelPath;
    params.devType   = DLR_DEVTYPE_CPU;
    params.devId     = 0;

    status = CM_dlrNodeCntxtInit(&appCntxt->dlrObj, &params);
    if (status < 0)
    {
        PTK_printf("[%s:%d] CM_dlrNodeCntxtInit() failed.\n",
                    __FUNCTION__, __LINE__);

        vxStatus = VX_FAILURE;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        status = appInit();
        if (status < 0)
        {
            PTK_printf("[%s:%d] appInit() failed.\n",
                       __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
    }

    // OpenVX initialization
    appCntxt->vxContext = vxCreateContext();
    PTK_assert(appCntxt->vxContext);

    // create graph 
    appCntxt->vxGraph = vxCreateGraph(appCntxt->vxContext);
    if (appCntxt->vxGraph == NULL)
    {
        PTK_printf("[%s:%d] vxCreateGraph() failed\n",
                    __FUNCTION__, __LINE__);
        status = -1;
    }

    if (status >= 0)
    {
        vxSetReferenceName((vx_reference)appCntxt->vxGraph, "AMR E-Stop Graph");
    }

    if (status >= 0)
    {
        /* load TILDL kernels */
        tivxTIDLLoadKernels(appCntxt->vxContext);

        /* load image processing kernel */
        tivxImgProcLoadKernels(appCntxt->vxContext);

        /* Load HWA kernels */
        tivxHwaLoadKernels(appCntxt->vxContext);

        /* Load stereo kernels */
        tivxStereoLoadKernels(appCntxt->vxContext);
    }

    /*
     * 1 Setup Stereo LDC nodes
     */
    if (status >= 0)
    {
        status = ESTOP_APP_init_LDC(appCntxt);
    }

    /*
     * 2 Setup SDE nodes
     */
    if (status >= 0)
    {
        status = ESTOP_APP_init_SDE(appCntxt);
    }

    /*
     * Setup Semantic Segmentation nodes 
     */
    if (status >= 0)
    {
        status = ESTOP_APP_init_SS(appCntxt);
    }

    /*
     * Setup OG map based detection nodes
     */
    if (status >= 0)
    {
        status = ESTOP_APP_init_SS_Detection(appCntxt);
    }

    if (status >= 0)
    {
        /*
         * set up the pipeline. 
         */
        vxStatus = ESTOP_APP_setupPipeline(appCntxt);

        /* Verify graph */
        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            vxStatus = vxVerifyGraph(appCntxt->vxGraph);

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] vxVerifyGraph() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
            }

            /* Set the MSC coefficients. */
            if (vxStatus == (vx_status)VX_SUCCESS)
            {
                CM_ScalerNodeCntxt * scalerObj = SEMSEG_CNN_APPLIB_getScalerNodeCntxt(appCntxt->ssCnnHdl);
                vxStatus = CM_scalerNodeCntxtSetCoeff(scalerObj);

                if (vxStatus != (vx_status)VX_SUCCESS)
                {
                    PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_setCoeff() failed\n",
                                __FUNCTION__, __LINE__);
                }
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            appPerfPointSetName(&appCntxt->estopPerf , "Emergency Stop GRAPH");
        }
    }

    if (status >= 0)
    {
        // init sacler for ML SDE
        if (appCntxt->sdeAlgoType == 1)
        {
            ML_SDEAPPLIB_initScaler(appCntxt->mlSdeHdl);
        }

        if (appCntxt->exportGraph == 1)
        {
            tivxExportGraphToDot(appCntxt->vxGraph, ".", "vx_app_estop");
        }

        if (appCntxt->rtLogEnable == 1)
        {
            tivxLogRtTraceEnable(appCntxt->vxGraph);
        }

        appCntxt->exitInputDataProcess = false;
        appCntxt->dataReadySem         = new Semaphore(appCntxt->pipelineDepth);

        appCntxt->exitDisplayThread    = false;
        appCntxt->displayCtrlSem       = new Semaphore(0);

        appCntxt->exitDlrThread        = false;
        appCntxt->dlrDataReadySem      = new Semaphore(0);

        if (1 == appCntxt->display_option)
        {
#if !defined(PC)
            app_grpx_init_prms_t grpx_prms;
            appGrpxInitParamsInit(&grpx_prms, appCntxt->vxContext);
            grpx_prms.draw_callback = ESTOP_APP_drawGraphics;
            appGrpxInit(&grpx_prms);
#endif
            ESTOP_APP_createDraw(appCntxt);
        }
    }    
}


/*
 * Create color coded disparity imag
 * the RGB image for display
 */
void ESTOP_APP_createDisparityCCImage(ESTOP_APP_Context *appCntxt)
{
    vx_status status;
    vx_rectangle_t rect;
    vx_imagepatch_addressing_t src_image_addr;
    vx_imagepatch_addressing_t dst_image_addr;
    vx_map_id map_src_id;
    vx_map_id map_dst_id;
    uint16_t *src_ptr;
    uint8_t  *dst_ptr;

    uint16_t width  = appCntxt->width;
    uint16_t height = appCntxt->height;

    tivx_sde_disparity_vis_params_t sdeVizParams;


    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = width;
    rect.end_y = height;

    // S16 and RGB both have 1 plane
    status = vxMapImagePatch(appCntxt->vxDisparity16,
                             &rect,
                             0,
                             &map_src_id,
                             &src_image_addr,
                             (void **)&src_ptr,
                             VX_READ_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);
    if(status != VX_SUCCESS)
    {
        PTK_printf("[%s:%d] vxMapImagePatch() failed.\n", __FUNCTION__,  __LINE__);
    }

    status = vxMapImagePatch(appCntxt->vxDisparityCC,
                             &rect,
                             0,
                             &map_dst_id,
                             &dst_image_addr,
                             (void **)&dst_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);
    if(status != VX_SUCCESS)
    {
        PTK_printf("[%s:%d] vxMapImagePatch() failed.\n", __FUNCTION__,  __LINE__);
    }

    sdeVizParams.disparity_min  = 0; 
    sdeVizParams.disparity_max  = 1; // 0 ~ 127
    sdeVizParams.disparity_only = 0;
    sdeVizParams.vis_confidence = appCntxt->confidence_threshold;

    ptkdemo_visualizeSdeDisparity(
        &sdeVizParams,
        (int16_t *)src_ptr,
        (uint8_t *)dst_ptr,
        src_image_addr.dim_x,
        src_image_addr.dim_y,
        src_image_addr.stride_y/src_image_addr.stride_x,
        dst_image_addr.stride_y
    );

    vxUnmapImagePatch(appCntxt->vxDisparity16, map_src_id);
    vxUnmapImagePatch(appCntxt->vxDisparityCC, map_dst_id);
}

void ESTOP_APP_overlay3DBB(ESTOP_APP_Context *appCntxt)
{
    int32_t                          i, j;
    uint32_t                         value;
    vx_status                        status;
    vx_rectangle_t                   rect;
    vx_imagepatch_addressing_t       image_addr;
    vx_map_id                        map_id;
    vx_df_image                      img_format;
    uint8_t                        * data_ptr;
    PTK_Alg_StereoOG_obs3DBox      * obs3DBB;
    PTK_Alg_StereoOG_BoxProp       * boxProp; 

    // for 2D bounding box
    float bl_x, bl_y, br_x, br_y;
    float tl_x, tl_y, tr_x, tr_y;

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = appCntxt->width;
    rect.end_y = appCntxt->height;

    vxQueryImage(appCntxt->vxDispRightImage, VX_IMAGE_FORMAT, &img_format, sizeof(vx_df_image));

    status = vxMapImagePatch(appCntxt->vxDispRightImage,
                             &rect,
                             0,
                             &map_id,
                             &image_addr,
                             (void **)&data_ptr,
                             VX_READ_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);

    vxUnmapImagePatch(appCntxt->vxDispRightImage, map_id);

    if (status == VX_SUCCESS)
    {
        Draw2D_LinePrm sLinePrm;
        Draw2D_FontPrm sDistance;

        char     strDistance[100];
        uint16_t RGB_565_val;
        int16_t  midX, midY;

        if (img_format == VX_DF_IMAGE_UYVY)
        {
            for (j = 0; j < (int32_t)image_addr.dim_y; j++)
            {
                for (i = 0; i < appCntxt->width; i++)
                {
                    value = data_ptr[j * image_addr.stride_y + i*2 + 1];
                    appCntxt->pDisplayBuf565[j * appCntxt->width + i] = (uint16_t)RGB888_TO_RGB565(value, value, value);
                }
            }
        }
        else
        {
            for (j = 0; j < (int32_t)image_addr.dim_y; j++)
            {
                for (i = 0; i < appCntxt->width; i++)
                {
                    value = data_ptr[j * image_addr.stride_y + i];
                    appCntxt->pDisplayBuf565[j * appCntxt->width + i] = (uint16_t)RGB888_TO_RGB565(value, value, value);
                }
            }
        }

        obs3DBB = (tivx_ss_sde_obs_3d_bound_box_t *) ptkdemo_getUserObjDataPayload(appCntxt->vxDisp3DBoundBox);

        if (obs3DBB == NULL)
        {
            PTK_printf("[%s:%d] ptkdemo_getUserObjDataPayload() failed\n",
                        __FUNCTION__, __LINE__);

            return;
        }

        boxProp = (PTK_Alg_StereoOG_BoxProp *) PTK_Alg_StereOG_get3DBB(obs3DBB);

        if (boxProp == NULL)
        {
            PTK_printf("[%s:%d] PTK_Alg_StereOG_get3DBB() failed\n",
                        __FUNCTION__, __LINE__);
            return;
        }

        // draw bounding boxes
        sLinePrm.lineSize = 3;
        sLinePrm.lineColorFormat = DRAW2D_DF_BGR16_565;

        sDistance.fontIdx = 3;

        for (i = 0; i < obs3DBB->numObject; i++)
        {
            // change color based on class ID - TO REVISIT
#if DISTANCE_LINE_COLOR
            if (boxProp[i].frontDepth <= 5000)
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(0, 0, 255);
            }
            else
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(255, 0, 0);
            }
#else
            if ( boxProp[i].classId == 11 )
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(0, 0, 255);
            } 
            else if ( boxProp[i].classId == 12 )
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(0, 255, 0);
            }
            else if ( boxProp[i].classId == 13 )
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(255, 0, 0);
            }
            else if ( boxProp[i].classId == 14 )
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(255, 255, 0);
            }
            else if ( boxProp[i].classId == 18 )
            {
                sLinePrm.lineColor = RGB888_TO_RGB565(0, 255, 255);
            }
#endif

            if (appCntxt->boundingBoxType == 1)
            {
                // Front box
                // P4 P3
                // P1 P2
                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf1x,
                                boxProp[i].pf1y,
                                boxProp[i].pf2x,
                                boxProp[i].pf2y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf2x,
                                boxProp[i].pf2y,
                                boxProp[i].pf3x,
                                boxProp[i].pf3y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf3x,
                                boxProp[i].pf3y,
                                boxProp[i].pf4x,
                                boxProp[i].pf4y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf4x,
                                boxProp[i].pf4y,
                                boxProp[i].pf1x,
                                boxProp[i].pf1y,
                                &sLinePrm);

                // Rear box
                // P4 P3
                // P1 P2
                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pr1x,
                                boxProp[i].pr1y,
                                boxProp[i].pr2x,
                                boxProp[i].pr2y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pr2x,
                                boxProp[i].pr2y,
                                boxProp[i].pr3x,
                                boxProp[i].pr3y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pr3x,
                                boxProp[i].pr3y,
                                boxProp[i].pr4x,
                                boxProp[i].pr4y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pr4x,
                                boxProp[i].pr4y,
                                boxProp[i].pr1x,
                                boxProp[i].pr1y,
                                &sLinePrm);

                // pf1 - pr1, pf2 - pr2, pf3 - pr3, pf4 - pr4
                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf1x,
                                boxProp[i].pf1y,
                                boxProp[i].pr1x,
                                boxProp[i].pr1y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf2x,
                                boxProp[i].pf2y,
                                boxProp[i].pr2x,
                                boxProp[i].pr2y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf3x,
                                boxProp[i].pf3y,
                                boxProp[i].pr3x,
                                boxProp[i].pr3y,
                                &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                                boxProp[i].pf4x,
                                boxProp[i].pf4y,
                                boxProp[i].pr4x,
                                boxProp[i].pr4y,
                                &sLinePrm);

                sprintf(strDistance, "%.1f m", float(boxProp[i].frontDepth/ 1000));
                midX = (boxProp[i].pf1x + boxProp[i].pf2x) / 2;
                midY = (boxProp[i].pf1y + boxProp[i].pf2y) / 2;
                Draw2D_drawString(appCntxt->pHndl,midX-15, midY-15, (char *)strDistance, &sDistance);
            } else
            {
                // init 2D BB corners
                bl_x = br_x = boxProp[i].pf1x; bl_y = br_y = boxProp[i].pf1y;
                tl_x = tr_x = boxProp[i].pf4x; tl_y = tr_y = boxProp[i].pf4y;

                // compare with pf2
                if (boxProp[i].pf2x < bl_x)
                {
                    bl_x = boxProp[i].pf2x; bl_y = boxProp[i].pf2y;
                    tl_x = boxProp[i].pf3x; tl_y = boxProp[i].pf3y;
                }

                if (boxProp[i].pf2x > br_x)
                {
                    br_x = boxProp[i].pf2x; br_y = boxProp[i].pf2y;
                    tr_x = boxProp[i].pf3x; tr_y = boxProp[i].pf3y;
                }

                // compare with pr1
                if (boxProp[i].pr1x < bl_x)
                {
                    bl_x = boxProp[i].pr1x; bl_y = boxProp[i].pr1y;
                    tl_x = boxProp[i].pr4x; tl_y = boxProp[i].pr4y;
                }

                if (boxProp[i].pr1x > br_x)
                {
                    br_x = boxProp[i].pr1x; br_y = boxProp[i].pr1y;
                    tr_x = boxProp[i].pr4x; tr_y = boxProp[i].pr4y;
                }

                // compare with pr2
                if (boxProp[i].pr2x < bl_x)
                {
                    bl_x = boxProp[i].pr2x; bl_y = boxProp[i].pr2y;
                    tl_x = boxProp[i].pr3x; tl_y = boxProp[i].pr3y;
                }

                if (boxProp[i].pr2x > br_x)
                {
                    br_x = boxProp[i].pr2x; br_y = boxProp[i].pr2y;
                    tr_x = boxProp[i].pr3x; tr_y = boxProp[i].pr3y;
                }

                Draw2D_drawLine(appCntxt->pHndl,
                    bl_x,
                    bl_y,
                    br_x,
                    br_y,
                    &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                    br_x,
                    br_y,
                    tr_x,
                    tr_y,
                    &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                    tr_x,
                    tr_y,
                    tl_x,
                    tl_y,
                    &sLinePrm);

                Draw2D_drawLine(appCntxt->pHndl,
                    tl_x,
                    tl_y,
                    bl_x,
                    bl_y,
                    &sLinePrm);

                sprintf(strDistance, "%.1f m", float(boxProp[i].frontDepth/ 1000));
                midX = (bl_x + br_x) / 2;
                midY = (bl_y + tl_y) / 2;
                Draw2D_drawString(appCntxt->pHndl,midX-15, midY-15, (char *)strDistance, &sDistance);
            }
        }

        // get properties of vxBBImage
        status = vxMapImagePatch(appCntxt->vxBBImage,
                             &rect,
                             0,
                             &map_id,
                             &image_addr,
                             (void **)&data_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);


        // copy RGB565 to RGB888
        for (j = 0; j < appCntxt->height; j++)
        {
            for (i = 0; i < appCntxt->width; i++)
            {
                RGB_565_val = appCntxt->pDisplayBuf565[j * appCntxt->width + i];

                data_ptr[0] = (RGB_565_val & 0x1F) << 3;
                data_ptr[1] = ((RGB_565_val >> 5) & 0x3F) << 2;
                data_ptr[2] = ((RGB_565_val >> 11) & 0x1F) << 3;

                data_ptr += 3;
            }

            data_ptr += (image_addr.stride_y - appCntxt->width*3);
        }

        vxUnmapImagePatch(appCntxt->vxBBImage, map_id);
    }
}


vx_status ESTOP_APP_run(ESTOP_APP_Context *appCntxt)
{
    uint32_t  i;
    vx_status vxStatus;
    char      temp[ESTOP_APP_MAX_LINE_LEN];

    ESTOP_APP_graphParams* gpDesc;

    vxStatus           = VX_SUCCESS;
    appCntxt->frameCnt = 0;

    for (i = appCntxt->start_fileno; i <= appCntxt->end_fileno; i++)
    {
        /* Wait for the data ready semaphore. */
        if (appCntxt->dataReadySem)
        {
            appCntxt->dataReadySem->wait();
        }

#if defined(PC)
        PTK_printf("Processing frame %d...\n", i);
#endif

        vxStatus = ESTOP_APP_popFreeInputDesc(appCntxt, &gpDesc);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ESTOP_APP_popFreeInputDesc() failed\n",
                        __FUNCTION__, __LINE__);
        }

        // left image
        strcpy(appCntxt->left_img_file_name, appCntxt->left_img_file_path);
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            sprintf(temp, "/%010d.bmp", i);
            if (strlen(appCntxt->left_img_file_name) + strlen(temp) >= ESTOP_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->left_img_file_name, temp);
                vxStatus = tivx_utils_load_vximage_from_bmpfile(gpDesc->vxInputLeftImage, appCntxt->left_img_file_name, vx_true_e);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] tivx_utils_load_vximage_from_bmpfile() failed\n",
                            __FUNCTION__, __LINE__);
            }
        } else
        {
            sprintf(temp, "/%010d.yuv", i);
            if (strlen(appCntxt->left_img_file_name) + strlen(temp) >= ESTOP_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->left_img_file_name, temp);
                vxStatus = ptkdemo_load_vximage_from_yuvfile(gpDesc->vxInputLeftImage, appCntxt->left_img_file_name);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ptkdemo_load_vximage_from_yuvfile() failed\n",
                            __FUNCTION__, __LINE__);
            }
        }

        // rigth image
        strcpy(appCntxt->right_img_file_name, appCntxt->right_img_file_path);
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            sprintf(temp, "/%010d.bmp", i);
            if (strlen(appCntxt->right_img_file_name) + strlen(temp) >= ESTOP_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->right_img_file_name, temp);
                vxStatus = tivx_utils_load_vximage_from_bmpfile(gpDesc->vxInputRightImage, appCntxt->right_img_file_name, vx_true_e);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] tivx_utils_load_vximage_from_bmpfile() failed\n",
                            __FUNCTION__, __LINE__);
            }
        } else
        {
            sprintf(temp, "/%010d.yuv", i);
            if (strlen(appCntxt->right_img_file_name) + strlen(temp) >= ESTOP_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->right_img_file_name, temp);
                vxStatus = ptkdemo_load_vximage_from_yuvfile(gpDesc->vxInputRightImage, appCntxt->right_img_file_name);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ptkdemo_load_vximage_from_yuvfile() failed\n",
                            __FUNCTION__, __LINE__);
            }
        }

        // then run AMR applib
        vxStatus = ESTOP_APP_process(appCntxt, gpDesc);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ESTOP_APP_process() failed\n",
                        __FUNCTION__, __LINE__);
            break;
        }

        appCntxt->frameCnt++;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        ESTOP_APP_waitGraph(appCntxt);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ESTOP_APP_waitGraph() failed\n",
                        __FUNCTION__, __LINE__);
        }

        PTK_printf("\e[KProcessed %d frames.\n\e[A", appCntxt->frameCnt);
    }

    return vxStatus;
}

void ESTOP_APP_deInit(ESTOP_APP_Context *appCntxt)
{
    int32_t status;
    uint8_t i;

    if (appCntxt->dataReadySem)
    {
        delete appCntxt->dataReadySem;
    }

    if (appCntxt->dlrDataReadySem)
    {
        delete appCntxt->dlrDataReadySem;
    }

    if (appCntxt->displayCtrlSem)
    {
        delete appCntxt->displayCtrlSem;
    }

    // release input image object
    vxReleaseImage(&appCntxt->vxLeftRectImage);
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        vxReleaseImage(&appCntxt->vxInputLeftImage[i]);
        vxReleaseImage(&appCntxt->vxInputRightImage[i]);
        vxReleaseImage(&appCntxt->vxRightRectImage[i]);
    }

    // release dispairty object
    if (appCntxt->sdeAlgoType == 0)
    {
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            vxReleaseImage(&appCntxt->vxSde16BitOutput[i]);
        }
    } else
    {
        if (appCntxt->ppMedianFilterEnable)
        {
            for (i = 0; i < appCntxt->pipelineDepth; i++)
            {
                vxReleaseImage(&appCntxt->vxMedianFilteredDisparity[i]);
            }
        }

        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            vxReleaseImage(&appCntxt->vxMergeDisparityL0[i]);
        }
    }

    // release graph
    vxReleaseGraph(&appCntxt->vxGraph);

    // Unload HWA kernels
    tivxStereoUnLoadKernels(appCntxt->vxContext);
    tivxImgProcUnLoadKernels(appCntxt->vxContext);
    tivxHwaUnLoadKernels(appCntxt->vxContext);

    if (1 == appCntxt->display_option)
    {
        tivxMemFree(appCntxt->pDisplayBuf565, appCntxt->width * appCntxt->height * sizeof(uint16_t), TIVX_MEM_EXTERNAL);
        Draw2D_delete(appCntxt->pHndl);
#if !defined(PC)
        appGrpxDeInit();
#endif
    }

    /* Release the context. */
    vxReleaseContext(&appCntxt->vxContext);

    /* Delete the DLR Model handle. */
    status = CM_dlrNodeCntxtDeInit(&appCntxt->dlrObj);

    if (status < 0)
    {
        PTK_printf("[%s:%d] CM_dlrNodeCntxtDeInit() failed.\n",
                    __FUNCTION__, __LINE__);
    }

#if 0
    status = appDeInit();
    PTK_assert(status == 0);
#endif
}

vx_status ESTOP_APP_createDisplayGraph(ESTOP_APP_Context *appCntxt)
{
    vx_status   vxStatus;
    uint8_t   * bbMem;


    // create graph
    appCntxt->vxDispGraph = vxCreateGraph(appCntxt->vxContext);
    APP_ASSERT_VALID_REF(appCntxt->vxDispGraph);
    vxSetReferenceName((vx_reference)appCntxt->vxDispGraph, "Display");

    // create objects
    appCntxt->vxDispSSImage = vxCreateImage(appCntxt->vxContext, appCntxt->tensor_width, appCntxt->tensor_height, VX_DF_IMAGE_NV12);
    APP_ASSERT_VALID_REF(appCntxt->vxDispSSImage);
    vxSetReferenceName((vx_reference)appCntxt->vxDispSSImage, "Segmentation_Display_Image");

    appCntxt->vxBBImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_RGB);
    APP_ASSERT_VALID_REF(appCntxt->vxBBImage);
    vxSetReferenceName((vx_reference)appCntxt->vxBBImage, "BoundingBox_Display_Image");

    appCntxt->vxDispRightImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
    APP_ASSERT_VALID_REF(appCntxt->vxDispRightImage);
    vxSetReferenceName((vx_reference)appCntxt->vxDispRightImage, "Rectified_Right_Display_Image");

    appCntxt->bbSize = PTK_Alg_StereoOG_getObsBBSize(&appCntxt->ssDetectCreateParams.ogCfg.ogParams);
    bbMem  = new uint8_t[appCntxt->bbSize];
    if (bbMem == NULL)
    {
        PTK_printf("[%s:%d] Memory allocation failed.\n",
                   __FUNCTION__, __LINE__);

        vxStatus = VX_FAILURE;
    }

    appCntxt->vxDisp3DBoundBox = vxCreateUserDataObject(appCntxt->vxContext,
                                                        "Display_Obs3DBoundingBox",
                                                        appCntxt->bbSize,
                                                        NULL);
    PTK_assert(appCntxt->vxDisp3DBoundBox);
    vxSetReferenceName((vx_reference)appCntxt->vxDisp3DBoundBox, "Display_Obs3DBoundingBox");

    delete [] bbMem;

    appCntxt->vxDisparityCC = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_RGB);
    APP_ASSERT_VALID_REF(appCntxt->vxDisparityCC);
    vxSetReferenceName((vx_reference)appCntxt->vxDisparityCC, "CC_Disparity");

    appCntxt->vxDisparity16 = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_S16);
    APP_ASSERT_VALID_REF(appCntxt->vxDisparity16);
    vxSetReferenceName((vx_reference)appCntxt->vxDisparity16, "Raw_Disparity16");


    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == appCntxt->display_option))
    {
#if !defined(PC)
        memset(&appCntxt->ss_display_params, 0, sizeof(tivx_display_params_t));
        appCntxt->ss_display_config = vxCreateUserDataObject(appCntxt->vxContext, "tivx_display_params_t",
                                                                    sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(appCntxt->ss_display_config);

        vxSetReferenceName((vx_reference)appCntxt->ss_display_config, "SemSeg_DisplayConfiguration");

        appCntxt->ss_display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        appCntxt->ss_display_params.pipeId = 2;
        appCntxt->ss_display_params.outWidth  = OUTPUT_DISPLAY_WIDTH;
        appCntxt->ss_display_params.outHeight = OUTPUT_DISPLAY_HEIGHT;
        appCntxt->ss_display_params.posX = 960;
        appCntxt->ss_display_params.posY = 300;

        vxStatus = vxCopyUserDataObject(appCntxt->ss_display_config, 0, sizeof(tivx_display_params_t), &appCntxt->ss_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(vxStatus == VX_SUCCESS);

        // create disparity display node
        appCntxt->node_disparity_display = tivxDisplayNode(
            appCntxt->vxDispGraph,
            appCntxt->ss_display_config,
            appCntxt->vxDispSSImage);
        vxStatus = vxGetStatus((vx_reference)appCntxt->node_disparity_display);
        if (VX_SUCCESS != vxStatus)
        {
            PTK_printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        vxStatus = vxSetNodeTarget(appCntxt->node_disparity_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
        APP_ASSERT(vxStatus == VX_SUCCESS);
        vxSetReferenceName((vx_reference)appCntxt->node_disparity_display, "DisparityDisplay");
#endif
    }

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (1 == appCntxt->display_option))
    {
#if !defined(PC)
        memset(&appCntxt->bb_display_params, 0, sizeof(tivx_display_params_t));
        appCntxt->bb_display_config = vxCreateUserDataObject(appCntxt->vxContext, "tivx_display_params_t",
                                                                sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(appCntxt->bb_display_config);

        vxSetReferenceName((vx_reference)appCntxt->bb_display_config, "3DBoundingBox_DisplayConfiguration");

        appCntxt->bb_display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        appCntxt->bb_display_params.pipeId = 0;
        appCntxt->bb_display_params.outWidth = INPUT_DISPLAY_WIDTH;
        appCntxt->bb_display_params.outHeight = INPUT_DISPLAY_HEIGHT;
        appCntxt->bb_display_params.posX = 0;
        appCntxt->bb_display_params.posY = 300;

        vxStatus = vxCopyUserDataObject(appCntxt->bb_display_config, 0, sizeof(tivx_display_params_t), &appCntxt->bb_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(vxStatus == VX_SUCCESS);

        // create disparity display node
        appCntxt->node_image_display = tivxDisplayNode(
            appCntxt->vxDispGraph,
            appCntxt->bb_display_config,
            appCntxt->vxBBImage);

        vxStatus = vxGetStatus((vx_reference)appCntxt->node_image_display);
        if (VX_SUCCESS != vxStatus)
        {
            PTK_printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        vxStatus = vxSetNodeTarget(appCntxt->node_image_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY2);
        APP_ASSERT(vxStatus == VX_SUCCESS);
        vxSetReferenceName((vx_reference)appCntxt->node_image_display, "ImageDisplay");
#endif
    }

    vxStatus = vxVerifyGraph(appCntxt->vxDispGraph);
    APP_ASSERT(vxStatus == VX_SUCCESS);


    return vxStatus;
}

void ESTOP_APP_deleteDisplayGraph(ESTOP_APP_Context *appCntxt)
{
    vxReleaseImage(&appCntxt->vxDispSSImage);
    vxReleaseImage(&appCntxt->vxBBImage);
    vxReleaseImage(&appCntxt->vxDispRightImage);
    vxReleaseImage(&appCntxt->vxDisparityCC);
    vxReleaseImage(&appCntxt->vxDisparity16);
    vxReleaseUserDataObject(&appCntxt->vxDisp3DBoundBox);

#if !defined(PC)
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == appCntxt->display_option))
    {
        vxReleaseUserDataObject(&appCntxt->ss_display_config);
    }
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (1 == appCntxt->display_option))
    {
        vxReleaseUserDataObject(&appCntxt->bb_display_config);
    }
#endif

    vxReleaseGraph(&appCntxt->vxDispGraph);
}

static void ESTOP_APP_exitProcThreads(ESTOP_APP_Context *appCntxt,
                                      bool            detach)
{
    vx_status vxStatus;

    appCntxt->exitInputDataProcess = true;

    if (appCntxt->inputDataThread.joinable())
    {
        if (detach)
        {
            /* Exiting under CTRL-C. Detach. */
            appCntxt->inputDataThread.detach();
        }
        else
        {
            /* Block on the input data thread exit. */
            appCntxt->inputDataThread.join();
        }
    }

    /* Let the event handler thread exit. */
    vxStatus = vxSendUserEvent(appCntxt->vxContext,
                               ESTOP_APP_USER_EVT_EXIT,
                               NULL);

    if (vxStatus != VX_SUCCESS)
    {
        PTK_printf("[%s:%d] vxSendUserEvent() failed.\n");
    }

    if (appCntxt->evtHdlrThread.joinable())
    {
        appCntxt->evtHdlrThread.join();
    }

    /* Let the dlr thread exit. */
    appCntxt->exitDlrThread = true;

    if (appCntxt->dlrDataReadySem)
    {
        appCntxt->dlrDataReadySem->notify();
    }

    if (appCntxt->dlrThread.joinable())
    {
        appCntxt->dlrThread.join();
    }

    /* Let the display thread exit. */
    appCntxt->exitDisplayThread = true;

    if (appCntxt->displayCtrlSem)
    {
        appCntxt->displayCtrlSem->notify();
    }

    if (appCntxt->dispThread.joinable())
    {
        appCntxt->dispThread.join();
    }
}

void ESTOP_APP_cleanupHdlr(ESTOP_APP_Context *appCntxt)
{
    /* Wait for the threads to exit. */
    ESTOP_APP_exitProcThreads(appCntxt, false);

    PTK_printf("\nPress ENTER key to exit.\n");
    fflush(stdout);
    getchar();

    if (appCntxt->rtLogEnable == 1)
    {
        char name[256];

        snprintf(name, 255, "%s.bin", APP_ESTOP_NAME);
        tivxLogRtTraceExportToFile(name);
    }

    /* Release the objects. */
    SDELDCAPPLIB_delete(&appCntxt->sdeLdcHdl);

    if (appCntxt->sdeAlgoType == 0)
    {
        SL_SDEAPPLIB_delete(&appCntxt->slSdeHdl);
    }
    else if (appCntxt->sdeAlgoType == 1)
    {
        ML_SDEAPPLIB_delete(&appCntxt->mlSdeHdl);
    }

    SS_DETECT_APPLIB_delete(&appCntxt->ssDetectHdl);

    SEMSEG_CNN_APPLIB_delete(&appCntxt->ssCnnHdl);

    /* De-initialize the Application context. */
    ESTOP_APP_deleteDisplayGraph(appCntxt);

    ESTOP_APP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);
}

static void ESTOP_APP_reset(ESTOP_APP_Context * appCntxt)
{
    vx_status vxStatus;

    vx_node ogNode = SS_DETECT_APPLIB_getOGNode(appCntxt->ssDetectHdl);
    vxStatus = tivxNodeSendCommand(ogNode,
                                   0,
                                   TIVX_KERNEL_OCCUPANCY_GRID_DETECTION_RESET,
                                   NULL,
                                   0);
    if (vxStatus != VX_SUCCESS)
    {
        PTK_printf("[%s:%d] tivxNodeSendCommand() failed.\n",
                   __FUNCTION__,
                   __LINE__);

    }

    /* Reset the performance capture initialization flag. */
    appCntxt->startPerfCapt = false;
}

static void ESTOP_APP_inputDataThread(ESTOP_APP_Context *appCntxt)
{
 
    vx_status   vxStatus;

    PTK_printf("[%s] Launched input data processing thread.\n", __FUNCTION__);

    // init display frame no
    appCntxt->displayFrmNo    = appCntxt->start_fileno;
    appCntxt->processFinished = false;

    while (true)
    {
        /* Reset ground model */
        ESTOP_APP_reset(appCntxt);

        /* Execute the graph. */
        vxStatus = ESTOP_APP_run(appCntxt);

        if (appCntxt->exitInputDataProcess)
        {
            break;
        }

        // reinit frame no to be displayed
        appCntxt->displayFrmNo = appCntxt->start_fileno;
    }

    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        raise(SIGINT);
    }

    appCntxt->processFinished = true;
}


static void ESTOP_APP_evtHdlrThread(ESTOP_APP_Context *appCntxt)
{
    vx_event_t evt;
    vx_status vxStatus = VX_SUCCESS;

    PTK_printf("[%s] Launched.\n", __FUNCTION__);

    /* Clear any pending events. The third argument is do_not_block = true. */
    while (vxStatus == VX_SUCCESS)
    {
        vxStatus = vxWaitEvent(appCntxt->vxContext, &evt, vx_true_e);
    }

    while (true)
    {
        vxStatus = vxWaitEvent(appCntxt->vxContext, &evt, vx_false_e);

        if (vxStatus == VX_SUCCESS)
        {
            if (evt.type == VX_EVENT_USER)
            {
                if (evt.app_value == ESTOP_APP_USER_EVT_EXIT)
                {
                    break;
                } 
                else if (evt.app_value == ESTOP_APP_SCALER_OUT_AVAIL_EVENT)
                {
                    // enqueue vxOutTensor
                    ESTOP_APP_graphParams * desc;
                    vxStatus = ESTOP_APP_popDLRInputDesc(appCntxt, &desc);

                    vxStatus = vxGraphParameterEnqueueReadyRef(appCntxt->vxGraph,
                                                               appCntxt->numGraphParams - 1,
                                                               (vx_reference*)&desc->vxOutTensor,
                                                               1);
                    if (vxStatus != (vx_status)VX_SUCCESS)
                    {
                        PTK_printf("[%s:%d] vxGraphParameterEnqueueReadyRef(%d) "
                                   "failed\n", __FUNCTION__, __LINE__, appCntxt->numGraphParams - 1);
                     
                    }
                }
            }

            if (evt.type == VX_EVENT_GRAPH_COMPLETED || evt.type == VX_EVENT_NODE_COMPLETED)
            {
                ESTOP_APP_processEvent(appCntxt, &evt);
            }
        }

    } // while (true)
}

static void ESTOP_APP_dlrThread(ESTOP_APP_Context *appCntxt)
{
    SEMSEG_CNN_APPLIB_createParams *ssCnnCreateParams;

    CM_DLRNodeCntxt                *dlrObj;
    CM_DLRNodeInputInfo            *dlrInput;
    CM_DLRNodeOutputInfo           *dlrOutput;


    ssCnnCreateParams = &appCntxt->ssCnnCreateParams;

    dlrObj            = &appCntxt->dlrObj;
    dlrInput          = SEMSEG_CNN_APPLIB_getDLRNodeInputInfo(appCntxt->ssCnnHdl);
    dlrOutput         = SEMSEG_CNN_APPLIB_getDLRNodeOutputInfo(appCntxt->ssCnnHdl);

    while (true)
    {
        ESTOP_APP_graphParams  *desc;
        vx_status               vxStatus;

        if (appCntxt->exitDlrThread == true)
        {
            break;
        }

        /* Wait fot the input buffer. */
        appCntxt->dlrDataReadySem->wait();

        vxStatus = ESTOP_APP_getDLRInputDesc(appCntxt, &desc);

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            vxStatus = SEMSEG_CNN_APPLIB_preProcess(desc->vxScalerOut, desc->dlrInputBuff, ssCnnCreateParams);

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_preProcess() "
                           "failed.\n",
                            __FUNCTION__, __LINE__);
            }
            else
            {
                int32_t status;

                dlrInput->info[0].data  = desc->dlrInputBuff;
                dlrOutput->info[0].data = desc->dlrOutputBuff;

                status = CM_dlrNodeCntxtProcess(dlrObj, dlrInput, dlrOutput);

                if (status < 0)
                {
                    PTK_printf("[%s:%d] CM_dlrNodeCntxtProcess() failed.\n",
                                __FUNCTION__, __LINE__);
                }
                else
                {
                    if (ssCnnCreateParams->enablePostProcNode)
                    {
                        /* Create the output object for display. */
                        vxStatus =
                            SEMSEG_CNN_APPLIB_postProcess(desc->vxScalerOut, desc->dlrOutputBuff, ssCnnCreateParams);

                        if (vxStatus != (vx_status)VX_SUCCESS)
                        {
                            PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_postProcess() "
                                       "failed.\n",
                                        __FUNCTION__, __LINE__);
                        }
                    }
                    
                    // we should create output tensor always
                    {
                        /* Create an output tensor. */
                        vxStatus =
                            SEMSEG_CNN_APPLIB_createOutTensor(desc->vxOutTensor, desc->dlrOutputBuff, ssCnnCreateParams);


                        if (vxStatus != (vx_status)VX_SUCCESS)
                        {
                            PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_createOutTensor() "
                                       "failed.\n",
                                        __FUNCTION__, __LINE__);
                        }
                    }

                    if (vxStatus == (vx_status)VX_SUCCESS)
                    {
                        /* Let the usr know that output is ready. */
                        vxStatus =
                            vxSendUserEvent(appCntxt->vxContext,
                                            ESTOP_APP_SCALER_OUT_AVAIL_EVENT,
                                            NULL);

                        if (vxStatus != (vx_status)VX_SUCCESS)
                        {
                            PTK_printf("[%s:%d] vxSendUserEvent() failed.\n",
                                        __FUNCTION__, __LINE__);
                        }
                    }
                }
            }

        } // if (vxStatus == (vx_status)VX_SUCCESS)

    } // while (true)

    PTK_printf("[%s] Exiting.\n", __FUNCTION__);

}

static void ESTOP_APP_dispThread(ESTOP_APP_Context *appCntxt)
{
    vx_status vxStatus = VX_SUCCESS;

#if defined(PC)
    char output_file_name_ss_img[ESTOP_APP_MAX_LINE_LEN];
    char output_file_name_bb_img[ESTOP_APP_MAX_LINE_LEN];
    char output_file_name_disp_img[ESTOP_APP_MAX_LINE_LEN];
#endif

    PTK_printf("[%s] Launched.\n", __FUNCTION__);

    while (true)
    {
        /* Wait for the display ctrl ready semaphore. */
        if (appCntxt->displayCtrlSem)
        {
            appCntxt->displayCtrlSem->wait();
        }

        if (appCntxt->exitDisplayThread == true)
        {
            break;
        }

        ESTOP_APP_getOutBuff(appCntxt, appCntxt->vxDispRightImage, appCntxt->vxDispSSImage, appCntxt->vxDisp3DBoundBox, appCntxt->vxDisparity16);
        ESTOP_APP_releaseOutBuff(appCntxt);
  
        /* Wakeup the input data thread. */
        if (appCntxt->dataReadySem)
        {
            appCntxt->dataReadySem->notify();
        }

        if (1 == appCntxt->display_option)
        {
            // Create image to be displayed
            // Color-coded disparity map is created, but not displayed
            ESTOP_APP_createDisparityCCImage(appCntxt);
            ESTOP_APP_overlay3DBB(appCntxt);

#if !defined(PC)
            vxStatus = vxScheduleGraph(appCntxt->vxDispGraph);
            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] vxScheduleGraph() failed\n",
                            __FUNCTION__, __LINE__);

                break;
            }

            if (vxStatus == (vx_status)VX_SUCCESS)
            {
                vxStatus = vxWaitGraph(appCntxt->vxDispGraph);
                if (vxStatus != (vx_status)VX_SUCCESS)
                {
                    PTK_printf("[%s:%d] vxWaitGraph() failed\n",
                                __FUNCTION__, __LINE__);

                    break;
                }
            }
#endif

#if defined(PC)
            // save output
            snprintf(output_file_name_bb_img, ESTOP_APP_MAX_LINE_LEN, "%s/bb_%05d.bmp",
                     appCntxt->output_file_path,
                     appCntxt->displayFrmNo);

            snprintf(output_file_name_disp_img, ESTOP_APP_MAX_LINE_LEN, "%s/disparity_%05d.bmp",
                     appCntxt->output_file_path,
                     appCntxt->displayFrmNo);

            tivx_utils_save_vximage_to_bmpfile(output_file_name_bb_img, appCntxt->vxBBImage);
            tivx_utils_save_vximage_to_bmpfile(output_file_name_disp_img, appCntxt->vxDisparityCC);

            if (appCntxt->enablePostProcNode)
            {
                snprintf(output_file_name_ss_img, ESTOP_APP_MAX_LINE_LEN, "%s/ss_%05d.yuv",
                         appCntxt->output_file_path,
                         appCntxt->displayFrmNo);

                vxStatus = ptkdemo_save_vximage_to_yuvfile(appCntxt->vxDispSSImage, output_file_name_ss_img);
                if (vxStatus != (vx_status)VX_SUCCESS)
                {
                    PTK_printf("[%s:%d] ptkdemo_save_vximage_to_yuvfile() "
                               "failed\n", __FUNCTION__, __LINE__);
                }
            }
#endif
        }

        appCntxt->displayFrmNo++;
    }

    PTK_printf("[%s] Exiting.\n", __FUNCTION__);
}


void ESTOP_APP_launchProcThreads(ESTOP_APP_Context *appCntxt)
{

    /* Launch the input data thread. */
    appCntxt->inputDataThread = std::thread(ESTOP_APP_inputDataThread, appCntxt);

    /* Launch the event handler thread. */
    appCntxt->evtHdlrThread = std::thread(ESTOP_APP_evtHdlrThread, appCntxt);

    /* Launch the dlr thread */
    appCntxt->dlrThread = std::thread(ESTOP_APP_dlrThread, appCntxt);

    /* Launch the display thread. */
    appCntxt->dispThread = std::thread(ESTOP_APP_dispThread, appCntxt);

}

void ESTOP_APP_intSigHandler(ESTOP_APP_Context *appCntxt, int sig)
{
    // wait for frame processing to finish
    appCntxt->end_fileno = 0;
    appCntxt->exitInputDataProcess = true;

    while(!appCntxt->processFinished)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    /* Wait for the threads to exit. */
    ESTOP_APP_exitProcThreads(appCntxt, true);

    /* Release the objects. */
    SDELDCAPPLIB_delete(&appCntxt->sdeLdcHdl);

    if (appCntxt->sdeAlgoType == 0)
    {
        SL_SDEAPPLIB_delete(&appCntxt->slSdeHdl);
    }
    else if (appCntxt->sdeAlgoType == 1)
    {
        ML_SDEAPPLIB_delete(&appCntxt->mlSdeHdl);
    }

    SS_DETECT_APPLIB_delete(&appCntxt->ssDetectHdl);

    SEMSEG_CNN_APPLIB_delete(&appCntxt->ssCnnHdl);

    /* Delete display graph */
    ESTOP_APP_deleteDisplayGraph(appCntxt);

    /* De-initialize the Application context. */
    ESTOP_APP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);

    exit(0);
}
