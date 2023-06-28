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

#include <cm_common.h>
#include <app_ptk_demo_common.h>
#include <app_ptk_demo_profile.h>
#include <app_semseg_cnn.h>

#define APP_SEMSEG_CNN_DEFAULT_IMAGE_WIDTH      (1280U)
#define APP_SEMSEG_CNN_DEFAULT_IMAGE_HEIGHT     (720U)
#define APP_SEMSEG_LDC_DS_FACTOR                (2)
#define APP_SEMSEG_LDC_BLOCK_WIDTH              (64)
#define APP_SEMSEG_LDC_BLOCK_HEIGHT             (16)
#define APP_SEMSEG_LDC_PIXEL_PAD                (1)

#define APP_SEMSEG_CNN_SEMSEG_GRAPH_EVT_BASE    (100U)
#define APP_SEMSEG_CNN_SEMSEG_GRAPH_EVT_MAX     (200U)

#if !defined(PC)
static void SEMSEG_CNN_APP_drawGraphics(
        Draw2D_Handle *handle,
        Draw2D_BufInfo *draw2dBufInfo,
        uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    return;
}

static vx_status SEMSEG_CNN_APP_createDisplayGraph(SEMSEG_CNN_APP_Context *appCntxt)
{
    vx_status   vxStatus = VX_SUCCESS;

    if (appCntxt->dispEnable == 1)
    {
        app_grpx_init_prms_t    grpx_prms;

        appGrpxInitParamsInit(&grpx_prms, appCntxt->vxContext);
        grpx_prms.draw_callback = SEMSEG_CNN_APP_drawGraphics;

        appGrpxInit(&grpx_prms);

        appCntxt->vxDispGraph = vxCreateGraph(appCntxt->vxContext);

        if (appCntxt->vxDispGraph == NULL)
        {
            PTK_printf("[%s:%d] vxCreateGraph() failed\n",
                        __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
        else
        {
            vxSetReferenceName((vx_reference)appCntxt->vxDispGraph,
                               "Display Graph");
        }

        if ((vxStatus == (vx_status)VX_SUCCESS) &&
            (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)))
        {
            tivx_display_params_t   input_display_params;

            memset(&input_display_params,
                   0,
                   sizeof(tivx_display_params_t));

            input_display_params.opMode =
                      TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
            input_display_params.pipeId    = 2;
            input_display_params.outWidth  = INPUT_DISPLAY_WIDTH;
            input_display_params.outHeight = INPUT_DISPLAY_HEIGHT;
            input_display_params.posX      = INPUT_START_X_OFFSET;
            input_display_params.posY      = INPUT_START_Y_OFFSET;

            appCntxt->vxInDispCfg =
                vxCreateUserDataObject(appCntxt->vxContext,
                                       "tivx_display_params_t",
                                       sizeof(tivx_display_params_t),
                                       &input_display_params);

            if (appCntxt->vxInDispCfg == NULL)
            {
                PTK_printf("[%s:%d] vxCreateUserDataObject() failed\n",
                            __FUNCTION__, __LINE__);

                vxStatus = VX_FAILURE;
            }
            else
            {
                vxSetReferenceName((vx_reference)appCntxt->vxInDispCfg,
                                   "Input_DisplayConfiguration");
            }

            if (vxStatus == (vx_status)VX_SUCCESS)
            {
                // create disparity display node
                appCntxt->vxInDispNode =
                    tivxDisplayNode(appCntxt->vxDispGraph,
                                    appCntxt->vxInDispCfg,
                                    appCntxt->vxDispInputImage);

                if (appCntxt->vxInDispNode == NULL)
                {
                    PTK_printf("[%s:%d] tivxDisplayNode() failed\n",
                                __FUNCTION__, __LINE__);

                    vxStatus = VX_FAILURE;
                }
                else
                {
                    vxStatus = vxSetNodeTarget(appCntxt->vxInDispNode,
                                               VX_TARGET_STRING,
                                               TIVX_TARGET_DISPLAY1);

                    if (vxStatus != (vx_status)VX_SUCCESS)
                    {
                        PTK_printf("[%s:%d] vxSetNodeTarget() failed\n",
                                    __FUNCTION__, __LINE__);
                    }
                    else
                    {
                        vxSetReferenceName((vx_reference)appCntxt->vxInDispNode,
                                           "Input Display");
                    }
                }
            }
        }

        if ((vxStatus == (vx_status)VX_SUCCESS) &&
            (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)))
        {
            tivx_display_params_t   output_display_params;

            memset(&output_display_params,
                   0,
                   sizeof(tivx_display_params_t));

            output_display_params.opMode =
                      TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
            output_display_params.pipeId    = 0;
            output_display_params.outWidth  = OUTPUT_DISPLAY_WIDTH;
            output_display_params.outHeight = OUTPUT_DISPLAY_HEIGHT;
            output_display_params.posX      =
                      INPUT_START_X_OFFSET + INPUT_DISPLAY_WIDTH;
            output_display_params.posY      = INPUT_START_Y_OFFSET;

            appCntxt->vxOutDispCfg =
                vxCreateUserDataObject(appCntxt->vxContext,
                                       "tivx_display_params_t",
                                       sizeof(tivx_display_params_t),
                                       &output_display_params);

            if (appCntxt->vxOutDispCfg == NULL)
            {
                PTK_printf("[%s:%d] vxCreateUserDataObject() failed\n",
                            __FUNCTION__, __LINE__);

                vxStatus = VX_FAILURE;
            }
            else
            {
                vxSetReferenceName((vx_reference)appCntxt->vxOutDispCfg,
                                   "Output_DisplayConfiguration");
            }

            if (vxStatus == (vx_status)VX_SUCCESS)
            {
                // create disparity display node
                appCntxt->vxOutDispNode =
                    tivxDisplayNode(appCntxt->vxDispGraph,
                                    appCntxt->vxOutDispCfg,
                                    appCntxt->vxOutputImage);

                if (appCntxt->vxInDispNode == NULL)
                {
                    PTK_printf("[%s:%d] tivxDisplayNode() failed\n",
                                __FUNCTION__, __LINE__);

                    vxStatus = VX_FAILURE;
                }
                else
                {
                    vxStatus = vxSetNodeTarget(appCntxt->vxOutDispNode,
                                               VX_TARGET_STRING,
                                               TIVX_TARGET_DISPLAY2);

                    if (vxStatus != (vx_status)VX_SUCCESS)
                    {
                        PTK_printf("[%s:%d] vxSetNodeTarget() failed\n",
                                    __FUNCTION__, __LINE__);
                    }
                    else
                    {
                        vxSetReferenceName((vx_reference)appCntxt->vxOutDispNode,
                                           "Output Display");
                    }
                }
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            vxStatus = vxVerifyGraph(appCntxt->vxDispGraph);

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] vxVerifyGraph() failed\n",
                            __FUNCTION__, __LINE__);
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            if (appCntxt->exportGraph == 1)
            {
                tivxExportGraphToDot(appCntxt->vxDispGraph, ".",
                                     "vx_app_semseg_cnn_display");
            }

            if (appCntxt->rtLogEnable == 1)
            {
                tivxLogRtTraceEnable(appCntxt->vxDispGraph);
            }
        }
    } /* if ((appCntxt->dispEnable == 1)) && ... */

    return vxStatus;
}

