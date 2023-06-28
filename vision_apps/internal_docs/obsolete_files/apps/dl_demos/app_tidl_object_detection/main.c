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
#include <TI/tivx_task.h>
#include <TI/tivx_target_kernel.h>
#include "tivx_kernels_host_utils.h"
#include <TI/tivx_img_proc.h>

#include <TI/j7_tidl.h>
#include <tivx_utils_file_rd_wr.h>
#include <tivx_utils_graph_perf.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <float.h>
#include <math.h>
#include <utils/draw2d/include/draw2d.h>
#include <utils/perf_stats/include/app_perf_stats.h>

#include "itidl_ti.h"
#include "tiadalg_interface.h"
#include "fisheye_angle_table.h"

//#define APP_DEBUG

#define APP_MAX_FILE_PATH           (256u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));

#define APP_MAX_NUM_CLASSES         (4u)
#define APP_MAX_TENSORS             (4u)
#define APP_MAX_TENSOR_DIMS         (4u)
#define APP_TIDL_MAX_PARAMS         (16u)

#define ABS_FLT(a) ((a) > 0)?(a):(-(a))

static const char *tensor_num_str[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

#define MAX_IMG_WIDTH  (2048)
#define MAX_IMG_HEIGHT (1024)
#define DISPLAY_WIDTH  (1280)
#define DISPLAY_HEIGHT (720)

typedef struct {

    /* config options */
    char tidl_config_file_path[APP_MAX_FILE_PATH];
    char tidl_network_file_path[APP_MAX_FILE_PATH];

    char input_file_path[APP_MAX_FILE_PATH];
    char output_file_path[APP_MAX_FILE_PATH];
    char input_file_list[APP_MAX_FILE_PATH];
    char ti_logo_file_path[APP_MAX_FILE_PATH];

    uint32_t num_input_tensors;
    uint32_t num_output_tensors;
    vx_df_image df_image;
    void *data_ptr;

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

    vx_int32 width;
    vx_int32 height;
    vx_int32 dl_width;
    vx_int32 dl_height;
    vx_int32 en_out_img_write;
    vx_int32 en_out_log_write;

    vx_float32*    (*angle_table)[2];
    vx_float32*    (*rev_angle_table)[2];
    vx_image   in_img;
    vx_tensor  fwd_table_tensor;
    vx_tensor  rev_table_tensor;
    vx_tensor  kp_tensor;
    vx_tensor  kp_valid;

    vx_node  node_img_pre_proc;
    vx_node  node_od_post_proc;

    vx_array img_pre_proc_config;
    tivxImgPreProcParams img_pre_proc_params;

    vx_array od_post_proc_config;
    tivxODPostProcParams od_post_proc_params;
    char label_names[APP_MAX_NUM_CLASSES][APP_MAX_FILE_PATH];

    vx_float32 viz_th;
    vx_int32 raw_tidl_op;
    vx_int32 ip_rgb_or_yuv;

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

    uint32_t enable_psd;

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
static void createInputTensors(vx_context context, vx_user_data_object config, vx_tensor *input_tensors, vx_int32* pad_pixel);
static int32_t createOutputTensors(vx_context context, vx_user_data_object config, vx_tensor *output_tensors);
static void displayOutput(AppObj *obj, vx_user_data_object config, vx_tensor *output_tensors, vx_char *log_file, vx_char *log_file_bin, vx_tensor kp_tensor,vx_tensor kp_valid);
static void drawBox(AppObj *obj, TIDL_ODLayerObjInfo * pPSpots);
static void drawPoints(AppObj *obj, vx_int16* kp_ptr, vx_int32 num_points,vx_int32 label);
static void drawJoinedPoints(AppObj *obj, vx_int16* kp_ptr, vx_int32 num_points, vx_int32 label);
static void lineInterp(vx_int16 startPoint[2], vx_int16 endPoint[2], vx_int16 *outPoints, vx_int32 totalLineParts);
static vx_status loadvxImageYUVNV12(char* file_name, vx_image in_img);
static void fillTableTensors(AppObj *obj);
static vx_status createPostProcNode(AppObj *obj);
static vx_status createPreProcNode(AppObj *obj);

static char* itoa(int value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}


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

vx_int32 app_tidl_od_main(vx_int32 argc, vx_char* argv[])
{
    AppObj *obj = &gAppObj;
    int32_t status;

    app_parse_cmd_line_args(obj, argc, argv);
    {
      /* update parameters based on config file read */
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
        Draw2D_LinePrm sLinePrm;
        Draw2D_FontPrm sTop5Prm;

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

        sTop5Prm.fontIdx = 8;

        Draw2D_insertBmp(obj->pHndl, banner_file, 0, 0);

        sLinePrm.lineColor = RGB888_TO_RGB565(255, 255, 255);
        sLinePrm.lineSize  = 3;
        sLinePrm.lineColorFormat = DRAW2D_DF_BGR16_565;

        /* Draw a vertial line */
        Draw2D_drawLine(obj->pHndl, DISPLAY_WIDTH/2, 0, DISPLAY_WIDTH/2, DISPLAY_HEIGHT, &sLinePrm);

        if(obj->enable_psd)
        {
            Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH/2) + 40, 140, "Parking Spot Detection (PSD)", &sTop5Prm);
        }
        else
        {
            Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH/2) + 40, 140, "Vehicle Detection (VD)", &sTop5Prm);
        }
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

    vxReleaseNode(&obj->tidl_node);
    vxReleaseNode(&obj->node_img_pre_proc);
    vxReleaseNode(&obj->node_od_post_proc);
    vxReleaseGraph(&obj->graph);

    vxReleaseUserDataObject(&obj->config);
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
    vxReleaseImage(&obj->in_img);
    vxReleaseTensor(&obj->fwd_table_tensor);
    vxReleaseTensor(&obj->rev_table_tensor);
    vxReleaseTensor(&obj->kp_tensor);
    vxReleaseTensor(&obj->kp_valid);
    vxReleaseArray(&obj->img_pre_proc_config);
    vxReleaseArray(&obj->od_post_proc_config);

    if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (obj->display_option == 1))
    {
      vxReleaseImage(&obj->disp_image);
      vxReleaseUserDataObject(&obj->disp_params_obj);
    }

}

