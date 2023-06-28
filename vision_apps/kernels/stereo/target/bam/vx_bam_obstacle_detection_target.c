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

#include "TI/tivx.h"
#include "TI/tivx_stereo.h"
#include "VX/vx.h"
#include "tivx_stereo_kernels_priv.h"
#include "tivx_kernel_obstacle_detection.h"
#include "TI/tivx_target_kernel.h"
#include "tivx_kernels_target_utils.h"
#include "tivx_bam_kernel_wrapper.h"

typedef struct
{
    tivx_bam_graph_handle graph_handle;
} tivxObstacleDetectionParams;

static tivx_target_kernel vx_obstacle_detection_target_kernel = NULL;

static vx_status VX_CALLBACK tivxObstacleDetectionProcess(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxObstacleDetectionCreate(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxObstacleDetectionDelete(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxObstacleDetectionControl(
       tivx_target_kernel_instance kernel,
       uint32_t node_cmd_id, tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);

static vx_status VX_CALLBACK tivxObstacleDetectionProcess(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg)
{
    vx_status status = VX_SUCCESS;
    tivxObstacleDetectionParams *prms = NULL;
    tivx_obj_desc_user_data_object_t *configuration_desc;
    tivx_obj_desc_image_t *input_image_desc;
    tivx_obj_desc_array_t *input_disparity_desc;
    tivx_obj_desc_user_data_object_t *input_ground_model_desc;
    tivx_obj_desc_array_t *output_obstacle_pos_desc;
    tivx_obj_desc_scalar_t *output_num_obstacles_desc;

    if ( (num_params != TIVX_KERNEL_OBSTACLE_DETECTION_MAX_PARAMS)
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_CONFIGURATION_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_IMAGE_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_DISPARITY_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_GROUND_MODEL_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_OBSTACLE_POS_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_NUM_OBSTACLES_IDX])
    )
    {
        status = VX_FAILURE;
    }

    if(VX_SUCCESS == status)
    {
        uint32_t size;
        configuration_desc = (tivx_obj_desc_user_data_object_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_CONFIGURATION_IDX];
        input_image_desc = (tivx_obj_desc_image_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_IMAGE_IDX];
        input_disparity_desc = (tivx_obj_desc_array_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_DISPARITY_IDX];
        input_ground_model_desc = (tivx_obj_desc_user_data_object_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_GROUND_MODEL_IDX];
        output_obstacle_pos_desc = (tivx_obj_desc_array_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_OBSTACLE_POS_IDX];
        output_num_obstacles_desc = (tivx_obj_desc_scalar_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_NUM_OBSTACLES_IDX];

        status = tivxGetTargetKernelInstanceContext(kernel,
            (void **)&prms, &size);
        if ((VX_SUCCESS != status) || (NULL == prms) ||
            (sizeof(tivxObstacleDetectionParams) != size))
        {
            status = VX_FAILURE;
        }
    }

    if(VX_SUCCESS == status)
    {
        vx_scalar output_num_obstacles_value;

        void *configuration_target_ptr;
        void *input_image_target_ptr;
        void *input_disparity_target_ptr;
        void *input_ground_model_target_ptr;
        void *output_obstacle_pos_target_ptr;
        void *output_num_obstacles_target_ptr;

        configuration_target_ptr = tivxMemShared2TargetPtr(&configuration_desc[i]->mem_ptr);
        tivxMemBufferMap(configuration_target_ptr,
           configuration_desc->mem_size, VX_MEMORY_TYPE_HOST,
           VX_READ_ONLY);

        input_image_target_ptr = tivxMemShared2TargetPtr(&input_image_desc->mem_ptr[0]);
        tivxMemBufferMap(input_image_target_ptr,
           input_image_desc->mem_size[0], VX_MEMORY_TYPE_HOST,
           VX_READ_ONLY);

        input_disparity_target_ptr = tivxMemShared2TargetPtr(&input_disparity_desc[i]->mem_ptr);
        tivxMemBufferMap(input_disparity_target_ptr,
           input_disparity_desc->mem_size, VX_MEMORY_TYPE_HOST,
           VX_READ_ONLY);

        input_ground_model_target_ptr = tivxMemShared2TargetPtr(&input_ground_model_desc[i]->mem_ptr);
        tivxMemBufferMap(input_ground_model_target_ptr,
           input_ground_model_desc->mem_size, VX_MEMORY_TYPE_HOST,
           VX_READ_ONLY);

        output_obstacle_pos_target_ptr = tivxMemShared2TargetPtr(&output_obstacle_pos_desc[i]->mem_ptr);
        tivxMemBufferMap(output_obstacle_pos_target_ptr,
           output_obstacle_pos_desc->mem_size, VX_MEMORY_TYPE_HOST,
           VX_WRITE_ONLY);


        /* < DEVELOPER_TODO: (Optional) Modify 'scalar data type' below to be correct type > */
        /*input_confidence_threshold_value = input_confidence_threshold_desc->data.<scalar data type>;*/

        {
            void *img_ptrs[1];
            VXLIB_bufParams2D_t vxlib_input_image;
            uint8_t *input_image_addr = NULL;

            tivxInitBufParams(input_image_desc, &vxlib_input_image);
            tivxSetPointerLocation(input_image_desc, &input_image_target_ptr, &input_image_addr);

            img_ptrs[0] = input_image_addr;
            /* < DEVELOPER_TODO: Uncomment once BAM graph has been created > */
            /* tivxBamUpdatePointers(prms->graph_handle, 1U, 0U, img_ptrs); */

            /* < DEVELOPER_TODO: Uncomment once BAM graph has been created > */
            /* status  = tivxBamProcessGraph(prms->graph_handle); */

            /* call kernel processing function */

            /* < DEVELOPER_TODO: Add target kernel processing code here > */

            /* kernel processing function complete */

        }
        tivxMemBufferUnmap(configuration_target_ptr,
           configuration_desc->mem_size, VX_MEMORY_TYPE_HOST,
            VX_READ_ONLY);

        tivxMemBufferUnmap(input_image_target_ptr,
           input_image_desc->mem_size[0], VX_MEMORY_TYPE_HOST,
            VX_READ_ONLY);

        tivxMemBufferUnmap(input_disparity_target_ptr,
           input_disparity_desc->mem_size, VX_MEMORY_TYPE_HOST,
            VX_READ_ONLY);

        tivxMemBufferUnmap(input_ground_model_target_ptr,
           input_ground_model_desc->mem_size, VX_MEMORY_TYPE_HOST,
            VX_READ_ONLY);

        tivxMemBufferUnmap(output_obstacle_pos_target_ptr,
           output_obstacle_pos_desc->mem_size, VX_MEMORY_TYPE_HOST,
            VX_WRITE_ONLY);


        /* < DEVELOPER_TODO: (Optional) Modify 'scalar data type' below to be correct type > */
        /*output_num_obstacles->data.<scalar data type> = output_num_obstacles_desc_value;*/

    }

    return status;
}

static vx_status VX_CALLBACK tivxObstacleDetectionCreate(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg)
{
    vx_status status = VX_SUCCESS;
    tivxObstacleDetectionParams *prms = NULL;

    /* < DEVELOPER_TODO: (Optional) Add any target kernel create code here (e.g. allocating */
    /*                   local memory buffers, one time initialization, etc) > */
    if ( (num_params != TIVX_KERNEL_OBSTACLE_DETECTION_MAX_PARAMS)
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_CONFIGURATION_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_IMAGE_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_DISPARITY_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_GROUND_MODEL_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_OBSTACLE_POS_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_NUM_OBSTACLES_IDX])
    )
    {
        status = VX_FAILURE;
    }
    else
    {
        tivx_obj_desc_user_data_object_t *configuration_desc;
        tivx_obj_desc_user_data_object_t *input_ground_model_desc;

        configuration_desc = (tivx_obj_desc_user_data_object_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_CONFIGURATION_IDX];
        input_ground_model_desc = (tivx_obj_desc_user_data_object_t *)obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_GROUND_MODEL_IDX];

        if (configuration_desc->mem_size != sizeof(tivx_obstacle_detection_params_t))
        {
            VX_PRINT(VX_ZONE_ERROR, "User data object size on target does not match the size on host, possibly due to misalignment in data structure\n");
            status = VX_FAILURE;
        }
        if (input_ground_model_desc->mem_size != sizeof(tivx_ground_model_params_t))
        {
            VX_PRINT(VX_ZONE_ERROR, "User data object size on target does not match the size on host, possibly due to misalignment in data structure\n");
            status = VX_FAILURE;
        }
        prms = tivxMemAlloc(sizeof(tivxObstacleDetectionParams), TIVX_MEM_EXTERNAL);
        if (NULL != prms)
        {

        }
        else
        {
            status = VX_ERROR_NO_MEMORY;
            VX_PRINT(VX_ZONE_ERROR, "Unable to allocate local memory\n");
        }

        if (NULL != prms)
        {
            /* < DEVELOPER_TODO: Create BAM graph using graph_handle > */
        }

        if (VX_SUCCESS == status)
        {
            tivxSetTargetKernelInstanceContext(kernel, prms,
                sizeof(tivxObstacleDetectionParams));
        }
        else
        {
            status = VX_ERROR_NO_MEMORY;
            VX_PRINT(VX_ZONE_ERROR, "Unable to allocate local memory\n");
        }
    }

    return status;
}

