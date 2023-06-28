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
#include <signal.h>
#include <thread>
#include <vector>
#include <string>
#include <getopt.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <utils/perf_stats/include/app_perf_stats.h>

#ifdef __cplusplus
}
#endif

#include <perception/utils/radar_ogmap_parse_config.h>
#include <perception/utils/radar_gtrack_parse_config.h>

#include "app_radar_gtrack_main.h"
#include "radar_gtrack_applib_priv.h"

#define APP_RGTRACK_MAX_LINE_LEN    (256U)
#define APP_RGTRACK_PERF_OUT_FILE   "apps_radar_gtrack"

using namespace std;
using namespace ptk;

/** sensor inputs */
typedef enum
{
    RADARGTAPP_SENSOR_RADAR_0 = 0,
    RADARGTAPP_SENSOR_RADAR_1,
    RADARGTAPP_SENSOR_RADAR_2,
    RADARGTAPP_SENSOR_RADAR_3,
    RADARGTAPP_SENSOR_INS,
    RADARGTAPP_SENSOR_MAX
} RADARGTAPP_SensorType;

static RADARGTAPP_Context  gTestAppCntxt{0};

/* Name of output file. */
char outFileName[50] = "file_out.txt";

static char
gSensorAppTags[RADARGTAPP_SENSOR_MAX][DBCONFIG_MAX_WORD_LEN] =
{
    {"RAD_0"},
    {"RAD_1"},
    {"RAD_2"},
    {"RAD_3"},
    {"INS"  }
};

static void RADARGTAPP_appShowUsage(const char *name)
{
    PTK_printf("\n");
    PTK_printf("Radar Group Tracker Demo - (c) Texas Instruments 2020\n");
    PTK_printf("========================================================\n");
    PTK_printf("\n");
    PTK_printf("Please refer to demo guide for prerequisites before running this demo\n");
    PTK_printf("\n");
    PTK_printf("USAGE:\n");
    PTK_printf("# %s PARAMETERS [OPTIONAL PARAMETERS]\n", name);
    PTK_printf("\n");
    PTK_printf("# PARAMETERS:\n");
    PTK_printf("#  --cfg        |-c <config file>\n");
    PTK_printf("# \n");
    PTK_printf("# OPTIONAL PARAMETERS:\n");
    PTK_printf("#  [--checksum  |-k] Perform checksum validation at the end.");
    PTK_printf(" Specifying this will turn off the interactive mode.\n");
    PTK_printf("#  [--help      |-h] Display help and exit.\n");
    PTK_printf("# \n");
    PTK_printf("# \n");
    PTK_printf("\n");

    exit(0);

} // RADARGTAPP_appShowUsage

static char menu[] = {
    "\n"
    "\n ========================================"
    "\n Demo : Radar Group Tracker"
    "\n ========================================"
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n e: Export performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};