static void SEMSEG_CNN_APP_deleteDisplayGraph(SEMSEG_CNN_APP_Context *appCntxt)
{
    if (appCntxt->dispEnable == 1)
    {
        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1))
        {
            vxReleaseUserDataObject(&appCntxt->vxInDispCfg);
            vxReleaseNode(&appCntxt->vxInDispNode);
        }

        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2))
        {
            vxReleaseUserDataObject(&appCntxt->vxOutDispCfg);
            vxReleaseNode(&appCntxt->vxOutDispNode);
        }

        vxReleaseGraph(&appCntxt->vxDispGraph);

        appGrpxDeInit();
    }
}
#endif // !defined(PC)

void SEMSEG_CNN_APP_parseCfgFile(
        SEMSEG_CNN_APP_Context *appCntxt,
        const char *cfgFileName)
{
    SEMSEG_CNN_APPLIB_createParams *createParams;
    FILE                           *fp;
    char                           *pParamStr;
    char                           *pValueStr;
    char                           *pSLine;
    char                           *filePath;
    char                           *globalBasePath;
    char                           *localBasePath;
    char                            localBasePathArr[SEMSEG_CNN_APP_MAX_LINE_LEN];
    char                            paramSt[SEMSEG_CNN_APP_MAX_LINE_LEN];
    char                            valueSt[SEMSEG_CNN_APP_MAX_LINE_LEN];
    char                            sLine[SEMSEG_CNN_APP_MAX_LINE_LEN];
    int32_t                         pos;

    createParams   = &appCntxt->createParams;
    globalBasePath = getenv("APP_STEREO_DATA_PATH");;
    localBasePath  = localBasePathArr;

    if (globalBasePath == NULL)
    {
        PTK_printf("Please define APP_STEREO_DATA_PATH environment "
                   "variable.\n");
        exit(-1);
    }

    /* get local base path (directory of cfgFileName) */
    pos = ptkdemo_find_slash((char *)cfgFileName, SEMSEG_CNN_APP_MAX_LINE_LEN);

    memset(localBasePath, 0, SEMSEG_CNN_APP_MAX_LINE_LEN);

    if (pos >= 0)
    {
        strncpy(localBasePath, cfgFileName, pos + 1);
    }
    else
    {
        strcpy(localBasePath, "./");
    }

    // set default parameters
    createParams->inputImageWidth    = APP_SEMSEG_CNN_DEFAULT_IMAGE_WIDTH;
    createParams->inputImageHeight   = APP_SEMSEG_CNN_DEFAULT_IMAGE_HEIGHT;
    createParams->tidlImageWidth     = APP_SEMSEG_CNN_DEFAULT_IMAGE_WIDTH;
    createParams->tidlImageHeight    = APP_SEMSEG_CNN_DEFAULT_IMAGE_HEIGHT;
    createParams->outImageWidth      = APP_SEMSEG_CNN_DEFAULT_IMAGE_WIDTH;
    createParams->outImageHeight     = APP_SEMSEG_CNN_DEFAULT_IMAGE_HEIGHT;
    createParams->enableLdcNode      = 1;
    createParams->enablePostProcNode = 1;
    createParams->ldcSsFactor        = APP_SEMSEG_LDC_DS_FACTOR;
    createParams->ldcBlockWidth      = APP_SEMSEG_LDC_BLOCK_WIDTH;
    createParams->ldcBlockHeight     = APP_SEMSEG_LDC_BLOCK_HEIGHT;
    createParams->ldcPixelPad        = APP_SEMSEG_LDC_PIXEL_PAD;
    createParams->pipelineDepth      = 1;
    createParams->vxEvtAppValBase    = APP_SEMSEG_CNN_SEMSEG_GRAPH_EVT_BASE;
    createParams->ldcLutFilePath     = appCntxt->ldcLutFilePath;
    createParams->dlrModelPath       = appCntxt->dlrModelPath;
    createParams->exportGraph        = 0;
    createParams->rtLogEnable        = 0;
    createParams->preProcMean[0]     = 128;
    createParams->preProcMean[1]     = 128;
    createParams->preProcMean[2]     = 128;
    createParams->preProcScale[0]    = 0.015625;
    createParams->preProcScale[1]    = 0.015625;
    createParams->preProcScale[2]    = 0.015625;
    appCntxt->dispEnable             = 0;
    appCntxt->is_interactive         = 0;
    appCntxt->interFrameDelay        = 0;

    if (createParams->enableLdcNode == 1)
    {
        appCntxt->inputImageType = VX_DF_IMAGE_UYVY;
    }
    else
    {
        appCntxt->inputImageType = VX_DF_IMAGE_NV12;
    }

    fp = fopen(cfgFileName, "r");

    if (fp == NULL)
    {
        PTK_printf("[%s:%d] Unable to open config file [%s]\n",
                    __FUNCTION__, __LINE__, cfgFileName);
        exit(0);
    }

    pParamStr = paramSt;
    pValueStr = valueSt;
    pSLine    = sLine;

    while (1)
    {
        pSLine = fgets(pSLine, SEMSEG_CNN_APP_MAX_LINE_LEN, fp);

        if ( pSLine == NULL )
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

        if (strcmp(pParamStr, "lut_file_path") == 0)
        {
            filePath = appCntxt->ldcLutFilePath;
            ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                  localBasePath, SEMSEG_CNN_APP_MAX_LINE_LEN);
        }
        else if (strcmp(pParamStr, "dlr_model_file_path") == 0)
        {
            filePath = appCntxt->dlrModelPath;
            ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                  localBasePath, SEMSEG_CNN_APP_MAX_LINE_LEN);
        }
        else if (strcmp(pParamStr, "input_file_path") == 0)
        {
            filePath = appCntxt->inFilePath;
            ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                  localBasePath, SEMSEG_CNN_APP_MAX_LINE_LEN);
        }
        else if (strcmp(pParamStr, "output_file_path") == 0)
        {
            filePath = appCntxt->outFilePath;
            ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                  localBasePath, SEMSEG_CNN_APP_MAX_LINE_LEN);
        }
        else if (strcmp(pParamStr, "start_seq") == 0)
        {
            appCntxt->startFileNum = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "end_seq") == 0)
        {
            appCntxt->endFileNum = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "width") == 0)
        {
            createParams->inputImageWidth = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "height") == 0)
        {
            createParams->inputImageHeight = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "dl_width") == 0)
        {
            createParams->tidlImageWidth = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "dl_height") == 0)
        {
            createParams->tidlImageHeight = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "out_width") == 0)
        {
            createParams->outImageWidth = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "out_height") == 0)
        {
            createParams->outImageHeight = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "num_classes") == 0)
        {
            createParams->numClasses = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "enable_post_proc") == 0)
        {
            createParams->enablePostProcNode = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "pre_proc_mean") == 0)
        {
            sscanf(pValueStr, "%f,%f,%f",
                   &createParams->preProcMean[0],
                   &createParams->preProcMean[1],
                   &createParams->preProcMean[2]);
        }
        else if (strcmp(pParamStr, "pre_proc_scale") == 0)
        {
            sscanf(pValueStr, "%f,%f,%f",
                   &createParams->preProcScale[0],
                   &createParams->preProcScale[1],
                   &createParams->preProcScale[2]);
        }
        else if (strcmp(pParamStr, "enable_display") == 0)
        {
            appCntxt->dispEnable = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "is_interactive") == 0)
        {
            appCntxt->is_interactive = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "pipeline_depth") == 0)
        {
            createParams->pipelineDepth = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "inter_frame_delay") == 0)
        {
            appCntxt->interFrameDelay =
                static_cast<uint64_t>(atof(pValueStr) * 1000);
        }
        else if (strcmp(pParamStr, "rtLogEnable") == 0)
        {
            createParams->rtLogEnable = (uint8_t)atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "exportGraph") == 0)
        {
            createParams->exportGraph = (uint8_t)atoi(pValueStr);
        }
    }

    appCntxt->exportGraph = createParams->exportGraph;
    appCntxt->rtLogEnable = createParams->rtLogEnable;

    fclose(fp);

    return;

} /* SEMSEG_CNN_APP_parseCfgFile */

