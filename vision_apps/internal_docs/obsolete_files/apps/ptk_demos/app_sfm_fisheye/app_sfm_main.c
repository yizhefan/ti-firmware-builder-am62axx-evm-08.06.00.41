/*
 *
 * Copyright (c) 2017 Texas Instruments Incorporated
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


/* brief: App takes sequence of DOF field files as input, creates camera-normalized
 *        tracks and performs triangulation on the tracks using camera poses derived
 *        from INS measurements. Output is a sequence of 3D point clouds in camera
 *        coordinates.
 *
 *        The app has two modes:
 *              without LDC: it is assumed that DOF was computed on original
 *                        fisheye images. Track normalization includes fisheye
 *                        lens distortion correction and pinhole intrinsic parameters
 *              with LDC: it is assumed that DOF was computed on rectified images
 *                        which have principal point in the image center. The only
 *                        intrinsic parameter to be corrected for when constructing
 *                        normalized tracks is the focal length
 */

#include <signal.h>

#include <TI/tivx.h>
#include <TI/tivx_park_assist.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include <applib_support.h>
#include <app_ptk_demo_common.h>
#include <perception/dbtools/c/dbtools.h>

#define APP_MAX_FILE_PATH           (4096u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));

/** sensor inputs */
enum sensors_e
{
    APP_SENSOR_CAMERA = 0,
    APP_SENSOR_INS,
    APP_SENSOR_MAX
};

static char sensorAppTags[APP_SENSOR_MAX][DBCONFIG_MAX_WORD_LEN] = {{"CAM"},{"INS"}};

typedef enum app_mode_e
{
    WITHOUT_LDC = 0,  //!< DOF assumed to be computed on original fisheye frames
    WITH_LDC          //!< DOF assumed to be computed on LDC-corrected images
} AppMode;

typedef struct app_obj_t
{
    /* Input/Output data configuration */
    PTK_DBConfig                dbConfig; //!< database config file (defining inputs and outputs)

    /* input data streams */
    sensorDataPlayerInsHandle   dataPlayer;

    /* output data streams */
    vscHandle                   pcOut;        //<! 3D point cloud in camera coordinates

    /* App Control */
    AppMode                     mode;
    uint32_t                    verbose;    //<! verbosity, 0=no prints, 1=info prints, >1=debug prints

    /* Reference Frames */
    PTK_RigidTransform          M_c_e, M_e_c; //!< ego-camera (extrinsic calibration)

    /* OVX Node Parameters*/
    tivx_dof_to_tracks_params_t dtNodePrms;
    tivx_triangulation_params_t trNodePrms;
    uint32_t                    maxNumTracks;
    uint32_t                    dofWidth;  //!< width of DOF input
    uint32_t                    dofHeight; //!< height of DOF input

    /* OVX Node References */
    vx_context                  context;

    //graphs
    vx_graph                    sfm_graph;

    //nodes
    vx_node                     dof2tracks_node;
    vx_node                     triang_node;

    //data objects
    vx_image                    dof2tracks_in_field;
    vx_user_data_object         dof2tracks_in_config;
    vx_lut                      dof2tracks_in_lut;
    vx_user_data_object         triang_in_config;
    vx_array                    triang_in_tracks;
    vx_user_data_object         triang_in_pose;
    vx_array                    triang_out_points3d;

} AppObj;

AppObj gAppObj;


static void app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static void app_openvx_create(AppObj *obj);
static void app_openvx_delete(AppObj *obj);
static void app_data_streams_init(AppObj *obj);
static void app_data_streams_deinit(AppObj *obj);
static void app_set_params(AppObj *obj);
static void app_save_result(AppObj *obj, uint64_t curTimestamp);
static void app_dof2tracks_load_dof(AppObj *obj, void * dataPtr);
static void app_triang_load_camera_poses(AppObj *obj, uint64_t prvTimestamp, uint64_t curTimestamp);
static void app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[]);

/* lens d2u look-up table for IMI camera */
extern float ldcLUT_D2U_table[1024]; //@todo read from database

void mySigintHandler(int sig)
{
    exit(0);
}

