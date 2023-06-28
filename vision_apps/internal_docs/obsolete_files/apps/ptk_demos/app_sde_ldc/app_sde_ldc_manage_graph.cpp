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


int32_t SDELDCAPP_init_LDC(SDELDCAPP_Context *appCntxt)
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
        createParams->outputPipelineDepth   = appCntxt->pipelineDepth;
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            // output left image
            if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
            {
                appCntxt->vxOutputLeftImage[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
            } else 
            {
                appCntxt->vxOutputLeftImage[i] = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
            }

            if (appCntxt->vxOutputLeftImage[i] == NULL)
            {
                PTK_printf("[%s:%d] vxCreateImage() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
                break;
            }

            // output right image
            if (appCntxt->inputFormat == PTK_IMG_FORMAT_Y)
            {
                appCntxt->vxOutputRightImage[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_U8);
            } else 
            {
                appCntxt->vxOutputRightImage[i]  = vxCreateImage(appCntxt->vxContext, appCntxt->width, appCntxt->height, VX_DF_IMAGE_NV12);
            }

            if (appCntxt->vxOutputRightImage[i] == NULL)
            {
                PTK_printf("[%s:%d] vxCreateImage() failed\n",
                            __FUNCTION__, __LINE__);
                status = -1;
                break;
            }

            // pass to LDC Applib createParams
            createParams->vxOutputLeftImage[i]  = appCntxt->vxOutputLeftImage[i];
            createParams->vxOutputRightImage[i] = appCntxt->vxOutputRightImage[i];
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



vx_status  SDELDCAPP_setupPipeline(SDELDCAPP_Context * appCntxt)
{
    SDELDCAPP_graphParams             * paramDesc;
    vx_graph_parameter_queue_params_t   q[SDELDCAPP_NUM_GRAPH_PARAMS];
    uint32_t                            i;
    uint32_t                            cnt = 0;
    vx_status                           vxStatus;

    vx_node leftLdcNode  = SDELCDAPPLIB_getLeftLDCNode(appCntxt->sdeLdcHdl);
    vx_node rightLdcNode = SDELCDAPPLIB_getRightLDCNode(appCntxt->sdeLdcHdl);

    /* LDC left node Param 6 ==> graph param 0. */
    ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                leftLdcNode,
                                6);
    q[cnt++].refs_list = (vx_reference*)appCntxt->vxInputLeftImage;

    /* LDC rigth node Param 6 ==> graph param 1. */
    ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                rightLdcNode,
                                6);
    q[cnt++].refs_list = (vx_reference*)appCntxt->vxInputRightImage;


    /* LDC left node Param 7 ==> graph param 2. */
    ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                leftLdcNode,
                                7);
    q[cnt++].refs_list = (vx_reference*)appCntxt->vxOutputLeftImage;

    /* LDC right node Param 7 ==> graph param 3. */
    ptkdemo_addParamByNodeIndex(appCntxt->vxGraph,
                                rightLdcNode,
                                7);
    q[cnt++].refs_list = (vx_reference*)appCntxt->vxOutputRightImage;


    for (i = 0; i < cnt; i++)
    {
        q[i].graph_parameter_index = i;
        q[i].refs_list_size        = appCntxt->pipelineDepth;
    }

    // allocate free Q
    for (i = 0; i < appCntxt->pipelineDepth; i++)
    {
        paramDesc                     = &appCntxt->paramDesc[i];
        paramDesc->vxInputLeftImage   = appCntxt->vxInputLeftImage[i];
        paramDesc->vxInputRightImage  = appCntxt->vxInputRightImage[i];
        paramDesc->vxOutputLeftImage  = appCntxt->vxOutputLeftImage[i];
        paramDesc->vxOutputRightImage = appCntxt->vxOutputRightImage[i];

        appCntxt->freeQ.push(paramDesc);
    }

    vxStatus = vxSetGraphScheduleConfig(appCntxt->vxGraph,
                                        VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                                        cnt,
                                        q);
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
        uint32_t    appValue;

        appValue = appCntxt->vxEvtAppValBase + SDELDCAPP_GRAPH_COMPLETE_EVENT;

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

    return vxStatus;
}


void SDELDCAPP_printStats(SDELDCAPP_Context * appCntxt)
{
    tivx_utils_graph_perf_print(appCntxt->vxGraph);
    appPerfPointPrint(&appCntxt->sdeLdcPerf);
    PTK_printf("\n");
    appPerfPointPrintFPS(&appCntxt->sdeLdcPerf);
    PTK_printf("\n");
}

