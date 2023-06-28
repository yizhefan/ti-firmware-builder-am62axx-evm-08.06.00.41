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

#include <utils/draw2d/include/draw2d.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/console_io/include/app_get.h>
#include <utils/grpx/include/app_grpx.h>
#include <VX/vx_khr_pipelining.h>

#include "app_common.h"
#include "app_sensor_module.h"
#include "app_capture_module.h"
#include "app_viss_module.h"
#include "app_aewb_module.h"
#include "app_ldc_module.h"
#include "app_scaler_module.h"
#include "app_pre_proc_module.h"
#include "app_tidl_module.h"
#include "app_post_proc_module.h"
#include "app_img_mosaic_module.h"
#include "app_display_module.h"
#include "app_test.h"

#define APP_BUFFER_Q_DEPTH   (4)
#define APP_PIPELINE_DEPTH   (7)

typedef struct {

    SensorObj     sensorObj;
    CaptureObj    captureObj;
    VISSObj       vissObj;
    AEWBObj       aewbObj;
    LDCObj        ldcObj;
    ScalerObj     scalerObj;
    PreProcObj    preProcObj;
    TIDLObj       tidlObj;
    PostProcObj   postProcObj;
    ImgMosaicObj  imgMosaicObj;
    DisplayObj    displayObj;

    vx_char input_file_path[APP_MAX_FILE_PATH];
    vx_char output_file_path[APP_MAX_FILE_PATH];

    /* OpenVX references */
    vx_context context;
    vx_graph   graph;

    vx_uint32 is_interactive;
    vx_uint32 test_mode;

    vx_uint32 num_frames_to_run;

    vx_uint32 num_frames_to_write;
    vx_uint32 num_frames_to_skip;

    tivx_task task;
    vx_uint32 stop_task;
    vx_uint32 stop_task_done;

    app_perf_point_t total_perf;
    app_perf_point_t fileio_perf;
    app_perf_point_t draw_perf;

    int32_t pipeline;

    int32_t enqueueCnt;
    int32_t dequeueCnt;

    int32_t write_file;

} AppObj;

AppObj gAppObj;
vx_uint8 g_update_result;
vx_uint8 g_num_top_results;
vx_uint32 g_classid[TIVX_OC_MAX_CLASSES];

extern const char imgnet_labels[1001][256];

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[]);
static vx_status app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static vx_status app_create_graph(AppObj *obj);
static vx_status app_verify_graph(AppObj *obj);
static vx_status app_run_graph(AppObj *obj);
static vx_status app_run_graph_interactive(AppObj *obj);
static void app_delete_graph(AppObj *obj);
static void app_default_param_set(AppObj *obj);
static void app_update_param_set(AppObj *obj);
static void app_pipeline_params_defaults(AppObj *obj);
static void add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index);
static void set_img_mosaic_params(AppObj *obj, ImgMosaicObj *imgMosaicObj);
#ifndef x86_64
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type);
#endif