int main(int argc, char* argv[])
{
    AppObj *obj = &gAppObj;
    vx_status status;
    void *dataPtr;
    size_t dataSize;
    uint64_t curTimestamp;
    uint64_t prvTimestamp;
    uint32_t sensorId;

    /* Register the signal handler. */
    signal(SIGINT, mySigintHandler);

    printf("APP SFM_FISHEYE STARTED!...\n\n");

    app_init(obj);

    app_parse_cmd_line_args(obj, argc, argv);

    PTK_printf("\n");

    /********************************************/
    /* inits                                    */
    /********************************************/
    app_data_streams_init(obj);

    app_set_params(obj);

    app_openvx_create(obj);

    /*******************************************/
    /* main loop (for each frame)              */
    /*******************************************/
    sensorId = SensorDataPlayerINS_get_next(obj->dataPlayer,
                                          &dataPtr,
                                          &dataSize,
                                          &curTimestamp);

    prvTimestamp = curTimestamp; /*triangulation on first frame will be junk */

    while (sensorId == APP_SENSOR_CAMERA)
    {
        PTK_printf("Processing frame at timestamp %" PRId64 "...\n",
                   curTimestamp);

        /* load DOF file */
        app_dof2tracks_load_dof(obj, (uint8_t *)dataPtr);

        /* load triangulation poses */
        app_triang_load_camera_poses(obj, prvTimestamp, curTimestamp);

        /* process graph */
        status = vxProcessGraph(obj->sfm_graph);
        APP_ASSERT(VX_SUCCESS == status);

        /* save result */
        app_save_result(obj, curTimestamp);

        PTK_printf("DONE\n\n");

        /* update */
        prvTimestamp = curTimestamp;

        /*get next record */
        sensorId = SensorDataPlayerINS_get_next(obj->dataPlayer,
                                                &dataPtr,
                                                &dataSize,
                                                &curTimestamp);
    }

    /*******************************************/
    /* deinits                                 */
    /*******************************************/
    app_openvx_delete(obj);

    app_data_streams_deinit(obj);

    app_deinit(&gAppObj);

    printf("\n\nDEMO FINISHED!\n");

    return 0;
}


/*===================================================================*/
/* SUPPORTING FUNCTIONS */
/*===================================================================*/
static void app_set_params(AppObj *obj)
{
    /* dof2tracks node config */
    //@todo: some of these must be passed from user or inferred from data files
    memset(&obj->dtNodePrms, 0, sizeof(tivx_dof_to_tracks_params_t));
    if (WITHOUT_LDC == obj->mode)
    {
        obj->dtNodePrms.dofConfidThresh = 9.f;
        obj->dtNodePrms.roiType = TIVX_DOF_TO_TRACKS_ROI_ELLIPSE;
        obj->dtNodePrms.roiPrm1 = 4.f;
        obj->dtNodePrms.roiPrm2 = 300.f * 300.f;
        obj->dtNodePrms.subsampleFactor = 2;
        obj->dtNodePrms.focalLength = 311.8333f;
        obj->dtNodePrms.principalPointX = 639.f;
        obj->dtNodePrms.principalPointY = 359.f;
        obj->dtNodePrms.d2uTableStep = 3.5889965e+02f;
        obj->dofWidth = 1280;
        obj->dofHeight = 720;
    }
    else
    {
        obj->dtNodePrms.dofConfidThresh = 9.f;
        obj->dtNodePrms.roiType = TIVX_DOF_TO_TRACKS_ROI_VERTICAL_CONE;
        obj->dtNodePrms.roiPrm1 = -4.f;
        obj->dtNodePrms.roiPrm2 = 648.f;
        obj->dtNodePrms.subsampleFactor = 2;
        obj->dtNodePrms.focalLength = 311.8333f;
        obj->dtNodePrms.principalPointX = 367.f; //640-1-tx
        obj->dtNodePrms.principalPointY = 479.f; //360-1-ty
        obj->dtNodePrms.d2uTableStep = 0.f; //not used
        obj->dofWidth = 736;
        obj->dofHeight = 928;
    }

    /* triangulation node config */
    memset(&obj->trNodePrms, 0, sizeof(tivx_triangulation_params_t));
    obj->trNodePrms.maxIters = 5;
    obj->trNodePrms.enableHighPrecision = 1;

    obj->maxNumTracks = 100000;

    /* References frames (extrinsic calibration) */
    ssHandle cam = SensorDataPlayerINS_get_sensorstream(obj->dataPlayer,
                                                        APP_SENSOR_CAMERA);
    float extCalib[12];
    sensorstream_get_extrinsic_calibration(cam, extCalib);
    PTK_RigidTransform_setRotation(&obj->M_c_e, extCalib);
    PTK_RigidTransform_setTranslation(&obj->M_c_e,
                                      extCalib[9],
                                      extCalib[10],
                                      extCalib[11]);
    if (obj->verbose)
    {
        PTK_printf("\nCamera Extrinsic Calibration (M_c_e): \n");
        PTK_RigidTransform_print(&obj->M_c_e);
    }
    PTK_RigidTransform_invert(&obj->M_e_c, &obj->M_c_e);
}

