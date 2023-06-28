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
#include <tivx_utils_file_rd_wr.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>


#define APP_MAX_FILE_PATH           (256u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));

typedef struct {

    /* config options */
    char input_file_path[APP_MAX_FILE_PATH];
    char output_file_path[APP_MAX_FILE_PATH];
    char input_file_prefix[APP_MAX_FILE_PATH];
    char output_file_prefix[APP_MAX_FILE_PATH];
    char input_mesh_file[APP_MAX_FILE_PATH];
    uint32_t start_fileno;
    uint32_t end_fileno;
    uint32_t width;
    uint32_t height;
    uint32_t width_out;
    uint32_t height_out;

    /* OpenVX references */
    vx_context context;
    vx_graph graph;
    vx_node  node_ldc;
    vx_image input_img;
    vx_image ldc_img;
    vx_image ldc_mesh;

    vx_matrix affine_matrix;

    int16_t matrix[6];

    vx_user_data_object ldc_config;
    tivx_vpac_ldc_params_t ldc_params;

    tivx_vpac_ldc_mesh_params_t mesh_params;
    vx_user_data_object mesh_config;

    tivx_vpac_ldc_region_params_t region_params;
    vx_user_data_object region_config;
} AppObj;

AppObj gAppObj;

static void app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[]);
static void app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static vx_status app_create_graph(AppObj *obj);
static vx_status app_run_graph(AppObj *obj);
static void app_delete_graph(AppObj *obj);

static vx_status app_load_vximage_mesh_from_text_file(char *filename, vx_image image);

int main(int argc, char* argv[])
{
    AppObj *obj = &gAppObj;

    app_parse_cmd_line_args(obj, argc, argv);
    app_init(obj);
    app_create_graph(obj);
    app_run_graph(obj);
    app_delete_graph(obj);
    app_deinit(obj);

    return 0;
}

static void app_init(AppObj *obj)
{
    tivxInit();

    obj->context = vxCreateContext();
    APP_ASSERT_VALID_REF(obj->context);

    tivxHwaLoadKernels(obj->context);
}

static void app_deinit(AppObj *obj)
{
    tivxHwaUnLoadKernels(obj->context);
    vxReleaseContext(&obj->context);
    tivxDeInit();
}

/*
 * Graph,
 *           ldc_config
 *               |
 *               v
 * input_img -> LDC -----> ldc_img
 * ldc_region ---^
 * ldc_mesh   ---^
 *
 */
