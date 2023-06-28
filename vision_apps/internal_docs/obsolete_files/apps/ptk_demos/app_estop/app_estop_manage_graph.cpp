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
#include <cm_postproc_node_cntxt.h>
#include <cm_tidl_node_cntxt.h>

#include "app_estop_main.h"
#include "app_estop.h"

int32_t ESTOP_APP_init_LDC(ESTOP_APP_Context *appCntxt)
{
    SDELDCAPPLIB_createParams * createParams;
    int32_t                     i, status = 0;

    createParams            = &appCntxt->sdeLdcCreateParams;
    createParams->vxContext = appCntxt->vxContext;
    createParams->vxGraph   = appCntxt->vxGraph;

    /*
     * Create input image objects 
     */
    createParams->createInputFlag      = 0;
    createParams->inputPipelineDepth   = appCntxt->pipelineDepth;
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        // input left image
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            appCntxt->vxInputLeftImage[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
        } else 
        {
            appCntxt->vxInputLeftImage[i] = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_UYVY);
        }

        if (appCntxt->vxInputLeftImage[i] == NULL)
        {
            PTK_printf("[%s:%d] vxCreateImage() failed\n",
                        __FUNCTION__, __LINE__);
            status = -1;
            break;
        }
        vxSetReferenceName((vx_reference)appCntxt->vxInputLeftImage[i], "InputLeftImage");

        // input right image
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            appCntxt->vxInputRightImage[i] = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
        } else 
        {
            appCntxt->vxInputRightImage[i] = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_UYVY);
        }

        if (appCntxt->vxInputRightImage[i] == NULL)
        {
            PTK_printf("[%s:%d] vxCreateImage() failed\n",
                        __FUNCTION__, __LINE__);
            status = -1;
            break;
        }
        vxSetReferenceName((vx_reference)appCntxt->vxInputRightImage[i], "InputRightImage");

        // pass to LDC Applib createParams
        createParams->vxInputLeftImage[i]  = appCntxt->vxInputLeftImage[i];
        createParams->vxInputRightImage[i] = appCntxt->vxInputRightImage[i];
    }

    if (status >= 0)
    {
        /*
         * Create output image objects 
         */
        createParams->createOutputFlag      = 0;
        createParams->outputPipelineDepth   = 1;

        // output left image
        if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
        {
            appCntxt->vxLeftRectImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
        } else 
        {
            appCntxt->vxLeftRectImage = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
        }

        if (appCntxt->vxLeftRectImage == NULL)
        {
            PTK_printf("[%s:%d] vxCreateImage() failed\n",
                        __FUNCTION__, __LINE__);
            status = -1;
        }
        vxSetReferenceName((vx_reference)appCntxt->vxLeftRectImage, "LeftRectifiedImage");

        // pass to LDC Applib createParams
        createParams->vxOutputLeftImage[0]  = appCntxt->vxLeftRectImage;

        // output right image
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
            {
                appCntxt->vxRightRectImage[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
            } else 
            {
                appCntxt->vxRightRectImage[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
            }

            if (appCntxt->vxRightRectImage[i] == NULL)
            {
                PTK_printf("[%s:%d] vxCreateImage() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
                break;
            }
            vxSetReferenceName((vx_reference)appCntxt->vxRightRectImage[i], "RightRectifiedImage");

            // pass to LDC Applib createParams
            createParams->vxOutputRightImage[i] = appCntxt->vxRightRectImage[i];
        }
    }

    if (status >= 0)
    {
        appCntxt->sdeLdcHdl = SDELDCAPPLIB_create(createParams);
        if (appCntxt->sdeLdcHdl == NULL)
        {
            PTK_printf("[%s:%d] SDELDCAPPLIB_create() failed\n",
                        __FUNCTION__, __LINE__);
            status = -1;
        }
    }

    return status;
}

int32_t ESTOP_APP_init_SDE(ESTOP_APP_Context *appCntxt)
{
    int32_t i, status = 0;

    if (appCntxt->sdeAlgoType == 0)
    {
        SL_SDEAPPLIB_createParams   * slSdeCreateParams;

        slSdeCreateParams            = &appCntxt->slSdeCreateParams;
        slSdeCreateParams->vxContext = appCntxt->vxContext;
        slSdeCreateParams->vxGraph   = appCntxt->vxGraph;

        /* 
         * Create input image objects for SL SDE
         */
        slSdeCreateParams->createInputFlag     = 0;
        slSdeCreateParams->inputPipelineDepth  = 1;
        slSdeCreateParams->vxLeftRectImage[0]  = appCntxt->vxLeftRectImage;
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            slSdeCreateParams->vxRightRectImage[i] = appCntxt->vxRightRectImage[i];
        }

        /* 
         * Create output image objects for SL SDE
         */
        slSdeCreateParams->createOutputFlag    = 0;
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            appCntxt->vxSde16BitOutput[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_S16);
            if (appCntxt->vxSde16BitOutput[i] == NULL)
            {
                PTK_printf("[%s:%d] vxCreateImage() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
                break;
            }
            vxSetReferenceName((vx_reference)appCntxt->vxSde16BitOutput[i], "RawDisparityMap");

            slSdeCreateParams->vxSde16BitOutput[i]  = appCntxt->vxSde16BitOutput[i];
        }

        if (status >= 0)
        {
            appCntxt->slSdeHdl = SL_SDEAPPLIB_create(slSdeCreateParams);
            if (appCntxt->slSdeHdl == NULL)
            {
                PTK_printf("[%s:%d] SL_SDEAPPLIB_create() failed\n",
                            __FUNCTION__, __LINE__);

                status = -1;
            }
        }
    }
    else if (appCntxt->sdeAlgoType == 1)
    {
        ML_SDEAPPLIB_createParams   * mlSdeCreateParams;

        mlSdeCreateParams            = &appCntxt->mlSdeCreateParams;
        mlSdeCreateParams->vxContext = appCntxt->vxContext;
        mlSdeCreateParams->vxGraph   = appCntxt->vxGraph;

        /* 
         * Create input image objects for SL SDE
         */
        mlSdeCreateParams->createInputFlag    = 0;
        mlSdeCreateParams->inputPipelineDepth = 1;
        mlSdeCreateParams->vxLeftRectImageL0[0]  = appCntxt->vxLeftRectImage;
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            mlSdeCreateParams->vxRightRectImageL0[i] = appCntxt->vxRightRectImage[i];
        }

        /* 
         * Create output image objects for ML SDE
         */
        mlSdeCreateParams->createOutputFlag  = 0;
        if (appCntxt->ppMedianFilterEnable)
        {
            for (i = 0; i < appCntxt->pipelineDepth; i++)
            {
                appCntxt->vxMedianFilteredDisparity[i] = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_S16);
                if (appCntxt->vxMedianFilteredDisparity[i] == NULL)
                {
                    PTK_printf("[%s:%d] vxCreateImage() failed\n",
                                __FUNCTION__, __LINE__);
                    status = -1;
                    break;
                }
                mlSdeCreateParams->vxMedianFilteredDisparity[i]  = appCntxt->vxMedianFilteredDisparity[i];
            }
        }

        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            appCntxt->vxMergeDisparityL0[i] = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_S16);
            if (appCntxt->vxMergeDisparityL0[i] == NULL)
            {
                PTK_printf("[%s:%d] vxCreateImage() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
                break;
            }
            mlSdeCreateParams->vxMergeDisparityL0[i]  = appCntxt->vxMergeDisparityL0[i];
        }

        if (status >= 0)
        {
            appCntxt->mlSdeHdl = ML_SDEAPPLIB_create(mlSdeCreateParams);
            if (appCntxt->mlSdeHdl == NULL)
            {
                PTK_printf("[%s:%d] ML_SDEAPPLIB_create() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
            }
        }
    }

    return status;
}