static void app_show_usage(vx_int32 argc, vx_char* argv[])
{
    printf("\n");
    printf(" TIDL-OD Demo - (c) Texas Instruments 2018\n");
    printf(" =========================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    int32_t i;
    snprintf(obj->tidl_config_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/tidl_models/od/tidl_io_jpsdNet_1.bin");
    snprintf(obj->tidl_network_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/tidl_models/od/tidl_net_jpsdNet.bin");
    snprintf(obj->input_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/app_tidl_psd");
    snprintf(obj->input_file_list,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/app_tidl_psd/names.txt");
    snprintf(obj->ti_logo_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/tivx/tidl_models/");
    snprintf(obj->output_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/psd_out");
    obj->width = 1280;
    obj->height =  720;
    obj->dl_width = 512;
    obj->dl_height =  512;
    obj->viz_th = 0.5;
    obj->en_out_img_write = 0;
    obj->raw_tidl_op = 0;
    obj->en_out_log_write = 0;
    obj->ip_rgb_or_yuv = 0;
    obj->display_option = 0;
    obj->delay_in_msecs = 0;
    obj->num_iterations = 1;
    obj->is_interactive = 0;
    obj->enable_psd = 1;

    memset(&obj->img_pre_proc_params, 0, sizeof(tivxImgPreProcParams));
    memset(&obj->od_post_proc_params, 0, sizeof(tivxODPostProcParams));

    for(i = 0; i< 3 ; i++){
      obj->img_pre_proc_params.scale_val[i] = 1.0;
    }

    obj->img_pre_proc_params.mean_pixel[0] = 0.0;
    obj->img_pre_proc_params.mean_pixel[1] = 0.0;
    obj->img_pre_proc_params.mean_pixel[2] = 0.0;

    obj->img_pre_proc_params.ip_rgb_or_yuv = 0;

    /*0 means 8 bit activations */
    obj->img_pre_proc_params.tidl_8bit_16bit_flag = 0;

    /*Focal length is hard coded for the given camera property. It can be exposed by cfg*/
    obj->od_post_proc_params.num_max_det = 100;
    obj->od_post_proc_params.points_per_line = 8;
    obj->od_post_proc_params.num_keypoints = 4;
    obj->od_post_proc_params.focal_length = 311.833f;
    obj->od_post_proc_params.inter_center_x_fact = 10;
    obj->od_post_proc_params.inter_center_y_fact = 2;
    obj->od_post_proc_params.num_table_rows = ANGLE_TABLE_ROWS;


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
        if(strcmp(token, "output_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->output_file_path, token);
        }
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
        if(strcmp(token, "en_out_img_write")==0)
        {
            token = strtok(NULL, s);
            obj->en_out_img_write = atoi(token);
        }
        else
        if(strcmp(token, "en_out_log_write")==0)
        {
            token = strtok(NULL, s);
            obj->en_out_log_write = atoi(token);
        }
        else
        if(strcmp(token, "viz_th")==0)
        {
            token = strtok(NULL, s);
            obj->viz_th = atof(token);
        }
        else
        if(strcmp(token, "points_per_line")==0)
        {
            token = strtok(NULL, s);
            obj->od_post_proc_params.points_per_line = atoi(token);
        }
        else
        if(strcmp(token, "raw_tidl_op")==0)
        {
            token = strtok(NULL, s);
            obj->raw_tidl_op = atoi(token);
        }
        else
        if(strcmp(token, "label_names")==0)
        {
            for(i = 0; i < APP_MAX_NUM_CLASSES; i++){
              if(token != NULL){
                token = strtok(NULL, s);
                if (token[strlen(token)-1] == '\n')
                {
                  token[strlen(token)-1]=0;
                }
                memcpy(&obj->label_names[i][0],token,APP_MAX_FILE_PATH);
                printf("%s\n",token);
              }
            }
        }
        else
        if(strcmp(token, "ip_rgb_or_yuv")==0)
        {
            token = strtok(NULL, s);
            obj->ip_rgb_or_yuv = atoi(token);
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
        if(strcmp(token, "enable_psd")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            obj->enable_psd = atoi(token);
            if(obj->enable_psd > 1)
                obj->enable_psd = 1;
        }
        else
        if(strcmp(token, "ti_logo_file_path")==0)
        {
            token = strtok(NULL, s);
            token[strlen(token)-1]=0;
            strcpy(obj->ti_logo_file_path, token);
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
    vxSetReferenceName((vx_reference)obj->graph, "Object Detect");

    /* Create array of input tensors */
    createInputTensors(obj->context, obj->config, obj->input_tensors, obj->img_pre_proc_params.pad_pixel);

    /* Create array of output tensors */
    obj->od_post_proc_params.output_buffer_offset=
    createOutputTensors(obj->context, obj->config, obj->output_tensors);

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
    "\n ========================="
    "\n Demo : TIDL Object Detect"
    "\n ========================="
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

#if 0
    status = tivxExportGraphToDot(obj->graph,".", "vx_app_tidl");
    APP_ASSERT(status==VX_SUCCESS);
#endif
    return status;
}

static void app_run_graph_for_one_frame(AppObj *obj, char *curFileName)
{
    vx_char input_file_name[APP_MAX_FILE_PATH];
    vx_char output_file_od_img[APP_MAX_FILE_PATH];
    vx_char log_file_od[APP_MAX_FILE_PATH];
    vx_char log_file_od_bin[APP_MAX_FILE_PATH];
    vx_status status = VX_SUCCESS;
    vx_df_image df;
    vx_map_id map_id;
    vx_char file_ext[8];

    vx_int32 in_img_width;
    vx_int32 in_img_height;

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

    /* Let the file extension of input file come from the names.txt, to get to know clearly whatimage files are running */
    snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%s.%s",
        obj->input_file_path,
        curFileName,
        file_ext
        );
    /* output file format being in the same format as input */
    snprintf(output_file_od_img, APP_MAX_FILE_PATH, "%s/%s.%s",
        obj->output_file_path,
        curFileName,
        file_ext
        );

    snprintf(log_file_od_bin, APP_MAX_FILE_PATH, "%s/%s.bin",
      obj->output_file_path,
      curFileName
      );

    /*Text dump of detection is needed for accuracy measurement from these dumps through external python script*/
    snprintf(log_file_od, APP_MAX_FILE_PATH, "%s/%s.txt",
      obj->output_file_path,
      curFileName
      );

    #ifdef APP_DEBUG
    printf(" Reading [%s] ...\n", input_file_name);
    #endif

    /*Below 3 lines of the code is for making the required directory*/
    //strcpy(output_file_path_local, obj->output_file_path);
    //strcat(output_file_path_local,token);
    //mkdir(output_file_path_local, 0750);

    appPerfPointBegin(&obj->fileio_perf);

    if(obj->ip_rgb_or_yuv == 0){
        tivx_utils_load_vximage_from_bmpfile(obj->in_img, input_file_name, vx_false_e);
    }else if(obj->ip_rgb_or_yuv == 2){
        /* TIAD data set is in PNG hence this flow is needed to measure the accuracy directly on that dataset */
        tivx_utils_load_vximage_from_pngfile(obj->in_img, input_file_name, vx_false_e);
    }else if(obj->ip_rgb_or_yuv == 1){
        loadvxImageYUVNV12(input_file_name,obj->in_img);
    }

    appPerfPointEnd(&obj->fileio_perf);

    tivxCheckStatus(&status, vxQueryImage(obj->in_img, VX_IMAGE_WIDTH, &in_img_width, sizeof(in_img_width)));
    tivxCheckStatus(&status, vxQueryImage(obj->in_img, VX_IMAGE_HEIGHT, &in_img_height, sizeof(in_img_height)));
    tivxCheckStatus(&status, vxQueryImage(obj->in_img, VX_IMAGE_FORMAT, &df, sizeof(df)));

    if((in_img_width != obj->dl_width) || (in_img_height != obj->dl_height)){
        printf("Input image resolution is not configured \n");
    }

    /* Execute the network */
    status = vxProcessGraph(obj->graph);

    /*Get the image pointer*/
    {
        /*Image data pointer after the API "tivx_utils_load_vximage_from_pngfile" is not available
          Hence getting the img pointer from vx_image in below code.
        */
        vx_rectangle_t rect;
        vx_imagepatch_addressing_t image_addr;

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = obj->dl_width;
        rect.end_y = obj->dl_height;

        status = vxMapImagePatch(obj->in_img,
                                 &rect,
                                 0,
                                 &map_id,
                                 &image_addr,
                                 &obj->data_ptr,
                                 VX_READ_ONLY,
                                 VX_MEMORY_TYPE_HOST,
                                 VX_NOGAP_X
                                 );
    }

    appPerfPointBegin(&obj->draw_perf);

    /* Display the output */
    displayOutput(obj,
                obj->config,
                obj->output_tensors,
                log_file_od,
                log_file_od_bin,
                obj->kp_tensor,
                obj->kp_valid);

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

    if (obj->en_out_img_write == 1)
    {
        printf(" Writing [%s] ...\n", output_file_od_img);

        if(obj->ip_rgb_or_yuv == 0){
          status = tivx_utils_bmp_write(output_file_od_img,
                                        obj->data_ptr,
                                        obj->dl_width,
                                        obj->dl_height,
                                        obj->dl_width * 3,
                                        df);
        }else if(obj->ip_rgb_or_yuv == 2){
          status = tivx_utils_png_file_write(output_file_od_img,
                                        obj->dl_width,
                                        obj->dl_height,
                                        obj->dl_width * 3,
                                        df,
                                        obj->data_ptr);
        }else{
          /* YUV file write. Need to place the code to dump the binary YUV data */
          status = VX_FAILURE;
        }
    }

    vxUnmapImagePatch(obj->in_img, map_id);

    appPerfPointEnd(&obj->total_perf);
}

static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_char curFileName[APP_MAX_FILE_PATH];
    uint64_t cur_time;
    FILE* test_case_file;

    if(VX_SUCCESS == status) {
      fillTableTensors(obj);
      int32_t x;

      for(x = 0; x < obj->num_iterations; x++)
      {
        if((x%1) == 0)
          printf("app_tidl: Iteration %d out of %d ...\n", x, obj->num_iterations);

        test_case_file =  fopen(obj->input_file_list,"r");
        if(test_case_file==NULL)
        {
            printf("Could not open test case list file \n");
            break;
        }
        while (fgets(curFileName, sizeof(curFileName), test_case_file)) {

            curFileName[strlen(curFileName) - 1] = 0;

            cur_time = tivxPlatformGetTimeInUsecs();

            app_run_graph_for_one_frame(obj, curFileName);

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
        fclose(test_case_file);

        #ifdef APP_DEBUG
        printf("app_tidl: Iteration %d of %d ... Done.\n", x, obj->num_iterations);
        #endif

        if(obj->stop_task)
            break;

    }// num_iterations
  }
  return status;
}


/*Parameter update which are updated after config file read*/
static void app_update_param_set(AppObj *obj){
    // Post porcessing default parameter

    obj->img_pre_proc_params.ip_rgb_or_yuv = obj->ip_rgb_or_yuv;

    if((obj->ip_rgb_or_yuv == 0) || (obj->ip_rgb_or_yuv == 2)){
      obj->img_pre_proc_params.color_conv_flag = TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB;
    }else{
      obj->img_pre_proc_params.color_conv_flag = TIADALG_COLOR_CONV_YUV420_BGR;
    }

    obj->od_post_proc_params.width = obj->width;
    obj->od_post_proc_params.height = obj->height;
    obj->od_post_proc_params.dl_width = obj->dl_width;
    obj->od_post_proc_params.dl_height = obj->dl_height;

    obj->od_post_proc_params.center_x = obj->width/2;
    obj->od_post_proc_params.center_y = obj->height/2;
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
static void createInputTensors(vx_context context, vx_user_data_object config, vx_tensor *input_tensors, vx_int32* pad_pixel)
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

        input_sizes[0] = ioBufDesc->inWidth[id]  + ioBufDesc->inPadL[id] + ioBufDesc->inPadR[id];
        input_sizes[1] = ioBufDesc->inHeight[id] + ioBufDesc->inPadT[id] + ioBufDesc->inPadB[id];
        input_sizes[2] = ioBufDesc->inNumChannels[id];

        input_tensors[id] = vxCreateTensor(context, 3, input_sizes, VX_TYPE_UINT8, 0);
    }

    pad_pixel[0] = ioBufDesc->inPadL[0];
    pad_pixel[1] = ioBufDesc->inPadT[0];
    pad_pixel[2] = ioBufDesc->inPadR[0];
    pad_pixel[3] = ioBufDesc->inPadB[0];

    vxUnmapUserDataObject(config, map_id_config);

    return;
}

static int32_t createOutputTensors(vx_context context, vx_user_data_object config, vx_tensor *output_tensors)
{
    vx_size output_sizes[APP_MAX_TENSOR_DIMS];
    vx_map_id map_id_config;
    int32_t out_buffer_offset;

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

        output_tensors[id] = vxCreateTensor(context, 3, output_sizes, VX_TYPE_FLOAT32, 0);
    }

  out_buffer_offset = (ioBufDesc->outPadT[0] * output_sizes[0]) + ioBufDesc->outPadL[0];

  vxUnmapUserDataObject(config, map_id_config);

  return out_buffer_offset;
}


static void displayOutput(AppObj *obj, vx_user_data_object config, vx_tensor *output_tensors,
                          vx_char *log_file, vx_char *log_file_bin,
                          vx_tensor kp_tensor, vx_tensor kp_valid)
{
    vx_status status = VX_SUCCESS;

    vx_size output_sizes[APP_MAX_TENSOR_DIMS];

    vx_map_id map_id_output;
    vx_size output_strides[APP_MAX_TENSOR_DIMS];
    vx_size start[APP_MAX_TENSOR_DIMS];

    vx_int16* kp_ptr;
    vx_map_id map_id_kp;

    vx_uint8* kp_valid_ptr;
    vx_map_id map_id_kp_valid;

    vx_map_id map_id_config;

    vx_int32 id, i, j;
    vx_int32 draw_kp = 1;
    vx_int32 dump_log = obj->en_out_log_write;
    FILE* fp = NULL;
    FILE* fpb = NULL;

    vx_int16 startPoint[2];
    vx_int16 endPoint[2];

    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params  *tidlParams;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                      (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;

    start[0] = start[1] = start[2] = start[3] = 0;

    if(obj->od_post_proc_params.num_keypoints > 0){
        output_sizes[0] = obj->od_post_proc_params.num_max_det;
        output_sizes[1] = obj->od_post_proc_params.points_per_line*\
                            obj->od_post_proc_params.num_keypoints*2;
        status = vxGetStatus((vx_reference)kp_tensor);

        output_strides[0] = 1;
        output_strides[1] = obj->od_post_proc_params.points_per_line*\
                            obj->od_post_proc_params.num_keypoints*2;

        if (VX_SUCCESS == status) {
          tivxMapTensorPatch(kp_tensor, 2, start, output_sizes, &map_id_kp, output_strides, (void**)&kp_ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
        }

        status = vxGetStatus((vx_reference)kp_valid);
        if (VX_SUCCESS == status) {
          tivxMapTensorPatch(kp_valid, 1, start, output_sizes, &map_id_kp_valid, output_strides, (void**)&kp_valid_ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
        }
    }

    /*Normally only one output buffer is given out for detected objects from TIDL. */
    for(id = 0; id < ioBufDesc->numOutputBuf; id++)
    {
        output_sizes[0] = ioBufDesc->outWidth[id]  + ioBufDesc->outPadL[id] + ioBufDesc->outPadR[id];
        output_sizes[1] = ioBufDesc->outHeight[id] + ioBufDesc->outPadT[id] + ioBufDesc->outPadB[id];
        output_sizes[2] = ioBufDesc->outNumChannels[id];

        status = vxGetStatus((vx_reference)output_tensors[id]);

        if (VX_SUCCESS == status)
        {
            void *output_buffer;

            output_strides[0] = 1;
            output_strides[1] = output_sizes[0];
            output_strides[2] = output_sizes[1] * output_strides[1];

            tivxMapTensorPatch(output_tensors[id], 3, start, output_sizes, &map_id_output, output_strides, &output_buffer, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

            {
                TIDL_ODLayerHeaderInfo *pHeader;
                TIDL_ODLayerObjInfo *pObjInfo;
                vx_float32*pOut;
                vx_uint32 numObjs;
                TIDL_ODLayerObjInfo * pPSpots;
                vx_int32 total_points_per_box;
                vx_int32 occupied, unoccupied, background;
                char occupied_spots_char[64];
                char unoccupied_spots_char[64];
                char num_spots_char[16];

                pOut = (vx_float32*)output_buffer + (ioBufDesc->outPadT[id] * output_sizes[0]) + ioBufDesc->outPadL[id];

                pHeader  = (TIDL_ODLayerHeaderInfo *)pOut;
                pObjInfo = (TIDL_ODLayerObjInfo *)((uint8_t *)pOut + (vx_uint32)pHeader->objInfoOffset);
                numObjs  = (vx_uint32)pHeader->numDetObjects;

                total_points_per_box = obj->od_post_proc_params.num_keypoints*obj->od_post_proc_params.points_per_line;

                //printf("Number of detected objects are: %d\n", numObjs);

                occupied = 0;
                unoccupied = 0;
                background = 0;
                for (i = 0; i < numObjs; i++)
                {
                  pPSpots = (TIDL_ODLayerObjInfo *) ((uint8_t *)pObjInfo + (i * ((vx_uint32)pHeader->objInfoSize)));
                  if(pPSpots->score >= obj->viz_th) {
                    if(pPSpots->label == 2) occupied++;
                    else if (pPSpots->label == 1) unoccupied++;
                    else background++;
                  }
                }

                if(obj->enable_psd)
                {
                    #ifdef APP_DEBUG
                    printf("Parking spots occupied: %d\n", occupied);
                    printf("Parking spots not-occupied: %d\n", unoccupied);
                    #endif

                    if(obj->display_option == 1)
                    {
                        Draw2D_FontPrm sClassPrm;

                        sClassPrm.fontIdx = 5;

                        Draw2D_clearRegion(obj->pHndl, (DISPLAY_WIDTH/2) + 450, 180, 60, 25);

                        strcpy(occupied_spots_char, "Parking Spots Occupied (Red) : ");
                        itoa(occupied, num_spots_char, 10);
                        strcat(occupied_spots_char, num_spots_char);
                        Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH/2) + 40, 180, occupied_spots_char, &sClassPrm);

                        Draw2D_clearRegion(obj->pHndl, (DISPLAY_WIDTH/2) + 450, 210, 60, 25);

                        strcpy(unoccupied_spots_char, "Parking Spots Unoccupied (Green) : ");
                        itoa(unoccupied, num_spots_char, 10);
                        strcat(unoccupied_spots_char, num_spots_char);
                        Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH/2) + 40, 210, unoccupied_spots_char, &sClassPrm);
                    }
                }
                else
                {
                    #ifdef APP_DEBUG
                    printf("Pedestrians detected: %d\n", occupied);
                    printf("Vehicles detected: %d\n", unoccupied);
                    #endif

                    #if 0
                    if(obj->display_option == 1)
                    {
                        Draw2D_FontPrm sClassPrm;

                        sClassPrm.fontIdx = 5;

                        Draw2D_clearRegion(obj->pHndl, (DISPLAY_WIDTH/2) + 330, 180, 60, 25);

                        strcpy(occupied_spots_char, "Pedestrians Detected : ");
                        itoa(occupied, num_spots_char, 10);
                        strcat(occupied_spots_char, num_spots_char);
                        Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH/2) + 40, 180, occupied_spots_char, &sClassPrm);

                        Draw2D_clearRegion(obj->pHndl, (DISPLAY_WIDTH/2) + 285, 210, 60, 25);

                        strcpy(unoccupied_spots_char, "Vehicles Detected : ");
                        itoa(unoccupied, num_spots_char, 10);
                        strcat(unoccupied_spots_char, num_spots_char);
                        Draw2D_drawString(obj->pHndl, (DISPLAY_WIDTH/2) + 40, 210, unoccupied_spots_char, &sClassPrm);
                    }
                    #endif
                }

                if(dump_log & 0x1){
                  fp = fopen(log_file,"w");
                  printf("Writing the log file %s \n",log_file);
                  if(fp == NULL)
                    printf("could not open the log file %s \n", log_file);
                }

                if(dump_log & 0x2){
                  fpb = fopen(log_file_bin,"wb");
                  if(fpb == NULL)
                    printf("could not open the binary log file %s \n", log_file_bin);

                  if(obj->raw_tidl_op)
                  {
                    fwrite(pHeader, sizeof(TIDL_ODLayerHeaderInfo), 1, fpb);
                  }
                }

                for (i = 0; i < numObjs; i++)
                {
                  pPSpots = (TIDL_ODLayerObjInfo *) ((uint8_t *)pObjInfo + (i * ((vx_uint32)pHeader->objInfoSize)));

                  /*Processing required for each object if raw tidl output needs to be consumed*/
                  if(obj->raw_tidl_op){
                    /*In this scenario, PSD post processing is skipped and TIDL output is directly consumed. Also keypoints
                      are interpolated to facilitate the line drawing.
                    */
                    for(j = 0; j < obj->od_post_proc_params.num_keypoints; j++)
                    {
                      startPoint[0] = pPSpots->keyPoints[j].x*obj->width;
                      startPoint[1] = pPSpots->keyPoints[j].y*obj->height;

                      endPoint[0] = pPSpots->keyPoints[(j + 1) & (obj->od_post_proc_params.num_keypoints - 1)].x*obj->width;
                      endPoint[1] = pPSpots->keyPoints[(j + 1) & (obj->od_post_proc_params.num_keypoints - 1)].y*obj->height;
                      lineInterp(startPoint, endPoint,
                                  &kp_ptr[i*total_points_per_box*2 + j*2*obj->od_post_proc_params.points_per_line],
                                  obj->od_post_proc_params.points_per_line);
                    }
                  }

                  /*For log purpose original key points output from dl network is used*/
                  if( dump_log != 0){

                    if ((dump_log & 0x1) && (fp != NULL))
                    {
                      fprintf(fp, "%s %d %d %d %06.2f %06.2f %06.2f %06.2f %d %d %d %d %d %d %d %05.4f",\
                          obj->label_names[(int)pPSpots->label],0,0,0,
                          pPSpots->xmin*obj->width, pPSpots->ymin*obj->height, pPSpots->xmax*obj->width, pPSpots->ymax*obj->height,
                          0,0,0,0,0,0,0,
                          pPSpots->score);
                    }

                    if(obj->raw_tidl_op){

                      if ((dump_log & 0x2) && (fpb != NULL))
                      {
                        fwrite(pPSpots, sizeof(TIDL_ODLayerObjInfo), 1, fpb);
                      }


                      if ((dump_log & 0x1) && (fp != NULL))
                      {
                          for(j = 0; j < obj->od_post_proc_params.num_keypoints; j++)
                          {
                            fprintf(fp, " %06.2f %06.2f", pPSpots->keyPoints[j].x*obj->width, pPSpots->keyPoints[j].y*obj->height);
                          }
                          fprintf(fp,"\n");
                      }

                    }else{
                        if ((dump_log & 0x1) && (fp != NULL))
                        {
                            for(j = 0; j < obj->od_post_proc_params.num_keypoints; j++)
                            {
                              fprintf(fp, " %06.2f %06.2f",\
                                        (vx_float32)kp_ptr[i*total_points_per_box*2 + j*obj->od_post_proc_params.points_per_line*2],\
                                        (vx_float32)kp_ptr[i*total_points_per_box*2 + j*obj->od_post_proc_params.points_per_line*2 + 1]);
                            }
                            fprintf(fp,"\n");
                        }
                    }
                  }
                  /*Drawing of object for display purpose should be done only when score is high and key point valid flag is 1*/
                  /*For drawing purpose interpolated and post processed key points are used*/
                    if((pPSpots->score >= obj->viz_th) && ((kp_valid_ptr[i] == 1) || obj->raw_tidl_op == 0x1) && (obj->data_ptr != 0x0))
                    {
                        uint32_t do_draw = 1;
                        if(obj->enable_psd==0)
                        {
                            /* VD mode, dont draw pedestrains */
                            if(pPSpots->label==2)
                            {
                                do_draw = 0;
                            }
                        }
                        if(do_draw)
                        {
                            if(draw_kp == 0)
                            {
                                drawBox(obj, pPSpots);
                            }
                            else
                            {
                                drawJoinedPoints(obj,&kp_ptr[i*total_points_per_box*2],total_points_per_box, pPSpots->label);
                            }
                        }
                    }
                }

                if((dump_log & 0x1) && (fp != NULL))
                {
                  fclose(fp);
                }
                if((dump_log & 0x2) && (fpb != NULL))
                {
                  fclose(fpb);
                }
            }

            if(obj->display_option == 1)
            {
                /* Convert RGB565 to RGB888 before writing output */
                for(i = 0, j = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT); i++)
                {
                  uint16_t RGB_565_val = obj->pDisplayBuf565[i];

                  obj->pDisplayBuf888[j + 0] = (RGB_565_val & 0x1F) << 3;
                  obj->pDisplayBuf888[j + 1] = ((RGB_565_val >> 5) & 0x3F) << 2;
                  obj->pDisplayBuf888[j + 2] = ((RGB_565_val >> 11) & 0x1F) << 3;

                  j  += 3;
                }

                if((obj->dl_width <= (DISPLAY_WIDTH/2)) && (obj->dl_height <= (DISPLAY_HEIGHT - 200)))
                {
                     uint32_t startX = ((DISPLAY_WIDTH/2) / 2) - (obj->dl_width / 2);
                     uint32_t startY = ((DISPLAY_HEIGHT/2)  / 2) - (obj->dl_height / 2);
                     uint32_t imgOffset = 250;

                     for(i = 0; i < obj->dl_height; i++)
                     {
                       uint8_t *pOut = &obj->pDisplayBuf888[((imgOffset + startY + i) * DISPLAY_WIDTH * 3) + (startX * 3)];
                       uint8_t *pIn  = obj->data_ptr + (i * obj->dl_width * 3);

                       for(j = 0; j < obj->dl_width; j++)
                       {
                         *pOut++ = *pIn++;
                         *pOut++ = *pIn++;
                         *pOut++ = *pIn++;
                       }
                    }
                }
            }

            tivxUnmapTensorPatch(output_tensors[id], map_id_output);
        }
    }

    vxUnmapUserDataObject(config, map_id_config);
    tivxUnmapTensorPatch(kp_tensor, map_id_kp);
    tivxUnmapTensorPatch(kp_valid, map_id_kp_valid);
}

static void fillTableTensors(AppObj *obj)
{
    vx_size    start[APP_MAX_TENSOR_DIMS];
    vx_size    input_strides[APP_MAX_TENSOR_DIMS];
    vx_size    input_sizes[APP_MAX_TENSOR_DIMS];
    vx_int32 i;
    vx_status status = VX_SUCCESS;

    vx_map_id map_id_fwd_table_tensor;
    vx_map_id map_id_rev_table_tensor;

    vx_float32* fwd_table_tensor_buffer;
    vx_float32* rev_table_tensor_buffer;

    input_sizes[0] = 2;
    input_sizes[1] = ANGLE_TABLE_ROWS;

    start[0] = start[1] = 0;

    input_strides[0] = 1;
    input_strides[1] = input_sizes[0];

    status = tivxMapTensorPatch(obj->fwd_table_tensor, 2, start, input_sizes, &map_id_fwd_table_tensor, input_strides,(void**) &fwd_table_tensor_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    if (status == VX_SUCCESS) {
        for(i = 0; i < ANGLE_TABLE_ROWS; i++){
          fwd_table_tensor_buffer[2*i + 0] = fisheye_angle_table[i][0];
          fwd_table_tensor_buffer[2*i + 1] = fisheye_angle_table[i][1];
        }
        tivxUnmapTensorPatch(obj->fwd_table_tensor, map_id_fwd_table_tensor);
    }

    status = tivxMapTensorPatch(obj->rev_table_tensor, 2, start, input_sizes, &map_id_rev_table_tensor, input_strides,(void**) &rev_table_tensor_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if (status == VX_SUCCESS) {
        for(i = 0; i < ANGLE_TABLE_ROWS; i++){
          rev_table_tensor_buffer[2*i + 0] = fisheye_angle_table_rev[i][0];
          rev_table_tensor_buffer[2*i + 1] = fisheye_angle_table_rev[i][1];
        }
    }
    tivxUnmapTensorPatch(obj->rev_table_tensor, map_id_rev_table_tensor);

}

static vx_status createPreProcNode(AppObj *obj)
{
    vx_enum config_type = VX_TYPE_INVALID;
    vx_status status;

    config_type = vxRegisterUserStruct(obj->context, sizeof(tivxImgPreProcParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);
    obj->img_pre_proc_config = vxCreateArray(obj->context, config_type, 1);
    APP_ASSERT_VALID_REF(obj->img_pre_proc_config);
    vxSetReferenceName((vx_reference)obj->img_pre_proc_config, "ODImgPreProcConfigArray");
    vxAddArrayItems(obj->img_pre_proc_config, 1, &obj->img_pre_proc_params, sizeof(tivxImgPreProcParams));
    APP_ASSERT_VALID_REF(obj->img_pre_proc_config)

    if((obj->ip_rgb_or_yuv == 0) || (obj->ip_rgb_or_yuv == 2)){
      obj->in_img = vxCreateImage(obj->context, obj->dl_width, obj->dl_height, VX_DF_IMAGE_RGB);
    }else{
      obj->in_img = vxCreateImage(obj->context, obj->dl_width, obj->dl_height, VX_DF_IMAGE_NV12);
    }

    APP_ASSERT_VALID_REF(obj->in_img);
    vxSetReferenceName((vx_reference)obj->in_img, "InputImageU8");

    obj->node_img_pre_proc = tivxImgPreProcNode(obj->graph,
                                                obj->img_pre_proc_config,
                                                obj->in_img,
                                                obj->input_tensors[0]);

      APP_ASSERT_VALID_REF(obj->node_img_pre_proc);

      status = vxSetNodeTarget(obj->node_img_pre_proc, VX_TARGET_STRING, TIVX_TARGET_DSP1);

      vxSetReferenceName((vx_reference)obj->node_img_pre_proc, "ODImgPreProcNode");

      return(status);
}
static vx_status createPostProcNode(AppObj *obj)
{
    vx_size temp_size[3];
    vx_enum config_type = VX_TYPE_INVALID;
    vx_status status;

    config_type = vxRegisterUserStruct(obj->context, sizeof(tivxODPostProcParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);
    obj->od_post_proc_config = vxCreateArray(obj->context, config_type, 1);
    APP_ASSERT_VALID_REF(obj->od_post_proc_config);
    vxSetReferenceName((vx_reference)obj->od_post_proc_config, "ODPostProcConfigArray");
    vxAddArrayItems(obj->od_post_proc_config, 1, &obj->od_post_proc_params, sizeof(tivxODPostProcParams));
    APP_ASSERT_VALID_REF(obj->od_post_proc_config)

    temp_size[0] = 2;
    temp_size[1] = ANGLE_TABLE_ROWS;
    obj->fwd_table_tensor = vxCreateTensor(obj->context, 2, temp_size, VX_TYPE_FLOAT32, 0);
    vxSetReferenceName((vx_reference)obj->fwd_table_tensor, "FwdLDCTable");

    temp_size[0] = 2;
    temp_size[1] = ANGLE_TABLE_ROWS;
    obj->rev_table_tensor = vxCreateTensor(obj->context, 2, temp_size, VX_TYPE_FLOAT32, 0);
    vxSetReferenceName((vx_reference)obj->rev_table_tensor, "RevLDCTable");


    temp_size[0] = obj->od_post_proc_params.num_max_det;
    temp_size[1] = obj->od_post_proc_params.points_per_line*\
      obj->od_post_proc_params.num_keypoints * 2;
    obj->kp_tensor = vxCreateTensor(obj->context, 2, temp_size, VX_TYPE_UINT16, 0);
    vxSetReferenceName((vx_reference)obj->kp_tensor, "OutDetectedObjs");

    temp_size[0] = obj->od_post_proc_params.num_max_det;
    obj->kp_valid = vxCreateTensor(obj->context, 1, temp_size, VX_TYPE_UINT8, 0);
    vxSetReferenceName((vx_reference)obj->kp_valid, "OutObjsValidFlag");

    obj->node_od_post_proc = tivxODPostProcNode(obj->graph,
                                                  obj->od_post_proc_config,
                                                  obj->output_tensors[0],
                                                  obj->fwd_table_tensor,
                                                  obj->rev_table_tensor,
                                                  obj->kp_tensor,
                                                  obj->kp_valid);

    APP_ASSERT_VALID_REF(obj->node_od_post_proc);
    status = vxSetNodeTarget(obj->node_od_post_proc, VX_TARGET_STRING, TIVX_TARGET_DSP1);
    vxSetReferenceName((vx_reference)obj->node_od_post_proc, "ODPostProc");

      return(status);
}
static void drawBox(AppObj *obj, TIDL_ODLayerObjInfo * pPSpots)
{
    vx_uint8 *pData;

    vx_uint32 img_width;
    vx_uint32 img_height;
    vx_uint32 img_stride;
    vx_uint32 xmin, xmax, ymin, ymax;
    vx_uint32 start_offset, stride, line_length;
    vx_int32 i;
    vx_uint8 color[3];

    img_width  = obj->dl_width;
    img_height = obj->dl_height;
    img_stride = obj->dl_width*3;

    xmin = (vx_uint32)(pPSpots->xmin * img_width);
    xmax = (vx_uint32)(pPSpots->xmax * img_width);
    ymin = (vx_uint32)(pPSpots->ymin * img_height);
    ymax = (vx_uint32)(pPSpots->ymax * img_height);

    if (xmax >= img_width)  xmax = img_width - 1;
    if (ymax >= img_height) ymax = img_height -1;

    if(pPSpots->label == 1)
    {
      color[0] = 0;
      color[1] = 255;
      color[2] = 0;
    }
    else if(pPSpots->label == 2)
    {
      color[0] = 255;
      color[1] = 0;
      color[2] = 0;
    }
    else
    {
      color[0] = 0;
      color[1] = 0;
      color[2] = 255;
    }

    /* Draw line from (ymin, xmin) to (ymin, xmax) with stride = 1 pixel */
    line_length = xmax - xmin;
    stride  = 3;
    start_offset = (ymin * img_stride) + (xmin * 3);
    pData = (vx_uint8 *)obj->data_ptr + start_offset;

    for(i = 0; i < line_length; i++)
    {
      pData[0] = color[0];
      pData[1] = color[1];
      pData[2] = color[2];

      pData += stride;
    }

    /* Draw line from (ymax, xmin) to (ymax, xmax) with stride = 1 pixel */
    line_length = xmax - xmin;
    stride  = 3;
    start_offset = (ymax * img_stride) + (xmin * 3);
    pData = (vx_uint8 *)obj->data_ptr + start_offset;

    for(i = 0; i < line_length; i++)
    {
      pData[0] = color[0];
      pData[1] = color[1];
      pData[2] = color[2];

      pData += stride;
    }

    /* Draw line from (ymin, xmin) to (ymax, xmin) widt stride = img_stride */
    line_length = ymax - ymin;
    stride  = img_stride;
    start_offset = (ymin * img_stride) + (xmin * 3);
    pData = (vx_uint8 *)obj->data_ptr + start_offset;

    for(i = 0; i < line_length; i++)
    {
      pData[0] = color[0];
      pData[1] = color[1];
      pData[2] = color[2];

      pData += stride;
    }

    /* Draw line from (ymin, xmax) to (ymax, xmax) with stride = img_stride */
    line_length = ymax - ymin;
    stride  = img_stride;
    start_offset = (ymin * img_stride) + (xmax * 3);
    pData = (vx_uint8 *)obj->data_ptr + start_offset;

    for(i = 0; i < line_length; i++)
    {
      pData[0] = color[0];
      pData[1] = color[1];
      pData[2] = color[2];

      pData += stride;
    }
}

static void drawPoints(AppObj *obj, vx_int16* kp_ptr, vx_int32 num_points, vx_int32 label)
{
    vx_uint8 *pData;

    vx_int32 img_width;
    vx_int32 img_height;
    vx_int32 img_stride;
    vx_int32 kp_x,kp_y;
    vx_int32 i,j,k;
    vx_uint8 color[3];
    vx_int32 kp_disp_dim = 2;

    /*Currently key points are generated in original image resolution 1280x720, hence
      to display it back on 512x512 imge it has to be rescaled.
     */
    vx_float32 scale_fact_x = (vx_float32)obj->dl_width/obj->width;
    vx_float32 scale_fact_y = (vx_float32)obj->dl_height/obj->height;

    img_width  = obj->dl_width;
    img_height = obj->dl_height;
    img_stride = obj->dl_width;

    if(label == 1)
    {
      color[0] = 0;
      color[1] = 255;
      color[2] = 0;
    }
    else if(label == 2)
    {
      color[0] = 255;
      color[1] = 0;
      color[2] = 0;
    }
    else
    {
      color[0] = 0;
      color[1] = 0;
      color[2] = 255;
    }

    for(i = 0; i < num_points; i++){

      kp_x = (vx_int32)((vx_float32)kp_ptr[2*i]*scale_fact_x);
      kp_y = (vx_int32)((vx_float32)kp_ptr[2*i + 1]*scale_fact_y);

      if((kp_x >= kp_disp_dim/2) &&
         (kp_x < (img_width - kp_disp_dim/2)) &&
         (kp_y > kp_disp_dim/2) &&
         (kp_y < (img_height - kp_disp_dim/2))){
        pData = (vx_uint8 *)obj->data_ptr + (kp_y * img_stride*3) + (kp_x * 3); // 3 because of RGB
        for(j = -kp_disp_dim/2; j < kp_disp_dim/2; j++){
          for(k = -kp_disp_dim/2; k < kp_disp_dim/2; k++){
            pData[j*img_stride*3 + k*3 + 0] = color[0];
            pData[j*img_stride*3 + k*3 + 1] = color[1];
            pData[j*img_stride*3 + k*3 + 2] = color[2];
          }
        }
      }
    }
}

static void drawJoinedPoints(AppObj *obj, vx_int16* kp_ptr, vx_int32 num_points, vx_int32 label)
{
    vx_uint8 *pData;

    vx_int32 img_width;
    vx_int32 img_height;
    vx_int32 img_stride;
    vx_int32 kp_x,kp_y;
    vx_int32 i,j,k,l;
    vx_uint8 color[3];
    vx_int32 kp_disp_dim = 4;
    vx_int16 cur_kp[2],next_kp[2];
    vx_int16 local_line[1024*2];
    vx_int32  local_line_num_points = 256;

    /*Currently key points are generated in original image resolution 1280x720, hence
      to display it back on 512x512 imge it has to be rescaled.
     */
    vx_float32 scale_fact_x = (vx_float32)obj->dl_width/obj->width;
    vx_float32 scale_fact_y = (vx_float32)obj->dl_height/obj->height;

    img_width  = obj->dl_width;
    img_height = obj->dl_height;
    img_stride = obj->dl_width;

    if(label == 1)
    {
      color[0] = 0;
      color[1] = 255;
      color[2] = 0;
    }
    else if(label == 2)
    {
      color[0] = 255;
      color[1] = 0;
      color[2] = 0;
    }
    else
    {
      color[0] = 0;
      color[1] = 0;
      color[2] = 255;
    }

    for(i = 0; i < num_points; i++){

      cur_kp[0] = (vx_uint16)((vx_float32)kp_ptr[2*i]*scale_fact_x);
      cur_kp[1] = (vx_uint16)((vx_float32)kp_ptr[2*i + 1]*scale_fact_y);

      next_kp[0] = (vx_uint16)((vx_float32)kp_ptr[2*((i+1)%num_points)]*scale_fact_x);
      next_kp[1] = (vx_uint16)((vx_float32)kp_ptr[2*((i+1)%num_points) + 1]*scale_fact_y);

      lineInterp(cur_kp, next_kp, local_line, local_line_num_points);

      for(l = 0; l < local_line_num_points; l++){

        kp_x = local_line[2*l];
        kp_y = local_line[2*l + 1];

        if((kp_x >= kp_disp_dim/2) &&
          (kp_x < (img_width - kp_disp_dim/2)) &&
          (kp_y > kp_disp_dim/2) &&
          (kp_y < (img_height - kp_disp_dim/2))){
          pData = (vx_uint8 *)obj->data_ptr + (kp_y * img_stride*3) + (kp_x * 3); // 3 because of RGB
          for(j = -kp_disp_dim/2; j < kp_disp_dim/2; j++){
            for(k = -kp_disp_dim/2; k < kp_disp_dim/2; k++){
              pData[j*img_stride*3 + k*3 + 0] = color[0];
              pData[j*img_stride*3 + k*3 + 1] = color[1];
              pData[j*img_stride*3 + k*3 + 2] = color[2];
            }
          }
        }
      }
    }
}

static void lineInterp(vx_int16 startPoint[2], vx_int16 endPoint[2], vx_int16 *outPoints, vx_int32 total_points)
{

  int32_t x_0 = startPoint[0];
  int32_t y_0 = startPoint[1];

  int32_t x_1 = endPoint[0];
  int32_t y_1 = endPoint[1];
  int32_t i;
  float y_inc,x_inc,x,y;
  int32_t dx,dy;
  int32_t steps;

  dx = x_1 - x_0;
  dy = y_1 - y_0;

  if(abs(dx) > abs(dy)){
    /*increment in x direction and have all the x axis pixels filled up*/
    steps = abs(dx);
  }else{
    steps = abs(dy);
  }

  if(steps > total_points){
    steps = total_points;
  }

  x_inc = ((float)dx)/(float)(steps);
  y_inc = ((float)dy)/(float)(steps);

  x = x_0;
  y = y_0;

  for(i = 0; i < steps; i++){

    outPoints[2*i + 0] = (uint16_t)x;
    outPoints[2*i + 1] = (uint16_t)y;

    x = x + x_inc;
    y = y + y_inc;
  }

  for(; i < total_points; i++){
    outPoints[2*i + 0] = outPoints[2*(i-1) + 0];
    outPoints[2*i + 1] = outPoints[2*(i-1) + 1];
  }
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