static void app_show_usage(vx_int32 argc, vx_char* argv[])
{
    printf("\n");
    printf(" TIDL Camera Demo - (c) Texas Instruments 2020\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static char menu[] = {
    "\n"
    "\n ========================="
    "\n Demo : TIDL Camera Demo"
    "\n ========================="
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
    "\n"
    "\n s: Save intermediate outputs"
#endif
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};

static void app_run_task(void *app_var)
{
    AppObj *obj = (AppObj *)app_var;
    vx_status status = VX_SUCCESS;
    while(!obj->stop_task && (status == VX_SUCCESS))
    {
        status = app_run_graph(obj);
    }
    obj->stop_task_done = 1;
}

static vx_status app_run_task_create(AppObj *obj)
{
    tivx_task_create_params_t params;
    vx_status status;

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

static vx_status app_run_graph_interactive(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    uint32_t done = 0;

    char ch;
    FILE *fp;
    app_perf_point_t *perf_arr[1];

    status = app_run_task_create(obj);
    if(status != VX_SUCCESS)
    {
        printf("app_tidl: ERROR: Unable to create task\n");
    }
    else
    {
        appPerfStatsResetAll();
        while(!done && (status == VX_SUCCESS))
        {
            printf(menu);
            ch = getchar();
            printf("\n");

            switch(ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    status = tivx_utils_graph_perf_print(obj->graph);
                    appPerfPointPrint(&obj->fileio_perf);
                    appPerfPointPrint(&obj->total_perf);
                    printf("\n");
                    appPerfPointPrintFPS(&obj->total_perf);
                    appPerfPointReset(&obj->total_perf);
                    printf("\n");
                    break;
                case 'e':
                    perf_arr[0] = &obj->total_perf;
                    fp = appPerfStatsExportOpenFile(".", "dl_demos_app_tidl_cam");
                    if (NULL != fp)
                    {
                        appPerfStatsExportAll(fp, perf_arr, 1);
                        status = tivx_utils_graph_perf_export(fp, obj->graph);
                        appPerfStatsExportCloseFile(fp);
                        appPerfStatsResetAll();
                    }
                    else
                    {
                        printf("fp is null\n");
                    }
                    break;
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
                case 's':
                    obj->write_file = 1;
                    break;
#endif
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

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->captureObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->vissObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->ldcObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->scalerObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->preProcObj.output_file_path,APP_MAX_FILE_PATH, ".");

    obj->captureObj.en_out_capture_write = 0;
    obj->vissObj.en_out_viss_write = 0;
    obj->ldcObj.en_out_ldc_write = 0;
    obj->scalerObj.en_out_scaler_write = 0;
    obj->preProcObj.en_out_pre_proc_write = 0;

    obj->num_frames_to_write = 0;
    obj->num_frames_to_skip = 0;

}

static void app_parse_cfg_file(AppObj *obj, vx_char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    vx_char line_str[1024];
    vx_char *token;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(-1);
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
        if(token != NULL)
        {
            if(strcmp(token, "sensor_index")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->sensorObj.sensor_index = atoi(token);
                }
            }
            else
            if(strcmp(token, "is_interactive")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->is_interactive = atoi(token);
                    if(obj->is_interactive > 1)
                    obj->is_interactive = 1;
                }
                obj->sensorObj.is_interactive = obj->is_interactive;
            }
            else
            if(strcmp(token, "tidl_config")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->tidlObj.config_file_path, token);
                }
            }
            else
            if(strcmp(token, "tidl_network")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->tidlObj.network_file_path, token);
                }
            }
            else
            if(strcmp(token, "num_top_results")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->postProcObj.params.num_top_results = atoi(token);
                }
                g_num_top_results = obj->postProcObj.params.num_top_results;
                printf("g_num_top_results %d\n", g_num_top_results);
            }
            else
            if(strcmp(token, "test_mode")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->test_mode = atoi(token);
                    obj->captureObj.test_mode = atoi(token);
                }
            }
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
            else
            if(strcmp(token, "num_frames_to_run")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_frames_to_run = atoi(token);
                }
            }
            else
            if(strcmp(token, "en_out_capture_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->captureObj.en_out_capture_write = atoi(token);

                    if(obj->captureObj.en_out_capture_write > 1)
                        obj->captureObj.en_out_capture_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_viss_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->vissObj.en_out_viss_write = atoi(token);

                    if(obj->vissObj.en_out_viss_write > 1)
                        obj->vissObj.en_out_viss_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_ldc_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->ldcObj.en_out_ldc_write = atoi(token);

                    if(obj->ldcObj.en_out_ldc_write > 1)
                        obj->ldcObj.en_out_ldc_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_scaler_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->scalerObj.en_out_scaler_write = atoi(token);

                    if(obj->scalerObj.en_out_scaler_write > 1)
                        obj->scalerObj.en_out_scaler_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_pre_proc_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->preProcObj.en_out_pre_proc_write = atoi(token);

                    if(obj->preProcObj.en_out_pre_proc_write > 1)
                        obj->preProcObj.en_out_pre_proc_write = 1;
                }
            }
            else
            if(strcmp(token, "output_file_path")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->captureObj.output_file_path, token);
                    strcpy(obj->vissObj.output_file_path, token);
                    strcpy(obj->ldcObj.output_file_path, token);
                    strcpy(obj->scalerObj.output_file_path, token);
                    strcpy(obj->preProcObj.output_file_path, token);
                }
            }
            else
            if(strcmp(token, "num_frames_to_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_frames_to_write = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_frames_to_skip")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_frames_to_skip = atoi(token);
                }
            }
