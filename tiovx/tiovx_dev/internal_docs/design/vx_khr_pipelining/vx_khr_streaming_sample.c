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

/*
 * New OpenVX APIs
 */
VX_API_ENTRY vx_status vxEnqueueFreeHandle(vx_reference data_ref, vx_reference buf_hndl);
VX_API_ENTRY vx_status vxDequeueFreeHandle(vx_reference data_ref, vx_reference *buf_hndl);
VX_API_ENTRY vx_status vxEnqueueFullHandle(vx_reference data_ref, vx_reference buf_hndl);
VX_API_ENTRY vx_status vxDequeueFullHandle(vx_reference data_ref, vx_reference *buf_hndl);

/* register a callback to call when a event occurs */
typedef void (*vx_event_callback_f)(vx_reference ref, vx_enum event_type);
VX_API_ENTRY vx_status VX_API_CALL vxRegisterEventCallback(vx_reference ref, vx_enum event_type, vx_event_callback_f callback);

/* context which holds all openvx objects */
static vx_context context;
/* input image data object, generated by source node */
static vx_image d0;
/* intermediate virtual image data object */
static vx_image d1;
/* output image data object, input to sink node */
static vx_image d2;
/* scalar data object for convert depth node */
static vx_scalar s0;
/* node for channel extract node */
static vx_node n0;
/* node for convert depth node */
static vx_node n1;
/* graph containing n0, n1, d0, d1, d2 and node_source, node_sink */
static vx_graph g0;

/* number of buffers within input data objects */
#define MAX_IN_BUFS      (3u)
/* import data object obtained after vxImportObjectsFromMemory() for input buffer handles */
static vx_import in_buf_import;
/* input buffer handles */
vx_reference in_buf[MAX_IN_BUFS];
/* vendor specific data structure to import buffer handles */
static void *in_buf_handle_mem_ptr;
/* size of memory pointed to by in_buf_handle_mem_ptr */
static vx_uint32 in_buf_handle_mem_size;

/* number of buffers within output data objects */
#define MAX_OUT_BUFS     (3u)
/* import data object obtained after vxImportObjectsFromMemory() for output buffer handles */
static vx_import out_buf_import;
/* input buffer handles */
vx_reference out_buf[MAX_OUT_BUFS];
/* vendor specific data structure to import buffer handles */
static void *out_buf_handle_mem_ptr;
/* size of memory pointed to by out_buf_handle_mem_ptr */
static vx_uint32 out_buf_handle_mem_size;

/* source kernel handle */
static vx_kernel user_node_source_kernel;
/* source kernel id */
static vx_enum user_node_source_kernel_id;
/* source kernel node */
static vx_node node_source;

/* sink kernel handle */
static vx_kernel user_node_sink_kernel;
/* sink kernel id */
static vx_enum user_node_sink_kernel_id;
/* sink kernel node */
static vx_node node_sink;

/* V4L2 camera device file handle */
static int v4l2_input_dev = -1;
/* V4L2 display device file handle */
static int v4l2_output_dev = -1;

/* user source, sink node callback functoins */
static vx_status user_node_source_callback(vx_reference parameter, vx_enum event_type);
static vx_status user_node_sink_callback(vx_reference parameter);

/* import buffer handles for input data objects */
static void create_in_buf_handles()
{
    /* vendor specific API to create data structure which will be input to vxImportObjectsFromMemory()
     * to create buffer handles which will later be used to enqueue and dequeue from data objects
     */
    AllocBufferIXMemory(&in_buf_handle_mem_ptr, &in_buf_handle_mem_size, v4l2_input_dev, MAX_IN_BUFS);

    /* import all input handles with a single API call */
    in_buf_import = vxImportObjectsFromMemory(
                context,
                MAX_IN_BUFS,
                &in_buf[0],
                NULL, /* 'uses' parameter is unused and NULL is passed */
                in_buf_handle_mem_ptr,
                in_buf_handle_mem_size
            );

    /* we dont need import handle, and IX memory any more so free it */
    vxReleaseImport(&in_buf_import);
    FreeBufferIXMemory(&in_buf_handle_mem_ptr, in_buf_handle_mem_size);
}

