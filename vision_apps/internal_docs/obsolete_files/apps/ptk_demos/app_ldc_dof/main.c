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


/* brief: App performs basic DOF pipeline on a stream of images and dumps resulting data
 *        to file.
 *        Data that can be dumped is output of LDC (image), the DOF vector field
 *        and the DOF visualization.
 *        Uses sensorstream API to read and dump data. */

#include <TI/tivx.h>
#include <tivx_utils_file_rd_wr.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include <perception/dbtools/c/dbtools.h>

#define APP_MAX_FILE_PATH           (4096u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));

typedef enum app_mode_e
{
    WITHOUT_LDC = 0,  //!< Do not use LDC remapping. Graph looks like Images->Pyr->DOF->DOF_vis
    WITH_LDC          //!< Include LDC remapping.    Graph looks like Images->LDC->Pyr->DOF->DOF_vis
} AppMode;

typedef struct app_obj_t
{
    /* Input/Output data configuration */
    PTK_DBConfig    dbConfig;                         //!< database config file (defining inputs and outputs)
    char            ldcMeshFile[APP_MAX_FILE_PATH];   //!< in case mode is WITH_LDC

    /* input data streams */
    ssHandle        cam;

    /* output data streams */
    vscHandle       ldc;         //<! output images of LDC
    vscHandle       dof;         //<! output from DOF
    vscHandle       dofVis;      //<! visualization for DOF

    /* App Control */
    AppMode         mode;
    uint32_t        verbose;    //<! verbosity, 0=no prints, 1=info prints, >1=debug prints

    /*Internal Variables */
    uint32_t        inputImgWidth;   //!< width of input frame
    uint32_t        inputImgHeight;  //!< height of input frame
    uint32_t        pyramidInWidth;  //!< width of input image to Pyramid node
    uint32_t        pyramidInHeight; //!< height of input image to Pyramid node

    /* OVX Node Parameters*/
    tivx_vpac_ldc_params_t              ldcPrms;
    tivx_vpac_ldc_region_params_t       ldcRegionPrms;
    tivx_vpac_ldc_mesh_params_t         ldcMeshPrms;
    int16_t                             ldcWarpPrms[6];
    tivx_dmpac_dof_params_t             dofPrms;

    vx_uint32       dofLevels;
    vx_uint32       dofVisConfidThresh; //!< threshold on DOF confidence for DOF visualization

    /* OVX Node References */
    vx_context      context;

    //graphs
    vx_graph        dof_graph;

    //nodes
    vx_node         ldc_node;
    vx_node         pyr_node;
    vx_node         dof_node;
    vx_node         dofvis_node;

    //data objects
    vx_image        input_image;
    vx_image        ldc_in_mesh;
    vx_matrix       ldc_in_warp;
    vx_user_data_object ldc_in_config;
    vx_user_data_object ldc_mesh_params;
    vx_user_data_object ldc_region_params;
    vx_image        pyr_in_image;
    vx_delay        pyr_out_delay;
    vx_user_data_object dof_in_config;
    vx_delay        dof_out_delay;
    vx_image        dofvis_out_image;
    vx_image        dofvis_out_confidence;
    vx_scalar       dofvis_out_confidence_threshold;

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
static void app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[]);
static vx_status app_load_vximage_mesh_from_text_file(char *filename, vx_image image);