static int32_t RADARGTAPP_parseCfgFile(RADARGTAPP_Context  *appCntxt,
                                       char                *cfgFileName)
{
    RADAR_GTRACK_APPLIB_createParams  *createParams;
    PTK_Alg_RadarGTrackParams         *gTrackParams;
    PTK_Alg_RadarOgmapParams          *ogConfig;
    char                              *globalBasePath;
    char                              *localBasePath;
    FILE                              *fptr;
    char                              *pParamStr;
    char                              *pValueStr;
    char                              *pSLine;
    char                              *filePath;
    char                               paramSt[APP_RGTRACK_MAX_LINE_LEN];
    char                               valueSt[APP_RGTRACK_MAX_LINE_LEN];
    char                               filePathArr[APP_RGTRACK_MAX_LINE_LEN];
    char                               sLine[APP_RGTRACK_MAX_LINE_LEN];
    char                               localBasePathArr[APP_RGTRACK_MAX_LINE_LEN] = {0};
    int32_t                            pos;
    int32_t                            status;

    status         = 0;
    pParamStr      = paramSt;
    pValueStr      = valueSt;
    pSLine         = sLine;
    createParams   = &appCntxt->createParams;
    gTrackParams   = &createParams->gTrackParams;
    ogConfig       = &createParams->ogConfig;
    globalBasePath = getenv("APP_CONFIG_BASE_PATH");
    localBasePath  = localBasePathArr;
    filePath       = filePathArr;

    if (globalBasePath == NULL)
    {
        PTK_printf("Please define APP_CONFIG_BASE_PATH environment variable.\n");
        status = -1;
    }

    if (status == 0)
    {
        /* get local base path (directory of cfgFileName) */
        pos = ptkdemo_find_slash(cfgFileName, APP_RGTRACK_MAX_LINE_LEN);

        if (pos >= 0)
        {
            strncpy(localBasePath, cfgFileName, pos + 1);
        }
        else
        {
            strcpy(localBasePath, "./");
        }

        fptr = fopen(cfgFileName, "r");

        if (fptr == NULL)
        {
            PTK_printf("Cannot open %s for reading.\n", cfgFileName);
            status = -1;
        }
    }

    if (status == 0)
    {
        appCntxt->expectedChecksum  = 0;
        appCntxt->inputRateControl  = false;
        appCntxt->sensorRateControl = false;
        createParams->pipelineDepth = RADAR_GTRACK_APPLIB_PIPELINE_DEPTH;
        createParams->exportGraph   = 0;
        createParams->rtLogEnable   = 0;

        while ( 1 )
        {
            pSLine = fgets(pSLine, APP_RGTRACK_MAX_LINE_LEN, fptr);

            if (pSLine == NULL)
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

            if ((pParamStr[0] == '\0') || (pValueStr[0] == '\0'))
            {
                continue;
            }

            if (strcmp(pParamStr, "verbose") == 0)
            {
                appCntxt->verbose = atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "is_interactive") == 0)
            {
                appCntxt->is_interactive = atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "pipeline_depth") == 0)
            {
                createParams->pipelineDepth = atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "rtLogEnable") == 0)
            {
                createParams->rtLogEnable = (uint8_t)atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "exportGraph") == 0)
            {
                createParams->exportGraph = (uint8_t)atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "sensor_rate_control") == 0)
            {
                appCntxt->sensorRateControl = (bool)atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "input_rate_control") == 0)
            {
                appCntxt->inputRateControl = (bool)atoi(pValueStr);
            }
            else if (strcmp(pParamStr, "expected_checksum") == 0)
            {
                appCntxt->expectedChecksum = strtol(pValueStr, NULL, 0);
            }
            else if (strcmp(pParamStr, "tiap_cfg_file") == 0)
            {
                ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                      localBasePath, APP_RGTRACK_MAX_LINE_LEN);

                PTK_DBConfig_parse(&appCntxt->dbConfig, filePath);

                /* TODO: Validate the number of sensors. Currently only one
                 * sensor is supported.
                 */
            }
            else if (strcmp(pParamStr, "gtrack_cfg_file") == 0)
            {
                ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                      localBasePath, APP_RGTRACK_MAX_LINE_LEN);

                PTK_Util_RadarGTrackParseConfig(filePath, gTrackParams);
            }
            else if (strcmp(pParamStr, "sensor_cfg_file") == 0)
            {
                ptkdemo_get_file_path(&filePath, pValueStr, globalBasePath,
                                      localBasePath, APP_RGTRACK_MAX_LINE_LEN);

                PTK_Util_RadarOgmapParseConfig(filePath, ogConfig);
            }
            else if (strcmp(pParamStr, "gtrack_deploy_core") == 0)
            {
                createParams->gTrackNodeCore =
                    app_common_get_coreName(pValueStr);
            }
        }

        fclose(fptr);

        /* If checksum is enabled then turn off the interactive mode. */
        if (appCntxt->doChecksum == true)
        {
            appCntxt->is_interactive = 0;
        }
    }

    return status;

} // RADARGTAPP_parseCfgFile

static int32_t RADARGTAPP_parseCmdLineArgs(RADARGTAPP_Context  *appCntxt,
                                           int32_t              argc,
                                           char                *argv[])
{
    char    name[APP_RGTRACK_MAX_LINE_LEN];
    int     longIndex;
    int     opt;
    int32_t status;
    static struct option long_options[] = {
        {"help",         no_argument,       0,  'h' },
        {"checksum",     no_argument,       0,  'k' },
        {"cfg",          required_argument, 0,  'c' },
        {0,              0,                 0,   0  }
    };

    appCntxt->doChecksum = false;
    name[0]              = '\0';
    status               = 0;

    while ((opt = getopt_long(argc, argv,"hkc:", 
                   long_options, &longIndex )) != -1)
    {
        switch (opt)
        {
            case 'k' :
                appCntxt->doChecksum = true;
                break;

            case 'c' :
                strncpy(name, optarg, APP_RGTRACK_MAX_LINE_LEN-1);
                break;

            case 'h' :
            default:
                RADARGTAPP_appShowUsage(argv[0]);
                break;
        }
    }

    if (name[0] == '\0')
    {
        PTK_printf("# ERROR: A valid configuration file MUST be specified.\n");
        RADARGTAPP_appShowUsage(argv[0]);
        status = -1;
    }

    if (status == 0)
    {
        status = RADARGTAPP_parseCfgFile(appCntxt, name);
    }

    return status;

} // RADARGTAPP_parseCmdLineArgs