/*
 *   dof2tracks -> triangulation
 */
void app_openvx_create(AppObj *obj)
{
    vx_status status;

    /* CREATE DATA OBJECTS */

    /* dof2tracks - Data Objects */
    //config
    obj->dof2tracks_in_config =
        vxCreateUserDataObject(obj->context,
                               "tivx_dof_to_tracks_params_t",
                               sizeof(tivx_dof_to_tracks_params_t),
                               &obj->dtNodePrms);

    APP_ASSERT_VALID_REF(obj->dof2tracks_in_config);
    vxSetReferenceName((vx_reference)obj->dof2tracks_in_config,
                       "DOF2TracksConfig");

        //lut
    if (WITHOUT_LDC == obj->mode)
    {
        obj->dof2tracks_in_lut =
            vxCreateLUT(obj->context,
                        VX_TYPE_FLOAT32,
                        sizeof(ldcLUT_D2U_table)/sizeof(float));

        APP_ASSERT_VALID_REF(obj->dof2tracks_in_lut);
        vxSetReferenceName((vx_reference)obj->dof2tracks_in_lut,
                           "DistortionCorrectionLUT");
        status = vxCopyLUT(obj->dof2tracks_in_lut,
                           ldcLUT_D2U_table,
                           VX_WRITE_ONLY,
                           VX_MEMORY_TYPE_HOST);

        assert(VX_SUCCESS == status);
    }
    else
    {
        obj->dof2tracks_in_lut = NULL;
    }

    //DOF input
    obj->dof2tracks_in_field = vxCreateImage(obj->context,
                                             obj->dofWidth,
                                             obj->dofHeight,
                                             VX_DF_IMAGE_U32);

    APP_ASSERT_VALID_REF(obj->dof2tracks_in_field);
    vxSetReferenceName((vx_reference)obj->dof2tracks_in_field,
                       "DenseOpticalFlowField");

    /* Triangulation - Data Objects */
    //config
    obj->triang_in_config =
        vxCreateUserDataObject(obj->context,
                               "tivx_triangulation_params_t",
                               sizeof(tivx_triangulation_params_t),
                               &obj->trNodePrms);

    APP_ASSERT_VALID_REF(obj->triang_in_config);
    vxSetReferenceName((vx_reference)obj->triang_in_config,
                       "TriangulationConfig");

    //tracks
    vx_enum tivx_triangulation_track_e =
        vxRegisterUserStruct(obj->context,
                             sizeof(tivx_triangulation_track_t));
    obj->triang_in_tracks = vxCreateArray(obj->context,
                                          tivx_triangulation_track_e,
                                          obj->maxNumTracks);
    APP_ASSERT_VALID_REF(obj->triang_in_tracks);
    vxSetReferenceName((vx_reference)obj->triang_in_tracks, "Tracks2d");

    //poses
    obj->triang_in_pose =
        vxCreateUserDataObject(obj->context,
                               "tivx_triangulation_pose_t",
                               sizeof(tivx_triangulation_pose_t),
                               NULL);

    APP_ASSERT_VALID_REF(obj->triang_in_pose);
    vxSetReferenceName((vx_reference)obj->triang_in_pose, "CameraPoses");

    //points3d
    vx_enum tivx_triangulation_point3d_e =
        vxRegisterUserStruct(obj->context,
                             sizeof(PTK_Point));
    obj->triang_out_points3d =
        vxCreateArray(obj->context,
                      tivx_triangulation_point3d_e,
                      obj->maxNumTracks);

    APP_ASSERT_VALID_REF(obj->triang_out_points3d);
    vxSetReferenceName((vx_reference)obj->triang_out_points3d, "Points3d");

    /* CREATE GRAPHS */

    /* SfM Graph */
    obj->sfm_graph = vxCreateGraph(obj->context);
    APP_ASSERT_VALID_REF(obj->sfm_graph);

    obj->dof2tracks_node = tivxDofToTracksNode(
                            obj->sfm_graph,
                            obj->dof2tracks_in_config,
                            obj->dof2tracks_in_field,
                            obj->dof2tracks_in_lut,
                            obj->triang_in_tracks);

    APP_ASSERT_VALID_REF(obj->dof2tracks_node);
    vxSetReferenceName((vx_reference)obj->dof2tracks_node, "DOF2Tracks");

    status = vxSetNodeTarget(obj->dof2tracks_node,
                             VX_TARGET_STRING,
                             TIVX_TARGET_DSP1);

    APP_ASSERT(status==VX_SUCCESS);

    obj->triang_node = tivxTriangulationNode(
                            obj->sfm_graph,
                            obj->triang_in_config,
                            obj->triang_in_tracks,
                            obj->triang_in_pose,
                            obj->triang_out_points3d);
    APP_ASSERT_VALID_REF(obj->triang_node);
    vxSetReferenceName((vx_reference)obj->triang_node, "Triangulation");

    status = vxSetNodeTarget(obj->triang_node,
                             VX_TARGET_STRING,
                             TIVX_TARGET_DSP1);

    APP_ASSERT(status==VX_SUCCESS);

    status = vxVerifyGraph(obj->sfm_graph);
    APP_ASSERT(status==VX_SUCCESS);

#if 0
    tivxExportGraphToDot(obj->sfm_graph, ".", "vx_app_sfm_fisheye");
#endif

   return;
}

