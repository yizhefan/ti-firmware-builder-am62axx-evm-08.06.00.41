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

#ifndef _APP_ESTOP_MAIN_H_
#define _APP_ESTOP_MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <TI/tivx.h>
#include <TI/tivx_debug.h>
#include <TI/tivx_stereo.h>

#include <perception/perception.h>
#include <perception/utils/ipc_chan.h>
#include <perception/utils/ptk_semaphore.h>

#include <sde_ldc_applib.h>
#include <sde_singlelayer_applib.h>
#include <sde_multilayer_applib.h>
#include <ss_sde_detection_applib.h>
#include <semseg_cnn_applib.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#include <utils/draw2d/include/draw2d.h>
#include <utils/grpx/include/app_grpx.h>
#include <utils/perf_stats/include/app_perf_stats.h>

#define APP_ASSERT(x)               PTK_assert((x))
#define APP_ASSERT_VALID_REF(ref)   APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS)

#ifdef __cplusplus
}
#endif


#define ESTOP_APP_MAX_LINE_LEN         (512U)
#define ESTOP_APP_NUM_BUFF_DESC        (1U)

#define OUTPUT_DISPLAY_WIDTH           960
#define OUTPUT_DISPLAY_HEIGHT          480

#define INPUT_DISPLAY_WIDTH            960
#define INPUT_DISPLAY_HEIGHT           480

#define ESTOP_APP_USER_EVT_EXIT        (1U)

#define ESTOP_APP_MAX_PIPELINE_DEPTH   (8U)
#define ESTOP_APP_NUM_GRAPH_PARAMS     (7U)

#define ESTOP_APP_GRAPH_COMPLETE_EVENT        (0U)
#define ESTOP_APP_SCALER_NODE_COMPLETE_EVENT  (ESTOP_APP_GRAPH_COMPLETE_EVENT + 1)
#define ESTOP_APP_SCALER_OUT_AVAIL_EVENT      (ESTOP_APP_SCALER_NODE_COMPLETE_EVENT + 1)

using namespace std;

typedef struct
{
    /* Graph parameter 0 */
    vx_image                vxInputLeftImage;

    /* Graph parameter 1 */
    vx_image                vxInputRightImage;

    /* Graph parameter 2 */
    vx_image                vxRightRectImage;

    /* Graph parameter 3 */
    vx_image                vxSde16BitOutput;

    /* Graph parameter 3 */
    vx_image                vxMergeDisparityL0;

    /* Graph parameter 3 */
    vx_image                vxMedianFilteredDisparity;

    /* Graph parameter 6 */
    vx_user_data_object     vx3DBoundBox;

    /* Graph parameter 4 */
    vx_image                vxScalerOut;

    /* Graph Parameter 5 */
    vx_tensor               vxOutTensor;

    /* Input to DLR node. */
    float                 * dlrInputBuff;

    /* Output from the DLR node. */
    int32_t               * dlrOutputBuff;

} ESTOP_APP_graphParams;

using ESTOP_APP_graphParamQ = std::queue<ESTOP_APP_graphParams*>;