/* import buffer handles for output data objects */
static void create_out_buf_handles()
{
    /* vendor specific API to create data structure which will be input to vxImportObjectsFromMemory()
     * to create buffer handles which will later be used to enqueue and dequeue from data objects
     */
    AllocBufferIXMemory(&out_buf_handle_mem_ptr, &out_buf_handle_mem_size, v4l2_output_dev, MAX_OUT_BUFS);

    /* import all input handles with a single API call */
    out_buf_import = vxImportObjectsFromMemory(
                context,
                MAX_OUT_BUFS,
                &out_buf[0],
                NULL, /* 'uses' parameter is unused and NULL is passed */
                out_buf_handle_mem_ptr,
                out_buf_handle_mem_size
            );

    /* we dont need import handle, and IX memory any more so free it */
    vxReleaseImport(&out_buf_import);
    FreeBufferIXMemory(&out_buf_handle_mem_ptr, out_buf_handle_mem_size);
}

/* free objects and memory created/allocated during create_in_buf_handles() */
static void release_in_buf_handles()
{
    vx_uint32 i;

    for(i=0; i<MAX_IN_BUFS; i++)
    {
        vxReleaseReference(&in_buf[i]);
    }
}

/* free objects and memory created/allocated during create_out_buf_handles() */
static void release_out_buf_handles()
{
    vx_uint32 i;

    for(i=0; i<MAX_OUT_BUFS; i++)
    {
        vxReleaseReference(&out_buf[i]);
    }
}

static int find_v4l2_buf_index(vx_reference ref[], vx_uint32 num_ref, vx_reference buf_handle)
{
    int v4l2_buf_index = -1;
    int i;

    for(i=0; i<num_ref; i++)
    {
        if(ref[i]==buf_handle)
        {
            v4l2_buf_index = i;
            break;
        }
    }
    return v4l2_buf_index;
}

/* wait for buffer from camera or display device, import into buffer handle and return it */
static void get_buf(int v4l2_dev, vx_reference *buf_handle)
{
    struct v4l2_buffer v4l2_buf;

    /* wait for buffer from camera or display device */
    ioctl(v4l2_dev, VIDIOC_DQBUF, &v4l2_buf);

    /* find handle associated with this V4L2 buffer */
    *buf_handle = NULL;
    if(v4l2_dev==v4l2_input_dev)
    {
        *buf_handle = in_buf[v4l2_buf.index];
    }
    if(v4l2_dev==v4l2_output_dev)
    {
        *buf_handle = out_buf[v4l2_buf.index];
    }
}

/* release buffer handle back to camera or display device */
static void put_buf(int v4l2_dev, vx_reference buf_handle)
{
    struct v4l2_buffer v4l2_buf;

    /* find V4L2 buffer associated with this handle*/
    if(v4l2_dev==v4l2_input_dev)
    {
        v4l2_buf.index = find_v4l2_buf_index(in_buf, MAX_IN_BUFS, buf_handle);
    }
    if(v4l2_dev==v4l2_output_dev)
    {
        v4l2_buf.index = find_v4l2_buf_index(out_buf, MAX_OUT_BUFS, buf_handle);
    }

    /* queue or release buffer to camera or display device */
    ioctl(v4l2_dev, VIDIOC_QBUF, &v4l2_buf);
}

static vx_status user_node_source_init(vx_node node, const vx_reference parameters[], vx_uint32 num)
{
    OpenV4l2Device(&v4l2_input_dev);
    create_in_buf_handles();

    /* set to call a callback, when parameters[0] generates a VX_EVENT_REF_CONSUMED event
     * This callback is user for early release of input buffer back to camera
     */
    vxRegisterEventCallback( parameters[0], VX_EVENT_REF_CONSUMED, (vx_event_callback_f)user_node_source_callback );

    return (vx_status)VX_SUCCESS;
}

static vx_status user_node_source_run(vx_node node, const vx_reference parameters[], vx_uint32 num)
{
    vx_reference buf;

    /* get new frame from camera */
    get_buf(v4l2_input_dev, &buf);
    /* enqueue the 'full' frame, so that next node in the graph can use it */
    vxEnqueueFullHandle(parameters[0], buf);

    return (vx_status)VX_SUCCESS;
}

/* callback called when VX_EVENT_REF_CONSUMED registered on 'parameter' is triggered
 * This indicates there is a 'free' buffer which can be dequeued and recycled to the
 * camera
 */
