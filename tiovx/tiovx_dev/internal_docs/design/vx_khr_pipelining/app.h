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
#ifndef _APP_H_
#define _APP_H_

#ifdef  __cplusplus
extern "C" {
#endif

/* This file has stib APIs and vendor specific API place holders
 * Actual implementation of these API will be vendor specific or
 * application specific
 */

/* Partial V4L2'ish API interface */
/* ioctl ID to dequeue buffer form V4L2 device */
#define VIDIOC_DQBUF    (1)
/* ioctl ID to queue buffer form V4L2 device */
#define VIDIOC_QBUF     (2)
/* data structure to represent V4L2 buffer */
struct v4l2_buffer {
    int index;
};

/* Open and start V4L2 device - can be camera or display */
void OpenV4l2Device(int *fd);
/* Stop and close V4L2 device - can be camera or display */
void CloseV4l2Device(int *fd);
/* V4L2 ioctl used to Q, DQ V4L2 buffers */
int ioctl(int fd, int cmd, void *prm);
/* Set Callback on device, for output device this callback is called
 * when buffer is displayed and needs to be returned to application
 */
void SetV2l2DeviceCallback(int fd, void *user_node_sink_callback,  void *parameters);

/* API to check if application loop should exit or not */
vx_bool DoExit();

/* Vendor specific API to allocate import/export data structure in memory */
void AllocBufferIXMemory(
        void **buf_handle_mem_ptr,
        vx_uint32 *buf_handle_mem_size,
        int v4l2_dev,
        vx_uint32 num_bufs
        );
/* Vendor specific API to free data structure allocated by AllocImageIXMemory */
void FreeBufferIXMemory(
        void **buf_handle_mem_ptr,
        vx_uint32 buf_handle_mem_size);

#ifdef  __cplusplus
}
#endif

#endif
