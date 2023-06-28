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
#include "app_common.h"
#include "tivx_utils_checksum.h"

/* checksums for the confidence image and the flow vector image
    confidence image (at least on plane 0) remains constant through
    the video whereas field vectore changes */
static vx_bool test_result = vx_true_e;

/* Some demos exhibit inconsistent behavior in the
    last few frames of execution - the TEST_BUFFER
    is used to prevent those checksums from being
    calculated */
#define TEST_BUFFER 10
#define NUM_CHECKSUM_SETS 2
#define NUM_CHECKSUM_SAMPLES 16
static vx_uint32 checksums_expected[NUM_CHECKSUM_SETS][NUM_CHECKSUM_SAMPLES] = {
  {0xa856f1b3,0x82a32054,0x82a32054,0x008c344b,
  0x7b8f6bdc,0x29e55c3f,0xb79b068f,0x13f29088,
  0x85f07e88,0x664290ba,0x1d292650,0x29728ae1,
  0xa97ca4da,0xd75ef670,0xdcf2a831,0x64684eae},
  {0xa856f1b3,0x82a32054,0x82a32054,0x008c344b,
  0x7b8f6bdc,0x29e55c3f,0xb79b068f,0x13f29088,
  0x85f07e88,0x664290ba,0x1d292650,0x29728ae1,
  0xa97ca4da,0xd75ef670,0xdcf2a831,0x64684eae}
};


/* use this to get new checksums if the input data
    changes or something about the logic changes */
static vx_uint32 checksums_gatherer[NUM_CHECKSUM_SETS][NUM_CHECKSUM_SAMPLES];
/* checksums_gatherer should be populated in the portion of the code
    where actual checksums are checked against */
static void print_new_checksum_structs()
{
    printf("\nHere is the construct for the new expected checksums: \n\n");
    printf("static vx_uint32 checksums_expected[NUM_CHECKSUM_SETS][NUM_CHECKSUM_SAMPLES] = {\n  ");
    for (int j=0; j<NUM_CHECKSUM_SETS; j++)
    {
        int i = 0;
        printf("{");
        while (i < (NUM_CHECKSUM_SAMPLES-1))
        {
            if (((i%4)==0) && (i!=0))
            {
                printf("\n  ");
            }
            if (checksums_gatherer[j][i] != checksums_expected[j][i])
            {
                printf("0x%08x,", checksums_gatherer[j][i]);
            }
            else
            {
                printf("0x%08x,", checksums_expected[j][i]);
            }
            i++;
        }
        if (j < NUM_CHECKSUM_SETS-1)
        {
            if (checksums_gatherer[j][i] != checksums_expected[j][i])
            {
                printf("0x%08x},\n  ", checksums_gatherer[j][i]);
            }
            else
            {
                printf("0x%08x},\n  ", checksums_expected[j][i]);
            }
        }
        else
        {
            if (checksums_gatherer[j][i] != checksums_expected[j][i])
            {
                printf("0x%08x}\n", checksums_gatherer[j][i]);
            }
            else
            {
                printf("0x%08x}\n", checksums_expected[j][i]);
            }
        }
    }
    printf("};\n\n");
}

static void populate_gatherer(vx_uint32 set_idx, vx_uint32 sample_idx, vx_uint32 calculated_checksum)
{
    checksums_gatherer[set_idx][sample_idx] = calculated_checksum;
}


static vx_bool app_test_check_image(vx_image img, vx_uint32 width,
                    vx_uint32 height, vx_uint32 expected_checksum,
                    vx_uint32 *actual_checksum)
{
    vx_rectangle_t rect;
    vx_bool return_bool = vx_false_e;
    rect.start_y = 0;
    rect.start_x = 0;
    rect.end_x = width;
    rect.end_y = height;
    *actual_checksum = tivx_utils_simple_image_checksum(img, 0, rect);
    APP_PRINTF("calculated checksum = 0x%08x vs. expected = 0x%08x\n",
                *actual_checksum, expected_checksum);

    if (*actual_checksum != expected_checksum)
    {
        return_bool = vx_false_e;
    }
    else
    {
        return_bool = vx_true_e;
    }
    return return_bool;
}