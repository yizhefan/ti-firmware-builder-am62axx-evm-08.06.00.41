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

/* context which holds all openvx objects */
static vx_context context;
/* input image data object */
static vx_image d0;
/* intermediate virtual image data object */
static vx_image d1;
/* output image data object */
static vx_image d2;
/* scalar data object for convert depth node */
static vx_scalar s0;
/* node for channel extract node */
static vx_node n0;
/* node for convert depth node */
static vx_node n1;
/* graph containing n0, n1, d0, d1, d2 */
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

/* V4L2 camera device file handle */
static int v4l2_input_dev = -1;
/* V4L2 display device file handle */
static int v4l2_output_dev = -1;

/* create data objects, nodes and graph which holds of these */
static void create_context_graph_data_node_objects()
{
    vx_int32 shift;
    vx_enum access_type;

    context = vxCreateContext();

    g0 = vxCreateGraph(context);

    /* create input image */
    d0 = vxCreateImage(context, 640, 480, (vx_df_image)VX_DF_IMAGE_RGB);
    /* set access type of VX_ACCESS_TYPE_HANDLE so that enqueue/dequeue can be called on it */
    access_type = VX_ACCESS_TYPE_HANDLE;
    vxSetReferenceAttribute((vx_reference)d0, VX_REFERENCE_ACCESS_TYPE, &access_type, sizeof(vx_enum));

    /* create intermediate virtual image */
    d1 = vxCreateVirtualImage(g0, 640, 480, (vx_df_image)VX_DF_IMAGE_VIRT);

    /* create output image */
    d2 = vxCreateImage(context, 640, 480, (vx_df_image)VX_DF_IMAGE_S16);
    /* set access type of VX_ACCESS_TYPE_HANDLE so that enqueue/dequeue can be called on it */
    access_type = VX_ACCESS_TYPE_HANDLE;
    vxSetReferenceAttribute((vx_reference)d2, VX_REFERENCE_ACCESS_TYPE, &access_type, sizeof(vx_enum));

    /* create first node */
    n0 = vxChannelExtractNode(g0, d0, (vx_enum)VX_CHANNEL_G, d1);

    /* create a sclar object required for second node */
    shift = 8;
    s0 = vxCreateScalar(context, (vx_enum)VX_TYPE_INT32, &shift);

    /* create second node */
    n1 = vxConvertDepthNode(g0, d1, d2, (vx_enum)VX_CONVERT_POLICY_SATURATE, s0);

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
    vxReleaseGraph(&g0);
    vxReleaseContext(&context);
}

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

/* open and init camera and display systems */
void open_camera_display_device()
{
    OpenV4l2Device(&v4l2_input_dev);
    OpenV4l2Device(&v4l2_output_dev);
}