static vx_status user_node_source_callback(vx_reference parameter, vx_enum event_type)
{
    vx_reference buf;

    /* input buffer is free so dequeue and recycle to camera */
    vxDequeueFreeHandle(parameter, &buf);
    put_buf(v4l2_input_dev, buf);

    return (vx_status)VX_SUCCESS;
}


static vx_status user_node_source_deinit(vx_node node, const vx_reference parameters[], vx_uint32 num)
{
    /* framework ensure all pending events are handled before calling deinit
     * this makes sure all buffers have been released to the camera before deinit is called
     */
    CloseV4l2Device(&v4l2_input_dev);
    release_in_buf_handles();

    return (vx_status)VX_SUCCESS;
}


static vx_status user_node_sink_init(vx_node node, const vx_reference parameters[], vx_uint32 num)
{
    vx_uint32 i;

    OpenV4l2Device(&v4l2_output_dev);
    create_out_buf_handles();
    /* give free (done) buffer's to data reference, so that previous node
     * can dequeue it, to fill data into it
     */
    for(i=0; i<MAX_OUT_BUFS; i++)
    {
        vxEnqueueFreeHandle(parameters[0], out_buf[i]);
    }

    /* Display driver specific API,
     * which calls user callback when a buffer is displayed and ready for release by driver
     */
    SetV2l2DeviceCallback(v4l2_output_dev, (void*)user_node_sink_callback, (void*)parameters[0]);

    return (vx_status)VX_SUCCESS;
}

static vx_status user_node_sink_run(vx_node node, const vx_reference parameters[], vx_uint32 num)
{
    vx_reference buf;

    /* get 'full' frame for display */
    vxDequeueFullHandle(parameters[0], &buf);
    put_buf(v4l2_output_dev, buf);

    return (vx_status)VX_SUCCESS;
}

/* callback called when display has displayed the frame and ready to
 * released it to application.
 * At this point the frame can be enqueued as a free frame back to the
 * data reference.
 */
static vx_status user_node_sink_callback(vx_reference parameter)
{
    vx_reference buf;

    /* get displayed frame from display */
    get_buf(v4l2_output_dev, &buf);
    vxEnqueueFreeHandle(parameter, buf);

    return (vx_status)VX_SUCCESS;
}

static vx_status user_node_sink_deinit(vx_node node, const vx_reference parameters[], vx_uint32 num)
{
    /* framework ensures all pending events are handled before calling deinit
     * this makes sure all buffers have been released to the display before deinit is called
     */
    CloseV4l2Device(&v4l2_output_dev);
    release_out_buf_handles();

    return (vx_status)VX_SUCCESS;
}