vx_status SEMSEG_CNN_APP_init(SEMSEG_CNN_APP_Context *appCntxt)
{
    SEMSEG_CNN_APPLIB_createParams *createParams;
    CM_DLRCreateParams              params;
    int32_t                         status;
    vx_status                       vxStatus;

    vxStatus = VX_SUCCESS;

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

    status = appInit();

    if (status < 0)
    {
        PTK_printf("[%s:%d] appInit() failed.\n",
                   __FUNCTION__, __LINE__);

        vxStatus = VX_FAILURE;
    }

    // OpenVX initialization
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->vxContext = vxCreateContext();

        if (appCntxt->vxContext == NULL)
        {
            PTK_printf("[%s:%d] vxCreateContext() failed.\n",
                       __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        /* load TILDL kernels */
        //tivxTIDLLoadKernels(appCntxt->vxContext);

        /* load image processing kernel */
        tivxImgProcLoadKernels(appCntxt->vxContext);

        /* load HWA kernels */
        tivxHwaLoadKernels(appCntxt->vxContext);

        /* create SDE CNNPP Applib */
        createParams            = &appCntxt->createParams;
        createParams->vxContext = appCntxt->vxContext;
        createParams->dlrObj    = &appCntxt->dlrObj;
        createParams->vxGraph   = NULL;

        appCntxt->sscnnHdl = SEMSEG_CNN_APPLIB_create(createParams);

        if (appCntxt->sscnnHdl == NULL)
        {
            PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_create() failed\n",
                        __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
        else
        {
            appCntxt->dataReadySem  =
                new UTILS::Semaphore(createParams->pipelineDepth);

            appCntxt->displayCtrlSem = new UTILS::Semaphore(0);

            appCntxt->exitInputDataProcess = 0;

        }
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        // create objects
        appCntxt->vxInputImage = vxCreateImage(appCntxt->vxContext,
                                               createParams->inputImageWidth,
                                               createParams->inputImageHeight,
                                               appCntxt->inputImageType);

        if (appCntxt->vxInputImage == NULL)
        {
            PTK_printf("[%s:%d] vxCreateImage() failed\n",
                        __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
        else
        {
            vxSetReferenceName((vx_reference)appCntxt->vxInputImage,
                               "InputImage");
        }
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->vxDispInputImage =
            vxCreateImage(appCntxt->vxContext,
                          createParams->inputImageWidth,
                          createParams->inputImageHeight,
                          VX_DF_IMAGE_NV12);

        if (appCntxt->vxDispInputImage == NULL)
        {
            PTK_printf("[%s:%d] vxCreateImage() failed\n",
                        __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
        else
        {
            vxSetReferenceName((vx_reference)appCntxt->vxDispInputImage,
                               "InputDisplayImage");
        }
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->vxOutputImage =
            vxCreateImage(appCntxt->vxContext,
                          createParams->outImageWidth,
                          createParams->outImageHeight,
                          VX_DF_IMAGE_NV12);

        if (appCntxt->vxOutputImage == NULL)
        {
            PTK_printf("[%s:%d] vxCreateImage() failed\n",
                        __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
        else
        {
            vxSetReferenceName((vx_reference)appCntxt->vxOutputImage,
                               "OutputImage");
        }
    }

#if !defined(PC)
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        /* Create display graph */
        vxStatus = SEMSEG_CNN_APP_createDisplayGraph(appCntxt);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] SEMSEG_CNN_APP_createDisplayGraph() failed.\n",
                       __FUNCTION__, __LINE__);

            vxStatus = VX_FAILURE;
        }
    }
#endif // !defined(PC)

    return vxStatus;
}

vx_status SEMSEG_CNN_APP_run(SEMSEG_CNN_APP_Context *appCntxt)
{
    char        curFileName[SEMSEG_CNN_APP_MAX_FILE_PATH];
    char        curFilePath[SEMSEG_CNN_APP_MAX_FILE_PATH];
    uint32_t    i;
    vx_status   vxStatus;

    vxStatus = VX_SUCCESS;

    appCntxt->totalFrameCount = 0;
    appCntxt->droppedFrameCnt = 0;
    appCntxt->framesProcessed = 0;

    /* For profiling */
    chrono::time_point<chrono::system_clock> start, end;
    float diff;

    for (i = appCntxt->startFileNum; i <= appCntxt->endFileNum; i++)
    {
        /* Wait for the data ready semaphore. */
#if defined(PC)
        if (appCntxt->dataReadySem)
#else
        if (appCntxt->dataReadySem && appCntxt->interFrameDelay == 0)
#endif
        {
            appCntxt->dataReadySem->wait();
        }


#if defined(PC)
        PTK_printf("Processing frame %d...\n", appCntxt->framesProcessed);
#endif

        snprintf(curFileName, 16, "/%010d.yuv", i);
        snprintf(curFilePath, SEMSEG_CNN_APP_MAX_FILE_PATH/2, "%s/%s",
                 appCntxt->inFilePath,
                 curFileName);

        start = GET_TIME();
        ptkdemo_load_vximage_from_yuvfile(appCntxt->vxInputImage, curFilePath);
        end   = GET_TIME();
        diff  = GET_DIFF(start, end);
        ptkdemo_report_proctime("input_image_load", diff);

        appCntxt->totalFrameCount++;

        vxStatus = SEMSEG_CNN_APPLIB_process(appCntxt->sscnnHdl,
                                             appCntxt->vxInputImage,
                                             0); // timestamp

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            appCntxt->droppedFrameCnt++;
        }
        else
        {
            appCntxt->framesProcessed++;
        }

#if !defined(PC)
        if (appCntxt->interFrameDelay != 0)
        {
            /* Wait for 'interFrameDelay' milli-seconds. */
            std::chrono::microseconds   t;

            t = std::chrono::microseconds(appCntxt->interFrameDelay);
            std::this_thread::sleep_for(std::chrono::microseconds(t));
        }
#endif
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = SEMSEG_CNN_APPLIB_waitGraph(appCntxt->sscnnHdl);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_waitGraph() failed\n",
                        __FUNCTION__, __LINE__);
        }
    }

    PTK_printf("\e[K[%d] Frames processed = %d/%d Frames dropped = %d/%d\n\e[A",
               appCntxt->runCtr,
               appCntxt->framesProcessed,
               appCntxt->totalFrameCount,
               appCntxt->droppedFrameCnt,
               appCntxt->totalFrameCount);

    return vxStatus;
}

void SEMSEG_CNN_APP_deInit(SEMSEG_CNN_APP_Context *appCntxt)
{
    int32_t status;

#if !defined(PC)
    /* Delete display graph */
    SEMSEG_CNN_APP_deleteDisplayGraph(appCntxt);
#endif // !defined(PC)

    vxReleaseImage(&appCntxt->vxInputImage);
    vxReleaseImage(&appCntxt->vxDispInputImage);
    vxReleaseImage(&appCntxt->vxOutputImage);

    if (appCntxt->dataReadySem)
    {
        delete appCntxt->dataReadySem;
    }

    if (appCntxt->displayCtrlSem)
    {
        delete appCntxt->displayCtrlSem;
    }

    tivxTIDLUnLoadKernels(appCntxt->vxContext);
    tivxImgProcUnLoadKernels(appCntxt->vxContext);
    tivxHwaUnLoadKernels(appCntxt->vxContext);

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

    if (status < 0)
    {
        PTK_printf("[%s:%d] appDeInit() failed\n", __FUNCTION__, __LINE__);
    }
#endif

}

static void SEMSEG_CNN_APP_exitProcThreads(
        SEMSEG_CNN_APP_Context * appCntxt,
        bool                    detach)
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
                               SEMSEG_CNN_APP_USER_EVT_EXIT,
                               NULL);

    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        PTK_printf("[%s:%d] vxSendUserEvent() failed.\n",
                    __FUNCTION__, __LINE__);
    }

    if (appCntxt->evtHdlrThread.joinable())
    {
        appCntxt->evtHdlrThread.join();
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

void SEMSEG_CNN_APP_cleanupHdlr(SEMSEG_CNN_APP_Context *appCntxt)
{
    /* Wait for the threads to exit. */
    SEMSEG_CNN_APP_exitProcThreads(appCntxt, false);

    PTK_printf("\nPress ENTER key to exit.\n");
    fflush(stdout);
    getchar();

    PTK_printf("========= BEGIN:PERFORMANCE STATS SUMMARY =========\n");
    appPerfStatsPrintAll();
    SEMSEG_CNN_APPLIB_printStats(appCntxt->sscnnHdl);
    PTK_printf("========= END:PERFORMANCE STATS SUMMARY ===========\n\n");

    if (appCntxt->rtLogEnable == 1)
    {
        char name[256];

        snprintf(name, 255, "%s.bin", SEMSEG_CNN_PERF_OUT_FILE);
        tivxLogRtTraceExportToFile(name);
    }

    /* Release the Application context. */
    SEMSEG_CNN_APPLIB_delete(&appCntxt->sscnnHdl);

    SEMSEG_CNN_APP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);
}