void app_openvx_delete(AppObj *obj)
{
    vxReleaseNode(&obj->triang_node);
    vxReleaseNode(&obj->dof2tracks_node);

    vxReleaseGraph(&obj->sfm_graph);

    vxReleaseUserDataObject(&obj->dof2tracks_in_config);
    if (NULL != obj->dof2tracks_in_lut)
    {
        vxReleaseLUT(&obj->dof2tracks_in_lut);
    }
    vxReleaseImage(&obj->dof2tracks_in_field);
    vxReleaseUserDataObject(&obj->triang_in_config);
    vxReleaseArray(&obj->triang_in_tracks);
    vxReleaseUserDataObject(&obj->triang_in_pose);
    vxReleaseArray(&obj->triang_out_points3d);

    return;
}

void app_data_streams_init(AppObj *obj)
{
    /*inputs*/
    obj->dataPlayer = SensorDataPlayerINS_create(&obj->dbConfig,
                           sensorAppTags,
                           APP_SENSOR_MAX,
                           APP_SENSOR_INS);

    /* outputs */
    obj->pcOut = VirtualSensorCreator_create_by_dbconfig(&obj->dbConfig, "POINTCLOUD");
}

void app_data_streams_deinit(AppObj *obj)
{
    SensorDataPlayerINS_delete(obj->dataPlayer);

    VirtualSensorCreator_delete(obj->pcOut);
}