int32_t ESTOP_APP_init_SS(ESTOP_APP_Context *appCntxt)
{
    int32_t status = 0;

    SEMSEG_CNN_APPLIB_createParams * ssCnnCreateParams;

    ssCnnCreateParams                      = &appCntxt->ssCnnCreateParams;
    ssCnnCreateParams->vxContext           = appCntxt->vxContext;
    ssCnnCreateParams->vxGraph             = appCntxt->vxGraph;

    ssCnnCreateParams->inputImageWidth     = appCntxt->width;
    ssCnnCreateParams->inputImageHeight    = appCntxt->height;
    ssCnnCreateParams->tidlImageWidth      = appCntxt->tensor_width;
    ssCnnCreateParams->tidlImageHeight     = appCntxt->tensor_height;
    ssCnnCreateParams->outImageWidth       = appCntxt->tensor_width;
    ssCnnCreateParams->outImageHeight      = appCntxt->tensor_height;
    ssCnnCreateParams->dlrModelPath        = appCntxt->dlrModelPath;
    ssCnnCreateParams->pipelineDepth       = appCntxt->pipelineDepth;
    ssCnnCreateParams->numClasses          = appCntxt->numClasses;

    ssCnnCreateParams->enableLdcNode       = 0;
    ssCnnCreateParams->enablePostProcNode  = appCntxt->enablePostProcNode;
    ssCnnCreateParams->createInputFlag     = 0;
    ssCnnCreateParams->vxInputImage[0]     = appCntxt->vxRightRectImage[0];
    ssCnnCreateParams->dlrObj              = &appCntxt->dlrObj;

    for (uint8_t i = 0; i < CM_PREPROC_MAX_IMG_CHANS; i++)
    {
        ssCnnCreateParams->preProcMean[i]  = appCntxt->preProcMean[i];
        ssCnnCreateParams->preProcScale[i]  = appCntxt->preProcScale[i];
    }

    appCntxt->ssCnnHdl = SEMSEG_CNN_APPLIB_create(ssCnnCreateParams);

    if (appCntxt->ssCnnHdl == NULL)
    {
        PTK_printf("[%s:%d] SEMSEG_CNN_APPLIB_create() failed\n",
                    __FUNCTION__, __LINE__);
        status = -1;
    }

    // we do not get output object from semantic segmentation applib

    return status;
}


