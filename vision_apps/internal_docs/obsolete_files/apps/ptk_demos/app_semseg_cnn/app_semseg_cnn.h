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
#ifndef _APP_SEMSEG_CNN_H_
#define _APP_SEMSEG_CNN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <thread>

#include <TI/tivx.h>
#include <TI/tivx_img_proc.h>
#include <TI/tivx_stereo.h>
#include <TI/j7_tidl.h>

#include <perception/perception.h>
#include <perception/utils/ptk_semaphore.h>

#include <semseg_cnn_applib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <utils/draw2d/include/draw2d.h>
#include <utils/grpx/include/app_grpx.h>
#include <utils/perf_stats/include/app_perf_stats.h>

#define SEMSEG_CNN_PERF_OUT_FILE        "apps_semseg_cnn"

#define SEMSEG_CNN_APP_MAX_FILE_PATH    (1024U)
#define APP_ASSERT(x)                   PTK_assert((x))

#define SEMSEG_CNN_APP_MAX_LINE_LEN     (1024U)
#define SEMSEG_CNN_APP_NUM_BUFF_DESC    (1U)

#define OUTPUT_DISPLAY_WIDTH            (960U)
#define OUTPUT_DISPLAY_HEIGHT           (480U)

#define INPUT_DISPLAY_WIDTH             (960U)
#define INPUT_DISPLAY_HEIGHT            (480U)

#define INPUT_START_X_OFFSET            (0U)
#define INPUT_START_Y_OFFSET            (300U)

#define SEMSEG_CNN_APP_MAX_TENSOR_DIMS  (4u)
#define SEMSEG_CNN_APP_USER_EVT_EXIT    (SEMSEG_CNN_APPLIB_OUT_AVAIL_EVT + 1)

struct SEMSEG_CNN_APP_Context
{
    /** */
    vx_graph                        vxDispGraph;

    /** */
    vx_node                         vxInDispNode;

    /** */
    vx_user_data_object             vxInDispCfg;

    /** */
    vx_node                         vxOutDispNode;

    /** */
    vx_user_data_object             vxOutDispCfg;

    /** */
    vx_image                        vxInputImage;

    /** */
    vx_image                        vxDispInputImage;

    /** */
    vx_image                        vxOutputImage;

    /** */
    vx_user_data_object             vxNNConfig;

    /** */
    vx_tensor                       vxOutputTensor;

    /** Image type. Refer to <tt>\ref vx_df_image_e</tt> for enumerations. */
    int32_t                         inputImageType;

    /** */
    char                            ldcLutFilePath[SEMSEG_CNN_APP_MAX_FILE_PATH];

    /** */
    char                            tidlCfgFilePath[SEMSEG_CNN_APP_MAX_FILE_PATH];

    /** */
    char                            tidlNwFilePath[SEMSEG_CNN_APP_MAX_FILE_PATH];

    /** */
    char                            dlrModelPath[SEMSEG_CNN_APP_MAX_FILE_PATH];

    /** */
    char                            inFilePath[SEMSEG_CNN_APP_MAX_LINE_LEN];

    /** */
    char                            outFilePath[SEMSEG_CNN_APP_MAX_LINE_LEN];

    /** */
    uint32_t                        startFileNum;

    /** */
    uint32_t                        endFileNum;

    /** Total radar frames processed. */
    uint32_t                        totalFrameCount;

    /** Total radar frames processed. */
    uint32_t                        framesProcessed;

    /** Total radar frames dropped. */
    uint32_t                        droppedFrameCnt;

    /** Counter for tracking number of times the data has been played.*/
    uint32_t                        runCtr;

    /** */
    uint32_t                        displayFrNum;

    /** Application interactive status, 0=non-interactive, 1=interactive. */
    uint8_t                         is_interactive;

    /** Display option (1=on, 0=off) */
    uint8_t                         dispEnable;

    /** */
    vx_int32                        inDispWidth;

    /** */
    vx_int32                        inDispHeight;

    /** */
    vx_int32                        outDispWidth;

    /** */
    vx_int32                        outDispHeight;

    /** */
    SEMSEG_CNN_APPLIB_createParams  createParams;

    /** */
    SEMSEG_CNN_APPLIB_Handle        sscnnHdl;

    /** */
    vx_context                      vxContext;

    /** DLR node context object. */
    CM_DLRNodeCntxt                 dlrObj;

    /** Flag to indicate that the graph processing thread should exit. */
    bool                            exitInputDataProcess;

    /** Flag to indicate that the display thread should exit. */
    bool                            exitDisplayThread;

    /** Graph processing thread. */
    std::thread                     inputDataThread;

    /** Event handler thread. */
    std::thread                     evtHdlrThread;

    /** Event handler thread. */
    std::thread                     dispThread;

    /** Semaphore to synchronize the input and graph processing threads. */
    UTILS::Semaphore               *dataReadySem;

    /** Semaphore to synchronize the output and display threads. */
    UTILS::Semaphore               *displayCtrlSem;

    /** Delay in milli-sec to introduce between successive input frames.
     * Valid only for file based input.
     */
    uint32_t                        interFrameDelay;

    /** Flag to indicate if the graph should be exported
     * 0 - disable
     * 1 - enable
     */
    uint8_t                         exportGraph;

    /** Real-time logging enable.
     * 0 - disable
     * 1 - enable
     */
    uint8_t                         rtLogEnable;
};

void SEMSEG_CNN_APP_parseCfgFile(SEMSEG_CNN_APP_Context *appCntxt, const char *cfg_file_name);

vx_status SEMSEG_CNN_APP_init(SEMSEG_CNN_APP_Context *appCntxt);

void SEMSEG_CNN_APP_launchProcThreads(SEMSEG_CNN_APP_Context *appCntxt);

void SEMSEG_CNN_APP_intSigHandler(SEMSEG_CNN_APP_Context *appCntxt);

void SEMSEG_CNN_APP_cleanupHdlr(SEMSEG_CNN_APP_Context *appCntxt);

#ifdef __cplusplus
}
#endif

#endif /* _APP_SEMSEG_CNN_H_ */