static vx_status user_node_source_validate(vx_node node, const vx_reference parameters[], vx_uint32 num, vx_meta_format metas[])
{
    /* if any verification checks do here */
    return (vx_status)VX_SUCCESS;
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static void user_node_source_add()
{
    vxAllocateUserKernelId(context, &user_node_source_kernel_id);

    user_node_source_kernel = vxAddUserKernel(
            context,
            "user_kernel.source",
            user_node_source_kernel_id,
            user_node_source_run,
            1,
            user_node_source_validate,
            user_node_source_init,
            user_node_source_deinit
            );

    vxAddParameterToKernel(user_node_source_kernel,
        0,
        (vx_enum)VX_OUTPUT,
        (vx_enum)VX_TYPE_IMAGE,
        (vx_enum)VX_PARAMETER_STATE_REQUIRED
        );

    vxFinalizeKernel(user_node_source_kernel);
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static void user_node_source_remove()
{
    vxRemoveKernel(user_node_source_kernel);
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static vx_node user_node_source_create_node(vx_graph graph, vx_image output)
{
    vx_node node = NULL;

    node = vxCreateGenericNode(graph, user_node_source_kernel);
    vxSetParameterByIndex(node, 0, (vx_reference)output);

    return node;
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static vx_status user_node_sink_validate(vx_node node, const vx_reference parameters[], vx_uint32 num, vx_meta_format metas[])
{
    /* if any verification checks do here */
    return (vx_status)VX_SUCCESS;
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static void user_node_sink_add()
{
    vxAllocateUserKernelId(context, &user_node_sink_kernel_id);

    user_node_sink_kernel = vxAddUserKernel(
            context,
            "user_kernel.sink",
            user_node_sink_kernel_id,
            user_node_sink_run,
            1,
            user_node_sink_validate,
            user_node_sink_init,
            user_node_sink_deinit
            );

    vxAddParameterToKernel(user_node_sink_kernel,
        0,
        (vx_enum)VX_INPUT,
        (vx_enum)VX_TYPE_IMAGE,
        (vx_enum)VX_PARAMETER_STATE_REQUIRED
        );

    vxFinalizeKernel(user_node_sink_kernel);
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static void user_node_sink_remove()
{
    vxRemoveKernel(user_node_sink_kernel);
}

/* Boiler plate code of standard OpenVX API, nothing specific to streaming API */
static vx_node user_node_sink_create_node(vx_graph graph, vx_image input)
{
    vx_node node = NULL;

    node = vxCreateGenericNode(graph, user_node_sink_kernel);
    vxSetParameterByIndex(node, 0, (vx_reference)input);

    return node;
}

/* create data objects, nodes and graph which holds these */
static void create_context_graph_data_node_objects()
{
    vx_int32 shift;
    vx_enum access_type;

    context = vxCreateContext();

    /* add user kernels to context */
    user_node_source_add();
    user_node_sink_add();

    g0 = vxCreateGraph(context);

    /* create input image */
    d0 = vxCreateVirtualImage(g0, 640, 480, (vx_df_image)VX_DF_IMAGE_RGB);
    /* set access type of VX_ACCESS_TYPE_HANDLE so that enqueue/dequeue can be called on it */
    access_type = VX_ACCESS_TYPE_HANDLE;
    vxSetReferenceAttribute((vx_reference)d0, VX_REFERENCE_ACCESS_TYPE, &access_type, sizeof(vx_enum));

    /* create intermediate virtual image */
    d1 = vxCreateVirtualImage(g0, 640, 480, (vx_df_image)VX_DF_IMAGE_VIRT);

    /* create output image */
    d2 = vxCreateVirtualImage(g0, 640, 480, (vx_df_image)VX_DF_IMAGE_S16);
    /* set access type of VX_ACCESS_TYPE_HANDLE so that enqueue/dequeue can be called on it */
    access_type = VX_ACCESS_TYPE_HANDLE;
    vxSetReferenceAttribute((vx_reference)d2, VX_REFERENCE_ACCESS_TYPE, &access_type, sizeof(vx_enum));

    /* create soure node */
    node_source = user_node_source_create_node(g0, d0);

    /* create next node */
    n0 = vxChannelExtractNode(g0, d0, (vx_enum)VX_CHANNEL_G, d1);

    /* create a sclar object required for second node */
    shift = 8;
    s0 = vxCreateScalar(context, (vx_enum)VX_TYPE_INT32, &shift);

    /* create second node */
    n1 = vxConvertDepthNode(g0, d1, d2, (vx_enum)VX_CONVERT_POLICY_SATURATE, s0);

    /* create sink node */
    node_sink = user_node_sink_create_node(g0, d2);

    /* all done, verify graph */
    vxVerifyGraph(g0);

    /* other than setting access type as VX_ACCESS_TYPE_HANDLE for input and output data objects
     * all other steps are same as OpenVX v1.1
     */
}

/* release objects created during create_context_graph_data_node_objects() */
static void release_context_graph_data_node_objects()
{
    vxReleaseScalar(&s0);
    vxReleaseImage(&d0);
    vxReleaseImage(&d1);
    vxReleaseImage(&d2);
    vxReleaseNode(&n0);
    vxReleaseNode(&n1);
    vxReleaseNode(&node_source);
    vxReleaseNode(&node_sink);
    vxReleaseGraph(&g0);

    /* remove user kernels from context */
    user_node_source_remove();
    user_node_sink_remove();

    vxReleaseContext(&context);
}

/* main entry point of the sample */
void vx_khr_streaming_sample()
{
    /* allocate resources */
    create_context_graph_data_node_objects();

    /* execute graph in streaming mode,
     * graph is retriggered when reference d0 is consumed by a graph execution
     */
    vxGraphStartEventTriggerMode(g0, VX_EVENT_REF_CONSUMED, (vx_reference)d0);
    while(!DoExit())
    {
        /* loop until user wants to exit */
    }
    vxGraphStopEventTriggerMode(g0);

    /* free previously allocated resources */
    release_context_graph_data_node_objects();
}
