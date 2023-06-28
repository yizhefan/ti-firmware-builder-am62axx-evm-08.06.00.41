 /*
 *******************************************************************************
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 *******************************************************************************
 */

#ifndef _APP_SS_SDE_DETECTION_MAIN_H_
#define _APP_SS_SDE_DETECTION_MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <TI/tivx.h>
#include <TI/tivx_stereo.h>

#include <perception/perception.h>
#include <perception/utils/ipc_chan.h>
#include <perception/utils/ptk_semaphore.h>

#include <ss_sde_detection_applib.h>

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


#define SS_DETECT_APP_MAX_LINE_LEN         (512U)
#define SS_DETECT_APP_NUM_BUFF_DESC        (1U)

#define OUTPUT_DISPLAY_WIDTH        960
#define OUTPUT_DISPLAY_HEIGHT       480

#define INPUT_DISPLAY_WIDTH         960
#define INPUT_DISPLAY_HEIGHT        480

#define SS_DETECT_APP_USER_EVT_EXIT (1U)




typedef struct
{
    /** Live online visualization (1=on,0=off) */
    uint32_t                               display_option;

#if 0
    /** Verbosity, 0=no prints, 1=info prints, >1=debug prints. */
    uint32_t                               verbose;
#endif

    vx_graph                               graph_display;

    vx_node                                node_disparity_display;
    vx_user_data_object                    disparity_display_config;
    tivx_display_params_t                  disparity_display_params;
    vx_node                                node_image_display;
    vx_user_data_object                    image_display_config;
    tivx_display_params_t                  image_display_params;

    vx_image                               vxRightImage;
    vx_image                               vxDisparity16;
    vx_tensor                              vxSSMapTensor;
    vx_image                               vxDisparityCC;
    vx_image                               vxBBImage;

    vx_user_data_object                    vx3DBoundBox;
    vx_image                               vxDispRightImage;
    vx_image                               vxDispDisparity16;

    int16_t                                width;
    int16_t                                height;
    int16_t                                tensor_width;
    int16_t                                tensor_height;
    uint8_t                                inputFormat;
    uint8_t                                confidence_threshold;
    uint8_t                                isROSInterface;

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

    char                                   image_file_path[SS_DETECT_APP_MAX_LINE_LEN];
    char                                   tensor_file_path[SS_DETECT_APP_MAX_LINE_LEN];
    char                                   disparity_file_path[SS_DETECT_APP_MAX_LINE_LEN];
    char                                   output_file_path[SS_DETECT_APP_MAX_LINE_LEN];

    char                                   image_file_name[SS_DETECT_APP_MAX_LINE_LEN];
    char                                   tensor_file_name[SS_DETECT_APP_MAX_LINE_LEN];
    char                                   disparity_file_name[SS_DETECT_APP_MAX_LINE_LEN];

    uint32_t                               start_fileno;
    uint32_t                               end_fileno;
    
    uint32_t                               displayFrmNo;

    /** Application interactive status, 0=non-interactive, 1=interactive. */
    uint32_t                               is_interactive;

    /** Total radar frames processed */
    uint32_t                               frameCnt;


    SS_DETECT_APPLIB_createParams          ssDetectCreateParams;
    SS_DETECT_APPLIB_Handle                ssDetectHdl;

    vx_context                             vxContext;

    /** Render periodicity in milli-sec. */
    uint64_t                               renderPeriod;

    /** Flag to indicate that the input data processing thread should exit. */
    bool                                   exitInputDataProcess;

    /** Flag to indicate that the input data processing has finished. */
    bool                                   processFinished;

    /** Input data thread. */
    std::thread                            inputDataThread;

    /** Event handler thread. */
    std::thread                            evtHdlrThread;

    /** Semaphore to synchronize the input and input data processing threads. */
    UTILS::Semaphore                     * dataReadySem;

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

} SS_DETECT_APP_Context;

#endif /* _APP_SS_SDE_DETECTION_MAIN_H_ */