#endif
        }
        if (obj->test_mode == 1)
        {
            obj->is_interactive = 0;
            obj->sensorObj.sensor_index = 0;
            obj->num_frames_to_run = TEST_BUFFER + (sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0]));
        }
    }

    fclose(fp);

}

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[])
{
    vx_int32 i;
    vx_bool set_test_mode = vx_false_e;

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
        }
        else
        if(strcmp(argv[i], "--help")==0)
        {
            app_show_usage(argc, argv);
            exit(0);
        }
        else
        if(strcmp(argv[i], "--test")==0)
        {
            set_test_mode = vx_true_e;
        }
    }

    if (set_test_mode == vx_true_e)
    {
        obj->test_mode = 1;
        obj->captureObj.test_mode = 1;
        obj->is_interactive = 0;
        obj->sensorObj.is_interactive = 0;
        obj->sensorObj.sensor_index = 0;
        obj->num_frames_to_run = TEST_BUFFER + (sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0]));
    }

    return;
}

vx_status app_tidl_cam_main(vx_int32 argc, vx_char* argv[])
{
    vx_status status = VX_SUCCESS;

    AppObj *obj = &gAppObj;

    /*Optional parameter setting*/
    app_default_param_set(obj);

    /*Config parameter reading*/
    app_parse_cmd_line_args(obj, argc, argv);

    /* Querry sensor parameters */
    status = app_querry_sensor(&obj->sensorObj);

    /*Update of parameters are config file read*/
    app_update_param_set(obj);

    if(status == VX_SUCCESS)
    {
        status = app_init(obj);
    }
    if(status == VX_SUCCESS)
    {
        APP_PRINTF("App Init Done! \n");

        status = app_create_graph(obj);

        if(status == VX_SUCCESS)
        {
            APP_PRINTF("App Create Graph Done! \n");

            status = app_verify_graph(obj);

            if(status == VX_SUCCESS)
            {
                APP_PRINTF("App Verify Graph Done! \n");

                if(obj->is_interactive)
                {
                    status = app_run_graph_interactive(obj);
                }
                else
                {
                    status = app_run_graph(obj);
                }
            }
        }
        APP_PRINTF("App Run Graph Done! \n");
    }

    app_delete_graph(obj);

    APP_PRINTF("App Delete Graph Done! \n");

    app_deinit(obj);

    APP_PRINTF("App De-init Done! \n");
    if(obj->test_mode == 1)
    {
        if((vx_false_e == test_result) || (status != VX_SUCCESS))
        {
            printf("\n\nTEST FAILED\n\n");
            print_new_checksum_structs();
            status = (status == VX_SUCCESS) ? VX_FAILURE : status;
        }
        else
        {
            printf("\n\nTEST PASSED\n\n");
        }
    }
    return status;
}