int main(int argc, char* argv[])
{
    AppObj *obj = &gAppObj;
    vx_status status;
    int32_t ssStatus;
    void *dataPtr;
    size_t dataSize;
    uint64_t curTimestamp;
    uint32_t curFrame;

    printf("APP LDC DOF STARTED!...\n\n");

    app_init(obj);

    app_parse_cmd_line_args(obj, argc, argv);

    printf("\n");

    /********************************************/
    /* initialize data streams                  */
    /********************************************/
    app_data_streams_init(obj);

    /********************************************/
    /* define parameters                        */
    /********************************************/
    app_set_params(obj);

    /*******************************************/
    /* OVX nodes/graphs initialization         */
    /*******************************************/
    app_openvx_create(obj);

    /*******************************************/
    /* main loop (for each frame)              */
    /*******************************************/
    sensorstream_set_stream_pos(obj->cam, obj->dbConfig.startFrame-1); //-1 for C indexing (DB starts at 1)
    curFrame = obj->dbConfig.startFrame;

    printf("\n");
    while (1)
    {
        /****************************************/
        /* PERCEPTION PROCESSING                */
        /****************************************/
        ssStatus = sensorstream_get_stream_next(obj->cam, (void *)&dataPtr, &dataSize, &curTimestamp);

        if (ssStatus < 0 || curFrame > obj->dbConfig.endFrame)
        {
            break;
        }

        vx_image load_png_into;

        if (WITHOUT_LDC == obj->mode)
        {
            load_png_into = obj->pyr_in_image;
        }
        if (WITH_LDC == obj->mode)
        {
            load_png_into = obj->input_image;
        }

        printf("Processing frame %d at timestamp %" PRId64 "...\n", curFrame, curTimestamp);

        /*read png and load into ovx image*/
        tivx_utils_load_vximage_from_pngfile(load_png_into, (char *)dataPtr, vx_true_e);

        /* process graph */
        status = vxProcessGraph(obj->dof_graph);
        assert(VX_SUCCESS == status);

        /* save result */
        app_save_result(obj, curTimestamp);

        /* update */
        curFrame++;

        printf("...DONE\n\n");
    }

    /*******************************************/
    /* OVX nodes/graphs deinitialization       */
    /*******************************************/
    app_openvx_delete(obj);

    /********************************************/
    /* deinitialize data streams                */
    /********************************************/
    app_data_streams_deinit(obj);

    printf("\n\nDEMO FINISHED!\n");

    app_deinit(&gAppObj);

    return 0;
}


/*===================================================================*/
/* SUPPORTING FUNCTIONS */
/*===================================================================*/
static void app_set_params(AppObj *obj)
{
    /* get input image size and width by querying first png file in camera stream */
    vx_status status;
    uint32_t stride;
    vx_df_image df;
    void *data_ptr = NULL;
    void *bmp_file_context = NULL;
    char *pFileName;
    size_t dataSizeOut; //unused
    uint64_t timestampOut; //unused

    sensorstream_get_pos(obj->cam, 1, (void **)&pFileName, &dataSizeOut, &timestampOut);

    status = tivx_utils_png_file_read(
                pFileName,
                vx_true_e,
                &obj->inputImgWidth, &obj->inputImgHeight, &stride, &df, &data_ptr,
                &bmp_file_context);
    APP_ASSERT(status==VX_SUCCESS);

    if (WITHOUT_LDC == obj->mode)
    {
        /* overwrite in case LDC is not used */
        obj->pyramidInWidth = obj->inputImgWidth;
        obj->pyramidInHeight = obj->inputImgHeight;
    }

    /* LDC node config */
    memset(&obj->ldcPrms, 0, sizeof(tivx_vpac_ldc_params_t));
    memset(&obj->ldcRegionPrms, 0, sizeof(tivx_vpac_ldc_region_params_t));
    memset(&obj->ldcMeshPrms, 0, sizeof(tivx_vpac_ldc_mesh_params_t));

    obj->ldcPrms.luma_interpolation_type = 1; //BILINEAR

    obj->ldcMeshPrms.mesh_frame_width = 800;
    obj->ldcMeshPrms.mesh_frame_height = 1040;
    obj->ldcMeshPrms.subsample_factor = 4;

    obj->ldcRegionPrms.out_block_width = 16;
    obj->ldcRegionPrms.out_block_height = 16;
    obj->ldcRegionPrms.pixel_pad = 0;

    obj->ldcWarpPrms[0] = 4096; //A
    obj->ldcWarpPrms[1] = 0;    //B
    obj->ldcWarpPrms[4] = 256;  //C
    obj->ldcWarpPrms[2] = 0;    //D
    obj->ldcWarpPrms[3] = 4096; //E
    obj->ldcWarpPrms[5] = 320;  //F

    /* DOF node config */
    obj->dofPrms.vertical_search_range[0] = 48;
    obj->dofPrms.vertical_search_range[1] = 48;
    obj->dofPrms.horizontal_search_range = 191;
    obj->dofPrms.median_filter_enable = 1;
    obj->dofPrms.motion_smoothness_factor = 24;
    obj->dofPrms.motion_direction = 0; /* 0: neutral */

    /* number of scale levels*/
    obj->dofLevels = 5;
    obj->dofVisConfidThresh = 9;
}

