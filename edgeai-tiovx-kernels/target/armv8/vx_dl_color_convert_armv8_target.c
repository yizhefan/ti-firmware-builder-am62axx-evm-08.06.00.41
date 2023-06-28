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

#include <edgeai_tiovx_img_proc.h>
#include <TI/tivx_target_kernel.h>
#include <tivx_kernels_target_utils.h>
#include <arm_neon.h>
#include <tivx_dl_color_convert_armv8_host.h>

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
#define RGB2Y(R, G, B) CLIP( ( 54*(R) + 183*(G) + 18*(B) + 128) >>8 )
#define RGB2U(R, G, B) CLIP(( ( -15*(R) - 49*(G) + 64*(B) + 64) >>7) + 128)
#define RGB2V(R, G, B) CLIP((( 64*(R) - 58*(G) - 6*(B) + 64) >> 7) + 128)

#define YUV2R_BT601(Y, U, V) CLIP(Y + ((22986 * V) >> 14) - 179)
#define YUV2G_BT601(Y, U, V) CLIP(Y - ((5636 * U + 11698 * V) >> 14) + 135)
#define YUV2B_BT601(Y, U, V) CLIP(Y + ((29048 * U) >> 14) - 226)

#define YUV2R_BT709(Y, U, V) CLIP(Y + ((25802 * V) >> 14) - 201)
#define YUV2G_BT709(Y, U, V) CLIP(Y - ((3068 * U + 7669 * V) >> 14) + 83)
#define YUV2B_BT709(Y, U, V) CLIP(Y + ((30402 * U) >> 14) - 237)

#define COLOR_SPACE_BT601_525 0x1     /* Defines BT601_525 color space */
#define COLOR_SPACE_BT601_625 0x2     /* Defines BT601_625 color space */

typedef struct
{
    uint32_t dim_x;        /* Width of buffer in X dimension in elements. */
    uint32_t dim_y;        /* Height of buffer in Y dimension in elements. */
    int32_t  stride_y;     /* Stride in Y dimension in bytes. */
} bufParams2D_t;

typedef struct {
    bufParams2D_t etn_src[TIVX_IMAGE_MAX_PLANES];
    bufParams2D_t etn_dst[TIVX_IMAGE_MAX_PLANES];
} tivxDLColorConvertArmv8KernelParams;

static tivx_target_kernel vx_dl_color_convert_armv8_target_kernel = NULL;

