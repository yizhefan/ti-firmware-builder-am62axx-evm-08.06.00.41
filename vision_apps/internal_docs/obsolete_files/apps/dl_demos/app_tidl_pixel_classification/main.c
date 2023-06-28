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

#include <TI/tivx.h>
#include <TI/tivx_target_kernel.h>
#include "tivx_kernels_host_utils.h"

#include <TI/tivx_task.h>
#include <TI/j7_tidl.h>
#include <tivx_utils_file_rd_wr.h>
#include <tivx_utils_graph_perf.h>
#include <TI/tivx_img_proc.h>
#include <utils/draw2d/include/draw2d.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <float.h>
#include <math.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include "itidl_ti.h"
#include "tiadalg_interface.h"

//#define APP_DEBUG

#define MAX_IMG_WIDTH  (2048)
#define MAX_IMG_HEIGHT (1024)
#define DISPLAY_WIDTH  (1280)
#define DISPLAY_HEIGHT (720)


#define APP_MAX_FILE_PATH           (256u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));

/*Maximum tensor for TIDL and is made dependent maximum input tensor for visualization (or TIDL output)*/
#define APP_MAX_TENSORS             (TIVX_PIXEL_VIZ_MAX_TENSOR)
#define APP_MAX_TENSOR_DIMS         (4u)
#define APP_TIDL_MAX_PARAMS         (16u)

#define ABS_FLT(a) ((a) > 0)?(a):(-(a))

static const char *tensor_num_str[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

typedef struct {

    /* config options */
    char tidl_config_file_path[APP_MAX_FILE_PATH];
    char tidl_network_file_path[APP_MAX_FILE_PATH];

    char input_file_path[APP_MAX_FILE_PATH];
    char input_dof_file_path[APP_MAX_FILE_PATH];
    char input_file_prefix[APP_MAX_FILE_PATH];

    char output_file_path[APP_MAX_FILE_PATH];
    char output_file_prefix[APP_MAX_FILE_PATH];

    char input_file_list[APP_MAX_FILE_PATH];
    char input_dof_file_list[APP_MAX_FILE_PATH];

    char ti_logo_file_path[APP_MAX_FILE_PATH];

    vx_int32 num_input_tensors;
    vx_int32 num_output_tensors;
    vx_int32 num_classes[TIVX_PIXEL_VIZ_MAX_CLASS];
    vx_int32 valid_region[TIVX_PIXEL_VIZ_MAX_CLASS][4];

    /* OpenVX references */
    vx_context context;
    vx_graph   graph;
    vx_kernel  kernel;
    vx_node    tidl_node;

    vx_user_data_object  config;
    vx_user_data_object  network;
    vx_user_data_object  createParams;
    vx_user_data_object  inArgs;
    vx_user_data_object  outArgs;

    vx_tensor  input_tensors[APP_MAX_TENSORS];
    vx_tensor  output_tensors[APP_MAX_TENSORS];
    vx_image   output_images[APP_MAX_TENSORS];

    vx_int32 width;
    vx_int32 height;
    vx_int32 dl_width;
    vx_int32 dl_height;
    vx_int32 en_out_img_write;
    vx_int32 pad_in_tidl;
    vx_int32 tidl_8bit_16bit_flag;
    vx_int32 ip_rgb_or_yuv;
    vx_int32 op_rgb_or_yuv;

    vx_image   in_img;
    vx_tensor  dof_tensor;

    vx_node  node_dof_plane_sep;
    vx_kernel pixel_viz_kernel;
    vx_node  node_pixel_viz;
    vx_node  node_img_proc;

    vx_array dof_plane_sep_config;
    tivxDofPlaneSepParams dof_plane_sep_params;

    vx_array img_proc_config;
    tivxImgPreProcParams img_pre_proc_params;

    vx_user_data_object pixel_viz_config;
    tivxPixelVizParams pixel_viz_params;

    uint8_t  *pDisplayBuf888;
    uint16_t *pDisplayBuf565;

    vx_graph  disp_graph;
    vx_node disp_node;
    vx_image disp_image;

    vx_user_data_object disp_params_obj;
    tivx_display_params_t disp_params;
    vx_rectangle_t disp_rect;
    vx_imagepatch_addressing_t image_addr;

    Draw2D_Handle  pHndl;

    vx_uint32 display_option;
    vx_uint32 delay_in_msecs;
    vx_uint32 num_iterations;

    uint32_t is_interactive;
    tivx_task task;
    uint32_t stop_task;
    uint32_t stop_task_done;

    app_perf_point_t total_perf;
    app_perf_point_t fileio_perf;
    app_perf_point_t draw_perf;

} AppObj;

AppObj gAppObj;


static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[]);
static int app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static vx_status app_create_graph(AppObj *obj);
static vx_status app_run_graph(AppObj *obj);
static vx_status app_verify_graph(AppObj *obj);
static vx_status app_run_graph_interactive(AppObj *obj);
static void app_delete_graph(AppObj *obj);
static void app_update_param_set(AppObj *obj);
static uint32_t num_params;
static uint32_t max_params;

static vx_user_data_object readConfig(vx_context context, vx_char *config_file, uint32_t *num_input_tensors, uint32_t *num_output_tensors);
static vx_user_data_object readNetwork(vx_context context, vx_char *network_file);
static vx_user_data_object setCreateParams(vx_context context);
static vx_user_data_object setInArgs(vx_context context);
static vx_user_data_object setOutArgs(vx_context context);

static void createInputTensors(vx_context context, vx_user_data_object config, vx_tensor *input_tensors,
                              tivxDofPlaneSepParams* dof_prm,tivxImgPreProcParams* img_pre_proc_params,
                              tivxPixelVizParams* viz_prm, vx_int32 pad_in_tidl, vx_int32 tidl_8bit_16bit_flag);

static void createOutputTensors(vx_context context, vx_user_data_object config, vx_tensor *output_tensors,
                                tivxPixelVizParams* viz_prm,
                                vx_int32 tidl_8bit_16bit_flag
                                );
static void dumpOutput(vx_image *output_images, vx_int32 num_output_tensors, vx_char *output_file, vx_int32 op_rgb_or_yuv);
static vx_status saveYUVNV12vxImage(char* file_name, vx_image in_img);
static vx_status loadvxImageYUVNV12(char* file_name, vx_image in_img);
static void fillDofTensor(AppObj *obj,vx_char* in_dof_file);
static vx_status createPostProcNode(AppObj *obj);
static vx_status createPreProcNode(AppObj *obj);
static vx_status createDofProcNode(AppObj *obj);

static vx_status displayOutput(AppObj *obj, vx_image *output_images, vx_int32 num_output_tensors);

static vx_uint8 motion_segment_color_map[2][3] = {{0,0,0},{128,64,128}};
static vx_uint8 semantic_segment_color_map[5][3] = {{152,251,152},{0,130,180},{220,20,60},{3,3,251},{190,153,153}};
static vx_uint8 depth_segment_color_map[63][3] ={{255,10,0},{255,20,0},{255,30,0},{255,40,0},{255,50,0},{255,60,0},{255,70,0},{255,80,0},
                                                  {255,91,0},{255,101,0},{255,111,0},{255,121,0},{255,131,0},{255,141,0},{255,151,0},{255,161,0},
                                                  {255,172,0},{255,182,0},{255,192,0},{255,202,0},{255,212,0},{255,222,0},{255,232,0},{255,242,0},
                                                  {255,252,0},{238,255,0},{218,255,0},{198,255,0},{178,255,0},{157,255,0},{137,255,0},{117,255,0},
                                                  {97,255,0},{76,255,0},{56,255,0},{36,255,0},{16,255,0},{0,250,4},{0,230,24},{0,210,44},
                                                  {0,190,64},{0,170,84},{0,149,105},{0,129,125},{0,109,145},{0,89,165},{0,68,186},{0,48,206},
                                                  {0,28,226},{0,8,246},{8,0,255},{21,0,255},{35,0,255},{48,0,255},{62,0,255},{75,0,255},
                                                  {89,0,255},{102,0,255},{116,0,255},{129,0,255},{143,0,255},{156,0,255},{170,0,255}
                                                };

static void initParam(vx_reference params[], uint32_t _max_params)
{
   num_params  = 0;
   max_params = _max_params;
}

static void addParam(vx_reference params[], vx_reference obj)
{
   APP_ASSERT(num_params <= max_params);

   params[num_params] = obj;

   num_params++;

}