static void RADARGTAPP_inputStreamInit(RADARGTAPP_Context *appCntxt)
{
    PTK_DBConfig   *cfg;
    vector<string>  appTagsVec;

    cfg = &appCntxt->dbConfig;

    for (uint32_t i = 0; i < RADARGTAPP_SENSOR_MAX; i++)
    {
        appTagsVec.push_back(string(gSensorAppTags[i]));
    }

    /* Initialize data streams. */
    appCntxt->dataPlayer = new SensorDataPlayerINS(cfg,
                                                   appTagsVec,
                                                   RADARGTAPP_SENSOR_INS,
                                                   appCntxt->sensorRateControl);

    return;

} // RADARGTAPP_inputStreamInit

static void RADARGTAPP_outputStreamInit(RADARGTAPP_Context *appCntxt)
{
    /* Open output file to write data */
    appCntxt->outFilePtr = fopen(outFileName,"w");
    PTK_assert(appCntxt->outFilePtr != NULL);

    return;

} // RADARGTAPP_outputStreamInit

static void RADARGTAPP_inputStreamDeInit(RADARGTAPP_Context *appCntxt)
{
    delete appCntxt->dataPlayer;

    return;

} // RADARGTAPP_inputStreamDeInit

static void RADARGTAPP_outputStreamDeInit(RADARGTAPP_Context *appCntxt)
{
    /* Close output file after writing data */
    fclose(appCntxt->outFilePtr);

    return;

} // RADARGTAPP_outputStreamDeInit

static void RADARGTAPP_processOutput(RADARGTAPP_Context *appCntxt)
{
    RADAR_GTRACK_APPLIB_OutputBuff *outBuff;
    uint32_t                        descCnt;
    vx_status                       vxStatus;

    outBuff = &appCntxt->outBuff;

    vxStatus = RADAR_GTRACK_APPLIB_getOutBuff(appCntxt->radarHdl, outBuff);
    
    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        PTK_printf("[%s:%d] RADAR_GTRACK_APPLIB_getOutBuff() failed.\n",
                   __FUNCTION__,
                   __LINE__);
    }

    if (vxStatus == (vx_status)VX_SUCCESS)
    {
        PTK_Alg_RadarGTrackTargetInfo  *outInfo;

        outInfo = outBuff->trackInfo;

        fprintf(appCntxt->outFilePtr, "%d\n", outInfo->numDesc);
        fprintf(appCntxt->outFilePtr, "%f\n", outInfo->egoLocation.x);
        fprintf(appCntxt->outFilePtr, "%f\n", outInfo->egoLocation.y);

        /* Output the track information to a file. */

        for (descCnt = 0; descCnt < outInfo->numDesc; descCnt++)
        {
            fprintf(appCntxt->outFilePtr, "%d\n", outInfo->desc[descCnt].tid);
            fprintf(appCntxt->outFilePtr, "%f\n", outInfo->desc[descCnt].S[0]);
            fprintf(appCntxt->outFilePtr, "%f\n", outInfo->desc[descCnt].S[1]);
            fprintf(appCntxt->outFilePtr, "%f\n", outInfo->desc[descCnt].S[2]);
        }

        /* Output the track information to a file. */
        fflush(appCntxt->outFilePtr);

        RADAR_GTRACK_APPLIB_releaseOutBuff(appCntxt->radarHdl);
    }

} // RADARGTAPP_processOutput