static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    char mesh_file_name[APP_MAX_FILE_PATH];

    obj->graph = vxCreateGraph(obj->context);
    APP_ASSERT_VALID_REF(obj->graph);

    /* In/Out Images */
    obj->input_img = vxCreateImage(obj->context, obj->width, obj->height, VX_DF_IMAGE_U8);
    APP_ASSERT_VALID_REF(obj->input_img);
    vxSetReferenceName((vx_reference)obj->input_img, "InputImageU8");


    obj->ldc_img = vxCreateImage(obj->context, obj->width_out, obj->height_out, VX_DF_IMAGE_U8);
    APP_ASSERT_VALID_REF(obj->ldc_img);
    vxSetReferenceName((vx_reference)obj->ldc_img, "OutputImageU8");

    /* Config params */
    memset(&obj->ldc_params, 0, sizeof(tivx_vpac_ldc_params_t));
    obj->ldc_config = vxCreateUserDataObject(obj->context, "tivx_vpac_ldc_params_t",
                                             sizeof(tivx_vpac_ldc_params_t), NULL);
    APP_ASSERT_VALID_REF(obj->ldc_config);
    vxSetReferenceName((vx_reference)obj->ldc_config, "LDC_Configuration");

    /* Mesh params */
    memset(&obj->mesh_params, 0, sizeof(tivx_vpac_ldc_mesh_params_t));
    obj->mesh_config = vxCreateUserDataObject(obj->context, "tivx_vpac_ldc_mesh_params_t",
                                             sizeof(tivx_vpac_ldc_mesh_params_t), NULL);
    APP_ASSERT_VALID_REF(obj->mesh_config);
    vxSetReferenceName((vx_reference)obj->mesh_config, "LDC_Mesh_Config");

    /* Region params */
    memset(&obj->region_params, 0, sizeof(tivx_vpac_ldc_region_params_t));
    obj->region_config = vxCreateUserDataObject(obj->context, "tivx_vpac_ldc_region_params_t",
                                             sizeof(tivx_vpac_ldc_region_params_t), NULL);
    APP_ASSERT_VALID_REF(obj->region_config);
    vxSetReferenceName((vx_reference)obj->region_config, "LDC_Region_Config");

    obj->ldc_params.luma_interpolation_type = 1; //BILINEAR

    obj->region_params.out_block_width = 16;
    obj->region_params.out_block_height = 16;
    obj->region_params.pixel_pad = 0;

    /* Mesh Table */
    obj->mesh_params.mesh_frame_width = 800;
    obj->mesh_params.mesh_frame_height = 1040;
    obj->mesh_params.subsample_factor = 4;

    obj->ldc_mesh = vxCreateImage(obj->context,
                                  obj->mesh_params.mesh_frame_width/(1<<obj->mesh_params.subsample_factor)+1,
                                  obj->mesh_params.mesh_frame_height/(1<<obj->mesh_params.subsample_factor)+1,
                                  VX_DF_IMAGE_U32);
    APP_ASSERT_VALID_REF(obj->ldc_mesh);
    vxSetReferenceName((vx_reference)obj->ldc_mesh, "LDC_Mesh");

    snprintf(mesh_file_name, APP_MAX_FILE_PATH, "%s",
        obj->input_mesh_file);

    app_load_vximage_mesh_from_text_file(mesh_file_name, obj->ldc_mesh);

    /* Affine Matrix */
    obj->affine_matrix = vxCreateMatrix(obj->context, VX_TYPE_INT16, 2, 3);
    APP_ASSERT_VALID_REF(obj->affine_matrix);
    vxSetReferenceName((vx_reference)obj->affine_matrix, "LDC_AffineMatrix");

    obj->matrix[0] = 4096;
    obj->matrix[1] = 0;
    obj->matrix[4] = 256;
    obj->matrix[2] = 0;
    obj->matrix[3] = 4096;
    obj->matrix[5] = 320;

    status = vxCopyMatrix(obj->affine_matrix, obj->matrix, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    /* This data structure must be added after the entire structure is configured (including mesh portion) */
    status = vxCopyUserDataObject(obj->ldc_config, 0, sizeof(tivx_vpac_ldc_params_t), &obj->ldc_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    status = vxCopyUserDataObject(obj->mesh_config, 0, sizeof(tivx_vpac_ldc_mesh_params_t), &obj->mesh_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    status = vxCopyUserDataObject(obj->region_config, 0, sizeof(tivx_vpac_ldc_region_params_t), &obj->region_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    obj->node_ldc = tivxVpacLdcNode(obj->graph,
                                    obj->ldc_config,
                                    obj->affine_matrix,
                                    obj->region_config,
                                    obj->mesh_config,
                                    obj->ldc_mesh,
                                    NULL,
                                    obj->input_img,
                                    obj->ldc_img,
                                    NULL);

    APP_ASSERT_VALID_REF(obj->node_ldc);
    status = vxSetNodeTarget(obj->node_ldc, VX_TARGET_STRING, TIVX_TARGET_VPAC_LDC1);
    APP_ASSERT(status==VX_SUCCESS);
    vxSetReferenceName((vx_reference)obj->node_ldc, "LDC_Processing");

    status = vxVerifyGraph(obj->graph);
    APP_ASSERT(status==VX_SUCCESS);

#if 0
    status = tivxExportGraphToDot(obj->graph,".", "vx_app_ldc");
    APP_ASSERT(status==VX_SUCCESS);
#endif

    return status;
}

static void app_delete_graph(AppObj *obj)
{
    vxReleaseNode(&obj->node_ldc);
    vxReleaseGraph(&obj->graph);
    vxReleaseImage(&obj->input_img);
    vxReleaseImage(&obj->ldc_img);
    vxReleaseImage(&obj->ldc_mesh);
    vxReleaseUserDataObject(&obj->ldc_config);
    vxReleaseMatrix(&obj->affine_matrix);
    vxReleaseUserDataObject(&obj->mesh_config);
    vxReleaseUserDataObject(&obj->region_config);
}

static vx_status app_run_graph(AppObj *obj)
{
    char input_file_name[APP_MAX_FILE_PATH];
    char output_file_name_img[APP_MAX_FILE_PATH];
    uint32_t curFileNum;
    vx_status status = VX_SUCCESS;

    /* create output directory is not already existing */
    mkdir(obj->output_file_path, S_IRWXU | S_IRWXG | S_IRWXO);

    for(curFileNum = obj->start_fileno; curFileNum <= obj->end_fileno; curFileNum++)
    {
        snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%s%05d.png",
            obj->input_file_path,
            obj->input_file_prefix,
            curFileNum
            );
        snprintf(output_file_name_img, APP_MAX_FILE_PATH, "%s/%s_ldc_%05d.png",
            obj->output_file_path,
            obj->output_file_prefix,
            curFileNum
            );

        printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, input_file_name);
        tivx_utils_load_vximage_from_pngfile(obj->input_img, input_file_name, vx_true_e);

        printf(" %d of %d: Running graph ...\n", curFileNum, obj->end_fileno);
        status = vxScheduleGraph(obj->graph);
        APP_ASSERT(status==VX_SUCCESS);
        status = vxWaitGraph(obj->graph);
        APP_ASSERT(status==VX_SUCCESS);

        printf(" %d of %d: Saving [%s] ...\n", curFileNum, obj->end_fileno, output_file_name_img);
        tivx_utils_save_vximage_to_pngfile(output_file_name_img, obj->ldc_img);
        printf(" %d of %d: Done !!!\n", curFileNum, obj->end_fileno);
    }
    return status;
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

#if 0
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

static void app_show_usage(int argc, char* argv[])
{
    printf("\n");
    printf(" LDC HWA Demo - (c) Texas Instruments 2018\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->input_file_path,APP_MAX_FILE_PATH, "/home/kedarc/code/j7_cmodels/database/sequence0001/camera002/data");
    snprintf(obj->output_file_path,APP_MAX_FILE_PATH, "./ldc_out");
    snprintf(obj->input_file_prefix,APP_MAX_FILE_PATH, "00000");
    snprintf(obj->output_file_prefix,APP_MAX_FILE_PATH, "00000");
    obj->start_fileno = 74;
    obj->end_fileno = 84;
    obj->width = 1280;
    obj->height = 720;
    obj->width_out = 1280;
    obj->height_out = 720;
}

static void app_parse_cfg_file(AppObj *obj, char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    char line_str[1024];
    char *token;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(0);
    }

    while(fgets(line_str, sizeof(line_str), fp)!=NULL)
    {
        char s[]=" \t";

        if (strchr(line_str, '#'))
        {
            continue;
        }

        /* get the first token */
        token = strtok(line_str, s);

        if (token == NULL)
        {
            continue;
        }

        if(strcmp(token, "input_file_path")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            token[strlen(token)-1]=0;
            strcpy(obj->input_file_path, token);
        }
        else
        if(strcmp(token, "output_file_path")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            token[strlen(token)-1]=0;
            strcpy(obj->output_file_path, token);
        }
        else
        if(strcmp(token, "input_file_prefix")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            token[strlen(token)-1]=0;
            strcpy(obj->input_file_prefix, token);
        }
        else
        if(strcmp(token, "output_file_prefix")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            token[strlen(token)-1]=0;
            strcpy(obj->output_file_prefix, token);
        }
        else
        if(strcmp(token, "input_mesh_file")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            token[strlen(token)-1]=0;
            strcpy(obj->input_mesh_file, token);
        }
        else
        if(strcmp(token, "start_seq")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            obj->start_fileno = atoi(token);
        }
        else
        if(strcmp(token, "end_seq")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            obj->end_fileno = atoi(token);
        }
        else
        if(strcmp(token, "width_in")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            obj->width = atoi(token);
        }
        else
        if(strcmp(token, "height_in")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            obj->height = atoi(token);
        }
        else
        if(strcmp(token, "width_out")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            obj->width_out = atoi(token);
        }
        else
        if(strcmp(token, "height_out")==0)
        {
            token = strtok(NULL, s);

            if (token == NULL)
            {
                continue;
            }

            obj->height_out = atoi(token);
        }
    }

    fclose(fp);

    if(obj->width<128)
        obj->width = 128;
    if(obj->height<128)
        obj->height = 128;
    if(obj->width_out<128)
        obj->width_out = 128;
    if(obj->height_out<128)
        obj->height_out = 128;
    if(obj->end_fileno < obj->start_fileno)
        obj->end_fileno = obj->start_fileno;

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