int32_t ESTOP_APP_init_SS_Detection(ESTOP_APP_Context *appCntxt)
{
    SS_DETECT_APPLIB_createParams  *ssDetectCreateParams;
    vx_tensor                      *vxOutTensor;
    int32_t                         status = 0;
    int32_t                         i;

    ssDetectCreateParams                = &appCntxt->ssDetectCreateParams;
    ssDetectCreateParams->vxContext     = appCntxt->vxContext;
    ssDetectCreateParams->vxGraph       = appCntxt->vxGraph;

    ssDetectCreateParams->width         = appCntxt->width;
    ssDetectCreateParams->height        = appCntxt->height;
    ssDetectCreateParams->tensorWidth   = appCntxt->tensor_width;
    ssDetectCreateParams->tensorHeight  = appCntxt->tensor_height;
    ssDetectCreateParams->pipelineDepth = appCntxt->pipelineDepth;
    ssDetectCreateParams->inputFormat   = appCntxt->inputFormat;

    /* 
     * Create input image objects for SL SDE
     */
    ssDetectCreateParams->createInputFlag = 0;
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        // right rectified image
        ssDetectCreateParams->vxRightRectImage[i] = appCntxt->vxRightRectImage[i];

        // raw disparity image
        if (appCntxt->sdeAlgoType == 0)
        {
            ssDetectCreateParams->vxSde16BitOutput[i] = appCntxt->vxSde16BitOutput[i];
        } else
        {
            if (appCntxt->ppMedianFilterEnable)
            {
                ssDetectCreateParams->vxSde16BitOutput[i] = appCntxt->vxMedianFilteredDisparity[i];
            } else
            {
                ssDetectCreateParams->vxSde16BitOutput[i] = appCntxt->vxMergeDisparityL0[i];
            }
        }
    }

    vxOutTensor = SEMSEG_CNN_APPLIB_getOutputTensor(appCntxt->ssCnnHdl);
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        ssDetectCreateParams->vxSSMapTensor[i] = vxOutTensor[i];
    }

    // output objects are created in applib
    ssDetectCreateParams->createOutputFlag = 0;

    if (status >= 0)
    {
        appCntxt->ssDetectHdl = SS_DETECT_APPLIB_create(ssDetectCreateParams);
        if (appCntxt->ssDetectHdl == NULL)
        {
            PTK_printf("[%s:%d] SS_DETECT_APPLIB_create() failed\n",
                        __FUNCTION__, __LINE__);
            status = -1;
        }
    }

    if (status >= 0)
    {
        // get the output object from ss_detect applib
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            appCntxt->vx3DBoundBox[i] =
                SS_DETECT_APPLIB_get3DBBObject(appCntxt->ssDetectHdl, i);
        }
    }

    return status;
}

vx_status  ESTOP_APP_setupPipeline(ESTOP_APP_Context * appCntxt)
{
    uint32_t   appValue;
    vx_status  vxStatus = VX_SUCCESS;

    if (appCntxt->sdeAlgoType == 0)
    {
        vxStatus = ESTOP_APP_setupPipeline_SL(appCntxt);
    } else
    {
        vxStatus = ESTOP_APP_setupPipeline_ML(appCntxt);
    }



    if (vxStatus == VX_SUCCESS)
    {
        appValue = appCntxt->vxEvtAppValBase + ESTOP_APP_GRAPH_COMPLETE_EVENT;

        vxStatus = vxRegisterEvent((vx_reference)appCntxt->vxGraph,
                                   VX_EVENT_GRAPH_COMPLETED,
                                   0,
                                   appValue);

        if (vxStatus != VX_SUCCESS)
        {
            PTK_printf("[%s:%d] vxRegisterEvent() failed\n",
                       __FUNCTION__, __LINE__);
        }
    }


    if (vxStatus == VX_SUCCESS)
    {
        CM_ScalerNodeCntxt * scalerObj = SEMSEG_CNN_APPLIB_getScalerNodeCntxt(appCntxt->ssCnnHdl);

        appValue = appCntxt->vxEvtAppValBase + ESTOP_APP_SCALER_NODE_COMPLETE_EVENT;

        vxStatus = vxRegisterEvent((vx_reference)scalerObj->vxNode,
                                   VX_EVENT_NODE_COMPLETED,
                                   0,
                                   appValue);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] vxRegisterEvent() failed\n",
                       __FUNCTION__, __LINE__);
        }
    }    


    return vxStatus;
}