typedef struct
{
    /** Live online visualization (1=on,0=off) */
    uint32_t                               display_option;

    vx_context                             vxContext;
    vx_graph                               vxGraph;
    vx_graph                               vxDispGraph;

    vx_node                                node_disparity_display;
    vx_user_data_object                    bb_display_config;
    tivx_display_params_t                  bb_display_params;
    vx_node                                node_image_display;
    vx_user_data_object                    ss_display_config;
    tivx_display_params_t                  ss_display_params;

    vx_image                               vxInputLeftImage[ESTOP_APP_MAX_PIPELINE_DEPTH];
    vx_image                               vxInputRightImage[ESTOP_APP_MAX_PIPELINE_DEPTH];
    vx_image                               vxLeftRectImage;
    vx_image                               vxRightRectImage[ESTOP_APP_MAX_PIPELINE_DEPTH];
    vx_image                               vxSde16BitOutput[ESTOP_APP_MAX_PIPELINE_DEPTH];
    vx_image                               vxMergeDisparityL0[ESTOP_APP_MAX_PIPELINE_DEPTH];
    vx_image                               vxMedianFilteredDisparity[ESTOP_APP_MAX_PIPELINE_DEPTH];
    vx_user_data_object                    vx3DBoundBox[ESTOP_APP_MAX_PIPELINE_DEPTH];

    vx_image                               vxBBImage;
    vx_image                               vxDispRightImage;
    vx_image                               vxDispSSImage;
    vx_user_data_object                    vxDisp3DBoundBox;

    vx_image                               vxDisparity16;
    vx_image                               vxDisparityCC;

    /** 3D bounding box memory size */
    int32_t                                bbSize;

    /** Input image width */
    int16_t                                width;
    /** Input image height */
    int16_t                                height;

    /** Tensor width */
    int16_t                                tensor_width;
    /** Tensor height */
    int16_t                                tensor_height;

    /** Number of classed that will be color coded in a post processing node */
    int16_t                                numClasses;

    /** Mean values to be used in pre-processing stage. */
    float                                  preProcMean[CM_PREPROC_MAX_IMG_CHANS];

    /** Scaling values to be used in pre-processing stage. */
    float                                  preProcScale[CM_PREPROC_MAX_IMG_CHANS];

    /** Flag to indicate if PostProc node will need to be created. If true, then
     *  the TIDL node will be the tail node, otherwise the PostProc node will
     *  be the tail node.
     */
    uint8_t                                enablePostProcNode;
    /** Input image format */
    uint8_t                                inputFormat;

    uint8_t                                pipelineDepth;

    uint8_t                                sdeAlgoType;
    uint8_t                                numLayers;
    uint8_t                                ppMedianFilterEnable;
    uint8_t                                confidence_threshold;

    tivx_dmpac_sde_params_t                sde_params;

    /** camera parameters */
    float                                  distCenterX;
    float                                  distCenterY;
    float                                  focalLength;
    float                                  camRoll;
    float                                  camPitch;
    float                                  camYaw;
    float                                  camHeight;
    float                                  baseline;

    /** OG map parameters */
    /** X and Y grid size */
    int32_t                                xGridSize;
    int32_t                                yGridSize;
    /** Min and Max X range in mm */
    int32_t                                xMinRange;
    int32_t                                xMaxRange;
    /** Min & Max Y range in mm */
    int32_t                                yMinRange;
    int32_t                                yMaxRange;
    /** Number of grid in X and Y dimension */
    int32_t                                xGridNum;
    int32_t                                yGridNum;
    /** Pixel count threshold of grid for occupied/non-occupied decision */
    int16_t                                thCnt;
    /** Pixel count threshold of object for occupied/non-occupied decision */
    int16_t                                thObjCnt;
    /** Maximum number of objects to be detected */
    int16_t                                maxNumObject;
    /** Number of neighboring grids to check for connected component analysis */
    int16_t                                cNeighNum;
    /** Enable flag of spatial object merge */
    uint8_t                                enableSpatialObjMerge;
    /** Enable flag of temporal object merge */
    uint8_t                                enableTemporalObjMerge;
    /** Enable flag of temporal object smoothing */
    uint8_t                                enableTemporalObjSmoothing;
    /** Method to compute distance between objects
     *  0: distance between centers
     *  1: distacne between corners
     */
    uint8_t                                objectDistanceMode;
    /** Object bounding box type
     *  0: 2D bouding box
     *  1: 3D bounding box
     */
    uint8_t                                boundingBoxType;

    /* graph parameter tracking */
    ESTOP_APP_graphParams                  paramDesc[ESTOP_APP_MAX_PIPELINE_DEPTH];

    /** A queue for holding free descriptors. */
    ESTOP_APP_graphParamQ                  freeQ;

    /** Queue for output processing. */
    ESTOP_APP_graphParamQ                  outputQ;

    /** Queue for DLR processing. */
    ESTOP_APP_graphParamQ                  dlrInputQ;

    /** Resource lock. Used get/put from/to freeQ. */
    std::mutex                             paramRsrcMutex;

    /** Resource lock. Used get/put from/to dlrInputQ. */
    std::mutex                             dlrRsrcMutex;

    /** Resource lock. Used get/put from/to outputQ. */
    std::mutex                             outRsrcMutex;

    char                                   left_img_file_path[ESTOP_APP_MAX_LINE_LEN];
    char                                   right_img_file_path[ESTOP_APP_MAX_LINE_LEN];
    char                                   output_file_path[ESTOP_APP_MAX_LINE_LEN];

    char                                   dlrModelPath[ESTOP_APP_MAX_LINE_LEN];

    char                                   left_img_file_name[ESTOP_APP_MAX_LINE_LEN];
    char                                   right_img_file_name[ESTOP_APP_MAX_LINE_LEN];

    char                                   left_LUT_file_name[ESTOP_APP_MAX_LINE_LEN];
    char                                   right_LUT_file_name[ESTOP_APP_MAX_LINE_LEN];

    uint32_t                               start_fileno;
    uint32_t                               end_fileno;
    
    uint32_t                               displayFrmNo;

    /** Application interactive status, 0=non-interactive, 1=interactive. */
    uint32_t                               is_interactive;

    /** Total radar frames processed */
    uint32_t                               frameCnt;

    /** LDC applib create params */
    SDELDCAPPLIB_createParams              sdeLdcCreateParams;

    /** LDC applib handler */
    SDELDCAPPLIB_Handle                    sdeLdcHdl;

    /** Single-layer SDE applib create params */
    SL_SDEAPPLIB_createParams              slSdeCreateParams;

    /** Single-layer SDE applib handler */
    SL_SDEAPPLIB_Handle                    slSdeHdl;

    /** Multi-layer SDE applib create params */
    ML_SDEAPPLIB_createParams              mlSdeCreateParams;

    /** Multi-layer SDE applib handler */
    ML_SDEAPPLIB_Handle                    mlSdeHdl;

    /** Semantic segmentation applib create params */
    SEMSEG_CNN_APPLIB_createParams         ssCnnCreateParams;

    /** Semantic segmentation applib handler */
    SEMSEG_CNN_APPLIB_Handle               ssCnnHdl;

    /** OG Map based detector applib create params */
    SS_DETECT_APPLIB_createParams          ssDetectCreateParams;

    /** OG Map based detector applib handler */
    SS_DETECT_APPLIB_Handle                ssDetectHdl;

    /** Number of graph params that depends on config */
    uint8_t                                numGraphParams;

    /** Number of output tensor that depends on config */
    uint8_t                                numOutTensors;

    /** Render periodicity in milli-sec. */
    uint64_t                               renderPeriod;

    /** Flag to indicate that the input data processing thread should exit. */
    bool                                   exitInputDataProcess;

    /** Flag to indicate that the DLR thread should exit. */
    bool                                   exitDlrThread;

    /** Flag to indicate that the display thread should exit. */
    bool                                   exitDisplayThread;

    /** Flag to indicate that the input data processing has finished. */
    bool                                   processFinished;

    /** DLR node context object. */
    CM_DLRNodeCntxt                        dlrObj;

    /** Input data thread. */
    std::thread                            inputDataThread;

    /** DLR thread */
    std::thread                            dlrThread;

    /** Event handler thread. */
    std::thread                            evtHdlrThread;

    /** Display thread. */
    std::thread                            dispThread;

    /** Semaphore to synchronize the input and input data processing threads. */
    UTILS::Semaphore                     * dataReadySem;

    /** Semaphore to synchronize the DLR iput data and DLR threads. */
    UTILS::Semaphore                     * dlrDataReadySem;

    /** Semaphore to synchronize the output and display threads. */
    UTILS::Semaphore                     * displayCtrlSem;

    /** Drawing */
    Draw2D_Handle                          pHndl;
    uint16_t                             * pDisplayBuf565;

    /** Flag to indicate if the graph should be exported
     * 0 - disable
     * 1 - enable
     */
    uint8_t                                exportGraph;

    /** Real-time logging enable.
     * 0 - disable
     * 1 - enable
     */
    uint8_t                                rtLogEnable;

    /** Base value to be used for any programmed VX events. */
    uint32_t                               vxEvtAppValBase;

    /** Performance monitoring. */
    app_perf_point_t                       estopPerf;

    /** Flag to track if the performance counter has been initialized. */
    bool                                   startPerfCapt;



} ESTOP_APP_Context;

#endif /* _APP_ESTOP_MAIN_H_ */

