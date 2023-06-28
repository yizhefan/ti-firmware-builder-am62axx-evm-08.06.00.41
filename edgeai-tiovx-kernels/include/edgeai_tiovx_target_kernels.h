/*
 *
 * Copyright (c) 2022 Texas Instruments Incorporated
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

#ifndef TIVX_EDGEAI_IMG_PROC_KERNELS_H_
#define TIVX_EDGEAI_IMG_PROC_KERNELS_H_

#include <VX/vx.h>
#include <VX/vx_kernels.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_edgeai_tiovx_kernels_img_proc TIVX Kernels for Image Pre/Post Processing
 *
 * \brief This section documents the kernels defined for Image Pre/Post Processing
 *
 * \ingroup group_edgeai_tiovx_kernels
 */
/*!
 * \file
 * \brief The list of supported kernels in this kernel extension.
 */

/*! \brief OpenVX module name
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
#define TIVX_MODULE_NAME_EDGEAI_IMG_PROC    "edgeai_img_proc"

/*! \brief Kernel Name: DL Pre processing Armv8
 *  \see group_edgeai_tiovx_kernels_dl_pre_proc_armv8
 */
#define TIVX_KERNEL_DL_PRE_PROC_ARMV8_NAME     "com.ti.img_proc.dl.pre.proc.armv8"

/*! \brief Kernel Name: DL color blend Armv8
 *  \see group_edgeai_tiovx_kernels_dl_color_blend_armv8
 */
#define TIVX_KERNEL_DL_COLOR_BLEND_ARMV8_NAME     "com.ti.img_proc.dl.color.blend.armv8"

/*! \brief Kernel Name: DL draw box Armv8
 *  \see group_edgeai_tiovx_kernels_dl_draw_box_armv8
 */
#define TIVX_KERNEL_DL_DRAW_BOX_ARMV8_NAME     "com.ti.img_proc.dl.draw.box.armv8"

/*! \brief Kernel Name: DL color convert Armv8
 *  \see group_vision_apps_kernels_dl_color_convert_armv8
 */
#define TIVX_KERNEL_DL_COLOR_CONVERT_ARMV8_NAME     "com.ti.img_proc.dl.color.convert.armv8"

/* Supported tensor formats in dl-pre-proc */
#define TIVX_DL_PRE_PROC_ARMV8_TENSOR_FORMAT_RGB   (0)
#define TIVX_DL_PRE_PROC_ARMV8_TENSOR_FORMAT_BGR   (1)

/* Supported channel ordering in dl-pre-proc */
#define TIVX_DL_PRE_PROC_ARMV8_CHANNEL_ORDER_NCHW   (0)
#define TIVX_DL_PRE_PROC_ARMV8_CHANNEL_ORDER_NHWC   (1)

/* Supported crop indexes in dl-pre-proc */
#define TIVX_DL_PRE_PROC_ARMV8_IMAGE_CROP_TOP     (0)
#define TIVX_DL_PRE_PROC_ARMV8_IMAGE_CROP_BOTTOM  (1)
#define TIVX_DL_PRE_PROC_ARMV8_IMAGE_CROP_LEFT    (2)
#define TIVX_DL_PRE_PROC_ARMV8_IMAGE_CROP_RIGHT   (3)

/* Macros to indicate max outputs, classes and colors in dl-draw-box */
#define TIVX_DL_DRAW_BOX_ARMV8_MAX_OUTPUTS     (4U)
#define TIVX_DL_DRAW_BOX_ARMV8_MAX_CLASSES     (256U)
#define TIVX_DL_DRAW_BOX_ARMV8_MAX_COLORS      (3U)
#define TIVX_DL_DRAW_BOX_ARMV8_MAX_CLASS_NAME  (64U)
#define TIVX_DL_DRAW_BOX_ARMV8_MAX_RECTANGLES  (256U)

/* Macros to indicate max outputs, classes and colors in dl-color-blend */
#define TIVX_DL_COLOR_BLEND_ARMV8_MAX_OUTPUTS    (4U)
#define TIVX_DL_COLOR_BLEND_ARMV8_MAX_CLASSES    (256U)
#define TIVX_DL_COLOR_BLEND_ARMV8_MAX_COLORS     (3U)