void SDELDCAPP_exportStats(SDELDCAPP_Context * appCntxt)
{
    FILE *fp;
    app_perf_point_t *perf_arr[1];

    perf_arr[0] = &appCntxt->sdeLdcPerf;
    fp = appPerfStatsExportOpenFile(".", "SDELDCAPP_datasheet");
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


void SDELDCAPP_waitGraph(SDELDCAPP_Context * appCntxt)
{
    vxWaitGraph(appCntxt->vxGraph);

    /* Wait for the output queue to get flushed. */
    while (appCntxt->freeQ.size() != appCntxt->pipelineDepth)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


vx_status SDELDCAPP_getFreeParamRsrc(SDELDCAPP_Context       *appCntxt,
                                     SDELDCAPP_graphParams   **gpDesc)
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


vx_status SDELDCAPP_process(SDELDCAPP_Context * appCntxt,  SDELDCAPP_graphParams * gpDesc)
{
    vx_status vxStatus = VX_SUCCESS;
    uint8_t   cnt = 0;

    vxStatus = vxGraphParameterEnqueueReadyRef(appCntxt->vxGraph,
                                               cnt++,
                                               (vx_reference*)&gpDesc->vxInputLeftImage,
                                               1);
    if(VX_SUCCESS != vxStatus)
    {
        return vxStatus;
    }

    vxStatus = vxGraphParameterEnqueueReadyRef(appCntxt->vxGraph,
                                               cnt++,
                                               (vx_reference*)&gpDesc->vxInputRightImage,
                                               1);
    if(VX_SUCCESS != vxStatus)
    {
        return vxStatus;
    }

    vxStatus = vxGraphParameterEnqueueReadyRef(appCntxt->vxGraph,
                                               cnt++,
                                               (vx_reference*)&gpDesc->vxOutputLeftImage,
                                               1);
    if(VX_SUCCESS != vxStatus)
    {
        return vxStatus;
    }

    vxStatus = vxGraphParameterEnqueueReadyRef(appCntxt->vxGraph,
                                               cnt++,
                                               (vx_reference*)&gpDesc->vxOutputRightImage,
                                               1);
    if(VX_SUCCESS != vxStatus)
    {
        return vxStatus;
    }

    return vxStatus;
}


int32_t   SDELDCAPP_processEvent(SDELDCAPP_Context * appCntxt, vx_event_t * event)
{
    vx_reference            ref;
    vx_reference            outref;
    uint32_t                numRefs;
    uint32_t                index;
    int32_t                 status;
    vx_status               vxStatus;

    vxStatus = VX_SUCCESS;

    if(event->type == VX_EVENT_GRAPH_COMPLETED)
    {
        uint32_t    i;

        if (appCntxt->startPerfCapt == false)
        {
            appCntxt->startPerfCapt = true;
        }
        else
        {
            appPerfPointEnd(&appCntxt->sdeLdcPerf);
        }

        appPerfPointBegin(&appCntxt->sdeLdcPerf);

        /* Node execution is complete. Deque all the parameters
         * for this node.
         */
        for (i = 0; i < SDELDCAPP_NUM_GRAPH_PARAMS; i++)
        {
            vxStatus = vxGraphParameterDequeueDoneRef(appCntxt->vxGraph,
                                                      i,
                                                      &ref,
                                                      1,
                                                      &numRefs);
            if (vxStatus != VX_SUCCESS)
            {
                PTK_printf("[%s:%d] vxGraphParameterDequeueDoneRef() failed\n",
                           __FUNCTION__, __LINE__);

                return -1;
            }            
        }

        /* The last one to deque is vxOutInstMap parameter. Search and
         * identify the resource index.
         */
        index = appCntxt->pipelineDepth;
        for (i = 0; i < appCntxt->pipelineDepth; i++)
        {
            outref = (vx_reference)appCntxt->vxOutputRightImage[i];

            if (ref == outref)
            {
                index = i;
                break;
            }
        }

        if (index >= appCntxt->pipelineDepth)
        {
            PTK_printf("[%s:%d] Resource look up failed\n",
                       __FUNCTION__, __LINE__);

            return -1;
        } else
        {
            /* Mark the dequeued resource as free. */
            status = SDELDCAPP_releaseParamRsrc(appCntxt, index);
            if (status < 0)
            {
                PTK_printf("[%s:%d] SDELDCAPP_releaseParamRsrc() failed.\n",
                           __FUNCTION__,
                           __LINE__);

                return -1;
            }
        }
    }

    return 0;
}


int32_t SDELDCAPP_releaseParamRsrc(SDELDCAPP_Context  *appCntxt,
                                   uint32_t            rsrcIndex)
{
    SDELDCAPP_graphParams        * desc;
    std::unique_lock<std::mutex>   lock(appCntxt->paramRsrcMutex);

    desc = &appCntxt->paramDesc[rsrcIndex];
    appCntxt->outputQ.push(desc);

    return 0;
}


int32_t SDELDCAPP_getOutBuff(SDELDCAPP_Context *appCntxt, vx_image outputLeftImage, vx_image outputRightImage)
{
    SDELDCAPP_graphParams         * desc;
    std::unique_lock<std::mutex>    lock(appCntxt->paramRsrcMutex);

    if (appCntxt->outputQ.empty())
    {
        return -1;
    }

    /* Get the descriptor. */
    desc              = appCntxt->outputQ.front();
    /*
    *outputLeftImage  = desc->vxOutputLeftImage;
    *outputRightImage = desc->vxOutputRightImage;
    */
    ptkdemo_copy_image_to_image(desc->vxOutputLeftImage, outputLeftImage);
    ptkdemo_copy_image_to_image(desc->vxOutputRightImage, outputRightImage);

    return 0;
}


void SDELDCAPP_releaseOutBuff(SDELDCAPP_Context * appCntxt)
{
    SDELDCAPP_graphParams    *desc;
    std::unique_lock<std::mutex>    lock(appCntxt->paramRsrcMutex);

    if (appCntxt->outputQ.empty())
    {
        PTK_printf("[%s:%d] No output buffers available.\n",
                   __FUNCTION__, __LINE__);

        return;
    }

    desc = appCntxt->outputQ.front();
    appCntxt->outputQ.pop();

    /* Push the descriptor into the free queue. */
    appCntxt->freeQ.push(desc);
}
