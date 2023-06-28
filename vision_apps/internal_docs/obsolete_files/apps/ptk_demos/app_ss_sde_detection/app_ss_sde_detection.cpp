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

#include "app_ss_sde_detection_main.h"
#include "app_ss_sde_detection.h"

#include <app_ptk_demo_common.h>
#include <app_ptk_demo_disparity.h>

#define APP_SS_SDE_DETECT_NAME    "apps_ss_sde_detection"

void SS_DETECT_APP_setPcParams(SS_DETECT_APP_Context *appCntxt);
void SS_DETECT_APP_setOgParams(SS_DETECT_APP_Context *appCntxt);

#if !defined(PC)
static void SS_DETECT_APP_drawGraphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    return;
}
#endif

void SS_DETECT_APP_parseCfgFile(SS_DETECT_APP_Context *appCntxt, const char *cfg_file_name)
{
    FILE  * fp = fopen(cfg_file_name, "r");
    char  * pParamStr;
    char  * pValueStr;
    char  * pSLine;
    char  * basePath;
    char    paramSt[SS_DETECT_APP_MAX_LINE_LEN];
    char    valueSt[SS_DETECT_APP_MAX_LINE_LEN];
    char    sLine[SS_DETECT_APP_MAX_LINE_LEN];

    // set default parameters
    appCntxt->display_option       = 0;
    appCntxt->width                = 1280;
    appCntxt->height               = 720;
    appCntxt->tensor_width         = 768;
    appCntxt->tensor_height        = 432;
    appCntxt->confidence_threshold = 0;
    appCntxt->is_interactive       = 0;
    appCntxt->isROSInterface       = 0;
    appCntxt->inputFormat          = 0;

    appCntxt->exportGraph          = 0;
    appCntxt->rtLogEnable          = 0;

    appCntxt->ssDetectCreateParams.pipelineDepth = SS_DETECT_APPLIB_MAX_PIPELINE_DEPTH;

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
        pSLine = fgets(pSLine, SS_DETECT_APP_MAX_LINE_LEN, fp);

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
        if (strcmp(pParamStr, "image_file_path") == 0)
        {
            snprintf(appCntxt->image_file_path, SS_DETECT_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        } 
        else if (strcmp(pParamStr, "tensor_file_path") == 0)
        {
            snprintf(appCntxt->tensor_file_path, SS_DETECT_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "disparity_file_path") == 0)
        {
            snprintf(appCntxt->disparity_file_path, SS_DETECT_APP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "output_file_path") == 0)
        {
            snprintf(appCntxt->output_file_path, SS_DETECT_APP_MAX_LINE_LEN,
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
        else if (strcmp(pParamStr, "display_option") == 0)
        {
            appCntxt->display_option = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "is_interactive") == 0)
        {
            appCntxt->is_interactive = atoi(pValueStr);
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
        else if (strcmp(pParamStr, "sde_confidence_threshold") == 0)
        {
            appCntxt->confidence_threshold = atoi(pValueStr);
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
            appCntxt->ssDetectCreateParams.pipelineDepth  = atoi(pValueStr);
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

    appCntxt->renderPeriod = 0;

    return;

} /* SS_DETECT_APP_parseCfgFile */

void SS_DETECT_APP_setPcParams(SS_DETECT_APP_Context *appCntxt)
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


void SS_DETECT_APP_setOgParams(SS_DETECT_APP_Context *appCntxt)
{
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.width        = appCntxt->width;
    appCntxt->ssDetectCreateParams.ogCfg.cfgParams.height       = appCntxt->height;

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


void SS_DETECT_APP_setAllParams(SS_DETECT_APP_Context *appCntxt)
{
    SS_DETECT_APPLIB_createParams * createParams = &appCntxt->ssDetectCreateParams;

    createParams->width            = appCntxt->width;
    createParams->height           = appCntxt->height;
    createParams->tensorWidth      = appCntxt->tensor_width;
    createParams->tensorHeight     = appCntxt->tensor_height;

    createParams->isROSInterface   = appCntxt->isROSInterface;
    createParams->inputFormat      = appCntxt->inputFormat;
    createParams->exportGraph      = appCntxt->exportGraph;
    createParams->rtLogEnable      = appCntxt->rtLogEnable;
    createParams->vxEvtAppValBase  = 0;

    SS_DETECT_APP_setPcParams(appCntxt);
    SS_DETECT_APP_setOgParams(appCntxt);
}


int32_t SS_DETECT_APP_createDraw(SS_DETECT_APP_Context *appCntxt)
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

void SS_DETECT_APP_init(SS_DETECT_APP_Context *appCntxt)
{
    int32_t status;
    SS_DETECT_APPLIB_createParams  * ssDetectCreateParams;

    status = appInit();
    PTK_assert(status == 0);

    // OpenVX initialization
    appCntxt->vxContext = vxCreateContext();
    PTK_assert(appCntxt->vxContext);

    // Load HWA kernels
    tivxStereoLoadKernels(appCntxt->vxContext);
    tivxHwaLoadKernels(appCntxt->vxContext);

    ssDetectCreateParams = &appCntxt->ssDetectCreateParams;
    ssDetectCreateParams->vxContext = appCntxt->vxContext;

    if (!appCntxt->isROSInterface)
    {
        vx_size input_size[3];

        // input right image
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            appCntxt->vxRightImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
        } else{
            appCntxt->vxRightImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
        }
        PTK_assert(appCntxt->vxRightImage);

        // input disparity images
        appCntxt->vxDisparity16 = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_S16);
        PTK_assert(appCntxt->vxDisparity16);

        // create tensor
        input_size[0] = appCntxt->tensor_width;
        input_size[1] = appCntxt->tensor_height;
        input_size[2] = 1;
        appCntxt->vxSSMapTensor = vxCreateTensor(appCntxt->vxContext, 3, input_size, VX_TYPE_UINT8, 0);
    }

    // create applib
    appCntxt->ssDetectHdl  = SS_DETECT_APPLIB_create(ssDetectCreateParams);
    appCntxt->dataReadySem = new Semaphore(ssDetectCreateParams->pipelineDepth);

    appCntxt->exitInputDataProcess = false;

    if (1 == appCntxt->display_option)
    {
#if !defined(PC)
        if (!appCntxt->isROSInterface)
        {
            app_grpx_init_prms_t grpx_prms;
            appGrpxInitParamsInit(&grpx_prms, appCntxt->vxContext);
            grpx_prms.draw_callback = SS_DETECT_APP_drawGraphics;
            appGrpxInit(&grpx_prms);
        }
#endif

        SS_DETECT_APP_createDraw(appCntxt);
    }
}


/*
 * Create color coded disparity imag
 * the RGB image for display
 */
void SS_DETECT_APP_createDisparityCCImage(SS_DETECT_APP_Context *appCntxt)
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
    status = vxMapImagePatch(appCntxt->vxDispDisparity16,
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

    vxUnmapImagePatch(appCntxt->vxDispDisparity16, map_src_id);
    vxUnmapImagePatch(appCntxt->vxDisparityCC, map_dst_id);
}

void SS_DETECT_APP_overlay3DBB(SS_DETECT_APP_Context *appCntxt)
{
    int32_t                          i, j;
    uint32_t                         value;
    vx_status                        vxStatus;
    vx_rectangle_t                   rect;
    vx_imagepatch_addressing_t       image_addr;
    vx_map_id                        map_id;
    vx_uint32                        img_format;
    uint8_t                        * data_ptr;
    PTK_Alg_StereoOG_obs3DBox      * obs3DBB;
    PTK_Alg_StereoOG_BoxProp       * boxProp; 

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = appCntxt->width;
    rect.end_y = appCntxt->height;

    vxQueryImage(appCntxt->vxDispRightImage, VX_IMAGE_FORMAT, &img_format, sizeof(vx_uint32));

    vxStatus = vxMapImagePatch(appCntxt->vxDispRightImage,
                             &rect,
                             0,
                             &map_id,
                             &image_addr,
                             (void **)&data_ptr,
                             VX_READ_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);

    vxUnmapImagePatch(appCntxt->vxDispRightImage, map_id);

    if (vxStatus == VX_SUCCESS)
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
        } else
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

        obs3DBB = (tivx_ss_sde_obs_3d_bound_box_t *) ptkdemo_getUserObjDataPayload(appCntxt->vx3DBoundBox);

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
            else
            {
                PTK_printf("[%s:%d] Unsupported Class Id\n",
                            __FUNCTION__, __LINE__);
            }

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
        }

        // get properties of vxBBImage
        vxStatus = vxMapImagePatch(appCntxt->vxBBImage,
                             &rect,
                             0,
                             &map_id,
                             &image_addr,
                             (void **)&data_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);

        if (vxStatus == VX_SUCCESS)
        {
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
}


void SS_DETECT_APP_run(SS_DETECT_APP_Context *appCntxt)
{
    uint32_t i;
    vx_status vxStatus = VX_SUCCESS;
    char     temp[SS_DETECT_APP_MAX_LINE_LEN];

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

        // left image
        strcpy(appCntxt->image_file_name, appCntxt->image_file_path);
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            sprintf(temp, "/%010d.bmp", i);
            if (strlen(appCntxt->image_file_name) + strlen(temp) >= SS_DETECT_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->image_file_name, temp);
                vxStatus = tivx_utils_load_vximage_from_bmpfile(appCntxt->vxRightImage, appCntxt->image_file_name, vx_true_e);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] tivx_utils_load_vximage_from_bmpfile() failed.\n");
            }
        } else
        {
            sprintf(temp, "/%010d.yuv", i);
            if (strlen(appCntxt->image_file_name) + strlen(temp) >= SS_DETECT_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->image_file_name, temp);
                vxStatus = ptkdemo_load_vximage_from_yuvfile(appCntxt->vxRightImage, appCntxt->image_file_name);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ptkdemo_load_vximage_from_yuvfile() failed.\n");
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            // read disparity maps
            strcpy(appCntxt->disparity_file_name, appCntxt->disparity_file_path);
            sprintf(temp, "/%010d.sde", i);
            if (strlen(appCntxt->disparity_file_name) + strlen(temp) >= SS_DETECT_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->disparity_file_name, temp);
                ptkdemo_load_vximage_from_sdefile(appCntxt->vxDisparity16, appCntxt->disparity_file_name);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ptkdemo_load_vximage_from_sdefile() failed.\n");
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            // read input tensor
            strcpy(appCntxt->tensor_file_name, appCntxt->tensor_file_path);
            sprintf(temp, "/%010d.bin", i);
            if (strlen(appCntxt->tensor_file_name) + strlen(temp) >= SS_DETECT_APP_MAX_LINE_LEN)
            {
                vxStatus = (vx_status)VX_FAILURE;
            } else
            {
                strcat(appCntxt->tensor_file_name, temp);
                ptkdemo_load_vxtensor_from_file(appCntxt->vxSSMapTensor, appCntxt->tensor_file_name);
            }

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ptkdemo_load_vxtensor_from_file() failed.\n");
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            // then run SS_DETECT applib
            SS_DETECT_APPLIB_process(appCntxt->ssDetectHdl, appCntxt->vxRightImage, appCntxt->vxDisparity16, appCntxt->vxSSMapTensor);
        }

        appCntxt->frameCnt++;
    }

    SS_DETECT_APPLIB_waitGraph(appCntxt->ssDetectHdl);


    PTK_printf("\e[KProcessed %d frames.\n\e[A", appCntxt->frameCnt);
}

