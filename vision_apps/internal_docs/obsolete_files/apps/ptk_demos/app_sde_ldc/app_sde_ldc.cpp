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

#include <app_ptk_demo_common.h>

#include "app_sde_ldc_main.h"
#include "app_sde_ldc.h"


#define APP_SDE_LDC_NAME    "apps_sde_ldc"



#if !defined(PC)
static void SDELDCAPP_drawGraphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    return;
}
#endif


void SDELDCAPP_parseCfgFile(SDELDCAPP_Context *appCntxt, const char *cfg_file_name)
{
    FILE      * fp = fopen(cfg_file_name, "r");
    char      * pParamStr;
    char      * pValueStr;
    char      * pSLine;
    char      * basePath;

    char        paramSt[SDELDCAPP_MAX_LINE_LEN];
    char        valueSt[SDELDCAPP_MAX_LINE_LEN];
    char        sLine[SDELDCAPP_MAX_LINE_LEN];

    // set default parameters
    appCntxt->display_option    = 0;
    appCntxt->width             = 1280;
    appCntxt->height            = 720;
    appCntxt->inputFormat       = 0;
    appCntxt->start_fileno      = 1;
    appCntxt->end_fileno        = 1;
    appCntxt->pipelineDepth     = SDELDCAPP_MAX_PIPELINE_DEPTH;
    appCntxt->is_interactive    = 0;
    appCntxt->exportGraph       = 0;
    appCntxt->rtLogEnable       = 0;


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
        pSLine = fgets(pSLine, SDELDCAPP_MAX_LINE_LEN, fp);

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

        if (strcmp(pParamStr, "left_img_file_path") == 0)
        {
            snprintf(appCntxt->left_img_file_path, SDELDCAPP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        if (strcmp(pParamStr, "right_img_file_path") == 0)
        {
            snprintf(appCntxt->right_img_file_path, SDELDCAPP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "output_file_path") == 0)
        {
            snprintf(appCntxt->output_img_file_path, SDELDCAPP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        if (strcmp(pParamStr, "left_LUT_file_name") == 0)
        {
            snprintf(appCntxt->left_LUT_file_name, SDELDCAPP_MAX_LINE_LEN,
                 "%s/%s", basePath, pValueStr);
        }
        if (strcmp(pParamStr, "right_LUT_file_name") == 0)
        {
            snprintf(appCntxt->right_LUT_file_name, SDELDCAPP_MAX_LINE_LEN,
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
        else if (strcmp(pParamStr, "pipeline_depth") == 0)
        {
            appCntxt->pipelineDepth = atoi(pValueStr);
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
    }

    fclose(fp);

    if (appCntxt->end_fileno < appCntxt->start_fileno)
    {
        appCntxt->end_fileno = appCntxt->start_fileno;
    }

    return;

} /* SDELDCAPP_parseCfgFile */


void SDELDCAPP_setAllParams(SDELDCAPP_Context *appCntxt)
{
    SDELDCAPPLIB_createParams *createParams = &appCntxt->sdeLdcCreateParams;

    appCntxt->vxEvtAppValBase      = 0;

    /* LDC create params */
    createParams->leftLutFileName  = appCntxt->left_LUT_file_name;
    createParams->rightLutFileName = appCntxt->right_LUT_file_name;

    createParams->width            = appCntxt->width;
    createParams->height           = appCntxt->height;
    createParams->inputFormat      = appCntxt->inputFormat;
    createParams->pipelineDepth    = appCntxt->pipelineDepth;
} /* SDELDCAPP_setAllParams */


int32_t SDELDCAPP_init(SDELDCAPP_Context *appCntxt)
{
    int32_t                     status;
    vx_status                   vxStatus;
    //SDELDCAPPLIB_createParams * createParams;

    status = appInit();
    PTK_assert(status == 0);

    // OpenVX initialization
    appCntxt->vxContext = vxCreateContext();
    PTK_assert(appCntxt->vxContext);

    // Create graph
    appCntxt->vxGraph = vxCreateGraph(appCntxt->vxContext);
    if (appCntxt->vxGraph == NULL)
    {
        PTK_printf("[%s:%d] vxCreateGraph() failed\n",
                    __FUNCTION__, __LINE__);
        status = -1;
    }

    if (status == 0)
    {
        vxSetReferenceName((vx_reference)appCntxt->vxGraph,
                           "SDE LDC Rectification Graph");
    }

    if (status == 0)
    {
        tivxHwaLoadKernels(appCntxt->vxContext);
        status = SDELDCAPP_init_LDC(appCntxt);
    }

    if (status >= 0)
    {
        appPerfPointSetName(&appCntxt->sdeLdcPerf , "Stereo LDC GRAPH");

        /*
         * set up the pipeline. 
         */
        vxStatus = SDELDCAPP_setupPipeline(appCntxt);

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
        }
    }

    if (status >= 0)
    {
        if (appCntxt->exportGraph == 1)
        {
            tivxExportGraphToDot(appCntxt->vxGraph, ".", "vx_app_sde_ldc");
        }

        if (appCntxt->rtLogEnable == 1)
        {
            tivxLogRtTraceEnable(appCntxt->vxGraph);
        }
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (status >= 0)
    {
        appCntxt->exitInputDataProcess = false;
        appCntxt->dataReadySem = new Semaphore(appCntxt->pipelineDepth);

        if (1 == appCntxt->display_option)
        {
#if !defined(PC)
            app_grpx_init_prms_t grpx_prms;
            appGrpxInitParamsInit(&grpx_prms, appCntxt->vxContext);
            grpx_prms.draw_callback = SDELDCAPP_drawGraphics;
            appGrpxInit(&grpx_prms);
#endif
        }
    }

    return status;
}


static void SDELDCAPP_run(SDELDCAPP_Context *appCntxt)
{
    uint32_t               i;
    vx_status              vxStatus = VX_SUCCESS;
    char                   temp[SDELDCAPP_MAX_LINE_LEN];

    SDELDCAPP_graphParams* gpDesc;

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

        vxStatus = SDELDCAPP_getFreeParamRsrc(appCntxt, &gpDesc);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] SDELDCAPP_getFreeParamRsrc() failed\n",
                        __FUNCTION__, __LINE__);
        }

        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            // left image
            strcpy(appCntxt->left_img_file_name, appCntxt->left_img_file_path);
            sprintf(temp, "/%010d.bmp", i);
            if (strlen(appCntxt->left_img_file_name) + strlen(temp) >= SDELDCAPP_MAX_LINE_LEN)
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
    
            // right image
            strcpy(appCntxt->right_img_file_name, appCntxt->right_img_file_path);
            sprintf(temp, "/%010d.bmp", i);
            if (strlen(appCntxt->right_img_file_name) + strlen(temp) >= SDELDCAPP_MAX_LINE_LEN)
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
            // left image
            strcpy(appCntxt->left_img_file_name, appCntxt->left_img_file_path);
            sprintf(temp, "/%010d.yuv", i);
            if (strlen(appCntxt->left_img_file_name) + strlen(temp) >= SDELDCAPP_MAX_LINE_LEN)
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

            // right image
            strcpy(appCntxt->right_img_file_name, appCntxt->right_img_file_path);
            sprintf(temp, "/%010d.yuv", i);
            if (strlen(appCntxt->right_img_file_name) + strlen(temp) >= SDELDCAPP_MAX_LINE_LEN)
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

        // run the app
        vxStatus = SDELDCAPP_process(appCntxt, gpDesc);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] SDELDCAPP_process() failed\n",
                        __FUNCTION__, __LINE__);
        }

        appCntxt->frameCnt++;
    }

    PTK_printf("\e[KProcessed %d frames.\n\e[A", appCntxt->frameCnt);

    /* Wait for the graph to consume all input. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        SDELDCAPP_waitGraph(appCntxt);
    }
}

static void SDELDCAPP_deInit(SDELDCAPP_Context *appCntxt)
{
    int32_t i;
    int32_t status;

    if (appCntxt->dataReadySem)
    {
        delete appCntxt->dataReadySem;
    }

    // release input image object
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        vxReleaseImage(&appCntxt->vxInputLeftImage[i]);
        vxReleaseImage(&appCntxt->vxInputRightImage[i]);
    }


    // release input image object
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        vxReleaseImage(&appCntxt->vxOutputLeftImage[i]);
        vxReleaseImage(&appCntxt->vxOutputRightImage[i]);
    }

    if (appCntxt->rtLogEnable == 1)
    {
        tivxLogRtTraceDisable(appCntxt->vxGraph);
    }

    vxReleaseGraph(&appCntxt->vxGraph);
    tivxHwaUnLoadKernels(appCntxt->vxContext);

    if (1 == appCntxt->display_option)
    {
#if !defined(PC)
        appGrpxDeInit();
#endif
    }

    /* Release the context. */
    vxReleaseContext(&appCntxt->vxContext);

    status = appDeInit();
    PTK_assert(status == 0);
}

