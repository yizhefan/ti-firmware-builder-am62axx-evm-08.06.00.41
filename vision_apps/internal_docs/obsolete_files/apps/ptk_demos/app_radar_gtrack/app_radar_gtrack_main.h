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
#ifndef _RADAR_GTRACK_MAIN_H_
#define _RADAR_GTRACK_MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <TI/tivx.h>
#include <TI/tivx_park_assist.h>

#include <perception/perception.h>
#include <perception/dbtools.h>
#include <perception/utils/radar_ogmap_parse_config.h>
#include <perception/utils/radar_gtrack_parse_config.h>
#include <perception/utils/ptk_semaphore.h>

#include "radar_gtrack_applib_priv.h"

using namespace std;
using namespace ptk;
using namespace UTILS;

/* The following user event is used to set the exit condition. */
#define RADARGTAPP_EVENT_BASE              (25U)
#define RADARGTAPP_RADAR_GRAPH_COMP_EVENT  (RADARGTAPP_EVENT_BASE + 1U)
#define RADARGTAPP_USER_EVT_EXIT           (RADARGTAPP_EVENT_BASE + 2U)

typedef struct
{
    /** Verbosity, 0=no prints, 1=info prints, >1=debug prints. */
    uint32_t                            verbose;

    /** Application interactive status, 0=non-interactive, 1=interactive. */
    uint32_t                            is_interactive;

    /** Sensor rate control flag. If set to true then the inter-frame delay
     * based on the timestamps associated with the data frames will be used.
     */
    bool                                sensorRateControl;

    /** Input rate control flag. If set to true then a semaphore will be
     * used to control the synchronization between the input data and the
     * graph processing threads.
     */
    bool                                inputRateControl;

    /** Stream handle for each sensor.
     *  Handle is null if sensor is not enabled.
     */
    SensorDataPlayer                   *dataPlayer;

    /** database config file (defines sensor inputs and outputs to
     *  be saved as virtual sensor) */
    PTK_DBConfig                        dbConfig;

    /** OpenVX references */
    vx_context                          vxContext;

    /** Create Parameters. */
    RADAR_GTRACK_APPLIB_createParams    createParams;

    /** Module handle. */
    RADAR_GTRACK_APPLIB_Handle          radarHdl;

    /** Output File Name */
    FILE *                              outFilePtr;

    /** Total radar frames processed. */
    uint32_t                            totalFrameCount;

    /** Total radar frames processed. */
    uint32_t                            framesProcessed;

    /** Total radar frames dropped. */
    uint32_t                            droppedFrameCnt;

    /** Flag to indicate that the graph processing thread should exit. */
    bool                                exitGraphProcess;

    /** Input data processing thread. */
    std::thread                         inputDataThread;

    /** Event handler thread. */
    std::thread                         evtHdlrThread;

    /** Semaphore for rate synchronizing the input data and
     * graph processing threads.
     */
    UTILS::Semaphore                   *dataReadySem;

    /** Counter for tracking number of times the data has been played.*/
    uint32_t                            runCtr;

    /** Pointer to the latest output buffer. This is used for final checksum
     * validation purposes only.
     */
    RADAR_GTRACK_APPLIB_OutputBuff      outBuff;

    /** Flag to indicate if checksum needs to be performed on the final
     *  output accumulated map.
     */
    bool                                doChecksum;

    /** Expected value of checksum. This is looked at only if doChecksum
     *  is true.
     */
    uint32_t                            expectedChecksum;

    /** Computed checksum. */
    uint32_t                            computedChecksum;

} RADARGTAPP_Context;

#endif // _RADAR_GTRACK_MAIN_H_