vx_status ESTOP_APP_setupPipeline_SL(ESTOP_APP_Context * appCntxt)
{
    ESTOP_APP_graphParams             * paramDesc;
    vx_graph_parameter_queue_params_t   q[ESTOP_APP_NUM_GRAPH_PARAMS];
    uint32_t                            i;
    uint32_t                            cnt = 0;
    vx_status                           vxStatus;

    vx_node leftLdcNode  = SDELCDAPPLIB_getLeftLDCNode(appCntxt->sdeLdcHdl);
    vx_node rightLdcNode = SDELCDAPPLIB_getRightLDCNode(appCntxt->sdeLdcHdl);
    vx_node sdeNode      = SL_SDEAPPLIB_getSDENode(appCntxt->slSdeHdl);
    vx_node pcNode       = SS_DETECT_APPLIB_getPCNode(appCntxt->ssDetectHdl);
    vx_node ogNode       = SS_DETECT_APPLIB_getOGNode(appCntxt->ssDetectHdl);

    CM_ScalerNodeCntxt   * scalerObj   = SEMSEG_CNN_APPLIB_getScalerNodeCntxt(appCntxt->ssCnnHdl);
    vx_tensor            * vxOutTensor = SEMSEG_CNN_APPLIB_getOutputTensor(appCntxt->ssCnnHdl);

    float             ** dlrInputBuff  = SEMSEG_CNN_APPLIB_getDLRInputBuff(appCntxt->ssCnnHdl);
    int32_t           ** dlrOutputBuff = SEMSEG_CNN_APPLIB_getDLROutputBuff(appCntxt->ssCnnHdl);

    appCntxt->numGraphParams = 5;

    /* LDC left node Param 6 (leftInputImg) ==> graph param 0. */
    vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                           leftLdcNode,
                                           6);
    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                    __FUNCTION__, __LINE__);
    }
    else
    {
        q[cnt++].refs_list = (vx_reference*)appCntxt->vxInputLeftImage;
    }

    /* LDC right node Param 6 (rightInputImg) ==> graph param 1. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               rightLdcNode,
                                               6);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxInputRightImage;
        }
    }

    /* LDC right node Param 7 (rightOutputImg)==> graph param 2. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               rightLdcNode,
                                               7);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxRightRectImage;
        }
    }

    /* vxDmpacSdeNode Param 3 (vxSde16BitOutput) ==> graph param 3 */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               sdeNode,
                                               3);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxSde16BitOutput;
        }
    }

    /* vxOGNode Param 2 (vx3DBoundBox) ==> graph param 4. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                    ogNode,
                                    2);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vx3DBoundBox;
        }
    } 


    /*  scalerObj->vxNode Param 1 (outImage)==> graph param 5 */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->numGraphParams += 1;

        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               scalerObj->vxNode,
                                               1);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)scalerObj->outImage;
        }
    }

    /*  pcNode Param 3 (vxSSMapTensor)==> graph param 6 */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->numGraphParams += 1;

        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               pcNode,
                                               3);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)vxOutTensor;
        }
    }


    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        for (i = 0; i < cnt; i++)
        {
            q[i].graph_parameter_index = i;
            q[i].refs_list_size        = appCntxt->pipelineDepth;
        }

        // allocate free Q
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            paramDesc                      = &appCntxt->paramDesc[i];
            paramDesc->vxInputLeftImage    = appCntxt->vxInputLeftImage[i];
            paramDesc->vxInputRightImage   = appCntxt->vxInputRightImage[i];
            paramDesc->vxRightRectImage    = appCntxt->vxRightRectImage[i];
            paramDesc->vxSde16BitOutput    = appCntxt->vxSde16BitOutput[i];
            paramDesc->vx3DBoundBox        = appCntxt->vx3DBoundBox[i];

            paramDesc->vxScalerOut         = scalerObj->outImage[i];
            paramDesc->vxOutTensor         = vxOutTensor[i];

            paramDesc->dlrInputBuff        = dlrInputBuff[i];
            paramDesc->dlrOutputBuff       = dlrOutputBuff[i];

            appCntxt->freeQ.push(paramDesc);
        }

        vxStatus = vxSetGraphScheduleConfig(appCntxt->vxGraph,
                                            VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                                            cnt,
                                            q);
    }

    if (vxStatus == VX_SUCCESS)
    {
        /* explicitly set graph pipeline depth */
        vxStatus = tivxSetGraphPipelineDepth(appCntxt->vxGraph,
                                             appCntxt->pipelineDepth);

        if (vxStatus != VX_SUCCESS)
        {
            PTK_printf("[%s:%d] tivxSetGraphPipelineDepth() failed\n",
                       __FUNCTION__, __LINE__);
        }
    }
    else
    {
        PTK_printf("[%s:%d] vxSetGraphScheduleConfig() failed\n",
                   __FUNCTION__, __LINE__);
    }


    if (vxStatus == VX_SUCCESS)
    {
        /*  vxLeftRectImage */
        vxStatus = tivxSetNodeParameterNumBufByIndex(leftLdcNode,
                                                     7,
                                                     appCntxt->pipelineDepth);
        PTK_assert(vxStatus == VX_SUCCESS);

        // vxPointCloud */
        vxStatus =
            tivxSetNodeParameterNumBufByIndex(pcNode,
                                              4, 
                                              appCntxt->pipelineDepth);
        PTK_assert(vxStatus == VX_SUCCESS);
    }


    return vxStatus;
}