static vx_status app_init(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    app_grpx_init_prms_t grpx_prms;

    /* Create OpenVx Context */
    obj->context = vxCreateContext();
    if(status == VX_SUCCESS)
    {
        status = vxGetStatus((vx_reference) obj->context);
    }
    APP_PRINTF("Creating context done!\n");

    if(status == VX_SUCCESS)
    {
        tivxHwaLoadKernels(obj->context);
        tivxImagingLoadKernels(obj->context);
        tivxImgProcLoadKernels(obj->context);
        tivxTIDLLoadKernels(obj->context);
        tivxFileIOLoadKernels(obj->context);
        APP_PRINTF("Kernel loading done!\n");
    }
    /* Initialize modules */
    if(status == VX_SUCCESS)
    {
        app_init_sensor(&obj->sensorObj, "sensor_obj");
        APP_PRINTF("Sensor init done!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_capture(obj->context, &obj->captureObj, &obj->sensorObj, "capture_obj", APP_BUFFER_Q_DEPTH);
    }
    if (status != VX_SUCCESS)
    {
        printf("Intializing test frame failed\n");
    }
    APP_PRINTF("Capture init done!\n");

    if(status == VX_SUCCESS)
    {
        status = app_init_viss(obj->context, &obj->vissObj, &obj->sensorObj, "viss_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("VISS init done!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_aewb(obj->context, &obj->aewbObj, &obj->sensorObj, "aewb_obj", 0, obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("AEWB init done!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_ldc(obj->context, &obj->ldcObj, &obj->sensorObj, "ldc_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("LDC init done!\n");
    }

    obj->scalerObj.output[0].width  = obj->ldcObj.table_width / 2;
    obj->scalerObj.output[0].height = obj->ldcObj.table_height / 2;

    obj->scalerObj.output[1].width  = obj->ldcObj.table_width / 4;
    obj->scalerObj.output[1].height = obj->ldcObj.table_height / 4;

    printf("Scaler output1 width   = %d\n", obj->scalerObj.output[0].width);
    printf("Scaler output1 height  = %d\n", obj->scalerObj.output[0].height);
    printf("Scaler output2 width   = %d\n", obj->scalerObj.output[1].width);
    printf("Scaler output2 height  = %d\n", obj->scalerObj.output[1].height);

    if(status == VX_SUCCESS)
    {
        status = app_init_scaler(obj->context, &obj->scalerObj, "scaler_obj", obj->sensorObj.num_cameras_enabled, 2);
        APP_PRINTF("Scaler init done!\n");
    }

    /* Initialize TIDL first to get tensor I/O information from network */
    if(status == VX_SUCCESS)
    {
        status = app_init_tidl(obj->context, &obj->tidlObj, "tidl_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("TIDL Init Done! \n");
    }

    /* Update pre-proc parameters with TIDL config before calling init */
    if(status == VX_SUCCESS)
    {
        status = app_update_pre_proc(obj->context, &obj->preProcObj, obj->tidlObj.config, obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("Pre Proc Update Done! \n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_pre_proc(obj->context, &obj->preProcObj, "pre_proc_obj");
        APP_PRINTF("Pre Proc Init Done! \n");
    }

    /* Update post-proc parameters with TIDL config before calling init */
    if(status == VX_SUCCESS)
    {
        status = app_update_post_proc(obj->context, &obj->postProcObj, obj->tidlObj.config);
        APP_PRINTF("Post Proc Update Done! \n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_post_proc(obj->context, &obj->postProcObj, "post_proc_obj", obj->sensorObj.num_cameras_enabled, APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Post Proc Init Done! \n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_img_mosaic(obj->context, &obj->imgMosaicObj, "img_mosaic_obj", APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Img Mosaic init done!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_display(obj->context, &obj->displayObj, "display_obj");
        APP_PRINTF("Display init done!\n");
    }
    #ifndef x86_64
    if(obj->displayObj.display_option == 1)
    {
        appGrpxInitParamsInit(&grpx_prms, obj->context);
        grpx_prms.draw_callback = app_draw_graphics;
        appGrpxInit(&grpx_prms);
    }
    #endif

    appPerfPointSetName(&obj->total_perf , "TOTAL");
    appPerfPointSetName(&obj->fileio_perf, "FILEIO");

    return status;
}

static void app_deinit(AppObj *obj)
{
    app_deinit_sensor(&obj->sensorObj);
    APP_PRINTF("Sensor deinit done!\n");

    app_deinit_capture(&obj->captureObj, APP_BUFFER_Q_DEPTH);
    APP_PRINTF("Capture deinit done!\n");

    app_deinit_viss(&obj->vissObj);
    APP_PRINTF("VISS deinit done!\n");

    app_deinit_aewb(&obj->aewbObj);
    APP_PRINTF("AEWB deinit done!\n");

    app_deinit_ldc(&obj->ldcObj);
    APP_PRINTF("LDC deinit done!\n");

    app_deinit_scaler(&obj->scalerObj);
    APP_PRINTF("Scaler deinit done!\n");

    app_deinit_pre_proc(&obj->preProcObj);
    APP_PRINTF("Pre proc deinit done!\n");

    app_deinit_tidl(&obj->tidlObj);
    APP_PRINTF("TIDL deinit done!\n");

    app_deinit_post_proc(&obj->postProcObj, APP_BUFFER_Q_DEPTH);
    APP_PRINTF("Post proc deinit done!\n");

    app_deinit_img_mosaic(&obj->imgMosaicObj, APP_BUFFER_Q_DEPTH);
    APP_PRINTF("Img Mosaic deinit done!\n");

    app_deinit_display(&obj->displayObj);
    APP_PRINTF("Display deinit done!\n");

    #ifndef x86_64
    if(obj->displayObj.display_option == 1)
    {
        appGrpxDeInit();
    }
    #endif

    tivxTIDLUnLoadKernels(obj->context);
    tivxHwaUnLoadKernels(obj->context);
    tivxImagingUnLoadKernels(obj->context);
    tivxImgProcUnLoadKernels(obj->context);
    tivxFileIOUnLoadKernels(obj->context);
    APP_PRINTF("Kernels unload done!\n");

    vxReleaseContext(&obj->context);
    APP_PRINTF("Release context done!\n");
}

static void app_delete_graph(AppObj *obj)
{
    app_delete_capture(&obj->captureObj);
    APP_PRINTF("Capture delete done!\n");

    app_delete_viss(&obj->vissObj);
    APP_PRINTF("VISS delete done!\n");

    app_delete_aewb(&obj->aewbObj);
    APP_PRINTF("AEWB delete done!\n");

    app_delete_ldc(&obj->ldcObj);
    APP_PRINTF("LDC delete done!\n");

    app_delete_scaler(&obj->scalerObj);
    APP_PRINTF("Scaler delete done!\n");

    app_delete_pre_proc(&obj->preProcObj);
    APP_PRINTF("Pre Proc delete done!\n");

    app_delete_tidl(&obj->tidlObj);
    APP_PRINTF("TIDL delete done!\n");

    app_delete_post_proc(&obj->postProcObj);
    APP_PRINTF("Post Proc delete done!\n");

    app_delete_img_mosaic(&obj->imgMosaicObj);
    APP_PRINTF("Img Mosaic delete done!\n");

    app_delete_display(&obj->displayObj);
    APP_PRINTF("Display delete done!\n");

    vxReleaseGraph(&obj->graph);
    APP_PRINTF("Graph delete done!\n");
}

static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_uint32 params_list_depth = 2;
    if(obj->test_mode == 1)
    {
        params_list_depth = 3;
    }
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[params_list_depth];
    vx_int32 graph_parameter_index;

    obj->graph = vxCreateGraph(obj->context);
    status = vxGetStatus((vx_reference)obj->graph);
    vxSetReferenceName((vx_reference)obj->graph, "app_tidl_cam_graph");
    APP_PRINTF("Graph create done!\n");

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_capture(obj->graph, &obj->captureObj);
        APP_PRINTF("Capture graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_viss(obj->graph, &obj->vissObj, obj->captureObj.raw_image_arr[0], TIVX_TARGET_VPAC_VISS1);
        APP_PRINTF("VISS graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_aewb(obj->graph, &obj->aewbObj, obj->vissObj.h3a_stats_arr);
        APP_PRINTF("AEWB graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_ldc(obj->graph, &obj->ldcObj, obj->vissObj.output_arr, TIVX_TARGET_VPAC_LDC1);
        APP_PRINTF("LDC graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_scaler(obj->context, obj->graph, &obj->scalerObj, obj->ldcObj.output_arr);
        APP_PRINTF("Scaler graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_pre_proc(obj->graph, &obj->preProcObj, obj->scalerObj.output[1].arr);
        APP_PRINTF("Pre proc graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_tidl(obj->context, obj->graph, &obj->tidlObj, obj->preProcObj.output_tensor_arr);
        APP_PRINTF("TIDL graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_post_proc(obj->graph, &obj->postProcObj, obj->tidlObj.out_args_arr, obj->tidlObj.output_tensor_arr[0]);
        APP_PRINTF("Post proc graph done!\n");
    }

    vx_int32 idx = 0;
    /* For 2MP resolutions provide scaler node output1 to mosaic */
    obj->imgMosaicObj.input_arr[idx++] = obj->scalerObj.output[0].arr;
    obj->imgMosaicObj.num_inputs = idx;

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_img_mosaic(obj->graph, &obj->imgMosaicObj, NULL);
        APP_PRINTF("Img Mosaic graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_display(obj->graph, &obj->displayObj, obj->imgMosaicObj.output_image[0]);
        APP_PRINTF("Display graph done!\n");
    }

    if(status == VX_SUCCESS)
    {
        graph_parameter_index = 0;
        add_graph_parameter_by_node_index(obj->graph, obj->captureObj.node, 1);
        obj->captureObj.graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->captureObj.raw_image_arr[0];
        graph_parameter_index++;

        add_graph_parameter_by_node_index(obj->graph, obj->postProcObj.node, 3);
        obj->postProcObj.graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->postProcObj.results[0];
        graph_parameter_index++;

        if(obj->test_mode == 1)
        {
            add_graph_parameter_by_node_index(obj->graph, obj->imgMosaicObj.node, 1);
            obj->imgMosaicObj.graph_parameter_index = graph_parameter_index;
            graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
            graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
            graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->imgMosaicObj.output_image[0];
            graph_parameter_index++;
        }

        if(status == VX_SUCCESS)
        {
            status = vxSetGraphScheduleConfig(obj->graph,
                                VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                                graph_parameter_index,
                                graph_parameters_queue_params_list);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetGraphPipelineDepth(obj->graph, APP_PIPELINE_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->vissObj.node, 6, APP_BUFFER_Q_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->vissObj.node, 9, APP_BUFFER_Q_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->aewbObj.node, 4, APP_BUFFER_Q_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->ldcObj.node, 7, APP_BUFFER_Q_DEPTH);
        }

        /*This output is accessed slightly later in the pipeline by mosaic node so queue depth is larger */
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->scalerObj.node, 1, 6);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->scalerObj.node, 2, 6);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->preProcObj.node, 2, APP_BUFFER_Q_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->tidlObj.node, 4, APP_BUFFER_Q_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->tidlObj.node, 7, APP_BUFFER_Q_DEPTH);
        }
        if(status == VX_SUCCESS)
        {
            if(!(obj->test_mode == 1))
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->imgMosaicObj.node, 1, 4);
            }
        }
        APP_PRINTF("Pipeline params setup done!\n");
    }

    return status;
}

static vx_status app_verify_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    status = vxVerifyGraph(obj->graph);

    if(status == VX_SUCCESS)
    {
        APP_PRINTF("Grapy verify done!\n");
    }

    #if 1
    if(VX_SUCCESS == status)
    {
        status = tivxExportGraphToDot(obj->graph,".", "vx_app_tidl_cam");
    }
    #endif

    if ((obj->captureObj.enable_error_detection || obj->test_mode) && (status == VX_SUCCESS))
    {
        status = app_send_error_frame(&obj->captureObj);
        APP_PRINTF("App Send Error Frame Done! %d \n", obj->captureObj.enable_error_detection);
    }

    /* wait a while for prints to flush */
    tivxTaskWaitMsecs(100);

    return status;
}

static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 frame_id)
{
    vx_status status = VX_SUCCESS;

    appPerfPointBegin(&obj->total_perf);

    CaptureObj *captureObj = &obj->captureObj;
    PostProcObj *postProcObj = &obj->postProcObj;
    ImgMosaicObj *imgMosaicObj = &obj->imgMosaicObj;

    if(obj->pipeline < 0)
    {
        /* Enqueue inputs during pipeup dont execute */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, postProcObj->graph_parameter_index, (vx_reference*)&postProcObj->results[obj->enqueueCnt], 1);
        }
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&captureObj->raw_image_arr[obj->enqueueCnt], 1);
        }
        if((status == VX_SUCCESS) && (obj->test_mode))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&imgMosaicObj->output_image[obj->enqueueCnt], 1);
        }
        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if(obj->pipeline == 0)
    {
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, postProcObj->graph_parameter_index, (vx_reference*)&postProcObj->results[obj->enqueueCnt], 1);
        }
        /* Execute 1st frame */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&captureObj->raw_image_arr[obj->enqueueCnt], 1);
        }
        if((status == VX_SUCCESS) && (obj->test_mode))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&imgMosaicObj->output_image[obj->enqueueCnt], 1);
        }
        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if(obj->pipeline > 0)
    {
        vx_image capture_input_image;
        vx_user_data_object results;
        uint32_t num_refs;
        vx_image test_output;

        /* Dequeue input */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&capture_input_image, 1, &num_refs);
        }
        /* Dequeue output */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, postProcObj->graph_parameter_index, (vx_reference*)&results, 1, &num_refs);
        }
        vx_map_id map_id_results;
        tivxOCPostProcOutput *pResults;

        if(status == VX_SUCCESS)
        {
            status = vxMapUserDataObject(results, 0, sizeof(tivxOCPostProcOutput), &map_id_results,
                                    (void **)&pResults, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);
        }

        if(status == VX_SUCCESS)
        {
            if(pResults != NULL)
            {
                vx_int32 i;
                for(i = 0; i < pResults->num_top_results; i++)
                {
                    g_classid[i] = pResults->class_id[i];
                }
                g_update_result = 1;
            }
        }
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&test_output, 1, &num_refs);
        }
        vx_uint32 expected_idx = frame_id - TEST_BUFFER;
        if((status == VX_SUCCESS) && (obj->test_mode == 1)
            && (expected_idx < (sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0])))
            && (expected_idx >= 0))
        {
            /* checksum_actual is the checksum determined by the realtime test
                checksum_expected is the checksum that is expected to be the pipeline output */
            uint32_t img_checksum_actual = 0, results_checksum_actual = 0;


            // results_checksum_actual = tivx_utils_user_data_object_checksum(results, 1, sizeof(tivxOCPostProcOutput));
            if(app_test_check_image(test_output, checksums_expected[0][expected_idx],
                &img_checksum_actual) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            if(app_test_check_object(results, sizeof(tivxOCPostProcOutput),
                checksums_expected[1][expected_idx], &results_checksum_actual)
                == vx_false_e)
            {
                test_result = vx_false_e;
            }
            /* in case test fails and needs to change */
            populate_gatherer(0, expected_idx, img_checksum_actual);
            populate_gatherer(1, expected_idx, results_checksum_actual);
        }
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&test_output, 1);
        }
        if(status == VX_SUCCESS)
        {
            vxUnmapUserDataObject(results, map_id_results);
        }

        /* Enqueue output */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, postProcObj->graph_parameter_index, (vx_reference*)&results, 1);
        }

        /* Enqueue input - start execution */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&capture_input_image, 1);
        }
        obj->enqueueCnt++;
        obj->dequeueCnt++;

        obj->enqueueCnt = (obj->enqueueCnt >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->dequeueCnt = (obj->dequeueCnt >= APP_BUFFER_Q_DEPTH)? 0 : obj->dequeueCnt;

    }

    appPerfPointEnd(&obj->total_perf);

    return status;
}