vx_int32 app_tidl_pc_main(vx_int32 argc, vx_char* argv[])
{
    AppObj *obj = &gAppObj;
    int32_t status;


    app_parse_cmd_line_args(obj, argc, argv);

    {
      app_update_param_set(obj);

      status = app_init(obj);
      if(status != -1) {
        app_create_graph(obj);
        status = app_verify_graph(obj);
        if(status == 0)
        {
            if(obj->is_interactive)
            {
                app_run_graph_interactive(obj);
            }
            else
            {
                app_run_graph(obj);
            }
        }
        app_delete_graph(obj);
        app_deinit(obj);
      }
    }

    return status;
}

static void app_update_param_set(AppObj *obj){

  int32_t i,j;


  /*0 means 8 bit activations */
  /* Update of the dependent parameter which might have been updated by cfg read */
  obj->pixel_viz_params.tidl_8bit_16bit_flag = obj->tidl_8bit_16bit_flag;
  obj->dof_plane_sep_params.tidl_8bit_16bit_flag = obj->tidl_8bit_16bit_flag;
  obj->img_pre_proc_params.tidl_8bit_16bit_flag = obj->tidl_8bit_16bit_flag;

  obj->dof_plane_sep_params.width = obj->dl_width;
  obj->dof_plane_sep_params.height = obj->dl_height;


  for(i=0; i < TIVX_PIXEL_VIZ_MAX_TENSOR; i++){
    obj->pixel_viz_params.num_classes[i] = obj->num_classes[i];
  }

  for(i=0; i < TIVX_PIXEL_VIZ_MAX_TENSOR; i++){
    obj->pixel_viz_params.valid_region[i][0] = obj->valid_region[i][0];
    obj->pixel_viz_params.valid_region[i][1] = obj->valid_region[i][1];
    obj->pixel_viz_params.valid_region[i][2] = obj->valid_region[i][2];
    obj->pixel_viz_params.valid_region[i][3] = obj->valid_region[i][3];
  }

  for(i=0; i < TIVX_PIXEL_VIZ_MAX_TENSOR; i++){
    /*Motion segmentation scenario*/
    if(obj->pixel_viz_params.num_classes[i] == 2){
      memcpy(obj->pixel_viz_params.color_map[i],motion_segment_color_map,obj->pixel_viz_params.num_classes[i]*3*sizeof(vx_uint8));
    }
    /*Semantic segmentation scenario*/
    else if(obj->pixel_viz_params.num_classes[i] == 5){

      memcpy(obj->pixel_viz_params.color_map[i],semantic_segment_color_map,obj->pixel_viz_params.num_classes[i]*3*sizeof(vx_uint8));
    }
    /*depth class*/
    else{
      float color_jump = 63.0/obj->pixel_viz_params.num_classes[i];
      for(j=0;j<obj->pixel_viz_params.num_classes[i];j++){
        obj->pixel_viz_params.color_map[i][j][0] = depth_segment_color_map[(int)(j * color_jump)][0];
        obj->pixel_viz_params.color_map[i][j][1] = depth_segment_color_map[(int)(j * color_jump)][1];
        obj->pixel_viz_params.color_map[i][j][2] = depth_segment_color_map[(int)(j * color_jump)][2];
      }
    }
  }

  obj->pixel_viz_params.op_rgb_or_yuv = obj->op_rgb_or_yuv;
  obj->pixel_viz_params.ip_rgb_or_yuv = obj->ip_rgb_or_yuv;
  obj->img_pre_proc_params.ip_rgb_or_yuv = obj->ip_rgb_or_yuv;

  if((obj->ip_rgb_or_yuv == 0) || (obj->ip_rgb_or_yuv == 2)){
    obj->img_pre_proc_params.color_conv_flag = TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB;
  }else{
    obj->img_pre_proc_params.color_conv_flag = TIADALG_COLOR_CONV_YUV420_RGB;
  }

}
static int app_init(AppObj *obj)
{
    int status = 0;

    vx_uint32 num_input_tensors = 0;
    vx_uint32 num_output_tensors = 0;

    obj->context = vxCreateContext();
    APP_ASSERT_VALID_REF(obj->context);

    /* Create a vx_array object and read the config data*/
    obj->config = readConfig(obj->context, &obj->tidl_config_file_path[0], &num_input_tensors, &num_output_tensors);

    APP_ASSERT_VALID_REF(obj->config)

    /* Save a copy of number of input/output tensors required as per config */
    obj->num_input_tensors  = num_input_tensors;
    obj->num_output_tensors = num_output_tensors;

    /*For visualization node number of input and output tensors will be same as TIDL node number of output tensors*/
    obj->pixel_viz_params.num_input_tensors = obj->num_output_tensors;
    obj->pixel_viz_params.num_output_tensors = obj->num_output_tensors;

    /* Create a vx_tensor object and read the network data */
    obj->network = readNetwork(obj->context, &obj->tidl_network_file_path[0]);
    APP_ASSERT_VALID_REF(obj->network)

    obj->createParams = setCreateParams(obj->context);
    APP_ASSERT_VALID_REF(obj->createParams)

    obj->inArgs = setInArgs(obj->context);
    APP_ASSERT_VALID_REF(obj->inArgs)

    obj->outArgs = setOutArgs(obj->context);
    APP_ASSERT_VALID_REF(obj->outArgs)

    /*TIDL kernel init*/
    obj->kernel = tivxAddKernelTIDL(obj->context, obj->num_input_tensors, obj->num_output_tensors);
    APP_ASSERT_VALID_REF(obj->kernel)

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (obj->display_option == 1))
    {
        obj->disp_image = vxCreateImage(obj->context, DISPLAY_WIDTH, DISPLAY_HEIGHT, VX_DF_IMAGE_RGB);
        APP_ASSERT_VALID_REF(obj->disp_image)

        obj->image_addr.dim_x = DISPLAY_WIDTH;
        obj->image_addr.dim_y = DISPLAY_HEIGHT;
        obj->image_addr.stride_x = 3; /* RGB */
        obj->image_addr.stride_y = DISPLAY_WIDTH * 3;
        obj->image_addr.scale_x = VX_SCALE_UNITY;
        obj->image_addr.scale_y = VX_SCALE_UNITY;
        obj->image_addr.step_x = 1;
        obj->image_addr.step_y = 1;

        obj->disp_rect.start_x = 0;
        obj->disp_rect.start_y = 0;
        obj->disp_rect.end_x = DISPLAY_WIDTH;
        obj->disp_rect.end_y = DISPLAY_HEIGHT;

        memset(&obj->disp_params, 0, sizeof(tivx_display_params_t));

        obj->disp_params.opMode = TIVX_KERNEL_DISPLAY_BUFFER_COPY_MODE;
        obj->disp_params.pipeId = 0;
        obj->disp_params.outWidth = DISPLAY_WIDTH;
        obj->disp_params.outHeight = DISPLAY_HEIGHT;
        obj->disp_params.posX = (1920-DISPLAY_WIDTH)/2;
        obj->disp_params.posY = (1080-DISPLAY_HEIGHT)/2;

        obj->disp_params_obj = vxCreateUserDataObject(obj->context, "tivx_display_params_t", sizeof(tivx_display_params_t), &obj->disp_params);
        APP_ASSERT_VALID_REF(obj->disp_params_obj)

        tivxHwaLoadKernels(obj->context);
    }

    tivxTIDLLoadKernels(obj->context);

    tivxImgProcLoadKernels(obj->context);

    /*For visualization node number of input and output tensors will be same as TIDL node number of output tensors*/
    obj->pixel_viz_params.num_input_tensors = obj->num_output_tensors;

    obj->pixel_viz_params.num_output_tensors = obj->num_output_tensors;

    obj->pixel_viz_params.tidl_8bit_16bit_flag = obj->tidl_8bit_16bit_flag;
    obj->dof_plane_sep_params.tidl_8bit_16bit_flag = obj->tidl_8bit_16bit_flag;
    obj->img_pre_proc_params.tidl_8bit_16bit_flag = obj->tidl_8bit_16bit_flag;

    obj->pixel_viz_kernel = tivxAddKernelPixelViz(obj->context, obj->num_output_tensors);
    status = vxGetStatus((vx_reference)obj->pixel_viz_kernel);

    obj->pDisplayBuf888 = tivxMemAlloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * 3, TIVX_MEM_EXTERNAL);
    if(obj->pDisplayBuf888 == NULL) {
        printf("app_tidl: ERROR: Unable to allocate memory for displayBuf888, size = %d", DISPLAY_WIDTH * DISPLAY_HEIGHT * 3);
        status = -1;
    }

    obj->pDisplayBuf565 = tivxMemAlloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t), TIVX_MEM_EXTERNAL);
    if(obj->pDisplayBuf565 == NULL) {
        printf("app_tidl: ERROR: Unable to allocate memory for displayBuf565, size = %ld", DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t));
        status = -1;
    }

    if(obj->display_option == 1)
    {
        Draw2D_BufInfo sBufInfo;

        char banner_file[APP_MAX_FILE_PATH];

        snprintf(banner_file, APP_MAX_FILE_PATH, "%s/ti_logo.bmp", obj->ti_logo_file_path);

        Draw2D_create(&obj->pHndl);

        sBufInfo.bufWidth    = DISPLAY_WIDTH;
        sBufInfo.bufHeight   = DISPLAY_HEIGHT;
        sBufInfo.bufPitch[0] = DISPLAY_WIDTH * 2;
        sBufInfo.dataFormat = DRAW2D_DF_BGR16_565;
        sBufInfo.transperentColor = 0;
        sBufInfo.transperentColorFormat = DRAW2D_DF_BGR16_565;

        sBufInfo.bufAddr[0] = (uint8_t *)obj->pDisplayBuf565;

        Draw2D_setBufInfo(obj->pHndl, &sBufInfo);

        Draw2D_clearBuf(obj->pHndl);

        Draw2D_insertBmp(obj->pHndl, banner_file, 0, 0);
    }

    appPerfPointSetName(&obj->total_perf , "TOTAL");
    appPerfPointSetName(&obj->draw_perf  , "DRAW");
    appPerfPointSetName(&obj->fileio_perf, "FILEIO");

    return status;
}

