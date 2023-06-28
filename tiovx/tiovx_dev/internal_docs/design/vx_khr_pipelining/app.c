/*
 * Copyright (c) 2012-2017 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
 * KHRONOS STANDARDS. THE UNMODIFIED, NORMATIVE VERSIONS OF KHRONOS
 * SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT
 *    https://www.khronos.org/registry/
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

/*
 * This file has sample API usage of pipelining extention API
 *
 * NOTE: the implementation in this file will work even on a
 *       vendor implementation which does not support pipelining.
 *       No change in below implementation is required.
 *
 */

#include <VX/vx.h>
#include <vx_khr_pipelining.h>
#include <vx_import.h>
#include <vx_khr_ix.h>
#include <app.h>
#include <stdlib.h>

/* Vendor specific data structre to contruct IX object in memory */
#define BUFFER_IX_IMAGE_MAX_PLANES (3u)
typedef struct
{
    vx_enum type;
    vx_imagepatch_addressing_t image_addr[BUFFER_IX_IMAGE_MAX_PLANES];
    void *ptrs[BUFFER_IX_IMAGE_MAX_PLANES];
    vx_df_image color;
    vx_enum memory_type;
} BufferIXImage;

/* V4L2 specific APIs for mapping buffers, getting device/buffer properties */
struct v4l2_app_buffer {
    void   *start;
    size_t  length;
};
void MapV4L2DeviceMemory(int fd, struct v4l2_app_buffer *, uint32_t num_bufs);
void GetV4L2BufferProperties(int fd, BufferIXImage *);

/* Vendor specific API to contruct IX object in memory */
void BufferIXAlloc(void **buf_handle_mem_ptr,
                    vx_uint32 *buf_handle_mem_size,
                    vx_uint32 num_entries,
                    vx_enum ref_types[]
                    )
{
    vx_uint32 mem_size, i, offset;
    void *mem_addr;

    /* The Buffer IX memory consists of  below fields
     * 4B : Buffer IX object identifier (0x12345678)
     * 4B : Number of entries
     * 4B : Offset to entry index 0
     * 4B : Offset to entry index 1
     * ..
     * 4B : Offset to entry index (Number of entries-1)
     * 0..N B: Data for entry 0 (N varies for each entry and is function of object type)
     * 0..N B: Data for entry 1
     * ..
     * 0..N B: Data for entry (Number of entries-1)
     */
    mem_size = 2*sizeof(vx_uint32) + (num_entries)*sizeof(vx_uint32);

    for(i=0; i<num_entries; i++)
    {
        if(ref_types[i]==(vx_enum)VX_TYPE_IMAGE)
            mem_size += sizeof(BufferIXImage);
        /* similar logic for other data types */
    }

    mem_addr = malloc(mem_size);

    /* fill header, and offset values of each entry */
    *((vx_uint32*)mem_addr + 0) = 0x12345678;
    *((vx_uint32*)mem_addr + 1) = num_entries;
    /* offset of first entry */
    offset = 2*sizeof(vx_uint32) + (num_entries)*sizeof(vx_uint32);
    for(i=0; i<num_entries; i++)
    {
        /* fill offset in header */
        *((vx_uint32*)mem_addr + 2 + i) = offset;
        /* calc offset of next entry */
        if(ref_types[i]==(vx_enum)VX_TYPE_IMAGE)
            offset += sizeof(BufferIXImage);
        /* similar logic for other data types */
    }

    *buf_handle_mem_ptr = mem_addr;
    *buf_handle_mem_size = mem_size;
}

/* Vendor specific API to contruct IX object in memory */
void BufferIXAddImageType(void *mem_addr,
            vx_uint32 mem_size,
            vx_uint32 entry_index,
            BufferIXImage *image_buf_properties)
{
    vx_uint32 offset;

    /* find offset of this entry in Buffer IX object */
    offset = *((vx_uint32*)mem_addr + 2 + entry_index);

    /* copy information at offset location */
    memcpy(
        (void*)((vx_uint32*)mem_addr + offset),
        (void*)image_buf_properties,
        sizeof(BufferIXImage)
        );
}

/* Vendor specific API to allocate import/export data structure in memory */
void AllocBufferIXMemory(
        void **buf_handle_mem_ptr,
        vx_uint32 *buf_handle_mem_size,
        int v4l2_dev,
        vx_uint32 num_bufs
        )
{
    vx_enum *ref_types = malloc(num_bufs*sizeof(vx_enum));
    struct v4l2_app_buffer *v4l2_app_buffers = malloc(num_bufs*sizeof(struct v4l2_app_buffer));
    BufferIXImage buffer_ix_image;
    vx_uint32 i;

    /*
     * See sample logic for this API at
     * https://linuxtv.org/downloads/v4l-dvb-apis/uapi/v4l/capture.c.html
     */
    MapV4L2DeviceMemory(v4l2_dev, v4l2_app_buffers, num_bufs);

    /* fill properties of image buffer in memory based on V4L2 device properties
     * ex, color format, stride, dim_x, dim_y etc
     */
    GetV4L2BufferProperties(v4l2_dev, &buffer_ix_image);

    for(i=0; i<num_bufs; i++)
        ref_types[i] = (vx_enum)VX_TYPE_IMAGE;

    BufferIXAlloc(buf_handle_mem_ptr, buf_handle_mem_size, num_bufs, ref_types);

    for(i=0; i<num_bufs; i++)
    {
        /* set buffer memory address */
        buffer_ix_image.ptrs[0] = v4l2_app_buffers[i].start;

        /* insert entry into buffer IX object */
        BufferIXAddImageType(*buf_handle_mem_ptr, *buf_handle_mem_size,
            i,
            &buffer_ix_image
            );
    }

    /* below alloc'ed data structures not required any more so free them */
    free(ref_types);
    free(v4l2_app_buffers);
}

void FreeBufferIXMemory(
        void **buf_handle_mem_ptr,
        vx_uint32 buf_handle_mem_size)
{
    free(*buf_handle_mem_ptr);
    *buf_handle_mem_ptr=NULL;
}