static void RADARGTAPP_run(RADARGTAPP_Context *appCntxt)
{
    uint32_t    sensorId;

    /* Read Radar object data, extract the {sensorId and timestamp}
     * fields, strip the header and pass it as 'data'.
     *
     * Read the IMU data, strip the header, interpret the first 8-bytes as
     * timestamp, pass &imuBuff[8] as 'data'. Set sensorId as 0.
     */
    appCntxt->totalFrameCount = 0;
    appCntxt->droppedFrameCnt = 0;
    appCntxt->framesProcessed = 0;
    sensorId                  = 0;

    /* Setup the reference frame. */
    {
        PTK_RigidTransform_d ecef_w;
        PTK_Position         ref;

        /* Set up world reference frame
         * (SensorDataPlayerINS handles initializing this at create time) */
        PTK_INS_getReferenceFrame(&ecef_w, &ref);

        /* Set the reference in the radar module. */
        RADAR_GTRACK_APPLIB_setWorldReference(appCntxt->radarHdl, &ecef_w);
    }

    while (true)
    {
        void       *sensorData;
        uint64_t    sensorTs;
        size_t      sensorDataSize;
        int32_t     status;

        sensorId = appCntxt->dataPlayer->get_next(sensorData,
                                                  sensorDataSize,
                                                  sensorTs);
        if (sensorId == UINT32_MAX)
        {
            break;
        }

        /* Wait for the data ready semaphore. */
        if (appCntxt->dataReadySem)
        {
            appCntxt->dataReadySem->wait();
        }

        appCntxt->totalFrameCount++;

        status = RADAR_GTRACK_APPLIB_process(appCntxt->radarHdl,
                                             (uint8_t*)sensorData,
                                             sensorId,
                                             sensorTs);

        if (status < 0)
        {
            appCntxt->droppedFrameCnt++;
        }
        else
        {
            appCntxt->framesProcessed++;
        }

    } // while (true)

    PTK_printf("\e[K[%d] Frames processed = %d/%d Frames dropped = %d/%d\n\e[A",
               appCntxt->runCtr,
               appCntxt->framesProcessed,
               appCntxt->totalFrameCount,
               appCntxt->droppedFrameCnt,
               appCntxt->totalFrameCount);

    /* Wait for the graph to consume all input. */
    RADAR_GTRACK_APPLIB_waitGraph(appCntxt->radarHdl);

    return;

} // RADARGTAPP_run

static int32_t RADARGTAPP_init(RADARGTAPP_Context *appCntxt)
{
    int32_t     status;

    status = appInit();
    
    if (status != 0)
    {
        PTK_printf("[%s:%d] appInit() failed.\n", __FUNCTION__, __LINE__);
    }

    if (status == 0)
    {
        appCntxt->vxContext = vxCreateContext();
        
        if (appCntxt->vxContext == NULL)
        {
            PTK_printf("[%s:%d] vxCreateContext() failed.\n",
                       __FUNCTION__, __LINE__);

            status = -1;
        }
    }

    if (status == 0)
    {
        tivxParkAssistLoadKernels(appCntxt->vxContext);

        tivxHwaLoadKernels(appCntxt->vxContext);

        /* Initialize the INS context. */
        PTK_INS_initializeBuffers();

        /* Initialize the input streams. */
        RADARGTAPP_inputStreamInit(appCntxt);

        /* Initialize the output streams. */
        RADARGTAPP_outputStreamInit(appCntxt);

        /* Create the semaphore. */
        if (appCntxt->inputRateControl)
        {
            appCntxt->dataReadySem =
                new Semaphore(appCntxt->createParams.pipelineDepth);
        }
        else
        {
            appCntxt->dataReadySem = nullptr;
        }
    }

    if (status == 0)
    {
        RADAR_GTRACK_APPLIB_createParams   *createParams;

        createParams                  = &appCntxt->createParams;
        createParams->vxContext       = appCntxt->vxContext;
        createParams->vxEvtAppValBase = RADARGTAPP_EVENT_BASE;

        appCntxt->radarHdl = RADAR_GTRACK_APPLIB_create(createParams);
        
        if (appCntxt->radarHdl == NULL)
        {
            PTK_printf("[%s:%d] RADAR_GTRACK_APPLIB_create() failed.\n",
                       __FUNCTION__, __LINE__);

            status = -1;
        }
    }

    return status;

} // RADARGTAPP_init

static int32_t RADARGTAPP_deInit(RADARGTAPP_Context *appCntxt)
{
    int32_t     status;

    /* De-initialize the input streams. */
    RADARGTAPP_inputStreamDeInit(appCntxt);

    /* De-initialize the output streams. */
    RADARGTAPP_outputStreamDeInit(appCntxt);

    tivxParkAssistUnLoadKernels(appCntxt->vxContext);

    tivxHwaUnLoadKernels(appCntxt->vxContext);

    /* Release the context. */
    vxReleaseContext(&appCntxt->vxContext);

    status = appDeInit();

    if (status != 0)
    {
        PTK_printf("[%s:%d] appDeInit() failed.\n", __FUNCTION__, __LINE__);
    }

    if (appCntxt->dataReadySem)
    {
        delete appCntxt->dataReadySem;
    }

    return status;

} // RADARGTAPP_deInit