void SEMSEG_CNN_APP_reset(SEMSEG_CNN_APP_Context * appCntxt)
{
    int32_t status;

    status = SEMSEG_CNN_APPLIB_reset(appCntxt->sscnnHdl);

    if (status < 0)
    {
        PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_reset() failed.\n",
                   __FUNCTION__,
                   __LINE__);
    }
}

static void SEMSEG_CNN_APP_inputDataThread(SEMSEG_CNN_APP_Context *appCntxt)
{
    vx_status   vxStatus;

    PTK_printf("[%s] Launched Graph processing thread.\n", __FUNCTION__);

    while (true)
    {
        appCntxt->runCtr++;

        /* Reset ground model */
        SEMSEG_CNN_APP_reset(appCntxt);

        /* Execute the graph. */
        vxStatus = SEMSEG_CNN_APP_run(appCntxt);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] SEMSEG_CNN_APP_run() failed\n",
                        __FUNCTION__, __LINE__);

            break;
        }

        if (appCntxt->exitInputDataProcess)
        {
            break;
        }

        // reinit frame no to be displayed
        appCntxt->displayFrNum = 0;
    }

    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        raise(SIGINT);
    }
}

static void SEMSEG_CNN_APP_evtHdlrThread(SEMSEG_CNN_APP_Context *appCntxt)
{
    vx_event_t  evt;
    vx_status   vxStatus;

    vxStatus = VX_SUCCESS;

    PTK_printf("[%s] Launched.\n", __FUNCTION__);

    /* Clear any pending events. The third argument is do_not_block = true. */
    while (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = vxWaitEvent(appCntxt->vxContext, &evt, vx_true_e);
    }

    while (true)
    {
        vxStatus = vxWaitEvent(appCntxt->vxContext, &evt, vx_false_e);

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            if (evt.type == VX_EVENT_USER)
            {
                if (evt.app_value == SEMSEG_CNN_APP_USER_EVT_EXIT)
                {
                    break;
                }
                else if (evt.app_value == SEMSEG_CNN_APPLIB_OUT_AVAIL_EVT)
                {
                    /* Wakeup the display thread. */
                    if (appCntxt->displayCtrlSem)
                    {
                        appCntxt->displayCtrlSem->notify();
                    }
                }
            }

            if (evt.type == VX_EVENT_NODE_COMPLETED)
            {
                vxStatus =
                    SEMSEG_CNN_APPLIB_processEvent(appCntxt->sscnnHdl, &evt);

                if (vxStatus != (vx_status)VX_SUCCESS)
                {
                    PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_processEvent() failed\n",
                                __FUNCTION__, __LINE__);
                }

            } // if (evt.type == VX_EVENT_GRAPH_COMPLETED)

        } // if (vxStatus == (vx_status)VX_SUCCESS)

    } // while (true)

    PTK_printf("[%s] Exiting.\n", __FUNCTION__);
}