void SDELDCAPP_createDispalyGraph(SDELDCAPP_Context *appCntxt)
{
    int32_t status;

    // create graph
    appCntxt->vxDispGraph = vxCreateGraph(appCntxt->vxContext);
    APP_ASSERT_VALID_REF(appCntxt->vxDispGraph);
    vxSetReferenceName((vx_reference)appCntxt->vxDispGraph, "Display");

    // create objects
    if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
    {
        appCntxt->vxDisplayLeftImage  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
        appCntxt->vxDisplayRightImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
    } else
    {
        appCntxt->vxDisplayLeftImage  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
        appCntxt->vxDisplayRightImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
    }
    APP_ASSERT_VALID_REF(appCntxt->vxDisplayLeftImage);
    vxSetReferenceName((vx_reference)appCntxt->vxDisplayLeftImage, "LeftImage_ToDisplay");
    APP_ASSERT_VALID_REF(appCntxt->vxDisplayRightImage);
    vxSetReferenceName((vx_reference)appCntxt->vxDisplayRightImage, "RightImage_ToDisplay");

    
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == appCntxt->display_option))
    {
#if !defined(PC)
        memset(&appCntxt->left_display_params, 0, sizeof(tivx_display_params_t));
        appCntxt->left_display_config = vxCreateUserDataObject(appCntxt->vxContext, "tivx_display_params_t",
                                                               sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(appCntxt->left_display_config);

        vxSetReferenceName((vx_reference)appCntxt->left_display_config, "LeftDisplayConfiguration");

        appCntxt->left_display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        appCntxt->left_display_params.pipeId = 2;
        appCntxt->left_display_params.outWidth = OUTPUT_DISPLAY_WIDTH;
        appCntxt->left_display_params.outHeight = OUTPUT_DISPLAY_HEIGHT;
        appCntxt->left_display_params.posX = 960;
        appCntxt->left_display_params.posY = 300;

        status = vxCopyUserDataObject(appCntxt->left_display_config, 0, sizeof(tivx_display_params_t), &appCntxt->left_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(status == VX_SUCCESS);


        // create left image display node
        appCntxt->node_left_display = tivxDisplayNode(
            appCntxt->vxDispGraph,
            appCntxt->left_display_config,
            appCntxt->vxDisplayLeftImage);
        status = vxGetStatus((vx_reference)appCntxt->node_left_display);
        if (VX_SUCCESS != status)
        {
            PTK_printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        APP_ASSERT(VX_SUCCESS == status);
        status = vxSetNodeTarget(appCntxt->node_left_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
        APP_ASSERT(status == VX_SUCCESS);
        vxSetReferenceName((vx_reference)appCntxt->node_left_display, "LeftDisplayNode");
#endif
    }

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (1 == appCntxt->display_option))
    {
#if !defined(PC)
        memset(&appCntxt->right_display_params, 0, sizeof(tivx_display_params_t));
        appCntxt->right_display_config = vxCreateUserDataObject(appCntxt->vxContext, "tivx_display_params_t",
                                                                sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(appCntxt->right_display_config);

        vxSetReferenceName((vx_reference)appCntxt->right_display_config, "RightDisplayConfiguration");

        appCntxt->right_display_params.opMode = TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        appCntxt->right_display_params.pipeId = 0;
        appCntxt->right_display_params.outWidth = INPUT_DISPLAY_WIDTH;
        appCntxt->right_display_params.outHeight = INPUT_DISPLAY_HEIGHT;
        appCntxt->right_display_params.posX = 0;
        appCntxt->right_display_params.posY = 300;

        status = vxCopyUserDataObject(appCntxt->right_display_config, 0, sizeof(tivx_display_params_t), &appCntxt->right_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(status == VX_SUCCESS);

        // create disparity display node
        appCntxt->node_right_display = tivxDisplayNode(
            appCntxt->vxDispGraph,
            appCntxt->right_display_config,
            appCntxt->vxDisplayRightImage);
        status = vxGetStatus((vx_reference)appCntxt->node_right_display);
        if (VX_SUCCESS != status)
        {
            PTK_printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        APP_ASSERT(VX_SUCCESS == status);
        status = vxSetNodeTarget(appCntxt->node_right_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY2);
        APP_ASSERT(status == VX_SUCCESS);
        vxSetReferenceName((vx_reference)appCntxt->node_right_display, "RightDisplayNode");
#endif
    }

    status = vxVerifyGraph(appCntxt->vxDispGraph);
    APP_ASSERT(status == VX_SUCCESS);
}


void SDELDCAPP_deleteDisplayGraph(SDELDCAPP_Context *appCntxt)
{
    vxReleaseImage(&appCntxt->vxDisplayLeftImage);
    vxReleaseImage(&appCntxt->vxDisplayRightImage);

#if !defined(PC)
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == appCntxt->display_option))
    {
        vxReleaseUserDataObject(&appCntxt->left_display_config);
    }
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (1 == appCntxt->display_option))
    {
        vxReleaseUserDataObject(&appCntxt->right_display_config);
    }
#endif

    vxReleaseGraph(&appCntxt->vxDispGraph);
}


static void SDELDCAPP_exitProcThreads(SDELDCAPP_Context *appCntxt,
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

    PTK_printf("[%s] Exiting process...\n", __FUNCTION__);

    /* Let the event handler thread exit. */
    vxStatus = vxSendUserEvent(appCntxt->vxContext,
                               SDELDCAPP_USER_EVT_EXIT,
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

void SDELDCAPP_cleanupHdlr(SDELDCAPP_Context *appCntxt)
{
    /* Wait for the threads to exit. */
    SDELDCAPP_exitProcThreads(appCntxt, false);

    PTK_printf("\nPress ENTER key to exit.\n");
    fflush(stdout);
    getchar();

    if (appCntxt->rtLogEnable == 1)
    {
        char name[256];

        snprintf(name, 255, "%s.bin", APP_SDE_LDC_NAME);
        tivxLogRtTraceExportToFile(name);
    }

    /* Release the Application context. */
    SDELDCAPPLIB_delete(&appCntxt->sdeLdcHdl);

    /* De-initialize the Application context. */
    SDELDCAPP_deleteDisplayGraph(appCntxt);

    SDELDCAPP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);
}

void SDELDCAPP_reset(SDELDCAPP_Context * appCntxt)
{
    /* Reset the performance capture initialization flag. */
    appCntxt->startPerfCapt = false;
}


static void SDELDCAPP_inputDataThread(SDELDCAPP_Context *appCntxt)
{
    PTK_printf("[%s] Launched Graph processing thread.\n", __FUNCTION__);

    // init display frame no
    appCntxt->displayFrmNo    = appCntxt->start_fileno;
    appCntxt->processFinished = false;

    while (true)
    {
        /* Reset ground model */
        SDELDCAPP_reset(appCntxt);

        /* Execute the graph. */
        SDELDCAPP_run(appCntxt);

        if (appCntxt->exitInputDataProcess)
        {
            break;
        }

        // reinit frame no to be displayed
        appCntxt->displayFrmNo = appCntxt->start_fileno;
    }

    appCntxt->processFinished = true;
}

static void SDELDCAPP_evtHdlrThread(SDELDCAPP_Context *appCntxt)
{
    vx_event_t evt;
    vx_status vxStatus = VX_SUCCESS;
    vx_status  status;
    
#if defined(PC)
    char output_file_name[SDELDCAPP_MAX_LINE_LEN];
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
                if (evt.app_value == SDELDCAPP_USER_EVT_EXIT)
                {
                    break;
                }
            }

            SDELDCAPP_processEvent(appCntxt, &evt);

            if (evt.type == VX_EVENT_GRAPH_COMPLETED)
            {
                SDELDCAPP_getOutBuff(appCntxt, appCntxt->vxDisplayLeftImage, appCntxt->vxDisplayRightImage);

#if !defined(PC)
                status = vxScheduleGraph(appCntxt->vxDispGraph);
                PTK_assert(VX_SUCCESS == status);
                status = vxWaitGraph(appCntxt->vxDispGraph);
#endif

                SDELDCAPP_releaseOutBuff(appCntxt);

                /* Wakeup the input data thread. */
                if (appCntxt->dataReadySem)
                {
                    appCntxt->dataReadySem->notify();
                }

#if defined(PC)
                if (1 == appCntxt->display_option)
                {
                    /* Save output */
                    if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
                        {
                        snprintf(output_file_name, SDELDCAPP_MAX_LINE_LEN, "%s/rect_left_%010d.bmp",
                                 appCntxt->output_img_file_path,
                                 appCntxt->displayFrmNo);
                        tivx_utils_save_vximage_to_bmpfile(output_file_name, appCntxt->vxDisplayLeftImage);

                        snprintf(output_file_name, SDELDCAPP_MAX_LINE_LEN, "%s/rect_right_%010d.bmp",
                                 appCntxt->output_img_file_path,
                                 appCntxt->displayFrmNo);
                        tivx_utils_save_vximage_to_bmpfile(output_file_name, appCntxt->vxDisplayRightImage);
                    } else
                    {
                        snprintf(output_file_name, SDELDCAPP_MAX_LINE_LEN, "%s/rect_left_%010d.yuv",
                                 appCntxt->output_img_file_path,
                                 appCntxt->displayFrmNo);
                        ptkdemo_save_vximage_to_yuvfile(appCntxt->vxDisplayLeftImage, output_file_name);

                        snprintf(output_file_name, SDELDCAPP_MAX_LINE_LEN, "%s/rect_right_%010d.yuv",
                                 appCntxt->output_img_file_path,
                                 appCntxt->displayFrmNo);
                        ptkdemo_save_vximage_to_yuvfile(appCntxt->vxDisplayRightImage, output_file_name);
                    }
                }
#endif

                /* increase display (output) frame number */
                appCntxt->displayFrmNo++;
            } 
        }

    } // while (true)
}


void SDELDCAPP_launchProcThreads(SDELDCAPP_Context *appCntxt)
{
    /* Launch the graph processing thread. */
    appCntxt->inputDataThread = std::thread(SDELDCAPP_inputDataThread, appCntxt);

    /* Launch the event handler thread. */
    appCntxt->evtHdlrThread   = std::thread(SDELDCAPP_evtHdlrThread, appCntxt);
}

void SDELDCAPP_intSigHandler(SDELDCAPP_Context *appCntxt, int sig)
{
    // wait for frame processing to finish
    appCntxt->end_fileno = 0;
    appCntxt->exitInputDataProcess = true;

    while(!appCntxt->processFinished)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    /* Wait for the threads to exit. */
    SDELDCAPP_exitProcThreads(appCntxt, true);

    /* Release the Application context. */
    SDELDCAPPLIB_delete(&appCntxt->sdeLdcHdl);

    /* Deletee display graph */
    SDELDCAPP_deleteDisplayGraph(appCntxt);

    /* De-initialize the Application context. */
    SDELDCAPP_deInit(appCntxt);

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);

    exit(0);
}

