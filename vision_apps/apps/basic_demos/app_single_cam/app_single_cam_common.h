/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
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
#include <TI/j7_imaging_aewb.h>
#include <tivx_utils_graph_perf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include <iss_sensors.h>
#include <iss_sensor_if.h>

#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/iss/include/app_iss.h>

#define APP_MAX_FILE_PATH           (256u)
#define MAX_FNAME 					  (256u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));
#define MAX_NUM_BUF  8
#define NUM_BUFS 4u

#define LDC_TABLE_WIDTH             (1920)
#define LDC_TABLE_HEIGHT            (1080)
#define LDC_DS_FACTOR               (2)
#define LDC_BLOCK_WIDTH             (64)
#define LDC_BLOCK_HEIGHT            (32)
#define LDC_PIXEL_PAD               (1)

/*Define this macro to enable prints*/
// #define APP_DEBUG_SINGLE_CAM

#ifdef APP_DEBUG_SINGLE_CAM
#define APP_PRINTF(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define APP_PRINTF(f_, ...)     
#endif

typedef struct {

    /* config options */
    uint32_t width_in;
    uint32_t height_in;
    uint32_t width_out;
    uint32_t height_out;
    uint8_t img_format1;
    uint8_t img_format2;
    uint32_t cam_dcc_id;

    vx_node capture_node;
    uint32_t num_cap_buf;
    uint32_t num_viss_out_buf;

    /* OpenVX references */
    vx_context context;
    vx_graph graph;
    vx_node  node_viss;
    vx_node  node_aewb;
    tivx_display_params_t display_params;
    vx_node displayNode;
    vx_enum  ae_awb_result_type;
    tivx_raw_image raw;
    tivx_raw_image fs_test_raw_image;
    vx_image y12;
    vx_image uv12_c1;
    vx_image y8_r8_c2;
    vx_image uv8_g8_c3;
    vx_image s8_b8_c4;
    vx_user_data_object h3a_aew_af;
    vx_user_data_object configuration;
    vx_distribution histogram;
    tivx_vpac_viss_params_t viss_params;
    tivx_aewb_config_t aewb_cfg;
    tivx_ae_awb_params_t ae_awb_params;
    vx_user_data_object ae_awb_result;
    vx_user_data_object aewb_config;
    vx_user_data_object dcc_param_viss;
    vx_user_data_object dcc_param_2a;
    vx_user_data_object dcc_param_ldc;
    vx_user_data_object display_param_obj;

    vx_object_array cap_frames[MAX_NUM_BUF];
    vx_image display_image;
    vx_image viss_out_luma[MAX_NUM_BUF];
    vx_uint8 sensor_sel;
    vx_uint32 num_frames_to_run;
    vx_uint8 sensor_wdr_mode;/*0=Linear, 1=WDR*/
    vx_uint8 selectedCam;/*0-7*/

    uint32_t is_interactive;
    uint32_t test_mode;
    tivx_task task;
    uint32_t stop_task;
    uint32_t stop_task_done;

    uint32_t ldc_enable;
    vx_node node_ldc;
    uint32_t table_width;
    uint32_t table_height;
    uint32_t ds_factor;
    vx_image ldc_out;
    vx_image mesh_img;
    tivx_vpac_ldc_params_t ldc_params;
    vx_user_data_object ldc_param_obj;
    tivx_vpac_ldc_mesh_params_t   mesh_params;
    vx_user_data_object mesh_params_obj;
    tivx_vpac_ldc_region_params_t region_params;
    vx_user_data_object region_params_obj;

    vx_bool scaler_enable;
    vx_image scaler_out_img;
    vx_node scalerNode;
    vx_user_data_object sc_coeff_obj;

    vx_image capt_yuv_image;

    app_perf_point_t total_perf;
    char *sensor_name;
#ifdef x86_64
	char test_folder_root[APP_MAX_FILE_PATH];
    char dcc_path[APP_MAX_FILE_PATH];
	vx_uint16 start_seq;
	vx_uint16 num_frames_to_process;
    vx_uint8 sensor_raw_bpp;

/*DCC read from FS. This will be appended to DCC from the driver*/
    uint8_t* fs_dcc_buf_viss;
    uint32_t fs_dcc_numbytes_viss;
    uint8_t* fs_dcc_buf_2a;
    uint32_t fs_dcc_numbytes_2a;
    uint8_t* fs_dcc_buf_ldc;
    uint32_t fs_dcc_numbytes_ldc;
#endif
#ifdef VPAC3
    uint32_t vpac3_dual_fcp_enable;
#endif
} AppObj;

vx_status app_create_viss(AppObj *obj, uint32_t sensor_wdr_mode);
vx_status app_delete_viss(AppObj *obj);

vx_status app_create_aewb(AppObj *obj, uint32_t sensor_wdr_mode);
vx_status app_delete_aewb(AppObj *obj);

vx_status app_delete_ldc(AppObj *obj);
vx_status app_create_ldc(AppObj *obj, vx_image ldc_in_image);

vx_int32 write_aewb_output(FILE * fp, tivx_ae_awb_params_t * aewb_result);
vx_int32 write_output_image_fp(FILE * fp, vx_image out_image);
vx_int32 write_output_image_yuv422_8bit(char * file_name, vx_image out_yuv);
vx_int32 write_output_image_nv12_8bit(char * file_name, vx_image out_nv12);
vx_int32 read_test_image_raw(char *raw_image_fname, tivx_raw_image raw_image, vx_uint32 test_mode);
vx_int32 write_output_image_raw(char * file_name, tivx_raw_image raw_image);
vx_int32 write_h3a_image(char * file_name, vx_user_data_object out_h3a);
void app_set_cfg_default(AppObj *obj);