vx_status SEMSEG_CNN_APP_saveOutput(SEMSEG_CNN_APP_Context *appCntxt,
                                    vx_image                inputImage,
                                    vx_image                outputImage)
{
    char        inImgFile[SEMSEG_CNN_APP_MAX_LINE_LEN];
    vx_status   vxStatus;

    PTK_printf("Done processing frame %d. Saving the output.\n",
               appCntxt->displayFrNum);

    snprintf(inImgFile,
             SEMSEG_CNN_APP_MAX_LINE_LEN/2,
             "%s/input_%05d.yuv",
             appCntxt->outFilePath,
             appCntxt->displayFrNum);

    vxStatus = ptkdemo_save_vximage_to_yuvfile(inputImage, inImgFile);

    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        PTK_printf("[%s:%d] ptkdemo_save_vximage_to_yuvfile() failed\n",
                    __FUNCTION__, __LINE__);

    }

    if ((appCntxt->createParams.enablePostProcNode) &&
        (vxStatus == (vx_status)VX_SUCCESS))
    {
        char    outImgFile[SEMSEG_CNN_APP_MAX_LINE_LEN];

        snprintf(outImgFile,
                 SEMSEG_CNN_APP_MAX_LINE_LEN/2,
                 "%s/output_%05d.yuv",
                 appCntxt->outFilePath,
                 appCntxt->displayFrNum);

        vxStatus = ptkdemo_save_vximage_to_yuvfile(outputImage, outImgFile);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_save_vximage_to_yuvfile() "
                       "failed\n", __FUNCTION__, __LINE__);

        }
    }

    return vxStatus;
}

