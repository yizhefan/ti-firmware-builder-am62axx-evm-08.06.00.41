/*
 *
 * Copyright (c) 2021 Texas Instruments Incorporated
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

#include "multi_cam_codec_img_mosaic_module.h"

static vx_status tiovx_img_mosaic_module_configure_params(vx_context context, TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;

    obj->config = vxCreateUserDataObject(context, "ImgMosaicConfig", sizeof(tivxImgMosaicParams), NULL);
    status = vxGetStatus((vx_reference)obj->config);

    if(status == VX_SUCCESS)
    {
        vxSetReferenceName((vx_reference)obj->config, "mosaic_node_config");

        status = vxCopyUserDataObject(obj->config, 0, sizeof(tivxImgMosaicParams),\
                    &obj->params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        if(status != VX_SUCCESS)
        {
            TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to copy mosaic params! \n");
        }
    }
    else
    {
        TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to create config object! \n");
    }

    return status;
}

static vx_status tiovx_img_mosaic_module_create_kernel(vx_context context, TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;

    obj->kernel = tivxAddKernelImgMosaic(context, obj->num_inputs);
    status = vxGetStatus((vx_reference)obj->kernel);

    if(status != VX_SUCCESS)
    {
        TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to create kernel with %d inputs!\n", obj->num_inputs);
    }

    return status;
}

static vx_status tiovx_img_mosaic_module_create_input(vx_context context, TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_int32 in, q;

    for(in = 0; in < obj->num_inputs; in++)
    {
        vx_image image = vxCreateImage(context, obj->inputs[in].width, obj->inputs[in].height, obj->color_format);
        status = vxGetStatus((vx_reference)image);

        if((vx_status)VX_SUCCESS == status)
        {
            for(q = 0; q < obj->inputs[in].bufq_depth; q++)
            {
                obj->inputs[in].arr[q] = vxCreateObjectArray(context, (vx_reference)image, obj->num_channels);
                status = vxGetStatus((vx_reference)obj->inputs[in].arr[q]);

                if(status != VX_SUCCESS)
                {
                    TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to create input array! \n");
                    break;
                }

                obj->inputs[in].image_handle[q] = (vx_image)vxGetObjectArrayItem((vx_object_array)obj->inputs[in].arr[q], 0);
            }
            vxReleaseImage(&image);
        }
        else
        {
            TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to create input image template of size %d x %d!\n", obj->inputs[in].width, obj->inputs[in].height);
            break;
        }
    }

    return status;
}

static vx_status tiovx_img_mosaic_module_create_background(vx_context context, TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_int32 q;

    for(q = 0; q < obj->out_bufq_depth; q++)
    {
        obj->background_image[q] = vxCreateImage(context, obj->out_width, obj->out_height, obj->color_format);
        status = vxGetStatus((vx_reference)obj->background_image[q]);
        if(status != VX_SUCCESS)
        {
            TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to create background image of size %d x %d!\n", obj->out_width, obj->out_height);
            break;
        }
        else
        {
            vx_char name[VX_MAX_REFERENCE_NAME];

            snprintf(name, VX_MAX_REFERENCE_NAME, "mosaic_node_background_image_%d", q);
            vxSetReferenceName((vx_reference)obj->background_image[q], name);
        }
    }

    return status;
}

static vx_status tiovx_img_mosaic_module_create_output(vx_context context, TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_int32 q;

    for(q = 0; q < obj->out_bufq_depth; q++)
    {
        obj->output_image[q] = vxCreateImage(context, obj->out_width, obj->out_height, obj->color_format);
        status = vxGetStatus((vx_reference)obj->output_image[q]);
        if(status != VX_SUCCESS)
        {
            TIOVX_MODULE_ERROR("[IMG-MOSAIC-MODULE] Unable to create output image of size %d x %d!\n", obj->out_width, obj->out_height);
            break;
        }
        else
        {
            vx_char name[VX_MAX_REFERENCE_NAME];

            snprintf(name, VX_MAX_REFERENCE_NAME, "mosaic_node_output_image_%d", q);
            vxSetReferenceName((vx_reference)obj->output_image[q], name);
        }
    }

    return status;
}

vx_status tiovx_img_mosaic_module_init(vx_context context, TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;

    TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Configuring params!\n");
    status = tiovx_img_mosaic_module_configure_params(context, obj);

    if(status == VX_SUCCESS)
    {
        TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Creating input image arrays!\n");
        status = tiovx_img_mosaic_module_create_input(context, obj);
    }

    if(status == VX_SUCCESS)
    {
        TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Creating background image!\n");
        tiovx_img_mosaic_module_create_background(context, obj);
    }

    if(status == VX_SUCCESS)
    {
        TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Creating output image!\n");
        status = tiovx_img_mosaic_module_create_output(context, obj);
    }

    if(status == VX_SUCCESS)
    {
        TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Creating image mosaic kernel!\n");
        status = tiovx_img_mosaic_module_create_kernel(context, obj);
    }

    return status;
}

vx_status tiovx_img_mosaic_module_deinit(TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_int32 q, in;

    TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Releasing config user data object!\n");
    status = vxReleaseUserDataObject(&obj->config);

    for(in = 0; in < obj->num_inputs; in++)
    {
        for(q = 0; q < obj->inputs[in].bufq_depth; q++)
        {
            if((vx_status)VX_SUCCESS == status)
            {
                TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Releasing input image handle %d, bufq %d!\n", in, q);
                status = vxReleaseImage(&obj->inputs[in].image_handle[q]);
            }
            if((vx_status)VX_SUCCESS == status)
            {
                TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Releasing input array %d, bufq %d!\n", in, q);
                status = vxReleaseObjectArray(&obj->inputs[in].arr[q]);
            }
        }
    }

    for(q = 0; q < obj->out_bufq_depth; q++)
    {
        if((vx_status)VX_SUCCESS == status)
        {
            TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Releasing output image bufq %d!\n", q);
            status = vxReleaseImage(&obj->output_image[q]);
        }
    }

    for(q = 0; q < obj->out_bufq_depth; q++)
    {
        if((vx_status)VX_SUCCESS == status)
        {
            TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Releasing background image bufq %d!\n", q);
            status = vxReleaseImage(&obj->background_image[q]);
        }
    }

    return status;
}

vx_status tiovx_img_mosaic_module_delete(TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;

    if(obj->node != NULL)
    {
        if((vx_status)VX_SUCCESS == status)
        {
            status = vxReleaseNode(&obj->node);
        }
    }
    if(obj->kernel != NULL)
    {
        if((vx_status)VX_SUCCESS == status)
        {
            status = vxRemoveKernel(obj->kernel);
        }
    }

    return status;
}

vx_status tiovx_img_mosaic_module_create(vx_graph graph, TIOVXImgMosaicModuleObj *obj, vx_image background, vx_object_array input_arr_user[], const char* target_string)
{

    vx_status status = VX_SUCCESS;

    vx_object_array input_arr[TIVX_IMG_MOSAIC_MAX_INPUTS];
    vx_int32 in;

    for(in = 0; in < obj->num_inputs; in++)
    {
        if(input_arr_user[in] == NULL)
        {
            input_arr[in] = obj->inputs[in].arr[0];
        }
        else
        {
            input_arr[in] = input_arr_user[in];
        }
    }

    obj->node = tivxImgMosaicNode(graph,
                                obj->kernel,
                                obj->config,
                                obj->output_image[0],
                                background,
                                input_arr,
                                obj->num_inputs);

    status = vxGetStatus((vx_reference)obj->node);

    if(status == VX_SUCCESS)
    {
        vxSetNodeTarget(obj->node, VX_TARGET_STRING, target_string);
        vxSetReferenceName((vx_reference)obj->node, "img_mosaic_node");
    }
    else
    {
        printf("[IMG-MOSAIC-MODULE] Unable to create mosaic node! \n");
    }

    return (status);
}

vx_status tiovx_img_mosaic_module_release_buffers(TIOVXImgMosaicModuleObj *obj)
{
    vx_status status = VX_SUCCESS;

    void      *virtAddr[TIOVX_MODULES_MAX_REF_HANDLES] = {NULL};
    vx_uint32   size[TIOVX_MODULES_MAX_REF_HANDLES];
    vx_uint32   numEntries;
    vx_int32   bufq, ch, in;

    /* Free input handles */
    for(in = 0; in < obj->num_inputs; in++)
    {
        for(bufq = 0; bufq < obj->inputs[in].bufq_depth; bufq++)
        {
            for(ch = 0; ch < obj->num_channels; ch++)
            {
                vx_reference ref = vxGetObjectArrayItem(obj->inputs[in].arr[bufq], ch);
                status = vxGetStatus((vx_reference)ref);

                if((vx_status)VX_SUCCESS == status)
                {
                    /* Export handles to get valid size information. */
                    status = tivxReferenceExportHandle(ref,
                                                    virtAddr,
                                                    size,
                                                    TIOVX_MODULES_MAX_REF_HANDLES,
                                                    &numEntries);

                    if((vx_status)VX_SUCCESS == status)
                    {
                        vx_int32 ctr;
                        /* Currently the vx_image buffers are alloated in one shot for multiple planes.
                            So if we are freeing this buffer then we need to get only the first plane
                            pointer address but add up the all the sizes to free the entire buffer */
                        vx_uint32 freeSize = 0;
                        for(ctr = 0; ctr < numEntries; ctr++)
                        {
                            freeSize += size[ctr];
                        }

                        if(virtAddr[0] != NULL)
                        {
                            TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Freeing input %d, bufq=%d, ch=%d, addr = 0x%016lX, size = %d \n", in, bufq, ch, (vx_uint64)virtAddr[0], freeSize);
                            tivxMemFree(virtAddr[0], freeSize, TIVX_MEM_EXTERNAL);
                        }

                        for(ctr = 0; ctr < numEntries; ctr++)
                        {
                            virtAddr[ctr] = NULL;
                        }

                        /* Assign NULL handles to the OpenVx objects as it will avoid
                            doing a tivxMemFree twice, once now and once during release */
                        status = tivxReferenceImportHandle(ref,
                                                        (const void **)virtAddr,
                                                        (const uint32_t *)size,
                                                        numEntries);
                    }
                    vxReleaseReference(&ref);
                }
            }
        }
    }

    /* Free output handles */
    for(bufq = 0; bufq < obj->out_bufq_depth; bufq++)
    {
        vx_reference ref = (vx_reference)obj->output_image[bufq];
        status = vxGetStatus((vx_reference)ref);

        if((vx_status)VX_SUCCESS == status)
        {
            /* Export handles to get valid size information. */
            status = tivxReferenceExportHandle(ref,
                                                virtAddr,
                                                size,
                                                TIOVX_MODULES_MAX_REF_HANDLES,
                                                &numEntries);

            if((vx_status)VX_SUCCESS == status)
            {
                vx_int32 ctr;
                /* Currently the vx_image buffers are alloated in one shot for multiple planes.
                    So if we are freeing this buffer then we need to get only the first plane
                    pointer address but add up the all the sizes to free the entire buffer */
                vx_uint32 freeSize = 0;
                for(ctr = 0; ctr < numEntries; ctr++)
                {
                    freeSize += size[ctr];
                }

                if(virtAddr[0] != NULL)
                {
                    TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Freeing output, bufq=%d, addr = 0x%016lX, size = %d \n", bufq, (vx_uint64)virtAddr[0], freeSize);
                    tivxMemFree(virtAddr[0], freeSize, TIVX_MEM_EXTERNAL);
                }

                for(ctr = 0; ctr < numEntries; ctr++)
                {
                    virtAddr[ctr] = NULL;
                }

                /* Assign NULL handles to the OpenVx objects as it will avoid
                    doing a tivxMemFree twice, once now and once during release */
                status = tivxReferenceImportHandle(ref,
                                                (const void **)virtAddr,
                                                (const uint32_t *)size,
                                                numEntries);
            }
        }
    }

    /* Free background handles */
    for(bufq = 0; bufq < obj->out_bufq_depth; bufq++)
    {
        vx_reference ref = (vx_reference)obj->background_image[bufq];
        status = vxGetStatus((vx_reference)ref);

        if((vx_status)VX_SUCCESS == status)
        {
            /* Export handles to get valid size information. */
            status = tivxReferenceExportHandle(ref,
                                                virtAddr,
                                                size,
                                                TIOVX_MODULES_MAX_REF_HANDLES,
                                                &numEntries);

            if((vx_status)VX_SUCCESS == status)
            {
                vx_int32 ctr;
                /* Currently the vx_image buffers are alloated in one shot for multiple planes.
                    So if we are freeing this buffer then we need to get only the first plane
                    pointer address but add up the all the sizes to free the entire buffer */
                vx_uint32 freeSize = 0;
                for(ctr = 0; ctr < numEntries; ctr++)
                {
                    freeSize += size[ctr];
                }

                if(virtAddr[0] != NULL)
                {
                    TIOVX_MODULE_PRINTF("[IMG-MOSAIC-MODULE] Freeing background, bufq=%d, addr = 0x%016lX, size = %d \n", bufq, (vx_uint64)virtAddr[0], freeSize);
                    tivxMemFree(virtAddr[0], freeSize, TIVX_MEM_EXTERNAL);
                }

                for(ctr = 0; ctr < numEntries; ctr++)
                {
                    virtAddr[ctr] = NULL;
                }

                /* Assign NULL handles to the OpenVx objects as it will avoid
                    doing a tivxMemFree twice, once now and once during release */
                status = tivxReferenceImportHandle(ref,
                                                (const void **)virtAddr,
                                                (const uint32_t *)size,
                                                numEntries);
            }
        }
    }

    if ((vx_status)VX_SUCCESS != status)
    {
        VX_PRINT(VX_ZONE_ERROR, "tivxReferenceExportHandle() failed.\n");
    }

    return status;
}
