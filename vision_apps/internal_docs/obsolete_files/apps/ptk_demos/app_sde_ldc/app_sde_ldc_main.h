 /*
 *******************************************************************************
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 *******************************************************************************
 */

#ifndef _APP_ROSTEST_MAIN_H_
#define _APP_ROSTEST_MAIN_H_

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

#include <sde_ldc_applib.h>

#ifdef __cplusplus
extern "C" {
#endif


#include <utils/draw2d/include/draw2d.h>
#include <utils/grpx/include/app_grpx.h>

#define APP_ASSERT(x)               PTK_assert((x))
#define APP_ASSERT_VALID_REF(ref)   APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS)

#ifdef __cplusplus
}
#endif

#define SDELDCAPP_MAX_LINE_LEN            (512U)
#define SDELDCAPP_NUM_BUFF_DESC           (1U)

#define OUTPUT_DISPLAY_WIDTH              960
#define OUTPUT_DISPLAY_HEIGHT             480

#define INPUT_DISPLAY_WIDTH               960
#define INPUT_DISPLAY_HEIGHT              480

#define SDELDCAPP_MAX_PIPELINE_DEPTH      (2U)
#define SDELDCAPP_NUM_GRAPH_PARAMS        (4U)
#define SDELDCAPP_GRAPH_COMPLETE_EVENT    (0U)


#define SDELDCAPP_USER_EVT_EXIT           (1U)

using namespace std;

typedef struct
{
    /* Graph parameter 0 */
    vx_image                vxInputLeftImage;

    /* Graph parameter 1 */
    vx_image                vxInputRightImage;

    /* Graph parameter 2 */
    vx_image                vxOutputLeftImage;

    /* Graph parameter 3 */
    vx_image                vxOutputRightImage;

} SDELDCAPP_graphParams;

using SDELDCAPP_graphParamQ = std::queue<SDELDCAPP_graphParams*>;

typedef struct
{
    /** Live online visualization (1=on,0=off) */
    uint32_t                               display_option;

    vx_context                             vxContext;
    vx_graph                               vxGraph;
    vx_graph                               vxDispGraph;

    vx_node                                node_left_display;
    vx_user_data_object                    left_display_config;
    tivx_display_params_t                  left_display_params;
    vx_node                                node_right_display;
    vx_user_data_object                    right_display_config;
    tivx_display_params_t                  right_display_params;

    /* graph parameter tracking */
    SDELDCAPP_graphParams                  paramDesc[SDELDCAPP_MAX_PIPELINE_DEPTH];

        /** A queue for holding free descriptors. */
    SDELDCAPP_graphParamQ                  freeQ;

    /** Queue for output processing. */
    SDELDCAPP_graphParamQ                  outputQ;

    vx_image                               vxInputLeftImage[SDELDCAPP_MAX_PIPELINE_DEPTH];
    vx_image                               vxInputRightImage[SDELDCAPP_MAX_PIPELINE_DEPTH];
    vx_image                               vxOutputLeftImage[SDELDCAPP_MAX_PIPELINE_DEPTH];
    vx_image                               vxOutputRightImage[SDELDCAPP_MAX_PIPELINE_DEPTH];

    vx_image                               vxDisplayLeftImage;
    vx_image                               vxDisplayRightImage;

    char                                   left_img_file_path[SDELDCAPP_MAX_LINE_LEN];
    char                                   right_img_file_path[SDELDCAPP_MAX_LINE_LEN];
    char                                   left_img_file_name[SDELDCAPP_MAX_LINE_LEN];
    char                                   right_img_file_name[SDELDCAPP_MAX_LINE_LEN];
    char                                   output_img_file_path[SDELDCAPP_MAX_LINE_LEN];
    char                                   left_LUT_file_name[SDELDCAPP_MAX_LINE_LEN];
    char                                   right_LUT_file_name[SDELDCAPP_MAX_LINE_LEN];

    uint16_t                               width;
    uint16_t                               height;
    uint32_t                               start_fileno;
    uint32_t                               end_fileno;
    uint8_t                                pipelineDepth;

    uint8_t                                inputFormat;

    /** Application interactive status, 0=non-interactive, 1=interactive. */
    uint32_t                               is_interactive;

    /** Total frames processed */
    uint32_t                               frameCnt;
    uint32_t                               displayFrmNo;

    SDELDCAPPLIB_createParams              sdeLdcCreateParams;
    SDELDCAPPLIB_Handle                    sdeLdcHdl;

    /** Render periodicity in milli-sec. */
    uint64_t                               renderPeriod;

    /** Flag to indicate that the graph processing thread should exit. */
    bool                                   exitInputDataProcess;

    /** Flag to indicate that the graph processing thread has finished. */
    bool                                   processFinished;

    /** Graph processing thread. */
    std::thread                            inputDataThread;

    /** Event handler thread. */
    std::thread                            evtHdlrThread;

    /** Semaphore to synchronize the input and graph processing threads. */
    UTILS::Semaphore                      *dataReadySem;

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
    app_perf_point_t                       sdeLdcPerf;

    /** Flag to track if the performance counter has been initialized. */
    bool                                   startPerfCapt;

    /** Resource lock. */
    std::mutex                             paramRsrcMutex;


} SDELDCAPP_Context;

#endif /* _APP_ROSTEST_MAIN_H_ */