/*
 * OVX GRAPH:
 *              ldc_config                                   dof_config
 *                   |                                           |
 *                   v                                           v
 * input_img -> VpacLdc -> GaussianPyramid -----> pyr_cur ---> DmpacDof -> flow_vector_field_out -> DofVisualize --- > flow_vector_field_img
 *             (optional)                    |                   ^  ^                |                   |
 *                                        pyr_delay              |  |       flow_vector_field_delay      +------> confidence_img
 *                                           |                   |  |                |
 *                                           +----> pyr_ref -----+  +------- flow_vector_field_in
 */
void app_openvx_create(AppObj *obj)
{
    vx_status status;

    /* CREATE DATA OBJECTS */

    /* LDC - Data Objects */
    obj->input_image = vxCreateImage(obj->context, obj->inputImgWidth, obj->inputImgHeight, VX_DF_IMAGE_U8);
    APP_ASSERT_VALID_REF(obj->input_image);
    vxSetReferenceName((vx_reference)obj->input_image, "InputImageU8");

    obj->ldc_in_config = vxCreateUserDataObject(obj->context, "tivx_vpac_ldc_params_t", sizeof(tivx_vpac_ldc_params_t), &obj->ldcPrms);
    APP_ASSERT_VALID_REF(obj->ldc_in_config);
    vxSetReferenceName((vx_reference)obj->ldc_in_config, "LDC_Config");

    obj->ldc_mesh_params = vxCreateUserDataObject(obj->context, "tivx_vpac_ldc_mesh_params_t", sizeof(tivx_vpac_ldc_mesh_params_t), &obj->ldcMeshPrms);
    APP_ASSERT_VALID_REF(obj->ldc_mesh_params);
    vxSetReferenceName((vx_reference)obj->ldc_mesh_params, "LDC_Mesh_Params");

    obj->ldc_region_params = vxCreateUserDataObject(obj->context, "tivx_vpac_ldc_region_params_t", sizeof(tivx_vpac_ldc_region_params_t), &obj->ldcRegionPrms);
    APP_ASSERT_VALID_REF(obj->ldc_region_params);
    vxSetReferenceName((vx_reference)obj->ldc_region_params, "LDC_Region_Params");

    obj->ldc_in_mesh = vxCreateImage(obj->context,
                                  obj->ldcMeshPrms.mesh_frame_width/(1<<obj->ldcMeshPrms.subsample_factor)+1,
                                  obj->ldcMeshPrms.mesh_frame_height/(1<<obj->ldcMeshPrms.subsample_factor)+1,
                                  VX_DF_IMAGE_U32);
    APP_ASSERT_VALID_REF(obj->ldc_in_mesh);
    vxSetReferenceName((vx_reference)obj->ldc_in_mesh, "LDC_Mesh");

    app_load_vximage_mesh_from_text_file(obj->ldcMeshFile, obj->ldc_in_mesh);

    obj->ldc_in_warp = vxCreateMatrix(obj->context, VX_TYPE_INT16, 2, 3);
    APP_ASSERT_VALID_REF(obj->ldc_in_warp);
    vxSetReferenceName((vx_reference)obj->ldc_in_warp, "LDC_AffineMatrix");
    status = vxCopyMatrix(obj->ldc_in_warp, obj->ldcWarpPrms, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    vx_pyramid pyr_ref;
    vx_pyramid pyr_cur;
    vx_image   dof_in_field;
    vx_image   dof_out_field;

    /* Pyramid - Data Objects */
    //input image
    obj->pyr_in_image = vxCreateImage(obj->context, obj->pyramidInWidth, obj->pyramidInHeight, VX_DF_IMAGE_U8);
    APP_ASSERT_VALID_REF(obj->pyr_in_image);
    vxSetReferenceName((vx_reference)obj->pyr_in_image, "LDC_OutputImageU8");

    //delay
    vx_pyramid pyr_exemplar = vxCreatePyramid(obj->context,
                        obj->dofLevels, VX_SCALE_PYRAMID_HALF,
                        obj->pyramidInWidth, obj->pyramidInHeight,
                        VX_DF_IMAGE_U8);
    APP_ASSERT_VALID_REF(pyr_exemplar);
    obj->pyr_out_delay = vxCreateDelay(obj->context, (vx_reference)pyr_exemplar, 2);
    APP_ASSERT_VALID_REF(obj->pyr_out_delay);
    vxReleasePyramid(&pyr_exemplar);

    pyr_ref = (vx_pyramid)vxGetReferenceFromDelay(obj->pyr_out_delay, -1);
    APP_ASSERT_VALID_REF(pyr_ref);
    vxSetReferenceName((vx_reference)pyr_ref, "DOF_PyramidReference");
    pyr_cur = (vx_pyramid)vxGetReferenceFromDelay(obj->pyr_out_delay,  0);
    APP_ASSERT_VALID_REF(pyr_cur);
    vxSetReferenceName((vx_reference)pyr_cur, "DOF_PyramidCurrent");

    /* DOF - Data Objects */
    //config
    obj->dof_in_config = vxCreateUserDataObject(obj->context, "tivx_dmpac_dof_params_t", sizeof(tivx_dmpac_dof_params_t), &obj->dofPrms);
    APP_ASSERT_VALID_REF(obj->dof_in_config);
    vxSetReferenceName((vx_reference)obj->dof_in_config, "DOF_Config");

    //delay
    vx_image  dof_field_exemplar = vxCreateImage(obj->context, obj->pyramidInWidth, obj->pyramidInHeight, VX_DF_IMAGE_U32);
    APP_ASSERT_VALID_REF(dof_field_exemplar);
    obj->dof_out_delay = vxCreateDelay(obj->context, (vx_reference)dof_field_exemplar, 2);
    APP_ASSERT_VALID_REF(obj->dof_out_delay);
    vxReleaseImage(&dof_field_exemplar);

    dof_in_field = (vx_image)vxGetReferenceFromDelay(obj->dof_out_delay, -1);
    APP_ASSERT_VALID_REF(dof_in_field);
    vxSetReferenceName((vx_reference)dof_in_field, "DOF_FlowVectorIn");
    dof_out_field = (vx_image)vxGetReferenceFromDelay(obj->dof_out_delay,  0);
    APP_ASSERT_VALID_REF(dof_out_field);
    vxSetReferenceName((vx_reference)dof_out_field, "DOF_FlowVectorOut");


    //dof visualization
    if (!VirtualSensorCreator_is_disabled(obj->dofVis))
    {
        obj->dofvis_out_image = vxCreateImage(obj->context, obj->pyramidInWidth, obj->pyramidInHeight, VX_DF_IMAGE_RGB);
        APP_ASSERT_VALID_REF(obj->dofvis_out_image);
        vxSetReferenceName((vx_reference)obj->dofvis_out_image, "DOF_OutputFlowVectorImageRGB");
        obj->dofvis_out_confidence = vxCreateImage(obj->context, obj->pyramidInWidth, obj->pyramidInHeight, VX_DF_IMAGE_U8);
        APP_ASSERT_VALID_REF(obj->dofvis_out_confidence);
        vxSetReferenceName((vx_reference)obj->dofvis_out_confidence, "DOF_OutputConfidenceImageU8");
        obj->dofvis_out_confidence_threshold = vxCreateScalar(obj->context, VX_TYPE_UINT32, &obj->dofVisConfidThresh);
        APP_ASSERT_VALID_REF(obj->dofvis_out_confidence_threshold);
        vxSetReferenceName((vx_reference)obj->dofvis_out_confidence_threshold, "DOF_OutputConfidenceThreshold");
    }

    /* CREATE GRAPHS */

    /* SfM Graph */
    obj->dof_graph = vxCreateGraph(obj->context);
    APP_ASSERT_VALID_REF(obj->dof_graph);

    if(WITH_LDC == obj->mode)
    {
        obj->ldc_node = tivxVpacLdcNode(obj->dof_graph,
                                        obj->ldc_in_config ,
                                        obj->ldc_in_warp,
                                        obj->ldc_region_params,
                                        obj->ldc_mesh_params,
                                        obj->ldc_in_mesh,
                                        NULL,
                                        obj->input_image,
                                        obj->pyr_in_image,
                                        NULL);


        APP_ASSERT_VALID_REF(obj->ldc_node);
        status = vxSetNodeTarget(obj->ldc_node, VX_TARGET_STRING, TIVX_TARGET_VPAC_LDC1);
        APP_ASSERT(status==VX_SUCCESS);
        vxSetReferenceName((vx_reference)obj->ldc_node, "LDC_Processing");
    }

    obj->pyr_node = vxGaussianPyramidNode(
                            obj->dof_graph,
                            obj->pyr_in_image,
                            pyr_ref);
    APP_ASSERT_VALID_REF(obj->pyr_node);
    status = vxSetNodeTarget(obj->pyr_node, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
    APP_ASSERT(status==VX_SUCCESS);
    vxSetReferenceName((vx_reference)obj->pyr_node, "GaussianPyramid");

    obj->dof_node = tivxDmpacDofNode(
                             obj->dof_graph,
                             obj->dof_in_config,
                             NULL,
                             NULL,
                             pyr_cur,
                             pyr_ref,
                             dof_in_field,
                             NULL,
                             NULL,
                             dof_out_field,
                             NULL);
     APP_ASSERT_VALID_REF(obj->dof_node);
     status = vxSetNodeTarget(obj->dof_node, VX_TARGET_STRING, TIVX_TARGET_DMPAC_DOF);
     APP_ASSERT(status==VX_SUCCESS);
     vxSetReferenceName((vx_reference)obj->dof_node, "DOF_Processing");

     if (!VirtualSensorCreator_is_disabled(obj->dofVis))
     {
         obj->dofvis_node = tivxDofVisualizeNode(
                                 obj->dof_graph,
                                 dof_out_field,
                                 obj->dofvis_out_confidence_threshold,
                                 obj->dofvis_out_image,
                                 obj->dofvis_out_confidence);
         APP_ASSERT_VALID_REF(obj->dofvis_node);
         status = vxSetNodeTarget(obj->dofvis_node,  VX_TARGET_STRING, TIVX_TARGET_MCU2_0);
         APP_ASSERT(status==VX_SUCCESS);
         vxSetReferenceName((vx_reference)obj->dofvis_node, "DOF_Visualize");
     }

     status = vxRegisterAutoAging(obj->dof_graph, obj->pyr_out_delay);
     APP_ASSERT(status==VX_SUCCESS);
     status = vxRegisterAutoAging(obj->dof_graph, obj->dof_out_delay);
     APP_ASSERT(status==VX_SUCCESS);

     status = vxVerifyGraph(obj->dof_graph);
     APP_ASSERT(status==VX_SUCCESS);

#if 0
     status = tivxExportGraphToDot(obj->dof_graph,".", "vx_app_ldc_dof");
     APP_ASSERT(status==VX_SUCCESS);
#endif

    return;
}

void app_openvx_delete(AppObj *obj)
{

    if(WITH_LDC == obj->mode)
    {
        vxReleaseNode(&obj->ldc_node);
    }
    vxReleaseNode(&obj->pyr_node);
    vxReleaseNode(&obj->dof_node);
    if (!VirtualSensorCreator_is_disabled(obj->dofVis))
    {
        vxReleaseNode(&obj->dofvis_node);
    }

    vxReleaseGraph(&obj->dof_graph);

    vxReleaseImage(&obj->input_image);
    vxReleaseMatrix(&obj->ldc_in_warp);
    vxReleaseImage(&obj->ldc_in_mesh);
    vxReleaseUserDataObject(&obj->ldc_in_config);
    vxReleaseUserDataObject(&obj->ldc_mesh_params);
    vxReleaseImage(&obj->pyr_in_image);
    vxReleaseDelay(&obj->pyr_out_delay);
    vxReleaseUserDataObject(&obj->dof_in_config);
    vxReleaseDelay(&obj->dof_out_delay);
    if (!VirtualSensorCreator_is_disabled(obj->dofVis))
    {
        vxReleaseImage(&obj->dofvis_out_image);
        vxReleaseImage(&obj->dofvis_out_confidence);
        vxReleaseScalar(&obj->dofvis_out_confidence_threshold);
    }
}

void app_data_streams_init(AppObj *obj)
{
    uint32_t i;
    /*inputs*/
    obj->cam   = sensorstream_create(obj->dbConfig.databasePath,
                                     obj->dbConfig.dataSeqId,
                                     obj->dbConfig.sensors[0].folder);

    /* outputs */
    obj->ldc = VirtualSensorCreator_create_by_dbconfig(&obj->dbConfig, "LDC");
    obj->dof = VirtualSensorCreator_create_by_dbconfig(&obj->dbConfig, "DOF");
    obj->dofVis = VirtualSensorCreator_create_by_dbconfig(&obj->dbConfig, "DOFVIS");
}

void app_data_streams_deinit(AppObj *obj)
{
    sensorstream_delete(obj->cam);

    VirtualSensorCreator_delete(obj->ldc);
    VirtualSensorCreator_delete(obj->dof);
    VirtualSensorCreator_delete(obj->dofVis);
}

void app_save_result(AppObj *obj, uint64_t curTimestamp)
{
    char buffer[APP_MAX_FILE_PATH] = {0};
    char *dstFilePath = buffer;
    vx_status status;
    vx_map_id map_id_dof;

    vx_size stride = sizeof(vx_size);
    vx_rectangle_t rect;
    vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;
    uint32_t w = obj->pyramidInWidth;
    uint32_t h = obj->pyramidInHeight;
    rect.start_x = 0;
    rect.end_x = w;
    rect.start_y = 0;
    rect.end_y = h;

    /* SAVE RESULT */

    /* DOF */
    uint32_t *dof;
    vx_image dof_out_field = (vx_image)vxGetReferenceFromDelay(obj->dof_out_delay,  -1);
    APP_ASSERT_VALID_REF(dof_out_field);
    status = vxMapImagePatch(dof_out_field, &rect, (vx_uint32)0, &map_id_dof, &addr, (void **)&dof,  VX_READ_ONLY, VX_MEMORY_TYPE_HOST, VX_NOGAP_X);
    assert(VX_SUCCESS == status);
    assert(NULL != dof);

    VirtualSensorCreator_add_record(obj->dof, (char **)&dstFilePath, curTimestamp, dof, sizeof(uint32_t)*w*h);

    status = vxUnmapImagePatch(dof_out_field, map_id_dof);
    assert(VX_SUCCESS == status);

    /* LDC */
    if (!VirtualSensorCreator_is_disabled(obj->ldc))
    {
        VirtualSensorCreator_add_record(obj->ldc, (char **)&dstFilePath, curTimestamp, NULL, 0);
        tivx_utils_save_vximage_to_pngfile(dstFilePath, obj->pyr_in_image);
    }

    /* DOF VIS */
    if (!VirtualSensorCreator_is_disabled(obj->dofVis))
    {
        VirtualSensorCreator_add_record(obj->dofVis, (char **)&dstFilePath, curTimestamp, NULL, 0);
        tivx_utils_save_vximage_to_pngfile(dstFilePath, obj->dofvis_out_image);
    }


}

static vx_status app_load_vximage_mesh_from_text_file(char *filename, vx_image image)
{
    vx_uint32 width, height;
    vx_imagepatch_addressing_t image_addr;
    vx_rectangle_t rect;
    vx_map_id map_id;
    vx_df_image df;
    void *data_ptr;
    vx_status status;

    status = vxGetStatus((vx_reference)image);
    if(status==VX_SUCCESS)
    {
        vxQueryImage(image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        status = vxMapImagePatch(image,
            &rect,
            0,
            &map_id,
            &image_addr,
            &data_ptr,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );

#if 0 // Passthrough without reading file
        int x,y;
        for(y=0; y < image_addr.dim_y; y++)
        {
            uint32_t *data = (uint32_t*)data_ptr + (image_addr.stride_y/4)*y;
            for(x=0; x < image_addr.dim_x; x++)
            {
                data[x] =  0;
            }
        }
#else
        if(status==VX_SUCCESS)
        {
            FILE *fp = fopen(filename,"r");

            if(fp!=NULL)
            {
                size_t ret;
                int x,y,xoffset,yoffset;

                for(y=0; y < image_addr.dim_y; y++)
                {
                    uint32_t *data = (uint32_t*)data_ptr + (image_addr.stride_y/4)*y;
                    for(x=0; x < image_addr.dim_x; x++)
                    {
                        ret = fscanf(fp, "%d %d", &xoffset, &yoffset);
                        if(ret!=2)
                        {
                            printf("# ERROR: Unable to read data from file [%s]\n", filename);
                        }
                        data[x] = (xoffset << 16) | (yoffset & 0x0ffff);
                    }
                }
                fclose(fp);
            }
            else
            {
                printf("# ERROR: Unable to open file for reading [%s]\n", filename);
                status = VX_FAILURE;
            }
            vxUnmapImagePatch(image, map_id);
        }
#endif
    }
    return status;
}

void app_init(AppObj *obj)
{
    PTK_CRT ptkConfig;
    /* Initialize PTK library. */
    ptkConfig.exit   = exit;
    ptkConfig.printf = printf;
    ptkConfig.time   = NULL;
    PTK_init(&ptkConfig);

    tivxInit();

    obj->context = vxCreateContext();

    tivxHwaLoadKernels(obj->context);
}

void app_deinit(AppObj *obj)
{
    tivxHwaUnLoadKernels(obj->context);

    vxReleaseContext(&obj->context);

    tivxDeInit();
}

static void app_show_usage(int argc, char* argv[])
{
    printf("\n");
    printf(" Dense Optical Flow with LDC App - (c) Texas Instruments 2018\n");
    printf(" ============================================================\n");
    printf("\n");
    printf("Please refer to demo guide for prerequisites before running this demo\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
    printf(" Please revisit sample configuration file (./config/app.cfg) in the app's folder.\n\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    obj->mode = WITHOUT_LDC;
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
        pParamStr[0] = '\0';
        pValueStr[0] = '\0';
        pSLine = fgets(pSLine, APP_MAX_FILE_PATH, fptr);

        if( pSLine == NULL )
        {
            break;
        }

        if (strchr(pSLine, '#'))
        {
            continue;
        }

        sscanf(pSLine,"%128s %128s", pParamStr, pValueStr);

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
        else if (strcmp(pParamStr, "ldc_mesh_file") == 0)
        {
            snprintf(obj->ldcMeshFile, APP_MAX_FILE_PATH, "%s/%s", basePath, pValueStr);
        }
        else if (strcmp(pParamStr, "ldc_width_out") == 0)
        {
            obj->pyramidInWidth = atoi(pValueStr);
        }
        else if (strcmp(pParamStr, "ldc_height_out") == 0)
        {
            obj->pyramidInHeight = atoi(pValueStr);
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

    /* arg checks */
    for (i = 0; i < obj->dbConfig.numOutputs; i++)
    {
        /* can't save LDC output if LDC is not enabled */
        if ((strcmp(obj->dbConfig.outputs[i].appTag, "LDC") == 0) && (WITH_LDC != obj->mode))
        {
            printf("ERROR: To save LDC output, LDC must be enabled\n");
            exit(-1);
        }
    }
}