void app_save_result(AppObj *obj, uint64_t curTimestamp)
{
    vx_status status;
    vx_map_id map_id;
    vx_size stride = sizeof(vx_size);
    char buffer[APP_MAX_FILE_PATH] = {0};
    char *dstFilePath = buffer;
    uint8_t *memPtr;

    /* SAVE RESULT */

    /* POINTCLOUD */
    PTK_Point *points;
    PTK_PointCloudConfig cfg;
    PTK_PointCloud *pc = NULL;
    vx_size numPoints;
    size_t pcSize;

    /* get number of points */
    status = vxQueryArray(obj->triang_out_points3d, VX_ARRAY_NUMITEMS, &numPoints, sizeof(numPoints));
    assert(VX_SUCCESS == status);

    /*create point cloud */
    cfg.maxPoints = numPoints;
    pcSize =  PTK_PointCloud_getSize(&cfg);

    memPtr = malloc(pcSize);
    if (memPtr == NULL)
    {
        PTK_printf("[%s:%d] Memory allocation failed.\n",
                   __FUNCTION__, __LINE__);

        assert(0);
    }

    pc = PTK_PointCloud_init(memPtr, &cfg);

    /* convert to PTK point cloud */
    if (numPoints > 0)
    {
        status = vxMapArrayRange(obj->triang_out_points3d,
                                    0,
                                    numPoints,
                                    &map_id,
                                    &stride,
                                    (void **)&points,
                                    VX_READ_ONLY,
                                    VX_MEMORY_TYPE_HOST,
                                    0);
        assert(VX_SUCCESS == status);
        assert(NULL != points);

        PTK_PointCloud_addv(pc, points, numPoints);

        status = vxUnmapArrayRange(obj->triang_out_points3d, map_id);
        assert(VX_SUCCESS == status);
    }

    /* write to file */
    VirtualSensorCreator_add_record(obj->pcOut, (char **)&dstFilePath, curTimestamp, pc, pcSize);

    free(memPtr);
}

void app_dof2tracks_load_dof(AppObj *obj, void * dataPtr)
{
    vx_status status;
    vx_map_id map_id;
    void *ptr;
    vx_rectangle_t rect;
    vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;

    /* open OVX image for writing */
    rect.start_x = 0;
    rect.end_x = obj->dofWidth;
    rect.start_y = 0;
    rect.end_y = obj->dofHeight;

    status = vxMapImagePatch(obj->dof2tracks_in_field, &rect, (vx_uint32)0, &map_id, &addr, (void **)&ptr,  VX_READ_AND_WRITE, VX_MEMORY_TYPE_HOST, VX_NOGAP_X);
    APP_ASSERT(VX_SUCCESS == status);

    status = vxCopyImagePatch(obj->dof2tracks_in_field, &rect, (vx_uint32)0, &addr, dataPtr, VX_WRITE_ONLY,  VX_MEMORY_TYPE_HOST);
    APP_ASSERT(VX_SUCCESS == status);

    status = vxUnmapImagePatch(obj->dof2tracks_in_field, map_id);
    APP_ASSERT(VX_SUCCESS == status);

    return;
}

void app_triang_load_camera_poses(AppObj *obj, uint64_t prvTimestamp, uint64_t curTimestamp)
{
    vx_status status;
    tivx_triangulation_pose_t poses;

    //estimate ego pose (0=prv, 1=cur)
    #define TRIANGULATION_MAX_TRACKS_LENGTH (2) //doing only 2-view triangulation in this demo
    uint64_t timestamps[TRIANGULATION_MAX_TRACKS_LENGTH];
    PTK_RigidTransform M_en_e0[TRIANGULATION_MAX_TRACKS_LENGTH];
    uint32_t numViews = TRIANGULATION_MAX_TRACKS_LENGTH;
    timestamps[0] = prvTimestamp;
    timestamps[1] = curTimestamp;

    PTK_INS_RetCode imu_status = PTK_INS_getIMUPosesFromVelAndAtt(timestamps, numViews, M_en_e0);
    //if (PTK_INS_RETURN_CODE_OK != imu_status)
    //    PTK_printf("WARNING: PTK INS return code (%d) NOT OK (getIMUPosesFromVelAndAtt at timestamp %" PRId64 ")\n", imu_status, timestamps[0]);

    //use calbration to convert ego-pose to camera poses 1=cur, 0=prv, ref=cur camera
    PTK_RigidTransform M_c1_ref;
    PTK_RigidTransform M_c0_ref;

    PTK_RigidTransform_makeIdentity(&M_c1_ref);
    PTK_RigidTransform M_e0_e1;
    PTK_RigidTransform_invert(&M_e0_e1, &M_en_e0[1]);
    PTK_RigidTransform M_c0_e1;
    PTK_RigidTransform_compose(&M_c0_e1, &obj->M_c_e, &M_e0_e1);
    PTK_RigidTransform_compose(&M_c0_ref, &M_c0_e1, &obj->M_e_c);

    poses.view[0] = M_c0_ref;
    poses.view[1] = M_c1_ref;

    status = vxCopyUserDataObject(obj->triang_in_pose,
                                  0,
                                  sizeof(tivx_triangulation_pose_t),
                                  &poses,
                                  VX_WRITE_ONLY,
                                  VX_MEMORY_TYPE_HOST);

    APP_ASSERT(VX_SUCCESS == status);

    return;
}