static vx_status VX_CALLBACK tivxObstacleDetectionDelete(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg)
{
    vx_status status = VX_SUCCESS;
    tivxObstacleDetectionParams *prms = NULL;
    uint32_t size;

    /* < DEVELOPER_TODO: (Optional) Add any target kernel delete code here (e.g. freeing */
    /*                   local memory buffers, etc) > */
    if ( (num_params != TIVX_KERNEL_OBSTACLE_DETECTION_MAX_PARAMS)
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_CONFIGURATION_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_IMAGE_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_DISPARITY_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_INPUT_GROUND_MODEL_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_OBSTACLE_POS_IDX])
        || (NULL == obj_desc[TIVX_KERNEL_OBSTACLE_DETECTION_OUTPUT_NUM_OBSTACLES_IDX])
    )
    {
        status = VX_FAILURE;
    }
    else
    {
        tivxGetTargetKernelInstanceContext(kernel, (void **)&prms, &size);

        if ((NULL != prms) &&
            (sizeof(tivxObstacleDetectionParams) == size))
        {
            /* < DEVELOPER_TODO: Uncomment once BAM graph has been created > */
            /* tivxBamDestroyHandle(prms->graph_handle); */
            tivxMemFree(prms, size, TIVX_MEM_EXTERNAL);
        }
    }

    return status;
}