void SS_DETECT_APP_deInit(SS_DETECT_APP_Context *appCntxt)
{
    int32_t status;

    if (!appCntxt->isROSInterface)
    {
        if (appCntxt->dataReadySem)
        {
            delete appCntxt->dataReadySem;
        }
        
        // Input right and disparity16 images
        vxReleaseImage(&appCntxt->vxRightImage);
        vxReleaseImage(&appCntxt->vxDisparity16);
        vxReleaseTensor(&appCntxt->vxSSMapTensor);
    }

    // Unload HWA kernels
    tivxStereoUnLoadKernels(appCntxt->vxContext);
    tivxHwaUnLoadKernels(appCntxt->vxContext);

    if (1 == appCntxt->display_option)
    {
        tivxMemFree(appCntxt->pDisplayBuf565, appCntxt->width * appCntxt->height * sizeof(uint16_t), TIVX_MEM_EXTERNAL);
        Draw2D_delete(appCntxt->pHndl);
#if !defined(PC)
        if (!appCntxt->isROSInterface)
        {
            appGrpxDeInit();
        }
#endif
    }

    /* Release the context. */
    vxReleaseContext(&appCntxt->vxContext);

    status = appDeInit();
    PTK_assert(status == 0);
}

void SS_DETECT_APP_createDispalyGraph(SS_DETECT_APP_Context *appCntxt)
{
    int32_t status;

    // create graph
    appCntxt->graph_display = vxCreateGraph(appCntxt->vxContext);
    APP_ASSERT_VALID_REF(appCntxt->graph_display);
    vxSetReferenceName((vx_reference)appCntxt->graph_display, "Display");

    // create objects
    appCntxt->vxBBImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_RGB);
    APP_ASSERT_VALID_REF(appCntxt->vxBBImage);
    vxSetReferenceName((vx_reference)appCntxt->vxBBImage, "InputImage_BoundingBox_U8");

    appCntxt->vxDisparityCC = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_RGB);
    APP_ASSERT_VALID_REF(appCntxt->vxDisparityCC);
    vxSetReferenceName((vx_reference)appCntxt->vxDisparityCC, "Stereo_Disparity_CC");


    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == appCntxt->display_option) && !appCntxt->isROSInterface)
    {
#if !defined(PC)
        memset(&appCntxt->disparity_display_params, 0, sizeof(tivx_display_params_t));
        appCntxt->disparity_display_config = vxCreateUserDataObject(appCntxt->vxContext, "tivx_display_params_t",
                                                                    sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(appCntxt->disparity_display_config);

        vxSetReferenceName((vx_reference)appCntxt->disparity_display_config, "DisparityDisplayConfiguration");

        appCntxt->disparity_display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        appCntxt->disparity_display_params.pipeId = 2;
        appCntxt->disparity_display_params.outWidth  = OUTPUT_DISPLAY_WIDTH;
        appCntxt->disparity_display_params.outHeight = OUTPUT_DISPLAY_HEIGHT;
        appCntxt->disparity_display_params.posX = 960;
        appCntxt->disparity_display_params.posY = 300;

        status = vxCopyUserDataObject(appCntxt->disparity_display_config, 0, sizeof(tivx_display_params_t), &appCntxt->disparity_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(status == VX_SUCCESS);

        // create disparity display node
        appCntxt->node_disparity_display = tivxDisplayNode(
            appCntxt->graph_display,
            appCntxt->disparity_display_config,
            appCntxt->vxDisparityCC);
        status = vxGetStatus((vx_reference)appCntxt->node_disparity_display);
        if (VX_SUCCESS != status)
        {
            PTK_printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        APP_ASSERT(VX_SUCCESS == status);
        status = vxSetNodeTarget(appCntxt->node_disparity_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
        APP_ASSERT(status == VX_SUCCESS);
        vxSetReferenceName((vx_reference)appCntxt->node_disparity_display, "DisparityDisplay");
#endif
    }

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (1 == appCntxt->display_option) && !appCntxt->isROSInterface)
    {
#if !defined(PC)
        memset(&appCntxt->image_display_params, 0, sizeof(tivx_display_params_t));
        appCntxt->image_display_config = vxCreateUserDataObject(appCntxt->vxContext, "tivx_display_params_t",
                                                                sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(appCntxt->image_display_config);

        vxSetReferenceName((vx_reference)appCntxt->image_display_config, "ImageDisplayConfiguration");

        appCntxt->image_display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        appCntxt->image_display_params.pipeId = 0;
        appCntxt->image_display_params.outWidth = INPUT_DISPLAY_WIDTH;
        appCntxt->image_display_params.outHeight = INPUT_DISPLAY_HEIGHT;
        appCntxt->image_display_params.posX = 0;
        appCntxt->image_display_params.posY = 300;

        status = vxCopyUserDataObject(appCntxt->image_display_config, 0, sizeof(tivx_display_params_t), &appCntxt->image_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(status == VX_SUCCESS);

        // create disparity display node
        appCntxt->node_image_display = tivxDisplayNode(
            appCntxt->graph_display,
            appCntxt->image_display_config,
            appCntxt->vxBBImage);
        status = vxGetStatus((vx_reference)appCntxt->node_image_display);
        if (VX_SUCCESS != status)
        {
            PTK_printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        APP_ASSERT(VX_SUCCESS == status);
        status = vxSetNodeTarget(appCntxt->node_image_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY2);
        APP_ASSERT(status == VX_SUCCESS);
        vxSetReferenceName((vx_reference)appCntxt->node_image_display, "ImageDisplay");
#endif
    }

    status = vxVerifyGraph(appCntxt->graph_display);
    APP_ASSERT(status == VX_SUCCESS);
}

void SS_DETECT_APP_deleteDisplayGraph(SS_DETECT_APP_Context *appCntxt)
{
    vxReleaseImage(&appCntxt->vxBBImage);
    vxReleaseImage(&appCntxt->vxDisparityCC);

#if !defined(PC)
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == appCntxt->display_option) && !appCntxt->isROSInterface)
    {
        vxReleaseUserDataObject(&appCntxt->disparity_display_config);
    }
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (1 == appCntxt->display_option) && !appCntxt->isROSInterface)
    {
        vxReleaseUserDataObject(&appCntxt->image_display_config);
    }
#endif

    vxReleaseGraph(&appCntxt->graph_display);
}

static void SS_DETECT_APP_exitProcThreads(SS_DETECT_APP_Context *appCntxt,
                                          bool            detach)
{
    vx_status vxStatus;

    if (!appCntxt->isROSInterface)
    {
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
    }

    /* Let the event handler thread exit. */
    vxStatus = vxSendUserEvent(appCntxt->vxContext,
                               SS_DETECT_APP_USER_EVT_EXIT,
                               NULL);

    if (vxStatus != VX_SUCCESS)
    {
        PTK_printf("[%s:%d] vxSendUserEvent() failed.\n");
    }

    if (appCntxt->evtHdlrThread.joinable())
    {
        appCntxt->evtHdlrThread.join();
    }
}

void SS_DETECT_APP_cleanupHdlr(SS_DETECT_APP_Context *appCntxt)
{
    /* Wait for the threads to exit. */
    SS_DETECT_APP_exitProcThreads(appCntxt, false);

    PTK_printf("\nPress ENTER key to exit.\n");
    fflush(stdout);
    getchar();

    if (appCntxt->rtLogEnable == 1)
    {
        char name[256];

        snprintf(name, 255, "%s.bin", APP_SS_SDE_DETECT_NAME);
        tivxLogRtTraceExportToFile(name);
    }

    /* Release the Application context. */
    SS_DETECT_APPLIB_delete(&appCntxt->ssDetectHdl);

    /* De-initialize the Application context. */
    SS_DETECT_APP_deleteDisplayGraph(appCntxt);

    SS_DETECT_APP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);
}

static void SS_DETECT_APP_reset(SS_DETECT_APP_Context * appCntxt)
{
    int32_t status;

    status = SS_DETECT_APPLIB_reset(appCntxt->ssDetectHdl);
    if (status < 0)
    {
        PTK_printf("[%s:%d] SS_DETECT_APPLIB_reset() failed.\n",
                   __FUNCTION__,
                   __LINE__);
    }
}

static void SS_DETECT_APP_inputDataThread(SS_DETECT_APP_Context *appCntxt)
{
    PTK_printf("[%s] Launched input data processing thread.\n", __FUNCTION__);

    // init display frame no
    appCntxt->displayFrmNo    = appCntxt->start_fileno;
    appCntxt->processFinished = false;

    while (true)
    {
        /* Reset ground model */
        SS_DETECT_APP_reset(appCntxt);

        /* Execute the graph. */
        SS_DETECT_APP_run(appCntxt);

        if (appCntxt->exitInputDataProcess)
        {
            break;
        }

        // reinit frame no to be displayed
        appCntxt->displayFrmNo = appCntxt->start_fileno;
    }

    appCntxt->processFinished = true;
}


static void SS_DETECT_APP_evtHdlrThread(SS_DETECT_APP_Context *appCntxt)
{
    vx_event_t evt;
    vx_status vxStatus;
    
    int32_t status;

#if defined(PC)
    char output_file_name_disparity_img[SS_DETECT_APP_MAX_LINE_LEN];
    char output_file_name_bb_img[SS_DETECT_APP_MAX_LINE_LEN]; 
#endif

    vxStatus = VX_SUCCESS;

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
                if (evt.app_value == SS_DETECT_APP_USER_EVT_EXIT)
                {
                    break;
                }
            }

            if (evt.type == VX_EVENT_GRAPH_COMPLETED)
            {
                SS_DETECT_APPLIB_processEvent(appCntxt->ssDetectHdl, &evt);
                SS_DETECT_APPLIB_getOutBuff(appCntxt->ssDetectHdl, &appCntxt->vxDispRightImage, &appCntxt->vxDispDisparity16, &appCntxt->vx3DBoundBox);

                if (1 == appCntxt->display_option)
                {
                    SS_DETECT_APP_createDisparityCCImage(appCntxt);
                    SS_DETECT_APP_overlay3DBB(appCntxt);
                }

#if !defined(PC)
                status = vxScheduleGraph(appCntxt->graph_display);
                PTK_assert(VX_SUCCESS == status);
                status = vxWaitGraph(appCntxt->graph_display);
#endif

                SS_DETECT_APPLIB_releaseOutBuff(appCntxt->ssDetectHdl);

                /* Wakeup the input data thread. */
                if (appCntxt->dataReadySem)
                {
                    appCntxt->dataReadySem->notify();
                }

                if (!appCntxt->isROSInterface)
                {
#if defined(PC)
                    if (1 == appCntxt->display_option)
                    {
                        // save output
                        snprintf(output_file_name_disparity_img, SS_DETECT_APP_MAX_LINE_LEN, "%s/disp_%05d.bmp",
                                 appCntxt->output_file_path,
                                 appCntxt->displayFrmNo);
    
                        snprintf(output_file_name_bb_img, SS_DETECT_APP_MAX_LINE_LEN, "%s/bb_%05d.bmp",
                                 appCntxt->output_file_path,
                                 appCntxt->displayFrmNo);

                        tivx_utils_save_vximage_to_bmpfile(output_file_name_disparity_img, appCntxt->vxDisparityCC);
                        tivx_utils_save_vximage_to_bmpfile(output_file_name_bb_img, appCntxt->vxBBImage);
                    }
#endif
                    // In interaction mode, sometimes diplayFramNo is updated after it was reset to
                    // the starting frame in input data thread (if writing output images takes too long).
                    // In this case, the first frame number is "starting frame no + 1"
                    // It only affects the saved file name index.
                    appCntxt->displayFrmNo++;
                }
            }
        }

    } // while (true)
}