void app_init(AppObj *obj)
{
    int32_t status;
    PTK_CRT ptkConfig;

    status = appInit();
    PTK_assert(status == 0);

    /* Initialize PTK library. */
    ptkConfig.exit   = exit;
    ptkConfig.printf = printf;
    ptkConfig.time   = NULL;
    PTK_init(&ptkConfig);

    obj->context = vxCreateContext();

    tivxParkAssistLoadKernels(obj->context);
}

void app_deinit(AppObj *obj)
{
    int32_t   status;

    tivxParkAssistUnLoadKernels(obj->context);

    vxReleaseContext(&obj->context);

    status = appDeInit();
    PTK_assert(status == 0);
}

static void app_show_usage(int argc, char* argv[])
{
    printf("\n");
    printf(" SfM Fisheye App - (c) Texas Instruments 2018\n");
    printf(" ============================================================\n");
    printf("\n");
    printf("Please refer to demo guide for prerequisites before running this demo\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
    printf(" Please see sample configuration file (app_sample.cfg) in the app folder.\n\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    obj->mode = WITH_LDC;
    obj->verbose = 0;
}

static void app_parse_cfg_file(AppObj *obj, char *cfg_file_name)
{
    FILE                      *fptr;
    char                      *pParamStr;
    char                      *pValueStr;
    char                      *pSLine;
    char                      *basePath;
    char                       paramSt[APP_MAX_FILE_PATH];
    char                       valueSt[APP_MAX_FILE_PATH];
    char                       sLine[APP_MAX_FILE_PATH];
    char                       filePath[APP_MAX_FILE_PATH];

    pParamStr  = paramSt;
    pValueStr  = valueSt;
    pSLine     = sLine;
    basePath   = getenv("APP_CONFIG_BASE_PATH");

    if (basePath == NULL)
    {
        printf("Please define APP_CONFIG_BASE_PATH environment variable.\n");
        exit(-1);
    }

    fptr = fopen(cfg_file_name, "r");

    if ( !fptr )
    {
        printf("Cannot open %s for reading.\n", cfg_file_name);
        exit(-1);
    }

    while ( 1 )
    {
        pSLine = fgets(pSLine, APP_MAX_FILE_PATH, fptr);

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

        if (strcmp(pParamStr, "tiap_config_file") == 0)
        {
            snprintf(filePath, APP_MAX_FILE_PATH, "%s/%s", basePath, pValueStr);
            PTK_DBConfig_parse(&obj->dbConfig, filePath);
        }
        else if (strcmp(pParamStr, "verbose") == 0)
        {
            obj->verbose = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "enable_ldc") == 0)
        {
            obj->mode = atoi(pValueStr);
        }
    }

    fclose(fptr);
}

static void app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[])
{
    int i;

    app_set_cfg_default(obj);

    if(argc==1)
    {
        app_show_usage(argc, argv);
        exit(0);
    }

    for(i=0; i<argc; i++)
    {
        if(strcmp(argv[i], "--cfg")==0)
        {
            i++;
            if(i>=argc)
            {
                app_show_usage(argc, argv);
            }
            app_parse_cfg_file(obj, argv[i]);
            break;
        }
        else
        if(strcmp(argv[i], "--help")==0)
        {
            app_show_usage(argc, argv);
            exit(0);
        }
    }
}