static void SEMSEG_CNN_APP_dispThread(SEMSEG_CNN_APP_Context *appCntxt)
{
    vx_status vxStatus = VX_SUCCESS;
    vx_uint64 timestamp;

    /* For profiling */
    chrono::time_point<chrono::system_clock> start, end;
    float diff;

    PTK_printf("[%s] Launched.\n", __FUNCTION__);

    while (true)
    {
        vx_reference    outRef{};
        vx_image        ssInputImage{};

        /* Wait for the data ready semaphore. */
        if (appCntxt->displayCtrlSem)
        {
            appCntxt->displayCtrlSem->wait();
        }

        if (appCntxt->exitDisplayThread == true)
        {
            break;
        }

        vxStatus = SEMSEG_CNN_APPLIB_getOutBuff(appCntxt->sscnnHdl,
                                                &ssInputImage,
                                                &outRef,
                                                &timestamp);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_getOutBuff() failed\n",
                        __FUNCTION__, __LINE__);
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            vxStatus =
            ptkdemo_swap_ref_handles((vx_reference)ssInputImage,
                                     (vx_reference)appCntxt->vxDispInputImage);

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ptkdemo_swap_ref_handles() failed\n",
                            __FUNCTION__, __LINE__);
            }
        }

        if (appCntxt->createParams.enablePostProcNode)
        {
            if (vxStatus == (vx_status)VX_SUCCESS)
            {
                vxStatus =
                ptkdemo_swap_ref_handles((vx_reference)outRef,
                                         (vx_reference)appCntxt->vxOutputImage);

                if (vxStatus != (vx_status)VX_SUCCESS)
                {
                    PTK_printf("[%s:%d] ptkdemo_swap_ref_handles() failed\n",
                                __FUNCTION__, __LINE__);
                }
            }
        }

        /* If SEMSEG_CNN_APPLIB_getOutBuff() was a success then we should call
         * the release outout buffers.
         */
        if ((ssInputImage != NULL) && (outRef != NULL))
        {
            vxStatus = SEMSEG_CNN_APPLIB_releaseOutBuff(appCntxt->sscnnHdl);

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_releaseOutBuff() failed\n",
                            __FUNCTION__, __LINE__);
            }
        }

        /* Wakeup the input data thread. */
        if (appCntxt->dataReadySem)
        {
            appCntxt->dataReadySem->notify();
        }