vx_status ESTOP_APP_setupPipeline_ML(ESTOP_APP_Context * appCntxt)
{
    ESTOP_APP_graphParams             * paramDesc;
    vx_graph_parameter_queue_params_t   q[ESTOP_APP_NUM_GRAPH_PARAMS];
    uint32_t                            i;
    uint32_t                            cnt = 0;
    vx_status                           vxStatus;

    vx_node leftLdcNode    = SDELCDAPPLIB_getLeftLDCNode(appCntxt->sdeLdcHdl);
    vx_node rightLdcNode   = SDELCDAPPLIB_getRightLDCNode(appCntxt->sdeLdcHdl);

    vx_node sdeNodeL0      = ML_SDEAPPLIB_getSDENodeL0(appCntxt->mlSdeHdl);
    vx_node sdeNodeL1      = ML_SDEAPPLIB_getSDENodeL1(appCntxt->mlSdeHdl);
    vx_node sdeNodeL2      = ML_SDEAPPLIB_getSDENodeL2(appCntxt->mlSdeHdl);
    vx_node medFilterNode  = ML_SDEAPPLIB_getMedFilterNode(appCntxt->mlSdeHdl);
    vx_node mergeNodeL1    = ML_SDEAPPLIB_getMergeNodeL1(appCntxt->mlSdeHdl);
    vx_node mergeNodeL2    = ML_SDEAPPLIB_getMergeNodeL2(appCntxt->mlSdeHdl);
    vx_node leftMscNodeL1  = ML_SDEAPPLIB_getLeftMSCNodeL1(appCntxt->mlSdeHdl);
    vx_node rightMscNodeL1 = ML_SDEAPPLIB_getRightMSCNodeL1(appCntxt->mlSdeHdl);
    vx_node leftMscNodeL2  = ML_SDEAPPLIB_getLeftMSCNodeL2(appCntxt->mlSdeHdl);
    vx_node rightMscNodeL2 = ML_SDEAPPLIB_getRightMSCNodeL2(appCntxt->mlSdeHdl);

    vx_node pcNode         = SS_DETECT_APPLIB_getPCNode(appCntxt->ssDetectHdl);
    vx_node ogNode         = SS_DETECT_APPLIB_getOGNode(appCntxt->ssDetectHdl);

    CM_ScalerNodeCntxt   * scalerObj   = SEMSEG_CNN_APPLIB_getScalerNodeCntxt(appCntxt->ssCnnHdl);
    vx_tensor            * vxOutTensor = SEMSEG_CNN_APPLIB_getOutputTensor(appCntxt->ssCnnHdl);

    float             ** dlrInputBuff  = SEMSEG_CNN_APPLIB_getDLRInputBuff(appCntxt->ssCnnHdl);
    int32_t           ** dlrOutputBuff = SEMSEG_CNN_APPLIB_getDLROutputBuff(appCntxt->ssCnnHdl);    

    appCntxt->numGraphParams = 5;

    /* LDC left node Param 6 (leftInputImg) ==> graph param 0. */
    vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                           leftLdcNode,
                                           6);
    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                    __FUNCTION__, __LINE__);
    }
    else
    {
        q[cnt++].refs_list = (vx_reference*)appCntxt->vxInputLeftImage;
    }

    /* LDC rigth node Param 6 (rightInputImg) ==> graph param 1. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                              rightLdcNode,
                                              6);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxInputRightImage;
        }
    }

    /* LDC right node Param 7  (rightOutputImg) ==> graph param 2. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               rightLdcNode,
                                               7);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxRightRectImage;
        }
    }

    /* vxDisparityMergeNodeL1 Param 3 (vxMergeDisparityL0) => graph param 3. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               mergeNodeL1,
                                               3);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxMergeDisparityL0;
        }
    }

    // Always ppMedianFilterEnable = 0 
    if (vxStatus == (vx_status)VX_SUCCESS && appCntxt->ppMedianFilterEnable)
    {
        appCntxt->numGraphParams += 1;

        /* vxMedianFilterNode Param 2 (vxMedianFilteredDisparity) => graph param 3. */
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               medFilterNode,
                                               2);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vxMedianFilteredDisparity;
        }
    }

    /* vxOGNode Param 2 ==> graph param 4. */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               ogNode,
                                               2);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)appCntxt->vx3DBoundBox;
        }
    }    


    /*  scalerObj->vxNode Param 1 (outImage)==> graph param 5 */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->numGraphParams += 1;

        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               scalerObj->vxNode,
                                               1);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)scalerObj->outImage;
        }
    }

    /*  pcNode Param 3 (vxSSMapTensor)==> graph param 6 */
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        appCntxt->numGraphParams += 1;

        vxStatus = ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                               pcNode,
                                               3);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_addParamByNodeIndex() failed\n",
                        __FUNCTION__, __LINE__);
        }
        else
        {
            q[cnt++].refs_list = (vx_reference*)vxOutTensor;
        }
    } 


    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        for (i = 0; i < cnt; i++)
        {
            q[i].graph_parameter_index = i;
            q[i].refs_list_size        = appCntxt->pipelineDepth;
        }

        // allocate free Q
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            paramDesc                      = &appCntxt->paramDesc[i];
            paramDesc->vxInputLeftImage    = appCntxt->vxInputLeftImage[i];
            paramDesc->vxInputRightImage   = appCntxt->vxInputRightImage[i];
            paramDesc->vxRightRectImage    = appCntxt->vxRightRectImage[i];
            paramDesc->vxMergeDisparityL0  = appCntxt->vxMergeDisparityL0[i];

            if (appCntxt->ppMedianFilterEnable)
            {
                paramDesc->vxMedianFilteredDisparity = appCntxt->vxMedianFilteredDisparity[i];
            }

            paramDesc->vx3DBoundBox        = appCntxt->vx3DBoundBox[i];

            paramDesc->vxScalerOut         = scalerObj->outImage[i];
            paramDesc->vxOutTensor         = vxOutTensor[i];

            paramDesc->dlrInputBuff        = dlrInputBuff[i];
            paramDesc->dlrOutputBuff       = dlrOutputBuff[i];

            appCntxt->freeQ.push(paramDesc);
        }

        vxStatus = vxSetGraphScheduleConfig(appCntxt->vxGraph,
                                            VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                                            cnt,
                                            q);
    }

    if (vxStatus == VX_SUCCESS)
    {
        /* explicitly set graph pipeline depth */
        vxStatus = tivxSetGraphPipelineDepth(appCntxt->vxGraph,
                                             appCntxt->pipelineDepth);

        if (vxStatus != VX_SUCCESS)
        {
            PTK_printf("[%s:%d] tivxSetGraphPipelineDepth() failed\n",
                       __FUNCTION__, __LINE__);
        }
    }
    else
    {
        PTK_printf("[%s:%d] vxSetGraphScheduleConfig() failed\n",
                   __FUNCTION__, __LINE__);
    }

    if (vxStatus == VX_SUCCESS)
    {
        /*  vxLeftRectImage */
        vxStatus = tivxSetNodeParameterNumBufByIndex(leftLdcNode,
                                                     7,
                                                     appCntxt->pipelineDepth);

        /* vxSde16BitOutputL0 */
        vxStatus = tivxSetNodeParameterNumBufByIndex(sdeNodeL0,
                                                     3,
                                                     appCntxt->pipelineDepth);
        PTK_assert(vxStatus == VX_SUCCESS);

        if (appCntxt->numLayers > 1)
        {
            /* vxLeftImageL1 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(leftMscNodeL1,
                                                         1,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);

            /* vxRightImageL1 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(rightMscNodeL1,
                                                         1,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);

            /* vxSde16BitOutputL1 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(sdeNodeL1,
                                                         3,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);
        }

        if (appCntxt->numLayers > 2)
        {
            /* vxLeftImageL2 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(leftMscNodeL2,
                                                         1,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);

            /* vxRightImageL2 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(rightMscNodeL2,
                                                         1,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);

            /* vxSde16BitOutputL2 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(sdeNodeL2,
                                                         3,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);

            /* vxMergeDisparityL1 */
            vxStatus = tivxSetNodeParameterNumBufByIndex(mergeNodeL2,
                                                         3,
                                                         appCntxt->pipelineDepth);
            PTK_assert(vxStatus == VX_SUCCESS);
        }

        // vxPointCloud */
        vxStatus =
            tivxSetNodeParameterNumBufByIndex(pcNode,
                                              4, 
                                              appCntxt->pipelineDepth);
        PTK_assert(vxStatus == VX_SUCCESS);
    }

    return vxStatus;
}