/*!
 * \brief DL Pre processing to be used with DL-RT
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
typedef struct {

  /** Skip processing */
  vx_int32 skip_flag;

  /* Scale values to be applied per channel in the range of 0.0 to 1.0 */
  vx_float32 scale[3];

  /* Mean value per channel to be subtracted, range depends on channel bit-depth */
  vx_float32 mean[3];

  /* Channel ordering, 0-NCHW, 1-NHWC */
  vx_int32 channel_order;

  /* Tensor format, 0-RGB, 1-BGR */
  vx_int32 tensor_format;

  /* Crop values to be applied, 0-Top, 1-Bottom, 2-Right, 3-Left */
  vx_int32 crop[4];

}tivxDLPreProcArmv8Params;

/*!
 * \brief DL color blend to be used with DL-RT
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
typedef struct {

    /** Skip processing */
    vx_int32 skip_flag;

    /** Flag to indicate, if 1 - use color map, 0 - auto assign colors to each class */
    vx_int32 use_color_map;

    /** Number of classes per output not to exceed TIVX_DL_COLOR_BLEND_ARMV8_MAX_CLASSES*/
    vx_int32 num_classes;

    /** Color map for each output, number of colors not to exceed TIVX_DL_COLOR_BLEND_MAX_COLORS */
    vx_uint8 color_map[TIVX_DL_COLOR_BLEND_ARMV8_MAX_CLASSES][TIVX_DL_COLOR_BLEND_ARMV8_MAX_COLORS];

}tivxDLColorBlendArmv8Params;

/*!
 * \brief DL rectangles to be used with DL Draw Box
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
typedef struct {

    /** Array of 4 values, which could be either
     *  output_type = 0 -> [xmin, ymin], [xmax, ymax]
     *  output_type = 1 -> [x, y], [width height]
     *  output_type is defined in tivxDLDrawBoxParams
     */
    vx_int32 pos[4];

    /** Confidence score in % value [0.0f - 100.0f]*/
    vx_float32 score;

    /** class_id of detected object which indexes into
     * array of class_names defined in tivxDLDrawBoxParams
    */
    vx_int32 class_id;

}tivxDLRectangleArmv8;

/*!
 * \brief DL draw box to be used with DL-RT
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
typedef struct {

    /** Skip processing */
    vx_int32 skip_flag;

    /** Flag to indicate, if 1 - use color map, 0 - auto assign colors to each class */
    vx_int32 use_color_map;

    /** Number of classes per output not to exceed TIVX_DL_DRAW_BOX_MAX_CLASSES*/
    vx_int32 num_classes;

    /** Color map for each output, number of colors not to exceed TIVX_DL_DRAW_BOX_MAX_COLORS */
    vx_uint8 color_map[TIVX_DL_DRAW_BOX_ARMV8_MAX_CLASSES][TIVX_DL_DRAW_BOX_ARMV8_MAX_COLORS];

    /** Dictionary of class names not to exceed TIVX_DL_DRAW_BOX_MAX_CLASSES
     * and each class name not to exceed TIVX_DL_DRAW_BOX_MAX_CLASS_NAME
     */
    vx_uint8 class_names[TIVX_DL_DRAW_BOX_ARMV8_MAX_CLASSES][TIVX_DL_DRAW_BOX_ARMV8_MAX_CLASS_NAME];

    /** Array of tivxDLRectangleArmv8 not to exceed TIVX_DL_DRAW_BOX_MAX_CLASSES
     * and number of rectangles per class not to exceed TIVX_DL_DRAW_BOX_MAX_RECTANGLES
     */
    tivxDLRectangleArmv8 rectangle[TIVX_DL_DRAW_BOX_ARMV8_MAX_CLASSES][TIVX_DL_DRAW_BOX_ARMV8_MAX_RECTANGLES];

}tivxDLDrawBoxArmv8Params;

/*********************************
 *      Functions
 *********************************/

/*!
 * \brief Used for the Application to load the img_proc kernels into the context.
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
void tivxEdgeaiImgProcLoadKernels(vx_context context);

/*!
 * \brief Used for the Application to unload the img_proc kernels from the context.
 * \ingroup group_edgeai_tiovx_kernels_img_proc
 */
void tivxEdgeaiImgProcUnLoadKernels(vx_context context);

/*!
 * \brief Function to register IMG_PROC Kernels on the Armv8 Target
 * \ingroup group_tivx_ext
 */
void tivxRegisterEdgeaiImgProcTargetArmv8Kernels(void);

/*!
 * \brief Function to un-register IMG_PROC Kernels on the Armv8 Target
 * \ingroup group_tivx_ext
 */
void tivxUnRegisterEdgeaiImgProcTargetArmv8Kernels(void);

#ifdef __cplusplus
}
#endif

#endif /* TIVX_EDGEAI_IMG_PROC_KERNELS_H_ */