void SS_DETECT_APP_launchProcThreads(SS_DETECT_APP_Context *appCntxt)
{
    /* Launch the input data thread. */
    if (!appCntxt->isROSInterface)
    {
        appCntxt->inputDataThread = std::thread(SS_DETECT_APP_inputDataThread, appCntxt);
    }

    /* Launch the event handler thread. */
    appCntxt->evtHdlrThread = std::thread(SS_DETECT_APP_evtHdlrThread, appCntxt);
}

void SS_DETECT_APP_intSigHandler(SS_DETECT_APP_Context *appCntxt, int sig)
{
    if (!appCntxt->isROSInterface)
    {
        // wait for frame processing to finish
        appCntxt->end_fileno = 0;
        appCntxt->exitInputDataProcess = true;

        while(!appCntxt->processFinished)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } else
    {
        /* Wait for the graph to consume all input. */
        SS_DETECT_APPLIB_waitGraph(appCntxt->ssDetectHdl);
    }

    /* Wait for the threads to exit. */
    SS_DETECT_APP_exitProcThreads(appCntxt, true);

    /* Release the Application context. */
    SS_DETECT_APPLIB_delete(&appCntxt->ssDetectHdl);

    /* Delete display graph */
    SS_DETECT_APP_deleteDisplayGraph(appCntxt);

    /* De-initialize the Application context. */
    SS_DETECT_APP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);

    exit(0);
}