void ESTOP_APP_printStats(ESTOP_APP_Context * appCntxt)
{
    tivx_utils_graph_perf_print(appCntxt->vxGraph);
    appPerfPointPrint(&appCntxt->estopPerf);
    PTK_printf("\n");
    appPerfPointPrintFPS(&appCntxt->estopPerf);
    PTK_printf("\n");
}

void ESTOP_APP_exportStats(ESTOP_APP_Context * appCntxt)
{
    FILE *fp;
    app_perf_point_t *perf_arr[1];

    perf_arr[0] = &appCntxt->estopPerf;
    fp = appPerfStatsExportOpenFile(".", "ESTOP_APP_datasheet");
    if (NULL != fp)
    {
        appPerfStatsExportAll(fp, perf_arr, 1);
        tivx_utils_graph_perf_export(fp, appCntxt->vxGraph);
        appPerfStatsExportCloseFile(fp);
        appPerfStatsResetAll();
    }
    else
    {
        printf("fp is null\n");
    }
}

void ESTOP_APP_waitGraph(ESTOP_APP_Context * appCntxt)
{
    vxWaitGraph(appCntxt->vxGraph);

    /* Wait for the output queue to get flushed. */
    while (appCntxt->freeQ.size() != appCntxt->pipelineDepth)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

vx_status ESTOP_APP_popFreeInputDesc(ESTOP_APP_Context       *appCntxt,
                                     ESTOP_APP_graphParams   **gpDesc)
{
    std::unique_lock<std::mutex>   lock(appCntxt->paramRsrcMutex);
    vx_status                      vxStatus = VX_SUCCESS;

    /* Check if we have free og node descriptors available. */
    if (appCntxt->freeQ.empty())
    {
        vxStatus = VX_FAILURE;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        *gpDesc = appCntxt->freeQ.front();
        appCntxt->freeQ.pop();
    }

    return vxStatus;
}

vx_status ESTOP_APP_getDLRInputDesc(ESTOP_APP_Context       *appCntxt,
                                    ESTOP_APP_graphParams  **gpDesc)
{
    std::unique_lock<std::mutex>    lock(appCntxt->dlrRsrcMutex);
    vx_status                       vxStatus = VX_SUCCESS;

    /* Check if we have descriptors available. */
    if (appCntxt->dlrInputQ.empty())
    {
        vxStatus = VX_FAILURE;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        *gpDesc = appCntxt->dlrInputQ.front();
    }

    return vxStatus;
}


vx_status ESTOP_APP_popDLRInputDesc(ESTOP_APP_Context       *appCntxt,
                                    ESTOP_APP_graphParams  **gpDesc)
{
    std::unique_lock<std::mutex>    lock(appCntxt->dlrRsrcMutex);
    vx_status                       vxStatus = VX_SUCCESS;

    /* Check if we have descriptors available. */
    if (appCntxt->dlrInputQ.empty())
    {
        vxStatus = VX_FAILURE;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        *gpDesc = appCntxt->dlrInputQ.front();
        appCntxt->dlrInputQ.pop();
    }

    return vxStatus;
}

vx_status ESTOP_APP_getOutputDesc(ESTOP_APP_Context       *appCntxt,
                                  ESTOP_APP_graphParams   *gpDesc)
{
    std::unique_lock<std::mutex>    lock(appCntxt->outRsrcMutex);
    vx_status                       vxStatus = VX_SUCCESS;

    /* Check if we have descriptors available. */
    if (appCntxt->outputQ.empty())
    {
        vxStatus = VX_FAILURE;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        *gpDesc = *appCntxt->outputQ.front();
    }

    return vxStatus;
}

vx_status ESTOP_APP_popOutputDesc(ESTOP_APP_Context       *appCntxt,
                                  ESTOP_APP_graphParams  **gpDesc)
{
    std::unique_lock<std::mutex>    lock(appCntxt->outRsrcMutex);
    vx_status                       vxStatus = VX_SUCCESS;

    /* Check if we have descriptors available. */
    if (appCntxt->outputQ.empty())
    {
        vxStatus = VX_FAILURE;
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        *gpDesc = appCntxt->outputQ.front();
        appCntxt->outputQ.pop();
    }

    return vxStatus;
}

void ESTOP_APP_enqueOutputDesc(ESTOP_APP_Context      *appCntxt,
                               ESTOP_APP_graphParams  *desc)
{
    std::unique_lock<std::mutex>   lock(appCntxt->outRsrcMutex);

    appCntxt->outputQ.push(desc);
}

void ESTOP_APP_enqueDLRInputDesc(ESTOP_APP_Context      *appCntxt,
                                 ESTOP_APP_graphParams  *desc)
{
    std::unique_lock<std::mutex>   lock(appCntxt->dlrRsrcMutex);

    appCntxt->dlrInputQ.push(desc);
}

void ESTOP_APP_enqueInputDesc(ESTOP_APP_Context      *appCntxt,
                              ESTOP_APP_graphParams  *desc)
{
    std::unique_lock<std::mutex>   lock(appCntxt->paramRsrcMutex);

    appCntxt->freeQ.push(desc);
}


vx_status  ESTOP_APP_process(ESTOP_APP_Context * appCntxt, ESTOP_APP_graphParams * gpDesc)
{
    vx_status vxStatus = VX_SUCCESS;
    uint16_t   i;
    uint8_t   cnt = 0;

    vx_reference  obj[ESTOP_APP_NUM_GRAPH_PARAMS];

    obj[cnt++] = (vx_reference)gpDesc->vxInputLeftImage;
    obj[cnt++] = (vx_reference)gpDesc->vxInputRightImage;
    obj[cnt++] = (vx_reference)gpDesc->vxRightRectImage;

    if (appCntxt->sdeAlgoType == 0)
    {
        obj[cnt++] = (vx_reference)gpDesc->vxSde16BitOutput;
    } 
    else
    {
        /* code */
        obj[cnt++] = (vx_reference)gpDesc->vxMergeDisparityL0;

        // ppMedianFilterEnable is always 0 
        if (appCntxt->ppMedianFilterEnable)
        {
            obj[cnt++] = (vx_reference)gpDesc->vxMedianFilteredDisparity;
        }
    }

    obj[cnt++] = (vx_reference)gpDesc->vx3DBoundBox;

    obj[cnt++] = (vx_reference)gpDesc->vxScalerOut;

    // Enqueue buffers
    // Enqueue vxOutTensor after DLR is completed
    for (i = 0; i < cnt; i++)
    {
        vxStatus = vxGraphParameterEnqueueReadyRef(appCntxt->vxGraph,
                                                   i,
                                                   &obj[i],
                                                   1);
        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] vxGraphParameterEnqueueReadyRef(%d) "
                       "failed\n", __FUNCTION__, __LINE__, i);
            break;
        }
    }


    /* Push the descriptor to the DLR input queue. */
    ESTOP_APP_enqueDLRInputDesc(appCntxt, gpDesc);

    return vxStatus;
}