static void RADARGTAPP_exitProcThreads(RADARGTAPP_Context   * appCntxt,
                                       bool                   detach)
{
    vx_status   vxStatus;

    appCntxt->exitGraphProcess = true;

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
                               RADARGTAPP_USER_EVT_EXIT,
                               NULL);

    if (vxStatus != VX_SUCCESS)
    {
        PTK_printf("[%s:%d] vxSendUserEvent() failed.\n");
    }

    if (appCntxt->evtHdlrThread.joinable())
    {
        appCntxt->evtHdlrThread.join();
    }

} // RADARGTAPP_exitProcThreads

static int32_t RADARGTAPP_cleanupHdlr(RADARGTAPP_Context  * appCntxt)
{
    float   frac;
    int32_t status1;
    int32_t status = 0;

    /* Wait for the threads to exit. */
    RADARGTAPP_exitProcThreads(appCntxt, false);

    if (appCntxt->doChecksum == false)
    {
        PTK_printf("\nPress ENTER key to exit.\n");
        fflush(stdout);
        getchar();
    }

    PTK_printf("Total Number of RADAR frames     = %d\n",
               appCntxt->totalFrameCount);

    frac = 100.0/appCntxt->totalFrameCount;

    PTK_printf("Number of RADAR frames processed = %d (%.2f%%)\n",
               appCntxt->framesProcessed,
               appCntxt->framesProcessed * frac);

    PTK_printf("Number of RADAR frames dropped   = %d (%.2f%%)\n\n",
               appCntxt->droppedFrameCnt,
               appCntxt->droppedFrameCnt * frac);

    PTK_printf("========= BEGIN:PERFORMANCE STATS SUMMARY =========\n");
    appPerfStatsPrintAll();
    RADAR_GTRACK_APPLIB_printStats(appCntxt->radarHdl);
    PTK_printf("========= END:PERFORMANCE STATS SUMMARY ===========\n");

    if (appCntxt->doChecksum == true)
    {
        const uint8_t  *data;
        uint32_t        size;

        /* Get the pointer to the last cached output buffer. */
        data = (const uint8_t *)appCntxt->outBuff.trackInfo;

        size = sizeof(PTK_Alg_RadarGTrackTargetInfo);

        appCntxt->computedChecksum = ptkdemo_compute_checksum(data, size);

        PTK_printf("EXPECTED_CHECKSUM: 0x%8.8X COMPUTED_CHECKSUM: 0x%8.8X\n",
                   appCntxt->expectedChecksum,
                   appCntxt->computedChecksum);

        if (appCntxt->expectedChecksum == appCntxt->computedChecksum)
        {
            PTK_printf("TEST_PASSED\n");
        }
        else
        {
            PTK_printf("TEST_FAILED\n");
            status = -1;
        }
    }

    if (appCntxt->createParams.rtLogEnable == 1)
    {
        char name[256];

        snprintf(name, 255, "%s.bin", APP_RGTRACK_PERF_OUT_FILE);
        tivxLogRtTraceExportToFile(name);
    }

    /* Release the Application context. */
    RADAR_GTRACK_APPLIB_delete(&appCntxt->radarHdl);

    /* De-initialize the Application context. */
    status1 = RADARGTAPP_deInit(appCntxt);

    if ((status == 0) && (status1 != 0))
    {
        status = status1;
    }

    PTK_printf("[%s] Clean-up complete.\n", __FUNCTION__);

    return status;

} // RADARGTAPP_cleanupHdlr

static void RADARGTAPP_reset(RADARGTAPP_Context  * appCntxt)
{
    int32_t status;

    status = RADAR_GTRACK_APPLIB_reset(appCntxt->radarHdl);

    if (status < 0)
    {
        PTK_printf("[%s:%d] RADAR_GTRACK_APPLIB_reset() failed.\n",
                   __FUNCTION__,
                   __LINE__);
    }
}