static void app_deinit(AppObj *obj)
{
    vxRemoveKernel(obj->kernel);

    tivxTIDLUnLoadKernels(obj->context);
    tivxImgProcUnLoadKernels(obj->context);

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (obj->display_option == 1))
    {
        tivxHwaUnLoadKernels(obj->context);
    }

    vxReleaseContext(&obj->context);

    tivxMemFree(obj->pDisplayBuf888, DISPLAY_WIDTH * DISPLAY_HEIGHT * 3, TIVX_MEM_EXTERNAL);
    tivxMemFree(obj->pDisplayBuf565 , DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t), TIVX_MEM_EXTERNAL);

    if(obj->display_option == 1)
    {
        Draw2D_delete(obj->pHndl);
    }
}

static void app_delete_graph(AppObj *obj)
{
    vx_uint32 id;

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (obj->display_option == 1))
    {
        vxReleaseNode(&obj->disp_node);
        vxReleaseGraph(&obj->disp_graph);
    }

    /*All nodes release*/
    vxReleaseNode(&obj->tidl_node);

    if(obj->num_input_tensors > 1){
        vxReleaseNode(&obj->node_dof_plane_sep);
    }
    vxReleaseNode(&obj->node_pixel_viz);
    vxRemoveKernel(obj->pixel_viz_kernel);
    vxReleaseNode(&obj->node_img_proc);

    /*All array release*/
    vxReleaseUserDataObject(&obj->config);
    vxReleaseArray(&obj->img_proc_config);
    vxReleaseUserDataObject(&obj->pixel_viz_config);
    if(obj->num_input_tensors > 1){
      vxReleaseArray(&obj->dof_plane_sep_config);
    }

    /*All input output release*/
    vxReleaseUserDataObject(&obj->network);
    vxReleaseUserDataObject(&obj->createParams);
    vxReleaseUserDataObject(&obj->inArgs);
    vxReleaseUserDataObject(&obj->outArgs);

    for(id = 0; id < obj->num_input_tensors; id++) {
      vxReleaseTensor(&obj->input_tensors[id]);
    }

    for(id = 0; id < obj->num_output_tensors; id++) {
      vxReleaseTensor(&obj->output_tensors[id]);
    }
    /*minus one for outArgs*/
    for(id = 0; id < obj->num_output_tensors; id++) {
      vxReleaseImage(&obj->output_images[id]);
    }

    if(obj->num_input_tensors > 1){
      vxReleaseTensor(&obj->dof_tensor);
    }
    vxReleaseImage(&obj->in_img);

    vxReleaseGraph(&obj->graph);

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (obj->display_option == 1))
    {
      vxReleaseImage(&obj->disp_image);
      vxReleaseUserDataObject(&obj->disp_params_obj);
    }
}

