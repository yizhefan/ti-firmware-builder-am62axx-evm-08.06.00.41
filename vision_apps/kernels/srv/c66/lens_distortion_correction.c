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

#include "TI/tivx_srv.h"
#include "lens_distortion_correction.h"
#include "srv_common.h"
#include <math.h>
#include <stdio.h>




 LDC_status LDC_UndistToDist(LensDistortionCorrection* ldc, dtype point_in[2], dtype point_out[2])
{
#if LDC_LIB_DATA_TYPE!=0 && LDC_LIB_DATA_TYPE!=1
        "LDC_LIB_DATA_TYPE must be 0 (float) or 1 (double) in lens_distortion_correction.h"
#endif
#if LDC_LIB_U2D_LUT_TYPE!=0 && LDC_LIB_U2D_LUT_TYPE!=1 && LDC_LIB_U2D_LUT_TYPE!=2
        "LDC_LIB_U2D_LUT_TYPE must be 0, 1 or 2"
#endif

        LDC_status status;
        dtype diffX, diffY;
        dtype lut_in_val;
        dtype lut_out_val;

        diffX = point_in[0] - ldc->distCenterX;
        diffY = point_in[1] - ldc->distCenterY;

#if LDC_LIB_U2D_LUT_TYPE == 0 || LDC_LIB_U2D_LUT_TYPE == 2
        dtype ru;
        #if LDC_LIB_DATA_TYPE==0
                ru = sqrtf(diffX*diffX + diffY*diffY);
                lut_in_val = atanf(ru*ldc->distFocalLengthInv);
        #elif LDC_LIB_DATA_TYPE==1
                ru = sqrt(diffX*diffX + diffY*diffY);
                lut_in_val = atan(ru*ldc->distFocalLengthInv);
        #endif
#elif LDC_LIB_U2D_LUT_TYPE == 1
        #if LDC_LIB_DATA_TYPE==0
                lut_in_val = sqrtf(diffX*diffX + diffY*diffY);
        #elif LDC_LIB_DATA_TYPE==1
                lut_in_val = sqrt(diffX*diffX + diffY*diffY);
        #endif
#endif

        lut_out_val = lut_lookup_floating(ldc->lut_u2d, lut_in_val, ldc->lut_u2d_indMax, ldc->lut_u2d_stepInv, &status);

#if LDC_LIB_U2D_LUT_TYPE == 0 || LDC_LIB_U2D_LUT_TYPE == 1
        point_out[0] = diffX * lut_out_val + ldc->distCenterX;
        point_out[1] = diffY * lut_out_val + ldc->distCenterY;
#elif LDC_LIB_U2D_LUT_TYPE == 2
        if (ru==0)
        {
                point_out[0] = ldc->distCenterX;
                point_out[1] = ldc->distCenterY;
        }
        else
        {
                point_out[0] = lut_out_val * diffX / ru + ldc->distCenterX;
                point_out[1] = lut_out_val * diffY / ru + ldc->distCenterY;
        }
#endif

        return status;
}


#if LDC_LIB_DATA_TYPE<2
 dtype lut_lookup_floating(dtype *lut, dtype inval, int32_t indMax, dtype stepInv, LDC_status *status)
{
        *status = LDC_STATUS_OK;
        dtype ind = inval * stepInv;
        if (ind >= (dtype)indMax)
        {
                *status = LDC_STATUS_FAIL;
                return lut[indMax];
        }


#if LDC_LIB_INTERPOLATION_METHOD==0
        return lut[(int32_t)ind];
#elif LDC_LIB_INTERPOLATION_METHOD==1
        return lut[(int32_t)(ind + 0.5)];
#elif LDC_LIB_INTERPOLATION_METHOD==2
        int32_t N = (int32_t)ind;
        dtype indMinN = ind - (dtype)N;
        return (1.0f - indMinN)*lut[N] + indMinN * lut[N + 1];
#endif
}
#endif