static void RADARGTAPP_evtHdlrThread(RADARGTAPP_Context *appCntxt)
{
    vx_event_t  evt;
    vx_status   vxStatus;

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
            if(evt.type == VX_EVENT_USER)
            {
                if(evt.app_value == RADARGTAPP_USER_EVT_EXIT)
                {
                    break;
                }
            }

            RADAR_GTRACK_APPLIB_processEvent(appCntxt->radarHdl, &evt);

            if (evt.type == VX_EVENT_GRAPH_COMPLETED)
            {
                /* Process output. */
                RADARGTAPP_processOutput(appCntxt);

                /* Wakeup the input data thread. */
                if (appCntxt->dataReadySem)
                {
                    appCntxt->dataReadySem->notify();
                }
            }

        }

    } // while (true)

} // RADARGTAPP_evtHdlrThread

static void RADARGTAPP_inputDataThread(RADARGTAPP_Context  * appCntxt)
{
    PTK_printf("[%s] Launched.\n", __FUNCTION__);

    while (true)
    {
        appCntxt->runCtr++;

        RADARGTAPP_reset(appCntxt);

        /* Execute the graph. */
        RADARGTAPP_run(appCntxt);

        if (appCntxt->exitGraphProcess)
        {
            break;
        }

        /* De-initialize the input streams. */
        RADARGTAPP_inputStreamDeInit(appCntxt);

        /* De-initialize the output streams. */
        RADARGTAPP_outputStreamDeInit(appCntxt);

        /* Reset the INS context. */
        PTK_INS_resetBuffers();

        /* Initialize the input streams. */
        RADARGTAPP_inputStreamInit(appCntxt);

        /* Initialize the output streams. */
        RADARGTAPP_outputStreamInit(appCntxt);
    }

} // RADARGTAPP_inputDataThread

static void RADARGTAPP_launchProcThreads(RADARGTAPP_Context  * appCntxt)
{
    /* Launch the input data thread. */
    appCntxt->inputDataThread =
        std::thread(RADARGTAPP_inputDataThread, appCntxt);

    /* Launch the event handler thread. */
    appCntxt->evtHdlrThread =
        std::thread(RADARGTAPP_evtHdlrThread, appCntxt);

} // RADARGTAPP_launchProcThreads

static void RADARGTAPP_intSigHandler(int sig)
{
    RADARGTAPP_Context   *appCntxt;

    appCntxt = &gTestAppCntxt;

    /* Wait for the threads to exit. */
    RADARGTAPP_exitProcThreads(appCntxt, true);

    exit(0);

} // RADARGTAPP_intSigHandler

int main(int argc, char* argv[])
{
    RADARGTAPP_Context *appCntxt;
    PTK_CRT             ptkConfig;
    int32_t             status;

    /* Register the signal handler. */
    signal(SIGINT, RADARGTAPP_intSigHandler);

    /* Initialize PTK library. */
    ptkConfig.exit   = exit;
    ptkConfig.printf = printf;
    ptkConfig.time   = NULL;

    PTK_init(&ptkConfig);

    appCntxt = &gTestAppCntxt;

    status = RADARGTAPP_parseCmdLineArgs(appCntxt, argc, argv);

    if (status == 0)
    {
        /* Initialize the Application context. */
        status = RADARGTAPP_init(appCntxt);
    }

    if (status == 0)
    {
        /* Launch the graph processing thread. */
        RADARGTAPP_launchProcThreads(appCntxt);
    }

    if ((status == 0) && (appCntxt->is_interactive != 0))
    {
        uint32_t    done = 0;

        appPerfStatsResetAll();

        while (!done)
        {
            char    ch;

            printf(menu);
            ch = getchar();
            printf("\n");

            switch(ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    RADAR_GTRACK_APPLIB_printStats(appCntxt->radarHdl);
                    break;

                case 'e':
                    {
                        FILE *fp = NULL;
                        char *name = APP_RGTRACK_PERF_OUT_FILE;

                        fp = appPerfStatsExportOpenFile(".", name);
                        if (fp != NULL)
                        {
                            RADAR_GTRACK_APPLIB_exportStats(appCntxt->radarHdl,
                                                            fp, true);
                            appPerfStatsExportCloseFile(fp);
                        }
                        else
                        {
                            PTK_printf("Could not open [%s] for exporting "
                                       "performance data\n", name);
                        }
                    }
                    break;

                case 'x':
                    done = 1;
                    break;

            } // switch(ch)

            /* Consume the newline character. */
            if (ch != '\n')
            {
                getchar();
            }

        } // while (!done)

    } // if (appCntxt->is_interactive)

    if (status == 0)
    {
        status = RADARGTAPP_cleanupHdlr(appCntxt);
    }

    return status;

} // main