/* close and deinit camera and display systems */
void close_camera_display_device()
{
    CloseV4l2Device(&v4l2_input_dev);
    CloseV4l2Device(&v4l2_output_dev);
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

/* execute graph execution in pipelined manner
 *
 * Here
 * - VX_GRAPH_PIPELINE_DEPTH is used to sequence vxScheduleGraph and vxWaitGraph
 * - VX_GRAPH_PIPELINE_DEPTH is used to pipedown the graph executions
 *
 */
static void execute_graph1()
{
    vx_uint32 max_pipe_depth_graph, num_pending;
    vx_reference in_buf, out_buf;

    vxQueryGraph(g0, VX_GRAPH_PIPELINE_DEPTH, &max_pipe_depth_graph, sizeof(vx_uint32));

    /* set number of pending graph submit count to zero */
    num_pending = 0;

    while(1)
    {
        if(num_pending < max_pipe_depth_graph)
        {
            get_buf(v4l2_input_dev, &in_buf);
            get_buf(v4l2_output_dev, &out_buf);

            vxEnqueueReadyHandle((vx_reference)d0, in_buf);
            vxEnqueueReadyHandle((vx_reference)d2, out_buf);
            vxScheduleGraph(g0);
            num_pending++;
        }
        if(num_pending==max_pipe_depth_graph)
        {
            vxWaitGraph(g0);
            num_pending--;
            vxDequeueDoneHandle((vx_reference)d0, &in_buf);
            vxDequeueDoneHandle((vx_reference)d2, &out_buf);
            put_buf(v4l2_input_dev, in_buf);
            put_buf(v4l2_output_dev, out_buf);
        }
        if(DoExit())
        {
            /* App wants to exit, break from main loop */
            break;
        }
    }

    /*
     * wait until all previous graph executions have completed
     */
    while(num_pending)
    {
        vxWaitGraph(g0);
        num_pending--;
        vxDequeueDoneHandle((vx_reference)d0, &in_buf);
        vxDequeueDoneHandle((vx_reference)d2, &out_buf);
        put_buf(v4l2_input_dev, in_buf);
        put_buf(v4l2_output_dev, out_buf);
    }
}

/* execute graph execution in pipelined manner
 *
 * Here
 * - vxIsScheduleGraphAllowed is used to sequence vxScheduleGraph and vxWaitGraph
 * - vxIsWaitGraphRequired is used to pipedown the graph executions
 */
static void execute_graph2()
{
    vx_reference in_buf, out_buf;

    while(1)
    {
        if(vxIsScheduleGraphAllowed(g0))
        {
            get_buf(v4l2_input_dev, &in_buf);
            get_buf(v4l2_output_dev, &out_buf);

            vxEnqueueReadyHandle((vx_reference)d0, in_buf);
            vxEnqueueReadyHandle((vx_reference)d2, out_buf);
            vxScheduleGraph(g0);
        }
        else
        {
            vxWaitGraph(g0);
            vxDequeueDoneHandle((vx_reference)d0, &in_buf);
            vxDequeueDoneHandle((vx_reference)d2, &out_buf);
            put_buf(v4l2_input_dev, in_buf);
            put_buf(v4l2_output_dev, out_buf);
        }
        if(DoExit())
        {
            /* App wants to exit, break from main loop */
            break;
        }
    }

    /* Pipe down, i.e wait for previous graph schedule to complete and recycle the buffers */
    while(vxIsWaitGraphRequired(g0))
    {
        vxWaitGraph(g0);
        vxDequeueDoneHandle((vx_reference)d0, &in_buf);
        vxDequeueDoneHandle((vx_reference)d2, &out_buf);
        put_buf(v4l2_input_dev, in_buf);
        put_buf(v4l2_output_dev, out_buf);
    }
}

/*
 * Utility API used to do below,
 * - check if graph can be sceheduled
 * - if yes, wait for input and output buffer and schedule the graph
 * - if no, dont do anything
 */
static void submit_graph()
{
    if(vxIsScheduleGraphAllowed(g0))
    {
        vx_reference in_buf, out_buf;

        get_buf(v4l2_input_dev, &in_buf);
        get_buf(v4l2_output_dev, &out_buf);

        vxEnqueueReadyHandle((vx_reference)d0, in_buf);
        vxEnqueueReadyHandle((vx_reference)d2, out_buf);
        vxScheduleGraph(g0);
    }
}

/* execute graph execution in pipelined manner
 *
 * Here
 * - vxIsScheduleGraphAllowed and events are used to sequence vxScheduleGraph and vxWaitGraph
 *   - Events allow early release of input buffer
 * - vxIsWaitGraphRequired is used to pipedown the graph executions
 */
static void execute_graph3()
{
    vx_reference in_buf, out_buf;
    vx_event_queue event_q;
    vx_event_t event;
    vx_uint32 max_pipe_depth_graph, max_event_queue_depth;

    /* create event queue,
     * - max size of event queue should be max pipeline depth x 2
     * - since for every graph schedule upto 2 events can be generated
     * - worst case there can be max pipeline depth x 2 events backlogged into the event queue
     */
    vxQueryGraph(g0, VX_GRAPH_PIPELINE_DEPTH, &max_pipe_depth_graph, sizeof(vx_uint32));
    max_event_queue_depth = max_pipe_depth_graph*2;

    event_q = vxCreateEventQueue(max_event_queue_depth);

    /* register events for input consumed and graph completed */
    vxRegisterEvent((vx_reference)d0, VX_EVENT_REF_CONSUMED, event_q);
    vxRegisterEvent((vx_reference)g0, (vx_enum)VX_EVENT_GRAPH_COMPLETED, event_q);

    /* scehedule graph twice to pipe up the pipeline */
    submit_graph();
    /* if pipeline depth is 1, i.e no pipelining, this call is a NOP */
    submit_graph();

    while(1)
    {
        /* wait for events, time is set to wait for ever */
        vxWaitEvent(event_q, &event, 0xFFFFFFFFu);

        /* event for input data ready for recycling, i.e early input release */
        if(event.type == VX_EVENT_REF_CONSUMED
            && event.ref == (vx_reference)d0
            )
        {
            vxDequeueDoneHandle((vx_reference)d0, &in_buf);
            put_buf(v4l2_input_dev, in_buf);
            /* attempt to submit another graph in pipeline, if allowed */
            submit_graph();
        }
        /* event for graph execution complete and output data ready for comsumption */
        if(event.type == (vx_enum)VX_EVENT_GRAPH_COMPLETED
            && event.ref == (vx_reference)g0
            )
        {
            vxWaitGraph(g0);
            vxDequeueDoneHandle((vx_reference)d2, &out_buf);
            put_buf(v4l2_output_dev, out_buf);
            /* attempt to submit another graph in pipeline, if allowed */
            submit_graph();
        }
        if(event.type == (vx_enum)VX_EVENT_USER
            && event.user_event_id == 0xDEADBEEF /* app code for exit */
            )
        {
            /* App wants to exit, break from main loop */
            break;
        }
    }

    /* Pipe down, i.e wait for previous graph schedule to complete and recycle the buffers */
    while(vxIsWaitGraphRequired(g0))
    {
        vxWaitGraph(g0);
        vxDequeueDoneHandle((vx_reference)d0, &in_buf);
        vxDequeueDoneHandle((vx_reference)d2, &out_buf);
        put_buf(v4l2_input_dev, in_buf);
        put_buf(v4l2_output_dev, out_buf);
    }

    vxReleaseEventQueue(&event_q);
}

/* main entry point of the sample */
void vx_khr_pipelining_sample()
{
    /* allocate resources */
    open_camera_display_device();
    create_context_graph_data_node_objects();
    create_in_buf_handles();
    create_out_buf_handles();

    /* execute graph: three different ways are shown below */
    execute_graph1();
    execute_graph2();
    execute_graph3();

    /* free previously allocated resources */
    release_in_buf_handles();
    release_out_buf_handles();
    release_context_graph_data_node_objects();
    close_camera_display_device();
}