#if !defined(PC)
        if ((vxStatus == (vx_status)VX_SUCCESS) &&
            (appCntxt->dispEnable == 1))
        {
            start   = GET_TIME();

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

            end   = GET_TIME();
            diff  = GET_DIFF(start, end);
            ptkdemo_report_proctime("display", diff);

        }
#else
        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            vxStatus = SEMSEG_CNN_APP_saveOutput(appCntxt,
                                                 appCntxt->vxDispInputImage,
                                                 appCntxt->vxOutputImage);
        }
#endif

        appCntxt->displayFrNum++;
    }

    PTK_printf("[%s] Exiting.\n", __FUNCTION__);
}

void SEMSEG_CNN_APP_launchProcThreads(SEMSEG_CNN_APP_Context *appCntxt)
{
    /* Launch the graph processing thread. */
    appCntxt->inputDataThread =
        std::thread(SEMSEG_CNN_APP_inputDataThread, appCntxt);

    /* Launch the event handler thread. */
    appCntxt->evtHdlrThread =
        std::thread(SEMSEG_CNN_APP_evtHdlrThread, appCntxt);

    /* Launch the display thread. */
    appCntxt->dispThread =
        std::thread(SEMSEG_CNN_APP_dispThread, appCntxt);
}

void SEMSEG_CNN_APP_intSigHandler(SEMSEG_CNN_APP_Context *appCntxt)
{
    /* Wait for the threads to exit. */
    SEMSEG_CNN_APP_exitProcThreads(appCntxt, true);

#if !defined(PC)
    /* Delete display graph */
    SEMSEG_CNN_APP_deleteDisplayGraph(appCntxt);
#endif // !defined(PC)

    exit(0);
}