static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    SensorObj *sensorObj = &obj->sensorObj;
    vx_int32 frame_id;
    int32_t ch_mask = obj->sensorObj.ch_mask;

    app_pipeline_params_defaults(obj);

    if(NULL == sensorObj->sensor_name)
    {
        printf("sensor name is NULL \n");
        return VX_FAILURE;
    }
    status = appStartImageSensor(sensorObj->sensor_name, ch_mask);
    APP_PRINTF("appStartImageSensor returned with status: %d\n", status);

    for(frame_id = 0; frame_id < obj->num_frames_to_run; frame_id++)
    {
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
        if(obj->write_file == 1)
        {
            if(obj->captureObj.en_out_capture_write == 1)
            {
                app_send_cmd_capture_write_node(&obj->captureObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->vissObj.en_out_viss_write == 1)
            {
                app_send_cmd_viss_write_node(&obj->vissObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->ldcObj.en_out_ldc_write == 1)
            {
                app_send_cmd_ldc_write_node(&obj->ldcObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->scalerObj.en_out_scaler_write == 1)
            {
                app_send_cmd_scaler_write_node(&obj->scalerObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->preProcObj.en_out_pre_proc_write == 1)
            {
                app_send_cmd_pre_proc_write_node(&obj->preProcObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            obj->write_file = 0;
        }
#endif
        app_run_graph_for_one_frame_pipeline(obj, frame_id);

        /* user asked to stop processing */
        if(obj->stop_task)
            break;
    }

    vxWaitGraph(obj->graph);

    obj->stop_task = 1;

    status = appStopImageSensor(obj->sensorObj.sensor_name, ch_mask);

    return status;
}

static void set_scaler_defaults(ScalerObj *scalerObj)
{
    scalerObj->color_format = VX_DF_IMAGE_NV12;
}

static void set_display_defaults(DisplayObj *displayObj)
{
    displayObj->display_option = 1;
}

static void app_pipeline_params_defaults(AppObj *obj)
{
    obj->pipeline       = -APP_BUFFER_Q_DEPTH + 1;
    obj->enqueueCnt     = 0;
    obj->dequeueCnt     = 0;
}

static void set_sensor_defaults(SensorObj *sensorObj)
{
    strcpy(sensorObj->sensor_name, SENSOR_SONY_IMX390_UB953_D3);

    sensorObj->num_sensors_found = 0;
    sensorObj->sensor_features_enabled = 0;
    sensorObj->sensor_features_supported = 0;
    sensorObj->sensor_dcc_enabled = 0;
    sensorObj->sensor_wdr_enabled = 0;
    sensorObj->sensor_exp_control_enabled = 0;
    sensorObj->sensor_gain_control_enabled = 0;
    sensorObj->ch_mask = 1;
    sensorObj->enable_ldc = 1;
    sensorObj->num_cameras_enabled = 1;
    sensorObj->usecase_option = APP_SENSOR_FEATURE_CFG_UC0;
    sensorObj->is_interactive = 1;

}

static void app_default_param_set(AppObj *obj)
{
    set_sensor_defaults(&obj->sensorObj);

    set_scaler_defaults(&obj->scalerObj);

    set_display_defaults(&obj->displayObj);

    app_pipeline_params_defaults(obj);

    obj->captureObj.enable_error_detection = 1; /* enable by default */
    obj->is_interactive = 1;
    obj->test_mode = 0;
    obj->write_file = 0;
    obj->num_frames_to_run = 1000000000;

}

static void set_img_mosaic_params(AppObj *obj, ImgMosaicObj *imgMosaicObj)
{
    vx_int32 idx;

    imgMosaicObj->out_width    = DISPLAY_WIDTH;
    imgMosaicObj->out_height   = DISPLAY_HEIGHT;
    imgMosaicObj->num_inputs   = 1;

    tivxImgMosaicParamsSetDefaults(&imgMosaicObj->params);

    idx = 0;

    imgMosaicObj->params.windows[idx].startX  = 100;
    imgMosaicObj->params.windows[idx].startY  = 200;
    imgMosaicObj->params.windows[idx].width   = obj->scalerObj.output[0].width;
    imgMosaicObj->params.windows[idx].height  = obj->scalerObj.output[0].height;
    imgMosaicObj->params.windows[idx].input_select   = 0;
    imgMosaicObj->params.windows[idx].channel_select = 0;
    idx++;

    imgMosaicObj->params.num_windows  = idx;

    /* Number of time to clear the output buffer before it gets reused */
    imgMosaicObj->params.clear_count  = APP_BUFFER_Q_DEPTH;
}

static void app_update_param_set(AppObj *obj)
{
    obj->sensorObj.sensor_index = 0; /* App works only for IMX390 2MP cameras */

    obj->scalerObj.output[0].width  = obj->sensorObj.image_width / 2;
    obj->scalerObj.output[0].height = obj->sensorObj.image_height / 2;
    obj->scalerObj.output[1].width  = obj->sensorObj.image_width / 4;
    obj->scalerObj.output[1].height = obj->sensorObj.image_height / 4;

    printf("Sensor width   = %d\n", obj->sensorObj.image_width);
    printf("Sensor height  = %d\n", obj->sensorObj.image_height);

    obj->postProcObj.params.num_top_results = 5;

    set_img_mosaic_params(obj, &obj->imgMosaicObj);
}

/*
 * Utility API used to add a graph parameter from a node, node parameter index
 */
static void add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index)
{
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);

    vxAddParameterToGraph(graph, parameter);
    vxReleaseParameter(&parameter);
}

#ifndef x86_64
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    if(update_type == 0)
    {
        Draw2D_FontPrm sHeading;
        Draw2D_LinePrm sLinePrm;

        sHeading.fontIdx = 4;
        Draw2D_drawString(handle, 520, 5, "TIDL Camera Demo - Object Classification", &sHeading);

        sLinePrm.lineColor = RGB888_TO_RGB565(255, 255, 255);
        sLinePrm.lineSize  = 3;
        sLinePrm.lineColorFormat = DRAW2D_DF_BGR16_565;

        /* Draw a vertial line */
        Draw2D_drawLine(handle, DISPLAY_WIDTH/2 + 150, 100, DISPLAY_WIDTH/2 + 150, DISPLAY_HEIGHT - 250, &sLinePrm);

        sHeading.fontIdx = 2;
        Draw2D_drawString(handle, DISPLAY_WIDTH/2 + 160, 200, "Top 5 classes", &sHeading);
    }

    if((update_type == 1) && (g_update_result == 1))
    {
        Draw2D_FontPrm sClases;
        vx_int32 i;

        sClases.fontIdx = 2;
        for(i = 0; i < g_num_top_results; i++)
        {
            Draw2D_clearRegion(handle, (DISPLAY_WIDTH/2) + 160, 300 + (i * 40), (DISPLAY_WIDTH - (DISPLAY_WIDTH/2) + 160), 40);
            Draw2D_drawString(handle, DISPLAY_WIDTH/2 + 160, 300 + (i * 40), (char *)&imgnet_labels[g_classid[i]], &sClases);
        }
        g_update_result = 0;
    }

    return;
}
#endif