vx_status ESTOP_APP_processEvent(ESTOP_APP_Context * appCntxt, vx_event_t * event)
{
    vx_reference            ref;
    uint8_t                 i;
    uint32_t                numRefs;
    uint32_t                index;
    vx_status               vxStatus;

    ref      = NULL;
    vxStatus = (vx_status)VX_SUCCESS;

    if(event->type == VX_EVENT_NODE_COMPLETED)
    {
        uint32_t appValue = appCntxt->vxEvtAppValBase + ESTOP_APP_SCALER_NODE_COMPLETE_EVENT;

        if (event->app_value != appValue)
        {
            /* Something wrong. We did not register for this event. */
            PTK_printf("[%s:%d] Unknown App Value [%d].\n",
                       __FUNCTION__, __LINE__, event->app_value);

            vxStatus = VX_FAILURE;
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            /* Wakeup the DLR thread. The DLR thread will process the
             * descriptor at the head of the queue.
             */
            appCntxt->dlrDataReadySem->notify();
        }

    } else
    if(event->type == VX_EVENT_GRAPH_COMPLETED)
    {
        uint32_t appValue = appCntxt->vxEvtAppValBase + ESTOP_APP_GRAPH_COMPLETE_EVENT;

        if (event->app_value != appValue)
        {
            /* Something wrong. We did not register for this event. */
            PTK_printf("[%s:%d] Unknown App Value [%d].\n",
                       __FUNCTION__, __LINE__, event->app_value);

            vxStatus = VX_FAILURE;
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            if (appCntxt->startPerfCapt == false)
            {
                appCntxt->startPerfCapt = true;
            }
            else
            {
                appPerfPointEnd(&appCntxt->estopPerf);
            }

            appPerfPointBegin(&appCntxt->estopPerf);

            /* Node execution is complete. Deque all the parameters
             * for this node.
             */
            for (i = 0; i < appCntxt->numGraphParams; i++)
            {
                vxStatus = vxGraphParameterDequeueDoneRef(appCntxt->vxGraph,
                                                          i,
                                                          &ref,
                                                          1,
                                                          &numRefs);
                if (vxStatus != (vx_status)VX_SUCCESS)
                {
                    PTK_printf("[%s:%d] vxGraphParameterDequeueDoneRef() failed\n",
                               __FUNCTION__, __LINE__);

                    break;
                }
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            vx_tensor * vxOutTensor = SEMSEG_CNN_APPLIB_getOutputTensor(appCntxt->ssCnnHdl);

            /* The last one to deque is vxOutInstMap parameter. Search and
             * identify the resource index.
             */
            index = appCntxt->pipelineDepth;
            for (i = 0; i < appCntxt->pipelineDepth; i++)
            {
                if (ref == (vx_reference) vxOutTensor[i])
                {
                    index = i;
                    break;
                }
            }

            if (index >= appCntxt->pipelineDepth)
            {
                PTK_printf("[%s:%d] Resource look up failed\n",
                           __FUNCTION__, __LINE__);
    
                vxStatus = VX_FAILURE;
            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            /* Mark the dequeued resource as free. */
            vxStatus = ESTOP_APP_releaseParamRsrc(appCntxt, index);

            if (vxStatus != (vx_status)VX_SUCCESS)
            {
                PTK_printf("[%s:%d] ESTOP_APP_releaseParamRsrc() failed.\n",
                           __FUNCTION__,
                           __LINE__);

            }
        }

        if (vxStatus == (vx_status)VX_SUCCESS)
        {
            /* Wakeup the display thread. */
            if (appCntxt->displayCtrlSem)
            {
                appCntxt->displayCtrlSem->notify();
            }
        }
    }

    return vxStatus;
}


vx_status ESTOP_APP_releaseParamRsrc(ESTOP_APP_Context  *appCntxt, uint32_t rsrcIndex)
{
    ESTOP_APP_graphParams  * desc = &appCntxt->paramDesc[rsrcIndex];

    ESTOP_APP_enqueOutputDesc(appCntxt, desc);

    return VX_SUCCESS;
}


vx_status ESTOP_APP_getOutBuff(ESTOP_APP_Context *appCntxt, vx_image rightRectImage, vx_image ssOutImage, vx_user_data_object obsBB, vx_image disparity16)
{
    vx_status                        vxStatus;
    vx_map_id                        map_id;

    ESTOP_APP_graphParams            desc;
    tivx_ss_sde_obs_3d_bound_box_t * boundingBox;

    vxStatus = ESTOP_APP_getOutputDesc(appCntxt, &desc);

    // get rectified right image
    vxStatus = ptkdemo_copy_image_to_image(desc.vxRightRectImage, rightRectImage);
    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        PTK_printf("[%s:%d] ptkdemo_copy_image_to_image() failed\n",
                    __FUNCTION__, __LINE__);
    }

    // get semantic segmentation image output
    if (vxStatus == (vx_status)VX_SUCCESS && appCntxt->enablePostProcNode)
    {
        vxStatus = ptkdemo_copy_image_to_image((vx_image)desc.vxScalerOut, ssOutImage);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_copy_image_to_image() failed\n",
                        __FUNCTION__, __LINE__);
        }
    }

    // get SDE disparity output
    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        if (appCntxt->sdeAlgoType == 0)
        {
            vxStatus = ptkdemo_copy_image_to_image(desc.vxSde16BitOutput, disparity16);
        } 
        else if (appCntxt->ppMedianFilterEnable)
        {
            vxStatus = ptkdemo_copy_image_to_image(desc.vxMedianFilteredDisparity, disparity16);
        } 
        else
        {
            vxStatus = ptkdemo_copy_image_to_image(desc.vxMergeDisparityL0, disparity16);
        }

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] ptkdemo_copy_image_to_image() failed\n",
                        __FUNCTION__, __LINE__);
        }
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = vxMapUserDataObject(desc.vx3DBoundBox,
                                       0,
                                       appCntxt->bbSize,
                                       &map_id,
                                       (void **)&boundingBox,
                                       VX_READ_ONLY,
                                       VX_MEMORY_TYPE_HOST,
                                       0);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] vxMapUserDataObject() failed\n",
                        __FUNCTION__, __LINE__);
        }
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        vxStatus = vxCopyUserDataObject(obsBB, 
                                        0,
                                        appCntxt->bbSize,
                                        boundingBox,
                                        VX_WRITE_ONLY,
                                        VX_MEMORY_TYPE_HOST);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] vxCopyUserDataObject() failed\n",
                        __FUNCTION__, __LINE__);
        }                                        
    }

    if (vxStatus == (vx_status)VX_SUCCESS) 
    {
        vxStatus = vxUnmapUserDataObject(desc.vx3DBoundBox, map_id);

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            PTK_printf("[%s:%d] vxUnmapUserDataObject() failed\n",
                        __FUNCTION__, __LINE__);
        }
    }

    return vxStatus;
}


vx_status ESTOP_APP_releaseOutBuff(ESTOP_APP_Context * appCntxt)
{
    vx_status                       vxStatus;
    ESTOP_APP_graphParams         * desc;

    /* Pop the output descriptor. */
    vxStatus = ESTOP_APP_popOutputDesc(appCntxt, &desc);

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        /* Enqueue the descriptor to the free descriptor queue. */
        ESTOP_APP_enqueInputDesc(appCntxt, desc);
    }

    return vxStatus;
}