static void app_show_usage(vx_int32 argc, vx_char* argv[])
{
    printf("\n");
    printf(" TIDL- Pixel Classification Demo - (c) Texas Instruments 2018\n");
    printf(" ============================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    uint32_t i;

    snprintf(obj->tidl_config_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/tidl_models/jSegNet_768x384/tidl_io_onnx_tiadsegNet_v2_1.bin");
    snprintf(obj->tidl_network_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/tidl_models/jSegNet_768x384/tidl_net_onnx_tiadsegNet_v2.bin");
    snprintf(obj->input_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/app_tidl_pc");
    snprintf(obj->input_dof_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->input_file_list,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/app_tidl_pixel_classification/names.txt");
    snprintf(obj->input_dof_file_list,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/app_tidl_pixel_classification/names_tifc.txt");
    snprintf(obj->output_file_path,APP_MAX_FILE_PATH, "./pc_out/");
    snprintf(obj->ti_logo_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/tivx/tidl_models/");

    obj->width  = 768;
    obj->height = 384;
    obj->dl_width = 768;
    obj->dl_height = 384;
    obj->en_out_img_write = 0;
    obj->ip_rgb_or_yuv = 0;
    obj->op_rgb_or_yuv = 0;
    obj->num_input_tensors = 1;
    obj->num_output_tensors = 1;
    obj->tidl_8bit_16bit_flag = 0;
    obj->pad_in_tidl = 0x0u;

    for(i=0; i < APP_MAX_TENSORS; i++){
      obj->num_classes[i] = 0;
    }

    for(i=0; i < TIVX_PIXEL_VIZ_MAX_CLASS; i++){
      obj->valid_region[i][0] = 0;
      obj->valid_region[i][1] = 0;
      obj->valid_region[i][2] = obj->dl_width - 1;
      obj->valid_region[i][3] = obj->dl_height - 1;
    }

    obj->display_option = 0;
    obj->delay_in_msecs = 0;
    obj->num_iterations = 1;
    obj->is_interactive = 0;

    memset(&obj->dof_plane_sep_params, 0, sizeof(tivxDofPlaneSepParams));
    memset(&obj->pixel_viz_params, 0, sizeof(tivxPixelVizParams));
    memset(&obj->img_pre_proc_params, 0, sizeof(tivxImgPreProcParams));

    for(i = 0; i< 3 ; i++){
      obj->img_pre_proc_params.scale_val[i] = 1.0;
    }

    obj->img_pre_proc_params.mean_pixel[0] = 0.0;
    obj->img_pre_proc_params.mean_pixel[1] = 0.0;
    obj->img_pre_proc_params.mean_pixel[2] = 0.0;

    obj->img_pre_proc_params.ip_rgb_or_yuv = 0;

    /*0 means 8 bit activations */
    obj->img_pre_proc_params.tidl_8bit_16bit_flag = 0;

}

static void app_parse_cfg_file(AppObj *obj, vx_char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    vx_char line_str[1024];
    vx_char *token;
    vx_int32 i;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(0);
    }

    while(fgets(line_str, sizeof(line_str), fp)!=NULL)
    {
        vx_char s[]=" \t";

        if (strchr(line_str, '#'))
        {
            continue;
        }

        /* get the first token */
        token = strtok(line_str, s);
        if(strcmp(token, "tidl_config_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->tidl_config_file_path, token);
        }
        else
        if(strcmp(token, "tidl_network_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->tidl_network_file_path, token);
        }
        else
        if(strcmp(token, "input_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->input_file_path, token);
        }
        else
        if(strcmp(token, "input_dof_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->input_dof_file_path, token);
        }
        if(strcmp(token, "input_file_prefix")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->input_file_prefix, token);
        }
        if(strcmp(token, "output_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->output_file_path, token);
        }
        if(strcmp(token, "output_file_prefix")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->output_file_prefix, token);
        }
        else
        if(strcmp(token, "num_classes")==0)
        {
            for(i = 0; i < APP_MAX_TENSORS; i++){
              token = strtok(NULL, s);
              if(token != NULL){
                obj->num_classes[i] = atoi(token);
              }else{
                break;
              }
            }
            for(;i < APP_MAX_TENSORS; i++){
              obj->num_classes[i] = 0;
            }

        }
        else
        if(strcmp(token, "valid_region")==0)
        {
            for(i = 0; i < TIVX_PIXEL_VIZ_MAX_CLASS*4; i++){
              token = strtok(NULL, s);
              if(token != NULL){
                obj->valid_region[i>>2][i&0x3] = atoi(token);
              }else{
                break;
              }
            }
            for(;i < TIVX_PIXEL_VIZ_MAX_CLASS*4; i++){
              obj->valid_region[i>>2][i&0x3] = -1;
            }

        }
        else
        if(strcmp(token, "width")==0)
        {
            token = strtok(NULL, s);
            obj->width = atoi(token);
        }
        else
        if(strcmp(token, "height")==0)
        {
            token = strtok(NULL, s);
            obj->height = atoi(token);
        }
        else
        if(strcmp(token, "dl_width")==0)
        {
            token = strtok(NULL, s);
            obj->dl_width = atoi(token);
        }
        else
        if(strcmp(token, "dl_height")==0)
        {
            token = strtok(NULL, s);
            obj->dl_height = atoi(token);
        }
        else
        if(strcmp(token, "input_file_list")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->input_file_list, token);
        }
        else
        if(strcmp(token, "input_dof_file_list")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->input_dof_file_list, token);
        }
        else
        if(strcmp(token, "en_out_img_write")==0)
        {
            token = strtok(NULL, s);
            obj->en_out_img_write = atoi(token);
        }
        else
        if(strcmp(token, "tidl_8bit_16bit_flag")==0)
        {
            token = strtok(NULL, s);
            obj->tidl_8bit_16bit_flag = atoi(token);
        }
        else
        if(strcmp(token, "ip_rgb_or_yuv")==0)
        {
            token = strtok(NULL, s);
            obj->ip_rgb_or_yuv = atoi(token);
        }
        else
        if(strcmp(token, "op_rgb_or_yuv")==0)
        {
            token = strtok(NULL, s);
            obj->op_rgb_or_yuv = atoi(token);
        }
        else
        if(strcmp(token, "display_option")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            obj->display_option = atoi(token);
            if(obj->display_option > 1)
                obj->display_option = 1;
        }
        else
        if(strcmp(token, "delay")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            obj->delay_in_msecs = atoi(token);
            if(obj->delay_in_msecs > 2000)
                obj->delay_in_msecs = 2000;
        }
        else
        if(strcmp(token, "num_iterations")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            obj->num_iterations = atoi(token);
            if(obj->num_iterations == 0)
                obj->num_iterations = 1;
        }
        else
        if(strcmp(token, "is_interactive")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            obj->is_interactive = atoi(token);
            if(obj->is_interactive > 1)
                obj->is_interactive = 1;
        }
        else
        if(strcmp(token, "ti_logo_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->ti_logo_file_path, token);
        }
        else
        if(strcmp(token, "raw_tidl_op")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            obj->pixel_viz_params.raw_tidl_op = atoi(token);
        }


    }

    fclose(fp);

}

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[])
{
    vx_int32 i;

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

static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    vx_reference params[APP_TIDL_MAX_PARAMS];

    vx_uint32 i;

    /* Create OpenVx Graph */
    obj->graph = vxCreateGraph(obj->context);
    APP_ASSERT_VALID_REF(obj->graph)
    vxSetReferenceName((vx_reference)obj->graph, "Pixel Classify");

    /* Create array of input tensors */
    createInputTensors(obj->context, obj->config, obj->input_tensors, &obj->dof_plane_sep_params,
                      &obj->img_pre_proc_params, &obj->pixel_viz_params, obj->pad_in_tidl, obj->tidl_8bit_16bit_flag);

    /* Create array of output tensors */
    createOutputTensors(obj->context, obj->config, obj->output_tensors, &obj->pixel_viz_params,obj->tidl_8bit_16bit_flag);

    /* Initialize param array */
    initParam(params, APP_TIDL_MAX_PARAMS);

    /* The 1st param MUST be config array */
    addParam(params, (vx_reference)obj->config);

    /* The 2nd param MUST be network tensor */
    addParam(params, (vx_reference)obj->network);

    /* The 3rd param MUST be create params */
    addParam(params, (vx_reference)obj->createParams);

    /* The 4th param MUST be inArgs */
    addParam(params, (vx_reference)obj->inArgs);

    /* The 5th param MUST be outArgs */
    addParam(params, (vx_reference)obj->outArgs);

    /* The 6th param MUST be NULL if trace data dump is not enabled */
    addParam(params, NULL);

    /* Create TIDL Node */
    obj->tidl_node = tivxTIDLNode(obj->graph, obj->kernel, params, obj->input_tensors, obj->output_tensors);
    APP_ASSERT_VALID_REF(obj->tidl_node)
    vxSetNodeTarget(obj->tidl_node, VX_TARGET_STRING, TIVX_TARGET_DSP_C7_1);
    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (obj->display_option == 1))
    {
        /* Create OpenVx Graph */
        obj->disp_graph = vxCreateGraph(obj->context);
        APP_ASSERT_VALID_REF(obj->disp_graph)
        vxSetReferenceName((vx_reference)obj->disp_graph, "Display");

        obj->disp_node = tivxDisplayNode(obj->disp_graph, obj->disp_params_obj, obj->disp_image);
        APP_ASSERT_VALID_REF(obj->disp_node)

        vxSetNodeTarget(obj->disp_node, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
    }

    /* Set names for diferent OpenVX objects */
    vxSetReferenceName((vx_reference)obj->config, "Config");
    vxSetReferenceName((vx_reference)obj->network, "Network");
    vxSetReferenceName((vx_reference)obj->createParams, "CreateParams");
    vxSetReferenceName((vx_reference)obj->inArgs, "InArgs");
    vxSetReferenceName((vx_reference)obj->outArgs, "OutArgs");

    for(i = 0; i < obj->num_input_tensors; i++) {
        vx_char tensor_name[] = "InputTensor_";
        vx_char ref_name[64];
        snprintf(ref_name, 64, "%s%s", tensor_name, tensor_num_str[i]);
        vxSetReferenceName((vx_reference)obj->input_tensors[i], ref_name);
    }

    for(i = 0; i < obj->num_output_tensors; i++) {
        vx_char tensor_name[] = "OutputTensor_";
        vx_char ref_name[64];
        snprintf(ref_name, 64, "%s%s", tensor_name, tensor_num_str[i]);
        vxSetReferenceName((vx_reference)obj->output_tensors[i], ref_name);
    }

    vxSetReferenceName((vx_reference)obj->kernel, "TIDLKernel");
    vxSetReferenceName((vx_reference)obj->tidl_node, "TIDLNode");

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (obj->display_option == 1))
    {
        vxSetReferenceName((vx_reference)obj->disp_params_obj, "DisplayParams");
        vxSetReferenceName((vx_reference)obj->disp_node, "DisplayNode");
    }

    /* pre process node creation */
    status = createPreProcNode(obj);
    APP_ASSERT(status==VX_SUCCESS);

    /* pre process node creation */
    if(obj->num_input_tensors > 1){
      /*Only if number of input channels are more than one then only pre processing node
        which is dof plane seperation is needed. Other preprocessing e.g. mean subtraction
        scaling is happening inside TIDL.
      */
      status = createDofProcNode(obj);
      APP_ASSERT(status==VX_SUCCESS);
    }

    /* post process node creation */
    status = createPostProcNode(obj);
    APP_ASSERT(status==VX_SUCCESS);

    return status;
}

static void app_run_task(void *app_var)
{
    AppObj *obj = (AppObj *)app_var;

    while(!obj->stop_task)
    {
        app_run_graph(obj);
    }
    obj->stop_task_done = 1;
}

static int32_t app_run_task_create(AppObj *obj)
{
    tivx_task_create_params_t params;
    int32_t status;

    tivxTaskSetDefaultCreateParams(&params);
    params.task_main = app_run_task;
    params.app_var = obj;

    obj->stop_task_done = 0;
    obj->stop_task = 0;

    status = tivxTaskCreate(&obj->task, &params);

    return status;
}

static void app_run_task_delete(AppObj *obj)
{
    while(obj->stop_task_done==0)
    {
         tivxTaskWaitMsecs(100);
    }

    tivxTaskDelete(&obj->task);
}

static char menu[] = {
    "\n"
    "\n ================================"
    "\n Demo : TIDL Pixel Classification"
    "\n ================================"
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};

static vx_status app_run_graph_interactive(AppObj *obj)
{
    vx_status status;
    uint32_t done = 0;
    char ch;

    status = app_run_task_create(obj);
    if(status!=0)
    {
        printf("app_tidl: ERROR: Unable to create task\n");
    }
    else
    {
        while(!done)
        {
            printf(menu);
            ch = getchar();
            printf("\n");

            switch(ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    tivx_utils_graph_perf_print(obj->graph);
                    tivx_utils_graph_perf_print(obj->disp_graph);
                    appPerfPointPrint(&obj->fileio_perf);
                    appPerfPointPrint(&obj->draw_perf);
                    appPerfPointPrint(&obj->total_perf);
                    printf("\n");
                    appPerfPointPrintFPS(&obj->total_perf);
                    printf("\n");
                    break;

                case 'x':
                    obj->stop_task = 1;
                    done = 1;
                    break;
            }
        }
        app_run_task_delete(obj);
    }
    return status;
}

static vx_status app_verify_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    #ifdef APP_DEBUG
    printf("app_tidl: Verifying graph ... \n");
    #endif

    /* Verify the TIDL Graph */
    status = vxVerifyGraph(obj->graph);
    if(status!=VX_SUCCESS)
    {
        printf("app_tidl: ERROR: Verifying graph ... Failed !!!\n");
        return status;
    }

    #ifdef APP_DEBUG
    printf("app_tidl: Verifying graph ... Done.\n");
    #endif

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (obj->display_option == 1))
    {
        #ifdef APP_DEBUG
        printf("app_tidl: Verifying display graph ... \n");
        #endif

        /* Verify the TIDL Graph */
        status = vxVerifyGraph(obj->disp_graph);
        if(status!=VX_SUCCESS)
        {
            printf("app_tidl: ERROR: Verifying display graph ... Failed !!!\n");
            return status;
        }

        #ifdef APP_DEBUG
        printf("app_tidl: Verifying display graph ... Done.\n");
        #endif
    }

    /* wait a while for prints to flush */
    tivxTaskWaitMsecs(100);

#if 1
    status = tivxExportGraphToDot(obj->graph,".", "vx_app_tidl");
    APP_ASSERT(status==VX_SUCCESS);
#endif
    return status;
}

static void app_run_graph_for_one_frame(AppObj *obj, char *curFileName, char *curDofFileName)
{
    vx_status status = VX_SUCCESS;

    vx_int32 in_img_width;
    vx_int32 in_img_height;
    vx_char input_file_name[APP_MAX_FILE_PATH];
    vx_char output_img_file[APP_MAX_FILE_PATH];
    vx_char input_dof_file_name[APP_MAX_FILE_PATH];
    vx_char output_file_path_local[APP_MAX_FILE_PATH];
    vx_df_image df;
    vx_char file_ext[8];

    appPerfPointBegin(&obj->total_perf);

    if(obj->ip_rgb_or_yuv == 0)
    {
        strcpy(file_ext, "bmp");
    }
    else if(obj->ip_rgb_or_yuv == 2)
    {
        strcpy(file_ext, "png");
    }
    else if(obj->ip_rgb_or_yuv == 1)
    {
        strcpy(file_ext, "yuv");
    }
    else
    {
        strcpy(file_ext, "bin");
    }

    snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%s.%s",
        obj->input_file_path,
        curFileName,
        file_ext
        );

    snprintf(output_img_file, APP_MAX_FILE_PATH, "%s/%s",
        obj->output_file_path,
        curFileName
        );

    char* token = strtok(curFileName, "/");

    #ifdef APP_DEBUG
    printf(" Reading [%s] ...\n", input_file_name);
    #endif

    /*Below 3 lines of the code is for making the required directory*/
    strcpy(output_file_path_local, obj->output_file_path);
    strcat(output_file_path_local,token);

    appPerfPointBegin(&obj->fileio_perf);

    //mkdir(output_file_path_local, 0750);
    if(obj->ip_rgb_or_yuv == 0){
      tivx_utils_load_vximage_from_bmpfile(obj->in_img, input_file_name, vx_false_e);
    }else if(obj->ip_rgb_or_yuv == 2){
      tivx_utils_load_vximage_from_pngfile(obj->in_img, input_file_name, vx_false_e);
    }else{
      loadvxImageYUVNV12(input_file_name,obj->in_img);
    }

    appPerfPointEnd(&obj->fileio_perf);

    if((obj->num_input_tensors > 1)){

      snprintf(input_dof_file_name, APP_MAX_FILE_PATH, "%s/%s.tifc",
          obj->input_dof_file_path,
          curDofFileName
          );

      /*only when number of input tensor is more than one than only dof input is required by the network*/
      printf("reading tifc file : %s\n",input_dof_file_name);
      fillDofTensor(obj,input_dof_file_name);
    }

    tivxCheckStatus(&status, vxQueryImage(obj->in_img, VX_IMAGE_WIDTH, &in_img_width, sizeof(in_img_width)));
    tivxCheckStatus(&status, vxQueryImage(obj->in_img, VX_IMAGE_HEIGHT, &in_img_height, sizeof(in_img_height)));
    tivxCheckStatus(&status, vxQueryImage(obj->in_img, VX_IMAGE_FORMAT, &df, sizeof(df)));

    if((in_img_width != obj->dl_width) || (in_img_height != obj->dl_height)){
      printf("Input image resolution is not as per the dl network expectation \n");
    }

    /* Execute the network */
    status = vxProcessGraph(obj->graph);

    appPerfPointBegin(&obj->draw_perf);

    displayOutput(obj, obj->output_images, obj->num_output_tensors);

    appPerfPointEnd(&obj->draw_perf);

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (obj->display_option == 1))
    {
      /* At this point, the output is ready to copy the updated buffer */
      vxCopyImagePatch(obj->disp_image,
                       &obj->disp_rect,
                       0,
                       &obj->image_addr,
                       (void *)obj->pDisplayBuf888,
                       VX_WRITE_ONLY,
                       VX_MEMORY_TYPE_HOST);

      if(VX_SUCCESS == status) {
        /* Execute the display graph */
        status = vxProcessGraph(obj->disp_graph);
      }
    }

    /* Dump the output. */
    if (obj->en_out_img_write){
        printf(" Writing [%s] ...\n", output_img_file);
        dumpOutput(obj->output_images, obj->num_output_tensors, output_img_file, obj->pixel_viz_params.op_rgb_or_yuv);
    }

    appPerfPointEnd(&obj->total_perf);
}

static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    vx_char curFileName[APP_MAX_FILE_PATH];
    vx_char curDofFileName[APP_MAX_FILE_PATH];

    FILE* test_case_file_names;
    FILE* dof_file_names = NULL;

    uint32_t x;

    uint64_t cur_time;

    if(VX_SUCCESS == status) {

      for(x = 0; x < obj->num_iterations; x++)
      {

        if((x%1) == 0)
          printf("app_tidl: Iteration %d out of %d ...\n", x, obj->num_iterations);

        test_case_file_names =  fopen(obj->input_file_list,"r");
        if (test_case_file_names == NULL)
        {
            printf("Could not open test case list file \n");
            break;
        }

        if(obj->num_input_tensors > 1){
          dof_file_names  = fopen(obj->input_dof_file_list,"r");

          if (dof_file_names == NULL)
          {
            printf("Could not open dof list file names \n");
          }
        }

        while (fgets(curFileName, sizeof(curFileName), test_case_file_names)) {

            curFileName[strlen(curFileName) - 1] = 0;

            if((obj->num_input_tensors > 1) && (fgets(curDofFileName, sizeof(curDofFileName), dof_file_names))){
                curDofFileName[strlen(curDofFileName) - 1] = 0;
            }

            cur_time = tivxPlatformGetTimeInUsecs();

            app_run_graph_for_one_frame(obj, curFileName, curDofFileName);

            cur_time = tivxPlatformGetTimeInUsecs() - cur_time;
            /* convert to msecs */
            cur_time = cur_time/1000;

            if(cur_time < obj->delay_in_msecs)
            {
                tivxTaskWaitMsecs(obj->delay_in_msecs - cur_time);
            }

            /* user asked to stop processing */
            if(obj->stop_task)
                break;

        }
        fclose(test_case_file_names);
        if(dof_file_names!=NULL)
            fclose(dof_file_names);

        #ifdef APP_DEBUG
        printf("app_tidl: Iteration %d of %d ... Done.\n", x, obj->num_iterations);
        #endif

        if(obj->stop_task)
            break;

      } //num_iterations
    }
    return status;
}
static vx_user_data_object readConfig(vx_context context, vx_char *config_file, vx_uint32 *num_input_tensors, vx_uint32 *num_output_tensors)
{
    vx_status status = VX_SUCCESS;

    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params  *tidlParams;
    vx_user_data_object   config;
    vx_uint32  capacity;
    vx_map_id map_id;
    vx_size read_count;

    FILE *fp_config;

    fp_config = fopen(config_file, "rb");

    if(fp_config == NULL)
    {
        printf("Unable to open file! %s \n", config_file);
        return NULL;
    }

    fseek(fp_config, 0, SEEK_END);
    capacity = ftell(fp_config);
    fseek(fp_config, 0, SEEK_SET);

    if( capacity != sizeof(sTIDL_IOBufDesc_t) )
    {
        printf("Config file size (%d bytes) does not match size of sTIDL_IOBufDesc_t (%d bytes)\n", capacity, (vx_uint32)sizeof(sTIDL_IOBufDesc_t));
        return NULL;
    }

    /* Create a user struct type for handling config data*/
    config = vxCreateUserDataObject(context, "tivxTIDLJ7Params", sizeof(tivxTIDLJ7Params), NULL );

    status = vxGetStatus((vx_reference)config);

    if (VX_SUCCESS == status)
    {
        status = vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id,
                        (void **)&tidlParams, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if (VX_SUCCESS == status)
        {
            if(tidlParams == NULL)
            {
                printf("Map of config object failed\n");
                return NULL;
            }

            tivx_tidl_j7_params_init(tidlParams);

            ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;

            read_count = fread(ioBufDesc, capacity, 1, fp_config);
            if(read_count != 1)
            {
                printf("Unable to read file!\n");
            }


            *num_input_tensors  = ioBufDesc->numInputBuf;
            *num_output_tensors = ioBufDesc->numOutputBuf;

            printf("Num Input Tensors : %d\n", ioBufDesc->numInputBuf);
            printf("Num Output Tensors : %d\n", ioBufDesc->numOutputBuf);

            vxUnmapUserDataObject(config, map_id);
        }
    }

    fclose(fp_config);

    return config;
}

static vx_user_data_object readNetwork(vx_context context, vx_char *network_file)
{
    vx_status status;

    vx_user_data_object  network;
    vx_map_id  map_id;
    vx_uint32  capacity;
    vx_size read_count;
    void      *network_buffer = NULL;

    FILE *fp_network;

    fp_network = fopen(network_file, "rb");

    if(fp_network == NULL)
    {
        printf("Unable to open file! %s \n", network_file);
        return NULL;
    }

    fseek(fp_network, 0, SEEK_END);
    capacity = ftell(fp_network);
    fseek(fp_network, 0, SEEK_SET);

    network = vxCreateUserDataObject(context, "TIDL_network", capacity, NULL );

    status = vxGetStatus((vx_reference)network);

    if (VX_SUCCESS == status)
    {
        status = vxMapUserDataObject(network, 0, capacity, &map_id,
                        (void **)&network_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if (VX_SUCCESS == status)
        {
            if(network_buffer)
            {
                read_count = fread(network_buffer, capacity, 1, fp_network);
                if(read_count != 1)
                {
                    printf("Unable to read file!\n");
                }
            }
            else
            {
                printf("Unable to allocate memory for reading network! %d bytes\n", capacity);
            }
            vxUnmapUserDataObject(network, map_id);
        }
    }

    fclose(fp_network);

    return network;
}
static vx_user_data_object setCreateParams(vx_context context)
{
    vx_status status;

    vx_user_data_object  createParams;
    vx_map_id  map_id;
    vx_uint32  capacity;
    void *createParams_buffer = NULL;

    capacity = sizeof(TIDL_CreateParams);
    createParams = vxCreateUserDataObject(context, "TIDL_CreateParams", capacity, NULL );

    status = vxGetStatus((vx_reference)createParams);

    if (VX_SUCCESS == status)
    {
        status = vxMapUserDataObject(createParams, 0, capacity, &map_id,
                        (void **)&createParams_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if (VX_SUCCESS == status)
        {
            if(createParams_buffer)
            {
              TIDL_CreateParams *prms = createParams_buffer;
              //write create params here
              TIDL_createParamsInit(prms);

              prms->isInbufsPaded                 = 1;
              prms->quantRangeExpansionFactor     = 1.0;
              prms->quantRangeUpdateFactor        = 0.0;

            }
            else
            {
                printf("Unable to allocate memory for create time params! %d bytes\n", capacity);
            }

            vxUnmapUserDataObject(createParams, map_id);
        }
    }

    return createParams;
}

static vx_user_data_object setInArgs(vx_context context)
{
    vx_status status;

    vx_user_data_object  inArgs;
    vx_map_id  map_id;
    vx_uint32  capacity;
    void *inArgs_buffer = NULL;

    capacity = sizeof(TIDL_InArgs);
    inArgs = vxCreateUserDataObject(context, "TIDL_InArgs", capacity, NULL );

    status = vxGetStatus((vx_reference)inArgs);

    if (VX_SUCCESS == status)
    {
        status = vxMapUserDataObject(inArgs, 0, capacity, &map_id,
                        (void **)&inArgs_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if (VX_SUCCESS == status)
        {
            if(inArgs_buffer)
            {
              TIDL_InArgs *prms = inArgs_buffer;
              prms->iVisionInArgs.size         = sizeof(TIDL_InArgs);
              prms->iVisionInArgs.subFrameInfo = 0;
            }
            else
            {
                printf("Unable to allocate memory for inArgs! %d bytes\n", capacity);
            }

            vxUnmapUserDataObject(inArgs, map_id);
        }
    }

    return inArgs;
}

static vx_user_data_object setOutArgs(vx_context context)
{
    vx_status status;

    vx_user_data_object  outArgs;
    vx_map_id  map_id;
    vx_uint32  capacity;
    void *outArgs_buffer = NULL;

    capacity = sizeof(TIDL_outArgs);
    outArgs = vxCreateUserDataObject(context, "TIDL_outArgs", capacity, NULL );

    status = vxGetStatus((vx_reference)outArgs);

    if (VX_SUCCESS == status)
    {
        status = vxMapUserDataObject(outArgs, 0, capacity, &map_id,
                        (void **)&outArgs_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if (VX_SUCCESS == status)
        {
            if(outArgs_buffer)
            {
              TIDL_outArgs *prms = outArgs_buffer;
              prms->iVisionOutArgs.size         = sizeof(TIDL_outArgs);
            }
            else
            {
                printf("Unable to allocate memory for outArgs! %d bytes\n", capacity);
            }

            vxUnmapUserDataObject(outArgs, map_id);
        }
    }

    return outArgs;
}
static void createInputTensors(vx_context context, vx_user_data_object config, vx_tensor *input_tensors,
                              tivxDofPlaneSepParams* dof_prm,tivxImgPreProcParams* img_pre_proc_params,
                              tivxPixelVizParams* viz_prm, vx_int32 pad_in_tidl, vx_int32 tidl_8bit_16bit_flag)
{
    vx_size   input_sizes[APP_MAX_TENSOR_DIMS];
    vx_map_id map_id_config;

    vx_uint32 id;

    tivxTIDLJ7Params *tidlParams;
    sTIDL_IOBufDesc_t *ioBufDesc;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                      (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;

    for(id = 0; id < ioBufDesc->numInputBuf; id++) {

        if(pad_in_tidl == 0x1){
          /*If padding is done in TIDL then input tensor should be allocated without padding*/
          input_sizes[0] = ioBufDesc->inWidth[id];
          input_sizes[1] = ioBufDesc->inHeight[id];
        }
        else{
          input_sizes[0] = ioBufDesc->inWidth[id]  + ioBufDesc->inPadL[id] + ioBufDesc->inPadR[id];
          input_sizes[1] = ioBufDesc->inHeight[id] + ioBufDesc->inPadT[id] + ioBufDesc->inPadB[id];
        }
        input_sizes[2] = ioBufDesc->inNumChannels[id];

        if(tidl_8bit_16bit_flag == 0){
          input_tensors[id] = vxCreateTensor(context, 3, input_sizes, VX_TYPE_UINT8, 0);
        }else if(tidl_8bit_16bit_flag == 1){
          input_tensors[id] = vxCreateTensor(context, 3, input_sizes, VX_TYPE_UINT16, 0);
        }

        /*First tensor is dof for MS and SS-MS joint network*/
        if(pad_in_tidl == 0x0){
          /*First Buffer is DOf when two input is provided*/
          if(id == 0){
            dof_prm->pad_pixel[0] = ioBufDesc->inPadL[id];
            dof_prm->pad_pixel[1] = ioBufDesc->inPadT[id];
            dof_prm->pad_pixel[2] = ioBufDesc->inPadR[id];
            dof_prm->pad_pixel[3] = ioBufDesc->inPadB[id];
          }
          /*Second Buffer is input image when two input is provided, otherwise it is first when only one input is given to tidl.*/
          /*In both scenario (1/2 input tensor to TIDL) below code is valid*/
          img_pre_proc_params->pad_pixel[0] = ioBufDesc->inPadL[id];
          img_pre_proc_params->pad_pixel[1] = ioBufDesc->inPadT[id];
          img_pre_proc_params->pad_pixel[2] = ioBufDesc->inPadR[id];
          img_pre_proc_params->pad_pixel[3] = ioBufDesc->inPadB[id];

        }else{
          dof_prm->pad_pixel[0] = 0;
          dof_prm->pad_pixel[1] = 0;
          dof_prm->pad_pixel[2] = 0;
          dof_prm->pad_pixel[3] = 0;

          img_pre_proc_params->pad_pixel[0] = 0;
          img_pre_proc_params->pad_pixel[1] = 0;
          img_pre_proc_params->pad_pixel[2] = 0;
          img_pre_proc_params->pad_pixel[3] = 0;

        }
        /* For pxel classifcation network input image is alaways at the last */
        if(id == (ioBufDesc->numInputBuf-1)){
          viz_prm->input_img_pitch       = input_sizes[0];
          viz_prm->input_img_offset      = viz_prm->input_img_pitch*ioBufDesc->inPadT[id] + ioBufDesc->inPadL[id];
          viz_prm->input_img_plane_pitch = viz_prm->input_img_pitch*input_sizes[1];
        }

    }


    vxUnmapUserDataObject(config, map_id_config);

    return;
}

static void createOutputTensors(vx_context context, vx_user_data_object config, vx_tensor *output_tensors,
                                tivxPixelVizParams* viz_prm, vx_int32 tidl_8bit_16bit_flag)
{
    vx_size output_sizes[APP_MAX_TENSOR_DIMS];
    vx_map_id map_id_config;

    vx_uint32 id;

    tivxTIDLJ7Params *tidlParams;
    sTIDL_IOBufDesc_t *ioBufDesc;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                      (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;
    for(id = 0; id < ioBufDesc->numOutputBuf; id++) {
        output_sizes[0] = ioBufDesc->outWidth[id]  + ioBufDesc->outPadL[id] + ioBufDesc->outPadR[id];
        output_sizes[1] = ioBufDesc->outHeight[id] + ioBufDesc->outPadT[id] + ioBufDesc->outPadB[id];
        output_sizes[2] = ioBufDesc->outNumChannels[id];

        if(tidl_8bit_16bit_flag == 0){
          output_tensors[id] = vxCreateTensor(context, 3, output_sizes, VX_TYPE_UINT8, 0);
        }else if(tidl_8bit_16bit_flag == 1){
          output_tensors[id] = vxCreateTensor(context, 3, output_sizes, VX_TYPE_UINT16, 0);
        }

        if(id < TIVX_PIXEL_VIZ_MAX_TENSOR){
          viz_prm->output_width[id] = ioBufDesc->outWidth[id];
          viz_prm->output_height[id] = ioBufDesc->outHeight[id];
          viz_prm->output_buffer_pitch[id] = ioBufDesc->outWidth[id]  + ioBufDesc->outPadL[id] + ioBufDesc->outPadR[id];
          viz_prm->output_buffer_offset[id] = viz_prm->output_buffer_pitch[id]*ioBufDesc->outPadT[id] + ioBufDesc->outPadL[id];
        }

    }

  vxUnmapUserDataObject(config, map_id_config);

}


static void dumpOutput(vx_image *output_images, vx_int32 num_output_tensors, vx_char *output_file, vx_int32 op_rgb_or_yuv)
{
  vx_int32 i;
  vx_char cur_dump_file[APP_MAX_FILE_PATH];

  for(i = 0; i < num_output_tensors; i++){
    if(op_rgb_or_yuv == 0){
      snprintf(cur_dump_file, APP_MAX_FILE_PATH, "%s_%d.bmp",
        output_file,
        i
      );
    tivx_utils_save_vximage_to_bmpfile(cur_dump_file,output_images[i]);

    }else if(op_rgb_or_yuv == 2){
        snprintf(cur_dump_file, APP_MAX_FILE_PATH, "%s_%d.png",
          output_file,
          i
        );
      tivx_utils_save_vximage_to_pngfile(cur_dump_file,output_images[i]);

    }else{
      snprintf(cur_dump_file, APP_MAX_FILE_PATH, "%s_%d.yuv",
        output_file,
        i
      );
      saveYUVNV12vxImage(cur_dump_file,output_images[i]);

    }
  }
}

static void fillDofTensor(AppObj *obj, vx_char* in_dof_file)
{
    vx_size    start[APP_MAX_TENSOR_DIMS];
    vx_size    input_strides[APP_MAX_TENSOR_DIMS];
    vx_size    input_sizes[APP_MAX_TENSOR_DIMS];
    vx_status status = VX_SUCCESS;

    vx_map_id map_id_dof_tensor;

    vx_int32* dof_tensor_buffer;

    FILE* fp;
    vx_int32 total_read=0;

    input_sizes[0] = obj->width*obj->height;

    start[0] = 0;

    input_strides[0] = input_sizes[0];

    status = tivxMapTensorPatch(obj->dof_tensor, 1, start, input_sizes, &map_id_dof_tensor,
                                input_strides,(void**) &dof_tensor_buffer,
                                VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    if (status == VX_SUCCESS) {
        fp = fopen(in_dof_file,"rb");
        if(fp == NULL){
          printf("input dof file %s could not be opened", in_dof_file);
        }else{
          /*16 byte header in tifc file format dumped by DOF simulator*/
          fseek(fp,16,SEEK_SET);
          total_read = fread(dof_tensor_buffer,4,obj->width*obj->height,fp);
          if(total_read != obj->width*obj->height){
            printf("Some problem in tifc dof file read");
          }
        }
        fclose(fp);

        tivxUnmapTensorPatch(obj->dof_tensor, map_id_dof_tensor);
    }
}

static vx_status createPreProcNode(AppObj *obj)
{
    vx_enum config_type = VX_TYPE_INVALID;
    vx_status status;
    vx_int32 img_tensor_idx;

    config_type = vxRegisterUserStruct(obj->context, sizeof(tivxImgPreProcParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);
    obj->img_proc_config = vxCreateArray(obj->context, config_type, 1);
    APP_ASSERT_VALID_REF(obj->img_proc_config);
    vxSetReferenceName((vx_reference)obj->img_proc_config, "ImgPreProcConfigArray");
    vxAddArrayItems(obj->img_proc_config, 1, &obj->img_pre_proc_params, sizeof(tivxImgPreProcParams));
    APP_ASSERT_VALID_REF(obj->img_proc_config)

    if((obj->ip_rgb_or_yuv == 0) || (obj->ip_rgb_or_yuv == 2)){
      obj->in_img = vxCreateImage(obj->context, obj->dl_width, obj->dl_height, VX_DF_IMAGE_RGB);
    }else{
      obj->in_img = vxCreateImage(obj->context, obj->dl_width, obj->dl_height, VX_DF_IMAGE_NV12);
    }

    APP_ASSERT_VALID_REF(obj->in_img);
    vxSetReferenceName((vx_reference)obj->in_img, "InputImageU8");

    if (obj->num_input_tensors > 1)
    {
      img_tensor_idx = 1;
    }
    else
    {
      img_tensor_idx = 0;
    }

    obj->node_img_proc = tivxImgPreProcNode(obj->graph,
                                                obj->img_proc_config,
                                                obj->in_img,
                                                obj->input_tensors[img_tensor_idx]);

    APP_ASSERT_VALID_REF(obj->node_img_proc);

    status = vxSetNodeTarget(obj->node_img_proc, VX_TARGET_STRING, TIVX_TARGET_DSP1);

    vxSetReferenceName((vx_reference)obj->node_img_proc, "ImgPreProcNode");

    return(status);
}

static vx_status createDofProcNode(AppObj *obj)
{
    vx_enum config_type = VX_TYPE_INVALID;
    vx_status status;
    vx_size temp_size[3];

    config_type = vxRegisterUserStruct(obj->context, sizeof(tivxDofPlaneSepParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);
    obj->dof_plane_sep_config = vxCreateArray(obj->context, config_type, 1);
    APP_ASSERT_VALID_REF(obj->dof_plane_sep_config);
    vxSetReferenceName((vx_reference)obj->dof_plane_sep_config, "DofPlaneSepConfigArray");
    vxAddArrayItems(obj->dof_plane_sep_config, 1, &obj->dof_plane_sep_params, sizeof(tivxDofPlaneSepParams));
    APP_ASSERT_VALID_REF(obj->dof_plane_sep_config)



    temp_size[0] = obj->width*obj->height;
    obj->dof_tensor = vxCreateTensor(obj->context, 1, temp_size, VX_TYPE_UINT32, 0);
    vxSetReferenceName((vx_reference)obj->dof_tensor, "RawDOF");


    obj->node_dof_plane_sep = tivxDofPlaneSepNode(obj->graph,
                                                obj->dof_plane_sep_config,
                                                obj->dof_tensor,
                                                obj->input_tensors[0]);

    APP_ASSERT_VALID_REF(obj->node_dof_plane_sep);

    status = vxSetNodeTarget(obj->node_dof_plane_sep, VX_TARGET_STRING, TIVX_TARGET_DSP1);

    vxSetReferenceName((vx_reference)obj->node_dof_plane_sep, "DofPlaneSepNode");

    return(status);
}

static vx_status createPostProcNode(AppObj *obj)
{
    vx_status status;
    vx_int32 i;

    obj->pixel_viz_config = vxCreateUserDataObject(obj->context, "tivxPixelVizParams",
                                                 sizeof(tivxPixelVizParams), NULL);
    APP_ASSERT_VALID_REF(obj->pixel_viz_config);
    vxSetReferenceName((vx_reference)obj->pixel_viz_config, "PixelVizConfig");
    status = vxCopyUserDataObject(obj->pixel_viz_config, 0, sizeof(tivxPixelVizParams), &obj->pixel_viz_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    if((obj->pixel_viz_params.op_rgb_or_yuv == 0) || (obj->pixel_viz_params.op_rgb_or_yuv == 2)){
      for(i = 0; i < obj->num_output_tensors; i++){
        obj->output_images[i] = vxCreateImage(obj->context, obj->dl_width, obj->dl_height, VX_DF_IMAGE_RGB);
      }
    }else{
      for(i = 0; i < obj->num_output_tensors; i++){
        obj->output_images[i] = vxCreateImage(obj->context, obj->dl_width, obj->dl_height, VX_DF_IMAGE_NV12);
      }
    }

    obj->node_pixel_viz = tivxPixelVizNode(obj->graph,
                                           obj->pixel_viz_kernel,
                                           obj->pixel_viz_config,
                                           obj->outArgs,
                                           obj->in_img,
                                           obj->pixel_viz_params.num_output_tensors,
                                           obj->output_tensors,
                                           obj->output_images);

    APP_ASSERT_VALID_REF(obj->node_pixel_viz);
    status = vxSetNodeTarget(obj->node_pixel_viz, VX_TARGET_STRING, TIVX_TARGET_DSP1);
    vxSetReferenceName((vx_reference)obj->node_pixel_viz, "PixelVizNode");

    return(status);
}

static vx_status saveYUVNV12vxImage(char* file_name, vx_image in_img)
{
  vx_status status;
  FILE * fp;

  status = vxGetStatus((vx_reference)in_img);

  if(status == VX_SUCCESS)
  {
    vx_rectangle_t rect;
    vx_imagepatch_addressing_t image_addr;
    vx_map_id map_id;
    void * data_ptr;
    vx_uint32  img_width;
    vx_uint32  img_height;

    fp = fopen(file_name,"wb");

    if(fp == NULL)
    {
      printf("File could not be opened \n");
      return (VX_FAILURE);
    }

    vxQueryImage(in_img, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
    vxQueryImage(in_img, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = img_width;
    rect.end_y = img_height;
    status = vxMapImagePatch(in_img,
                             &rect,
                             0,
                             &map_id,
                             &image_addr,
                             &data_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);
    if(status==VX_SUCCESS)
    {
      fwrite(data_ptr,1,img_width*img_height, fp);
      vxUnmapImagePatch(in_img, map_id);
    }

    status = vxMapImagePatch(in_img,
                             &rect,
                             1,
                             &map_id,
                             &image_addr,
                             &data_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);

    if(status==VX_SUCCESS)
    {
      fwrite(data_ptr,1,img_width*img_height/2, fp);
      vxUnmapImagePatch(in_img, map_id);
    }

    fclose(fp);
  }

  return(status);
}

static vx_status loadvxImageYUVNV12(char* file_name, vx_image in_img)
{
  vx_status status;
  FILE * fp;
  vx_int32 bytes_read;

  status = vxGetStatus((vx_reference)in_img);

  if(status == VX_SUCCESS)
  {
    vx_rectangle_t rect;
    vx_imagepatch_addressing_t image_addr;
    vx_map_id map_id;
    void * data_ptr;
    vx_uint32  img_width;
    vx_uint32  img_height;

    fp = fopen(file_name,"rb");

    if(fp == NULL)
    {
      printf("File could not be opened \n");
      return (VX_FAILURE);
    }

    vxQueryImage(in_img, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
    vxQueryImage(in_img, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = img_width;
    rect.end_y = img_height;
    status = vxMapImagePatch(in_img,
                             &rect,
                             0,
                             &map_id,
                             &image_addr,
                             &data_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);
    if(status==VX_SUCCESS)
    {
      bytes_read = fread(data_ptr,1,img_width*img_height, fp);

      if(bytes_read != img_width*img_height)
      {
        printf("Total bytes read is not same as requested");
      }
      vxUnmapImagePatch(in_img, map_id);
    }

    status = vxMapImagePatch(in_img,
                             &rect,
                             1,
                             &map_id,
                             &image_addr,
                             &data_ptr,
                             VX_WRITE_ONLY,
                             VX_MEMORY_TYPE_HOST,
                             VX_NOGAP_X);

    if(status==VX_SUCCESS)
    {
      bytes_read = fread(data_ptr,1,img_width*img_height/2, fp);

      if(bytes_read != img_width*img_height/2)
      {
        printf("Total bytes read is not same as requested");
      }
      vxUnmapImagePatch(in_img, map_id);
    }

    fclose(fp);
  }

  return(status);
}

static vx_status displayOutput(AppObj *obj, vx_image *output_images, vx_int32 num_output_tensors)
{
    vx_status status = VX_SUCCESS;

    Draw2D_FontPrm sTop5Prm;
    Draw2D_FontPrm sClassPrm;

    vx_rectangle_t rect;
    vx_imagepatch_addressing_t image_addr;
    vx_map_id map_id;
    void * data_ptr;
    vx_uint32  img_width;
    vx_uint32  img_height;

    int32_t i, j;

    sTop5Prm.fontIdx = 8;
    sClassPrm.fontIdx = 5;

    Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH - obj->dl_width)/2 + 120, 120, "Semantic Segmentation Demo", &sTop5Prm);
    Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH - obj->dl_width)/2 - 120, 220 + obj->dl_height, "Road (Green), Vehicles (Dark Blue), Pedestrians (Red), Sky (Light Blue)", &sClassPrm);

    /* Convert RGB565 to RGB888 before writing output */
    for(i = 0, j = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT); i++)
    {
      uint16_t RGB_565_val = obj->pDisplayBuf565[i];

      obj->pDisplayBuf888[j + 0] = (RGB_565_val & 0x1F) << 3;
      obj->pDisplayBuf888[j + 1] = ((RGB_565_val >> 5) & 0x3F) << 2;
      obj->pDisplayBuf888[j + 2] = ((RGB_565_val >> 11) & 0x1F) << 3;

      j  += 3;
    }

    vxQueryImage(output_images[0], VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
    vxQueryImage(output_images[0], VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = img_width;
    rect.end_y = img_height;

    status = vxMapImagePatch(output_images[0],
                            &rect,
                            0,
                            &map_id,
                            &image_addr,
                            &data_ptr,
                            VX_READ_ONLY,
                            VX_MEMORY_TYPE_HOST,
                            VX_NOGAP_X);

    if((obj->dl_width <= DISPLAY_WIDTH) && (obj->dl_height <= (DISPLAY_HEIGHT - 200)))
    {
         uint32_t startX = (DISPLAY_WIDTH - obj->dl_width) / 2;
         uint32_t startY = (DISPLAY_HEIGHT - obj->dl_height) / 2;

         for(i = 0; i < obj->dl_height; i++)
         {
           uint8_t *pOut = &obj->pDisplayBuf888[((startY + i) * DISPLAY_WIDTH * 3) + (startX * 3)];
           uint8_t *pIn  = data_ptr + (i * obj->dl_width * 3);

           for(j = 0; j < obj->dl_width; j++)
           {
             *pOut++ = *pIn++;
             *pOut++ = *pIn++;
             *pOut++ = *pIn++;
           }
         }

    }

    vxUnmapImagePatch(output_images[0], map_id);

    return status;
}