static vx_status VX_CALLBACK tivxObstacleDetectionControl(
       tivx_target_kernel_instance kernel,
       uint32_t node_cmd_id, tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg)
{
    vx_status status = VX_SUCCESS;

    /* < DEVELOPER_TODO: (Optional) Add any target kernel control code here (e.g. commands */
    /*                   the user can call to modify the processing of the kernel at run-time) > */

    return status;
}

void tivxAddTargetKernelBamObstacleDetection(void)
{
    vx_status status = VX_FAILURE;
    char target_name[TIVX_TARGET_MAX_NAME];
    vx_enum self_cpu;

    self_cpu = tivxGetSelfCpuId();

    if ( self_cpu == TIVX_CPU_ID_DSP1 )
    {
        strncpy(target_name, TIVX_TARGET_DSP1, TIVX_TARGET_MAX_NAME);
        status = VX_SUCCESS;
    }
    else
    if ( self_cpu == TIVX_CPU_ID_DSP2 )
    {
        strncpy(target_name, TIVX_TARGET_DSP2, TIVX_TARGET_MAX_NAME);
        status = VX_SUCCESS;
    }
    else
    if ( self_cpu == TIVX_CPU_ID_A72_0 )
    {
        strncpy(target_name, TIVX_TARGET_A72_0, TIVX_TARGET_MAX_NAME);
        status = VX_SUCCESS;
    }
    else
    {
        status = VX_FAILURE;
    }

    if (status == VX_SUCCESS)
    {
        vx_obstacle_detection_target_kernel = tivxAddTargetKernelByName(
                            TIVX_KERNEL_OBSTACLE_DETECTION_NAME,
                            target_name,
                            tivxObstacleDetectionProcess,
                            tivxObstacleDetectionCreate,
                            tivxObstacleDetectionDelete,
                            tivxObstacleDetectionControl,
                            NULL);
    }
}

void tivxRemoveTargetKernelBamObstacleDetection(void)
{
    vx_status status = VX_SUCCESS;

    status = tivxRemoveTargetKernel(vx_obstacle_detection_target_kernel);
    if (status == VX_SUCCESS)
    {
        vx_obstacle_detection_target_kernel = NULL;
    }
}