static vx_status VX_CALLBACK tivxKernelDLColorConvertArmv8Create(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxKernelDLColorConvertArmv8Delete(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxKernelDLColorConvertArmv8Process(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg);

static vx_status VX_CALLBACK tivxKernelDLColorConvertArmv8Create(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg)
{
    vx_status status = (vx_status)VX_SUCCESS;

    /* tivx_set_debug_zone(VX_ZONE_INFO); */

    if (num_params != TIVX_DL_COLOR_CONVERT_ARMV8_MAX_PARAMS)
    {
        status = (vx_status)VX_FAILURE;
    }
    else
    {
        uint32_t i;

        for (i = 0U; i < TIVX_DL_COLOR_CONVERT_ARMV8_MAX_PARAMS; i ++)
        {
            if (NULL == obj_desc[i])
            {
                status = (vx_status)VX_FAILURE;
                break;
            }
        }
    }

    tivxDLColorConvertArmv8KernelParams * kernelParams = NULL;
    kernelParams = tivxMemAlloc(sizeof(tivxDLColorConvertArmv8KernelParams), TIVX_MEM_EXTERNAL);

    if (NULL == kernelParams)
    {
        status = VX_FAILURE;
    }
    else
    {
        tivx_obj_desc_image_t *in_img_desc;
        tivx_obj_desc_image_t *out_img_desc;

        in_img_desc  = (tivx_obj_desc_image_t *)obj_desc[TIVX_DL_COLOR_CONVERT_ARMV8_INPUT_IDX];
        out_img_desc  = (tivx_obj_desc_image_t *)obj_desc[TIVX_DL_COLOR_CONVERT_ARMV8_OUTPUT_IDX];

        /* Setup input params */
        if (in_img_desc->format == VX_DF_IMAGE_RGB)
        {
            kernelParams->etn_src[0].dim_x = in_img_desc->imagepatch_addr[0].dim_x;
            kernelParams->etn_src[0].dim_y = in_img_desc->imagepatch_addr[0].dim_y;
            kernelParams->etn_src[0].stride_y = in_img_desc->imagepatch_addr[0].stride_y;
        }
        else if ((in_img_desc->format == VX_DF_IMAGE_NV12) || (in_img_desc->format == VX_DF_IMAGE_NV21))
        {
            kernelParams->etn_src[0].dim_x = in_img_desc->imagepatch_addr[0].dim_x;
            kernelParams->etn_src[0].dim_y = in_img_desc->imagepatch_addr[0].dim_y;
            kernelParams->etn_src[0].stride_y = in_img_desc->imagepatch_addr[0].stride_y;

            kernelParams->etn_src[1].dim_x = in_img_desc->imagepatch_addr[1].dim_x;
            kernelParams->etn_src[1].dim_y = in_img_desc->imagepatch_addr[1].dim_y /
                                             in_img_desc->imagepatch_addr[1].step_y;
            kernelParams->etn_src[1].stride_y = in_img_desc->imagepatch_addr[1].stride_y;
        }
        else if (in_img_desc->format == VX_DF_IMAGE_IYUV)
        {
            kernelParams->etn_src[0].dim_x = in_img_desc->imagepatch_addr[0].dim_x;
            kernelParams->etn_src[0].dim_y = in_img_desc->imagepatch_addr[0].dim_y;
            kernelParams->etn_src[0].stride_y = in_img_desc->imagepatch_addr[0].stride_y;

            kernelParams->etn_src[1].dim_x = in_img_desc->imagepatch_addr[1].dim_x /
                                             in_img_desc->imagepatch_addr[1].step_x;
            kernelParams->etn_src[1].dim_y = in_img_desc->imagepatch_addr[1].dim_y /
                                             in_img_desc->imagepatch_addr[1].step_y;
            kernelParams->etn_src[1].stride_y = in_img_desc->imagepatch_addr[1].stride_y;

            kernelParams->etn_src[2].dim_x = in_img_desc->imagepatch_addr[2].dim_x /
                                             in_img_desc->imagepatch_addr[2].step_x;
            kernelParams->etn_src[2].dim_y = in_img_desc->imagepatch_addr[2].dim_y /
                                             in_img_desc->imagepatch_addr[2].step_y;
            kernelParams->etn_src[2].stride_y = in_img_desc->imagepatch_addr[2].stride_y;
        }

        /* Setup output params */
        if (out_img_desc->format == VX_DF_IMAGE_RGB)
        {
            kernelParams->etn_dst[0].dim_x = out_img_desc->imagepatch_addr[0].dim_x;
            kernelParams->etn_dst[0].dim_y = out_img_desc->imagepatch_addr[0].dim_y;
            kernelParams->etn_dst[0].stride_y = out_img_desc->imagepatch_addr[0].stride_y;
        }
        else if (out_img_desc->format == VX_DF_IMAGE_NV12)
        {
            kernelParams->etn_dst[0].dim_x = out_img_desc->imagepatch_addr[0].dim_x;
            kernelParams->etn_dst[0].dim_y = out_img_desc->imagepatch_addr[0].dim_y;
            kernelParams->etn_dst[0].stride_y = out_img_desc->imagepatch_addr[0].stride_y;

            kernelParams->etn_dst[1].dim_x = out_img_desc->imagepatch_addr[1].dim_x;
            kernelParams->etn_dst[1].dim_y = out_img_desc->imagepatch_addr[1].dim_y /
                                             out_img_desc->imagepatch_addr[1].step_y;
            kernelParams->etn_dst[1].stride_y = out_img_desc->imagepatch_addr[1].stride_y;
        }
        else if (out_img_desc->format == VX_DF_IMAGE_IYUV)
        {
            kernelParams->etn_dst[0].dim_x = out_img_desc->imagepatch_addr[0].dim_x;
            kernelParams->etn_dst[0].dim_y = out_img_desc->imagepatch_addr[0].dim_y;
            kernelParams->etn_dst[0].stride_y = in_img_desc->imagepatch_addr[0].stride_y;

            kernelParams->etn_dst[1].dim_x = out_img_desc->imagepatch_addr[1].dim_x /
                                             out_img_desc->imagepatch_addr[1].step_x;
            kernelParams->etn_dst[1].dim_y = out_img_desc->imagepatch_addr[1].dim_y /
                                             out_img_desc->imagepatch_addr[1].step_y;
            kernelParams->etn_dst[1].stride_y = out_img_desc->imagepatch_addr[1].stride_y;

            kernelParams->etn_dst[2].dim_x = out_img_desc->imagepatch_addr[2].dim_x /
                                             out_img_desc->imagepatch_addr[2].step_x;
            kernelParams->etn_dst[2].dim_y = out_img_desc->imagepatch_addr[2].dim_y /
                                             out_img_desc->imagepatch_addr[2].step_y;
            kernelParams->etn_dst[2].stride_y = out_img_desc->imagepatch_addr[2].stride_y;
        }

        tivxSetTargetKernelInstanceContext(kernel, kernelParams,  sizeof(tivxDLColorConvertArmv8KernelParams));
    }

    /* tivx_clr_debug_zone(VX_ZONE_INFO); */

    return (status);
}

static vx_status VX_CALLBACK tivxKernelDLColorConvertArmv8Delete(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg)
{
    vx_status status = (vx_status)VX_SUCCESS;

    if (num_params != TIVX_DL_COLOR_CONVERT_ARMV8_MAX_PARAMS)
    {
        status = (vx_status)VX_FAILURE;
    }
    else
    {
        uint32_t i;

        for (i = 0U; i < TIVX_DL_COLOR_CONVERT_ARMV8_MAX_PARAMS; i ++)
        {
            if (NULL == obj_desc[i])
            {
                status = (vx_status)VX_FAILURE;
                break;
            }
        }
    }

    if (VX_SUCCESS == status)
    {
        uint32_t size;
        tivxDLColorConvertArmv8KernelParams *prms = NULL;

        status = tivxGetTargetKernelInstanceContext(kernel,
            (void **)&prms, &size);

        if (VX_SUCCESS == status)
        {
            tivxMemFree(prms, sizeof(tivxDLColorConvertArmv8KernelParams), TIVX_MEM_EXTERNAL);
        }
    }

    return (status);
}

#ifdef CHECK_PARAMS
static vx_status colorConvert_RGBtoNV12_i8u_o8u_checkParams(uint8_t src[],
                                              bufParams2D_t *src_prms,
                                              uint8_t dst0[],
                                              bufParams2D_t *dst0_prms,
                                              uint8_t dst1[],
                                              bufParams2D_t *dst1_prms)
{
    vx_status status = VX_SUCCESS;

    if((src == NULL) || (dst0 == NULL) || (dst1 == NULL) ) {
        status = VX_FAILURE;
    } else if( src_prms->dim_x != dst0_prms->dim_x ||
               src_prms->dim_y != dst0_prms->dim_y ||
               src_prms->stride_y < (3*src_prms->dim_x) ||
               dst0_prms->stride_y < (dst0_prms->dim_x) ) {
        status = VX_ERROR_INVALID_DIMENSION;
    }
    return (status);
}
#endif

static inline uint8x8_t rgb2y_vtr(uint8x8_t r, uint8x8_t g, uint8x8_t b)
{
    uint8x8_t y;
    uint16x8_t y_tmp, r_wide, g_wide, b_wide;

    r_wide = vmovl_u8(r);
    g_wide = vmovl_u8(g);
    b_wide = vmovl_u8(b);

    /* y' = 54*r + 183*g +  18*b + 128 */
    y_tmp = vaddq_u16(vaddq_u16(vaddq_u16(vmulq_n_u16(r_wide, 54),
                                          vmulq_n_u16(g_wide, 183)),
                                vmulq_n_u16(b_wide, 18)),
                       vdupq_n_u16(128));
    /* y = y'>>8
       and narrowing from 16 to 8 */
    y = vqshrn_n_u16(y_tmp, 8);

    return y;
}

static inline int16x8_t rgb2u_vtr(uint8x8_t r, uint8x8_t g, uint8x8_t b)
{
    int16x8_t u, r_wide, g_wide, b_wide;

    r_wide = vreinterpretq_s16_u16(vmovl_u8(r));
    g_wide = vreinterpretq_s16_u16(vmovl_u8(g));
    b_wide = vreinterpretq_s16_u16(vmovl_u8(b));

    /* u' = -15*r - 49*g + 64*b + 64*/
    u = vaddq_s16(vaddq_s16(vaddq_s16(vmulq_n_s16(r_wide, (-15)),
                                          vmulq_n_s16(g_wide, (-49))),
                                vmulq_n_s16(b_wide, 64)),
                       vdupq_n_s16(64));
    /* u' = u'>>7 */
    u = vshrq_n_s16(u, 7);
    /* u' = u'+128 */
    u = vaddq_s16(u, vdupq_n_s16(128));

    return u;
}

static inline int16x8_t rgb2v_vtr(uint8x8_t r, uint8x8_t g, uint8x8_t b)
{
    int16x8_t v, r_wide, g_wide, b_wide;

    r_wide = vreinterpretq_s16_u16(vmovl_u8(r));
    g_wide = vreinterpretq_s16_u16(vmovl_u8(g));
    b_wide = vreinterpretq_s16_u16(vmovl_u8(b));

    /* v' = 64*r - 58*g - 6*b + 64*/
    v = vaddq_s16(vaddq_s16(vaddq_s16(vmulq_n_s16(r_wide, 64),
                                          vmulq_n_s16(g_wide, (-58))),
                                vmulq_n_s16(b_wide, (-6))),
                       vdupq_n_s16(64));
    /* v' = v'>>7 */
    v = vshrq_n_s16(v, 7);
    /* v' = v'+128 */
    v = vaddq_s16(v, vdupq_n_s16(128));

    return v;
}

static vx_status colorConvert_RGBtoNV12_i8u_o8u_armv8(uint8_t src[],
                                  bufParams2D_t *src_prms,
                                  uint8_t dst0[],
                                  bufParams2D_t * dst0_prms,
                                  uint8_t dst1[],
                                  bufParams2D_t * dst1_prms)
{
    vx_status status = VX_SUCCESS;
    uint32_t x, y;

    #ifdef CHECK_PARAMS
    status = colorConvert_RGBtoNV12_i8u_o8u_checkParams(src,
                                                        src_prms, dst0, dst0_prms,
                                                        dst1, dst1_prms);
    if(status == VX_SUCCESS)
    #endif
    {
        for( y=0; y < dst0_prms->dim_y; y+=2 ) {

            uint8_t* srcRowPtr_0 = src + (y * src_prms->stride_y);
            uint8_t* srcRowPtr_1 = src + ((y+1) * src_prms->stride_y);
            uint8_t* dstRowPtr_y0 = dst0 + (y * dst0_prms->stride_y);
            uint8_t* dstRowPtr_y1 = dst0 + ((y+1) * dst0_prms->stride_y);
            uint8_t* dstRowPtr_uv = dst1 + (y * dst1_prms->stride_y / 2);

            for( x=0; x < (((dst0_prms->dim_x) >> 3) << 3); x+=8 ) {

                uint8x8x3_t RGB0, RGB1;
                /* Load RGB values from source */
                {
                    int srcColIdxBytes = x * 3;
                    RGB0 = vld3_u8(srcRowPtr_0 + srcColIdxBytes);
                    RGB1 = vld3_u8(srcRowPtr_1 + srcColIdxBytes);
                }

                uint8x8_t Y0 = rgb2y_vtr(RGB0.val[0],RGB0.val[1],RGB0.val[2]);
                uint8x8_t Y1 = rgb2y_vtr(RGB1.val[0],RGB1.val[1],RGB1.val[2]);

                int16x8_t U0 = rgb2u_vtr(RGB0.val[0],RGB0.val[1],RGB0.val[2]);
                int16x8_t U1 = rgb2u_vtr(RGB1.val[0],RGB1.val[1],RGB1.val[2]);

                int16x8_t V0 = rgb2v_vtr(RGB0.val[0],RGB0.val[1],RGB0.val[2]);
                int16x8_t V1 = rgb2v_vtr(RGB1.val[0],RGB1.val[1],RGB1.val[2]);

                /* Averaging U and V 
                   avg_U = (U00+U01+U10+U11)/4
                   avg_V = (V00+V01+V10+V11)/4 */

                /* Pairwise addition and widening - 
                   U00+U01, U02+U03, U04+U05, U06+U07
                   U10+U11, U12+U13, U14+U15, U16+U17 */
                int32x4_t U0_pair = vpaddlq_s16(U0);
                int32x4_t U1_pair = vpaddlq_s16(U1);
                int32x4_t V0_pair = vpaddlq_s16(V0);
                int32x4_t V1_pair = vpaddlq_s16(V1);

                /* (U00+U01+U10+U11) */
                int32x4_t U01_pair = vaddq_s32(U0_pair, U1_pair);
                int32x4_t V01_pair = vaddq_s32(V0_pair, V1_pair);

                /* (U00+U01+U10+U11) >> 2 */
                uint16x4_t avg_U = vqshrun_n_s32(U01_pair, 2);
                uint16x4_t avg_V = vqshrun_n_s32(V01_pair, 2);

                /* Interleaves the adjacent elements */
                uint16x4x2_t avg_UV_zip = vzip_u16(avg_U, avg_V); 

                uint8x8_t avg_UV = vqmovn_u16(vcombine_u16(avg_UV_zip.val[0], avg_UV_zip.val[1]));

                /* Store the result in destination */
                {
                    /* Store Y */
                    vst1_u8(dstRowPtr_y0+x, Y0);
                    vst1_u8(dstRowPtr_y1+x, Y1);

                    /* Store average U and V */
                    vst1_u8(dstRowPtr_uv+x, avg_UV);
                }
            }
            /* Process remaining pixels */
            for(; x < dst0_prms->dim_x; x+=2)
            {
                int srcColIdxBytes = x * 3;
                int dstColIdxBytes = x;

                uint8_t R00 = srcRowPtr_0[srcColIdxBytes+0];
                uint8_t G00 = srcRowPtr_0[srcColIdxBytes+1];
                uint8_t B00 = srcRowPtr_0[srcColIdxBytes+2];
                uint8_t R01 = srcRowPtr_0[srcColIdxBytes+3];
                uint8_t G01 = srcRowPtr_0[srcColIdxBytes+4];
                uint8_t B01 = srcRowPtr_0[srcColIdxBytes+5];

                uint8_t R10 = srcRowPtr_1[srcColIdxBytes+0];
                uint8_t G10 = srcRowPtr_1[srcColIdxBytes+1];
                uint8_t B10 = srcRowPtr_1[srcColIdxBytes+2];
                uint8_t R11 = srcRowPtr_1[srcColIdxBytes+3];
                uint8_t G11 = srcRowPtr_1[srcColIdxBytes+4];
                uint8_t B11 = srcRowPtr_1[srcColIdxBytes+5];

                uint8_t Y00 = RGB2Y(R00,G00,B00);
                uint8_t Y01 = RGB2Y(R01,G01,B01);
                uint8_t Y10 = RGB2Y(R10,G10,B10);
                uint8_t Y11 = RGB2Y(R11,G11,B11);

                uint8_t U00 = RGB2U(R00,G00,B00);
                uint8_t U01 = RGB2U(R01,G01,B01);
                uint8_t U10 = RGB2U(R10,G10,B10);
                uint8_t U11 = RGB2U(R11,G11,B11);

                uint8_t V00 = RGB2V(R00,G00,B00);
                uint8_t V01 = RGB2V(R01,G01,B01);
                uint8_t V10 = RGB2V(R10,G10,B10);
                uint8_t V11 = RGB2V(R11,G11,B11);

                uint8_t avg_U = (U00+U01+U10+U11)>>2;
                uint8_t avg_V = (V00+V01+V10+V11)>>2;

                dstRowPtr_y0[dstColIdxBytes]     = Y00;
                dstRowPtr_y0[dstColIdxBytes+1]   = Y01;
                dstRowPtr_y1[dstColIdxBytes]     = Y10;
                dstRowPtr_y1[dstColIdxBytes+1]   = Y11;
                dstRowPtr_uv[dstColIdxBytes]     = avg_U;
                dstRowPtr_uv[dstColIdxBytes+1]   = avg_V;
            }
        }
    }

    return status;
}

#ifdef CHECK_PARAMS
statis vx_status colorConvert_NVXXtoRGB_i8u_o8u_checkParams(uint8_t src0[],
                                                 bufParams2D_t * src0_prms,
                                                 uint8_t src1[],
                                                 bufParams2D_t * src1_prms,
                                                 uint8_t dst[],
                                                 bufParams2D_t * dst_prms)
{
    vx_status status = VX_SUCCESS;

    if((src0 == NULL) || (src1 == NULL) || (dst == NULL) ) {
        status = VX_FAILURE;
    } else if( src0_prms->dim_x != dst_prms->dim_x ||
               src0_prms->dim_y != dst_prms->dim_y ||
               src0_prms->stride_y < src0_prms->dim_x ||
               dst_prms->stride_y < (3*dst_prms->dim_x) ) {
        status = VX_ERROR_INVALID_DIMENSION;
    }
    return (status);
}
#endif

static inline uint8x8_t yuv2rgb_vtr(uint8x8_t y, int16x8_t calc)
{
    uint8x8_t rgb;
    int16x8_t y_tmp, rgb_tmp;

    /* Widening from 8x8 to 16x8
       and then uint16x8 to int16x8 */
    y_tmp = vreinterpretq_s16_u16(vmovl_u8(y));

    /* r = y + calc_v */
    rgb_tmp = vaddq_s16(y_tmp, calc);

    /* Saturating narrow from int16 to uint8 */
    rgb = vqmovun_s16(rgb_tmp);

    return rgb;
}

static vx_status colorConvert_NVXXtoRGB_i8u_o8u_armv8(uint8_t src0[],
                                     bufParams2D_t * src0_prms,
                                     uint8_t src1[],
                                     bufParams2D_t * src1_prms,
                                     uint8_t dst[],
                                     bufParams2D_t * dst_prms,
                                     uint8_t u_pix,
                                     uint8_t src_space)
{
    uint32_t    x, y;
    vx_status   status = VX_SUCCESS;
    uint8_t     v_pix = 1 - u_pix;

#ifdef CHECK_PARAMS
    status = colorConvert_NVXXtoRGB_i8u_o8u_checkParams(src0, src0_prms, src1, src1_prms, dst, dst_prms);
    if( status == VX_SUCCESS )
#endif
    {
        for( y=0; y < dst_prms->dim_y; y+=2 ) {

            /* src0 for y data */
            uint8_t* src0RowPtr_0 = src0 + (y * src0_prms->stride_y);
            uint8_t* src0RowPtr_1 = src0 + ((y+1) * src0_prms->stride_y);

            /* src1 for UV interleaved data */
            uint8_t* src1RowPtr = src1 + ((y/2) * src1_prms->stride_y);

            /* dst for RGB interleaved data */
            uint8_t* dstRowPtr_0 = dst + y * dst_prms->stride_y;
            uint8_t* dstRowPtr_1 = dst + ((y+1) * dst_prms->stride_y);

            for( x=0; x < (((dst_prms->dim_x) >> 3) << 3); x+=8 ) {

                uint8x8_t Y0, Y1;
                int32x4_t U_f, U_s, V_f, V_s;
                int16x4_t tmp_mid_f, tmp_mid_s;
                int16x8_t tmp_mid;
                uint8x8x3_t RGB0, RGB1;

                /* Load YUV values from both source pointers */
                {
                    int src0ColIdxBytes = x;
                    int src1ColIdxBytes = x;

                    /* For 4 y, only 1 u and 1 v are needed */

                    Y0 = vld1_u8(src0RowPtr_0 + src0ColIdxBytes);
                    Y1 = vld1_u8(src0RowPtr_1 + src0ColIdxBytes);

                    /* U_f will be used for first half of Y0 and Y1 */
                    int32_t u_ld = (int32_t) src1RowPtr[src1ColIdxBytes+u_pix+0];
                    U_f = vld1q_lane_s32(&u_ld, U_f, 0);
                    U_f = vld1q_lane_s32(&u_ld, U_f, 1);

                    u_ld = (int32_t) src1RowPtr[src1ColIdxBytes+u_pix+2];
                    U_f = vld1q_lane_s32(&u_ld, U_f, 2);
                    U_f = vld1q_lane_s32(&u_ld, U_f, 3);

                    /* U_s will be used for second half of Y0 and Y1 */
                    u_ld = (int32_t) src1RowPtr[src1ColIdxBytes+u_pix+4];
                    U_s = vld1q_lane_s32(&u_ld, U_s, 0);
                    U_s = vld1q_lane_s32(&u_ld, U_s, 1);

                    u_ld = (int32_t) src1RowPtr[src1ColIdxBytes+u_pix+6];
                    U_s = vld1q_lane_s32(&u_ld, U_s, 2);
                    U_s = vld1q_lane_s32(&u_ld, U_s, 3);

                    /* V_f will be used for first half of Y0 and Y1 */
                    int32_t v_ld = (int32_t) src1RowPtr[src1ColIdxBytes+v_pix+0];
                    V_f = vld1q_lane_s32(&v_ld, V_f, 0);
                    V_f = vld1q_lane_s32(&v_ld, V_f, 1);

                    v_ld = (int32_t) src1RowPtr[src1ColIdxBytes+v_pix+2];
                    V_f = vld1q_lane_s32(&v_ld, V_f, 2);
                    V_f = vld1q_lane_s32(&v_ld, V_f, 3);

                    /* V_s will be used for second half of Y0 and Y1 */
                    v_ld = (int32_t) src1RowPtr[src1ColIdxBytes+v_pix+4];
                    V_s = vld1q_lane_s32(&v_ld, V_s, 0);
                    V_s = vld1q_lane_s32(&v_ld, V_s, 1);

                    v_ld = (int32_t) src1RowPtr[src1ColIdxBytes+v_pix+6];
                    V_s = vld1q_lane_s32(&v_ld, V_s, 2);
                    V_s = vld1q_lane_s32(&v_ld, V_s, 3);
                }

                /* Conversion from YUV to RGB */
                if (src_space == COLOR_SPACE_BT601_525 ||
                            src_space == COLOR_SPACE_BT601_625)
                {
                    /* r = y + (22986 * v >> 14) - 179 */
                    /* calc = (22986 * v >> 14) - 179
                        These are Q14 format conversion formulae */
                    tmp_mid_f = vshrn_n_s32(vmulq_n_s32(V_f, 22986), 14);
                    tmp_mid_s = vshrn_n_s32(vmulq_n_s32(V_s, 22986), 14);

                    tmp_mid = vcombine_s16(tmp_mid_f, tmp_mid_s);
                    tmp_mid = vsubq_s16(tmp_mid, vdupq_n_s16(179));
                }
                else
                {
                    /* r = y + (25802 * v >> 14) - 202 */
                    /* calc = (25802 * v >> 14) - 202 */
                    tmp_mid_f = vshrn_n_s32(vmulq_n_s32(V_f, 25802), 14);
                    tmp_mid_s = vshrn_n_s32(vmulq_n_s32(V_s, 25802), 14);

                    tmp_mid = vcombine_s16(tmp_mid_f, tmp_mid_s);
                    tmp_mid = vsubq_s16(tmp_mid, vdupq_n_s16(202));
                }

                RGB0.val[0] = yuv2rgb_vtr(Y0, tmp_mid);
                RGB1.val[0] = yuv2rgb_vtr(Y1, tmp_mid);

                if (src_space == COLOR_SPACE_BT601_525 ||
                            src_space == COLOR_SPACE_BT601_625)
                {
                    /* g = y - ((5636 * u + 11698 * v)>>14) + 135 */
                    /* calc = 135 - ((5636 * u + 11698 * v)>>14) */
                    tmp_mid_f = vshrn_n_s32(vaddq_s32(vmulq_n_s32(U_f, 5636),
                                                      vmulq_n_s32(V_f, 11698)),
                                            14);
                    tmp_mid_s = vshrn_n_s32(vaddq_s32(vmulq_n_s32(U_s, 5636),
                                                      vmulq_n_s32(V_s, 11698)),
                                            14);

                    tmp_mid = vcombine_s16(tmp_mid_f, tmp_mid_s);
                    tmp_mid = vsubq_s16(vdupq_n_s16(135), tmp_mid);
                }
                else
                {
                    /* g = y - ((3069 * u + 7669 * v)>>14) + 84 */
                    /* calc_V = 84 - ((3069 * u + 7669 * v)>>14) */
                    tmp_mid_f = vshrn_n_s32(vaddq_s32(vmulq_n_s32(U_f, 3069),
                                                      vmulq_n_s32(V_f, 7669)),
                                            14);
                    tmp_mid_s = vshrn_n_s32(vaddq_s32(vmulq_n_s32(U_s, 3069),
                                                      vmulq_n_s32(V_s, 7669)),
                                            14);

                    tmp_mid = vcombine_s16(tmp_mid_f, tmp_mid_s);
                    tmp_mid = vsubq_s16(vdupq_n_s16(84), tmp_mid);
                }

                RGB0.val[1] = yuv2rgb_vtr(Y0, tmp_mid);
                RGB1.val[1] = yuv2rgb_vtr(Y1, tmp_mid);

                if (src_space == COLOR_SPACE_BT601_525 ||
                            src_space == COLOR_SPACE_BT601_625)
                {
                    /* b = y + (29048 * u >> 14) - 226 */
                    /* calc = (29048 * u >> 14) - 226 */
                    tmp_mid_f = vshrn_n_s32(vmulq_n_s32(U_f, 29048), 14);
                    tmp_mid_s = vshrn_n_s32(vmulq_n_s32(U_s, 29048), 14);

                    tmp_mid = vcombine_s16(tmp_mid_f, tmp_mid_s);
                    tmp_mid = vsubq_s16(tmp_mid, vdupq_n_s16(226));
                }
                else
                {
                    /* b = y + (30402 * u >> 14) - 237 */
                    /* calc = (30402 * u >> 14) - 237 */
                    tmp_mid_f = vshrn_n_s32(vmulq_n_s32(U_f, 30402), 14);
                    tmp_mid_s = vshrn_n_s32(vmulq_n_s32(U_s, 30402), 14);

                    tmp_mid = vcombine_s16(tmp_mid_f, tmp_mid_s);
                    tmp_mid = vsubq_s16(tmp_mid, vdupq_n_s16(237));
                }

                RGB0.val[2] = yuv2rgb_vtr(Y0, tmp_mid);
                RGB1.val[2] = yuv2rgb_vtr(Y1, tmp_mid);

                /* Store RGB values */
                {
                    int dstColIdxBytes = (x+0) * 3;
                    vst3_u8(dstRowPtr_0 + dstColIdxBytes, RGB0);
                    vst3_u8(dstRowPtr_1 + dstColIdxBytes, RGB1);
                }
            }
            /* Process remaining pixels */
            for(; x < dst_prms->dim_x; x+=2)
            {
                int src0ColIdxBytes = x;
                int src1ColIdxBytes = 2 * (x/2);
                int dstColIdxBytes  = x * 3;
                uint8_t R00, R01, R10, R11, G00, G01, G10, G11;
                uint8_t B00, B01, B10, B11;

                uint8_t Y00 = src0RowPtr_0[src0ColIdxBytes+0];
                uint8_t Y01 = src0RowPtr_0[src0ColIdxBytes+1];
                uint8_t Y10 = src0RowPtr_1[src0ColIdxBytes+0];
                uint8_t Y11 = src0RowPtr_1[src0ColIdxBytes+1];
                uint8_t U = src1RowPtr[src1ColIdxBytes+u_pix];
                uint8_t V = src1RowPtr[src1ColIdxBytes+v_pix];

                if (src_space == COLOR_SPACE_BT601_525 ||
                        src_space == COLOR_SPACE_BT601_625)
                {
                    R00 = YUV2R_BT601(Y00,U,V);
                    R01 = YUV2R_BT601(Y01,U,V);
                    R10 = YUV2R_BT601(Y10,U,V);
                    R11 = YUV2R_BT601(Y11,U,V);

                    G00 = YUV2G_BT601(Y00,U,V);
                    G01 = YUV2G_BT601(Y01,U,V);
                    G10 = YUV2G_BT601(Y10,U,V);
                    G11 = YUV2G_BT601(Y11,U,V);

                    B00 = YUV2B_BT601(Y00,U,V);
                    B01 = YUV2B_BT601(Y01,U,V);
                    B10 = YUV2B_BT601(Y10,U,V);
                    B11 = YUV2B_BT601(Y11,U,V);
                }
                else
                {
                    R00 = YUV2R_BT709(Y00,U,V);
                    R01 = YUV2R_BT709(Y01,U,V);
                    R10 = YUV2R_BT709(Y10,U,V);
                    R11 = YUV2R_BT709(Y11,U,V);

                    G00 = YUV2G_BT709(Y00,U,V);
                    G01 = YUV2G_BT709(Y01,U,V);
                    G10 = YUV2G_BT709(Y10,U,V);
                    G11 = YUV2G_BT709(Y11,U,V);

                    B00 = YUV2B_BT709(Y00,U,V);
                    B01 = YUV2B_BT709(Y01,U,V);
                    B10 = YUV2B_BT709(Y10,U,V);
                    B11 = YUV2B_BT709(Y11,U,V);
                }

                dstRowPtr_0[dstColIdxBytes+0] = R00;
                dstRowPtr_0[dstColIdxBytes+1] = G00;
                dstRowPtr_0[dstColIdxBytes+2] = B00;
                dstRowPtr_0[dstColIdxBytes+3] = R01;
                dstRowPtr_0[dstColIdxBytes+4] = G01;
                dstRowPtr_0[dstColIdxBytes+5] = B01;

                dstRowPtr_1[dstColIdxBytes+0] = R10;
                dstRowPtr_1[dstColIdxBytes+1] = G10;
                dstRowPtr_1[dstColIdxBytes+2] = B10;
                dstRowPtr_1[dstColIdxBytes+3] = R11;
                dstRowPtr_1[dstColIdxBytes+4] = G11;
                dstRowPtr_1[dstColIdxBytes+5] = B11;
            }
        }
    }

    return status;
}

#ifdef CHECK_PARAMS
statis vx_status colorConvert_NVXXtoIYUV_i8u_o8u_checkParams(uint8_t src0[],
                                                    bufParams2D_t *src0_prms,
                                                    uint8_t src1[],
                                                    bufParams2D_t *src1_prms,
                                                    uint8_t dst0[],
                                                    bufParams2D_t *dst0_prms,
                                                    uint8_t dst1[],
                                                    bufParams2D_t *dst1_prms,
                                                    uint8_t dst2[],
                                                    bufParams2D_t *dst2_prms)
{
    vx_status status = VX_SUCCESS;

    if((src0 == NULL) || (src1 == NULL) || (dst0 == NULL) || (dst1 == NULL) || (dst2 == NULL) ) {
        status = VX_FAILURE;
    } else if( src0_prms->dim_x != dst0_prms->dim_x ||
               src0_prms->dim_y != dst0_prms->dim_y ||
               src0_prms->stride_y < (src0_prms->dim_x) ||
               dst0_prms->stride_y < (dst0_prms->dim_x) ) {
        status = VX_ERROR_INVALID_DIMENSION;
    }
    return (status);
}
#endif

static vx_status colorConvert_NVXXtoIYUV_i8u_o8u_armv8(uint8_t src0[],
                                  bufParams2D_t *src0_prms,
                                  uint8_t src1[],
                                  bufParams2D_t *src1_prms,
                                  uint8_t dst0[],
                                  bufParams2D_t *dst0_prms,
                                  uint8_t dst1[],
                                  bufParams2D_t *dst1_prms,
                                  uint8_t dst2[],
                                  bufParams2D_t *dst2_prms,
                                  uint8_t u_pix)
{
    uint32_t    x, y, src0Index_row0, src0Index_row1, src1Index, dst0Index_row0, dst0Index_row1, dst1Index, dst2Index;
    vx_status   status = VX_SUCCESS;
    uint8_t     v_pix = 1 - u_pix;

#ifdef CHECK_PARAMS
    status = colorConvert_NVXXtoIYUV_i8u_o8u_checkParams(src0, src0_prms,
                src1, src1_prms, dst0, dst0_prms, dst1, dst1_prms, dst2, dst2_prms);
    if( status == VX_SUCCESS )
#endif
    {
        for( y=0; y < dst0_prms->dim_y; y+=2 ) {

            uint8_t* src0RowPtr_0 = src0 + y * src0_prms->stride_y;
            uint8_t* src0RowPtr_1 = src0 + (y+1) * src0_prms->stride_y;

            uint8_t* src1RowPtr = src1 + (y/2) * src1_prms->stride_y;

            uint8_t* dst0RowPtr_0 = dst0 + y * dst0_prms->stride_y;
            uint8_t* dst0RowPtr_1 = dst0 + (y+1) * dst0_prms->stride_y;

            uint8_t* dst1RowPtr = dst1 + (y/2) * dst1_prms->stride_y;
            uint8_t* dst2RowPtr = dst2 + (y/2) * dst2_prms->stride_y;

            for( x=0; x < (((dst0_prms->dim_x) >> 4) << 4); x+=16 ) {

                /* Transfer 8x16 of Y0 and 8x16 of Y1
                   and for 32 Y's we need 8 U's and 8 V'S */
                uint8x16_t Y0, Y1;
                uint8x8x2_t UV;

                /* Load the YUV values from NV12 data */
                {
                    int srcColIdxBytes = x;

                    Y0 = vld1q_u8(src0RowPtr_0 + srcColIdxBytes);
                    Y1 = vld1q_u8(src0RowPtr_1 + srcColIdxBytes);

                    UV = vld2_u8(src1RowPtr + srcColIdxBytes);
                }

                /* Store the YUV values in IYUV/I420 */
                {
                    int dst0ColIdxBytes = x;
                    int dst12ColIdxBytes = x/2;

                    vst1q_u8(dst0RowPtr_0 + dst0ColIdxBytes, Y0);
                    vst1q_u8(dst0RowPtr_1 + dst0ColIdxBytes, Y1);

                    if(!u_pix) /* NV12 format */
                    {
                        vst1_u8(dst1RowPtr + dst12ColIdxBytes, UV.val[0]);
                        vst1_u8(dst2RowPtr + dst12ColIdxBytes, UV.val[1]);
                    }
                    else /* NV21 format */
                    {
                        vst1_u8(dst1RowPtr + dst12ColIdxBytes, UV.val[1]);
                        vst1_u8(dst2RowPtr + dst12ColIdxBytes, UV.val[0]);
                    }
                }
            }
            /* Process remaining pixels */
            for(; x < dst0_prms->dim_x; x++ ) {
                src0Index_row0 = y * src0_prms->stride_y + x;
                src0Index_row1 = (y+1) * src0_prms->stride_y + x;
                src1Index = (y/2) * src1_prms->stride_y + 2*(x/2);
                dst0Index_row0 = y * dst0_prms->stride_y + x;
                dst0Index_row1 = (y+1) * dst0_prms->stride_y + x;
                dst1Index = (y/2) * dst1_prms->stride_y + (x/2);
                dst2Index = (y/2) * dst2_prms->stride_y + (x/2);

                dst0[dst0Index_row0] = src0[src0Index_row0];
                dst0[dst0Index_row1] = src0[src0Index_row1];
                dst1[dst1Index] = src1[src1Index + u_pix];
                dst2[dst2Index] = src1[src1Index + v_pix];
            }
        }
    }

    return status;
}

#ifdef CHECK_PARAMS
static vx_status colorConvert_IYUVtoNV12_i8u_o8u_checkParams(const uint8_t src0[],
                                                        bufParams2D_t *src0_prms,
                                                        uint8_t src1[],
                                                        bufParams2D_t *src1_prms,
                                                        uint8_t src2[],
                                                        bufParams2D_t *src2_prms,
                                                        uint8_t dst0[],
                                                        bufParams2D_t *dst0_prms,
                                                        uint8_t dst1[],
                                                        bufParams2D_t *dst1_prms)
{
    vx_status status = VX_SUCCESS;

    if((src0 == NULL) || (src1 == NULL) || (src2 == NULL) || (dst0 == NULL) || (dst1 == NULL) ) {
        status = VX_FAILURE;
    } else if( src0_prms->dim_x != dst0_prms->dim_x ||
               src0_prms->dim_y != dst0_prms->dim_y ||
               src0_prms->stride_y < (src0_prms->dim_x) ||
               dst0_prms->stride_y < (dst0_prms->dim_x) ) {
        status = VX_ERROR_INVALID_DIMENSION;
    }
    return (status);
}
#endif

static vx_status colorConvert_IYUVtoNV12_i8u_o8u_armv8(uint8_t src0[],
                                     bufParams2D_t * src0_prms,
                                     uint8_t src1[],
                                     bufParams2D_t * src1_prms,
                                     uint8_t src2[],
                                     bufParams2D_t * src2_prms,
                                     uint8_t dst0[],
                                     bufParams2D_t * dst0_prms,
                                     uint8_t dst1[],
                                     bufParams2D_t * dst1_prms)
{
    vx_status   status = VX_SUCCESS;
    uint32_t    x, y, src0Index_row0, src0Index_row1, src1Index, src2Index, dst0Index_row0, dst0Index_row1, dst1Index;

#ifdef CHECK_PARAMS
    status = colorConvert_IYUVtoNV12_i8u_o8u_checkParams(src0, src0_prms, src1, src1_prms,
                 src2, src2_prms, dst0, dst0_prms, dst1, dst1_prms);
    if( status == VX_SUCCESS )
#endif
    {
        for( y=0; y < dst0_prms->dim_y; y+=2 ) {

            uint8_t* src0RowPtr_0 = src0 + y * src0_prms->stride_y;
            uint8_t* src0RowPtr_1 = src0 + (y+1) * src0_prms->stride_y;

            uint8_t* src1RowPtr = src1 + (y/2) * src1_prms->stride_y;
            uint8_t* src2RowPtr = src2 + (y/2) * src2_prms->stride_y;

            uint8_t* dst0RowPtr_0 = dst0 + y * dst0_prms->stride_y;
            uint8_t* dst0RowPtr_1 = dst0 + (y+1) * dst0_prms->stride_y;

            uint8_t* dst1RowPtr = dst1 + (y/2) * dst1_prms->stride_y;

            for( x=0; x < (((dst0_prms->dim_x) >> 4) << 4); x+=16 ) {

                /* Transfer 8x16 of Y0 and 8x16 of Y1
                   and for 32 Y's we need 8 U's and 8 V'S */
                uint8x16_t Y0, Y1;
                uint8x8x2_t UV;

                /* Load the YUV values from IYUV/I420 data */
                {
                    int src0ColIdxBytes = x;
                    int src12ColIdxBytes = x/2;

                    Y0 = vld1q_u8(src0RowPtr_0 + src0ColIdxBytes);
                    Y1 = vld1q_u8(src0RowPtr_1 + src0ColIdxBytes);

                    UV.val[0] = vld1_u8(src1RowPtr + src12ColIdxBytes);
                    UV.val[1] = vld1_u8(src2RowPtr + src12ColIdxBytes);
                }

                /* Store the YUV values in NV12 */
                {
                    int dstColIdxBytes = x;

                    vst1q_u8(dst0RowPtr_0 + dstColIdxBytes, Y0);
                    vst1q_u8(dst0RowPtr_1 + dstColIdxBytes, Y1);

                    vst2_u8(dst1RowPtr + dstColIdxBytes, UV);
                }
            }
            /* Process remaining pixels */
            for(; x < dst0_prms->dim_x; x++ ) {
                src0Index_row0 = y * src0_prms->stride_y + x;
                src0Index_row1 = (y+1) * src0_prms->stride_y + x;
                src1Index = (y/2) * src1_prms->stride_y + (x/2);
                src2Index = (y/2) * src2_prms->stride_y + (x/2);
                dst0Index_row0 = y * dst0_prms->stride_y + x;
                dst0Index_row1 = (y+1) * dst0_prms->stride_y + x;
                dst1Index = (y/2) * dst1_prms->stride_y + 2*(x/2);

                dst0[dst0Index_row0] = src0[src0Index_row0];
                dst0[dst0Index_row1] = src0[src0Index_row1];
                dst1[dst1Index] = src1[src1Index];
                dst1[dst1Index + 1] = src2[src2Index];
            }
        }
    }
    return status;
}

static vx_status dl_color_convert_process
(
    tivxDLColorConvertArmv8KernelParams *prms,
    tivx_obj_desc_image_t *in_img_desc,
    tivx_obj_desc_image_t *out_img_desc,
    void *in_image_target_ptr[TIVX_IMAGE_MAX_PLANES],
    void *out_image_target_ptr[TIVX_IMAGE_MAX_PLANES]
)
{
    vx_status status = VX_SUCCESS;

    if (((vx_df_image)VX_DF_IMAGE_RGB == in_img_desc->format) && ((vx_df_image)VX_DF_IMAGE_NV12 == out_img_desc->format))
    {
        status = colorConvert_RGBtoNV12_i8u_o8u_armv8((uint8_t *)in_image_target_ptr[0],
            &prms->etn_src[0], (uint8_t *)out_image_target_ptr[0], &prms->etn_dst[0],
            (uint8_t *)out_image_target_ptr[1], &prms->etn_dst[1]);
    }
    else if ( ( ((vx_df_image)VX_DF_IMAGE_NV12 == in_img_desc->format) || ((vx_df_image)VX_DF_IMAGE_NV21 == in_img_desc->format) ) &&
                ((vx_df_image)VX_DF_IMAGE_RGB == out_img_desc->format))
    {
        uint8_t u_pix = (in_img_desc->format == (vx_df_image)VX_DF_IMAGE_NV12) ? 0U : 1U;
        status = colorConvert_NVXXtoRGB_i8u_o8u_armv8((uint8_t *)in_image_target_ptr[0], &prms->etn_src[0],
            (uint8_t *)in_image_target_ptr[1], &prms->etn_src[1], (uint8_t *)out_image_target_ptr[0], &prms->etn_dst[0], u_pix, in_img_desc->color_space);
    }
    else if ( ( ((vx_df_image)VX_DF_IMAGE_NV12 == in_img_desc->format) || ((vx_df_image)VX_DF_IMAGE_NV21 == in_img_desc->format) ) &&
                ((vx_df_image)VX_DF_IMAGE_IYUV == out_img_desc->format))
    {
        uint8_t u_pix = (in_img_desc->format == (vx_df_image)VX_DF_IMAGE_NV12) ? 0U : 1U;

        status = colorConvert_NVXXtoIYUV_i8u_o8u_armv8((uint8_t *)in_image_target_ptr[0], &prms->etn_src[0],
            (uint8_t *)in_image_target_ptr[1], &prms->etn_src[1], (uint8_t *)out_image_target_ptr[0], &prms->etn_dst[0], (uint8_t *)out_image_target_ptr[1],
            &prms->etn_dst[1], (uint8_t *)out_image_target_ptr[2], &prms->etn_dst[2], u_pix);
    }
    else if (((vx_df_image)VX_DF_IMAGE_IYUV == in_img_desc->format) && ((vx_df_image)VX_DF_IMAGE_NV12 == out_img_desc->format))
    {
        status = colorConvert_IYUVtoNV12_i8u_o8u_armv8((uint8_t *)in_image_target_ptr[0], &prms->etn_src[0],
            (uint8_t *)in_image_target_ptr[1], &prms->etn_src[1], (uint8_t *)in_image_target_ptr[2], &prms->etn_src[2],
            (uint8_t *)out_image_target_ptr[0], &prms->etn_dst[0], (uint8_t *)out_image_target_ptr[1], &prms->etn_dst[1]);
    }
    else
    {
        status = (vx_status)VX_FAILURE;
    }

    if (VX_SUCCESS != status)
    {
        status = (vx_status)VX_FAILURE;
    }

    return status;
}


static vx_status VX_CALLBACK tivxKernelDLColorConvertArmv8Process(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg)
{
    vx_status status = VX_SUCCESS;

    tivxDLColorConvertArmv8KernelParams *prms = NULL;
    vx_int32 i;

    for (i = 0U; i < num_params; i ++)
    {
        if (NULL == obj_desc[i])
        {
            status = VX_FAILURE;
            break;
        }
    }

    if(status==VX_SUCCESS)
    {
        uint32_t size;

        status = tivxGetTargetKernelInstanceContext(kernel,
            (void **)&prms, &size);
        if ((VX_SUCCESS != status) || (NULL == prms) ||
            (sizeof(tivxDLColorConvertArmv8KernelParams) != size))
        {
            status = VX_FAILURE;
        }
    }

    if (VX_SUCCESS == status)
    {
        tivx_obj_desc_image_t *in_img_desc;
        void* in_image_target_ptr[TIVX_IMAGE_MAX_PLANES] = {NULL, NULL, NULL};

        tivx_obj_desc_image_t *out_img_desc;
        void* out_image_target_ptr[TIVX_IMAGE_MAX_PLANES] = {NULL, NULL, NULL};

        vx_int32 i;

        in_img_desc  = (tivx_obj_desc_image_t *)obj_desc[TIVX_DL_COLOR_CONVERT_ARMV8_INPUT_IDX];
        for (i = 0; i < in_img_desc->planes; i++)
        {
            if(in_img_desc->mem_ptr[i].shared_ptr != 0)
            {
                in_image_target_ptr[i]  = (void *)in_img_desc->mem_ptr[i].host_ptr;
                tivxMemBufferMap(in_image_target_ptr[i], in_img_desc->mem_size[i], VX_MEMORY_TYPE_HOST, VX_READ_ONLY);
            }
        }

        out_img_desc  = (tivx_obj_desc_image_t *)obj_desc[TIVX_DL_COLOR_CONVERT_ARMV8_OUTPUT_IDX];
        for (i = 0; i < out_img_desc->planes; i++)
        {
            if(out_img_desc->mem_ptr[i].shared_ptr != 0)
            {
                out_image_target_ptr[i]  = (void *)out_img_desc->mem_ptr[i].host_ptr;
                tivxMemBufferMap(out_image_target_ptr[i], out_img_desc->mem_size[i], VX_MEMORY_TYPE_HOST, VX_WRITE_ONLY);
            }
        }

        dl_color_convert_process
        (
            prms,
            in_img_desc,
            out_img_desc,
            in_image_target_ptr,
            out_image_target_ptr
        );

        for (i = 0; i < in_img_desc->planes; i++)
        {
            if (in_image_target_ptr[i] != NULL)
            {
                tivxMemBufferUnmap(in_image_target_ptr[i], in_img_desc->mem_size[i], VX_MEMORY_TYPE_HOST, VX_READ_ONLY);
            }
        }

        for (i = 0; i < out_img_desc->planes; i++)
        {
            if (out_image_target_ptr[i] != NULL)
            {
                tivxMemBufferUnmap(out_image_target_ptr[i], out_img_desc->mem_size[i], VX_MEMORY_TYPE_HOST, VX_WRITE_ONLY);
            }
        }
    }

    tivxSetTargetKernelInstanceContext(kernel, prms,  sizeof(tivxDLColorConvertArmv8KernelParams));

    return (status);
}

void tivxAddTargetKernelDLColorConvertArmv8(void)
{
    vx_status status = (vx_status)VX_FAILURE;
    char target_name[TIVX_TARGET_MAX_NAME];
    vx_enum self_cpu;

    self_cpu = tivxGetSelfCpuId();

    if (self_cpu == TIVX_CPU_ID_A72_0)
    {
        strncpy(target_name, TIVX_TARGET_A72_0, TIVX_TARGET_MAX_NAME);
        status = (vx_status)VX_SUCCESS;
    }

    if( (vx_status)VX_SUCCESS == status)
    {
        vx_dl_color_convert_armv8_target_kernel = tivxAddTargetKernelByName
                                                  (
                                                      TIVX_KERNEL_DL_COLOR_CONVERT_ARMV8_NAME,
                                                      target_name,
                                                      tivxKernelDLColorConvertArmv8Process,
                                                      tivxKernelDLColorConvertArmv8Create,
                                                      tivxKernelDLColorConvertArmv8Delete,
                                                      NULL,
                                                      NULL
                                                  );
    }
}

void tivxRemoveTargetKernelDLColorConvertArmv8(void)
{
    vx_status status = VX_SUCCESS;

    status = tivxRemoveTargetKernel(vx_dl_color_convert_armv8_target_kernel);
    if (status == VX_SUCCESS)
    {
        vx_dl_color_convert_armv8_target_kernel = NULL;
    }
}
