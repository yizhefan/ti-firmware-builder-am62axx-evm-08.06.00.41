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
#ifndef _OPENVX_PIPELINING_H_
#define _OPENVX_PIPELINING_H_

#ifdef  __cplusplus
extern "C" {
#endif


/*!
 * \file
 * \brief The OpenVX Pipelining extension API.
 */

#define OPENVX_KHR_PIPELINING  "vx_khr_pipelining"

#include <VX/vx.h>

/*! \brief The graph attribute list.
 * \ingroup group_pipelining
 */
enum vx_pipeling_graph_attribute_e {

    /*! \brief Returns the pipeline depth for the graph. Read-only. Use a <tt>\ref vx_uint32</tt> parameter.
     *         This specifies the max number times of vxScheduleGraph can be called without caling vxWaitGraph
     *  \note [REQ] This value shall return '1' if queried on a graph which has not yet been validated.<br>
     *        [REQ] This value may be updated by the implementation each time <tt>\ref vxVerifyGraph</tt> is called.
     */
    VX_GRAPH_PIPELINE_DEPTH = ((( VX_ID_KHRONOS ) << 20) | ( VX_TYPE_GRAPH << 8)) + 0x5
};

/*! \brief Extra enums.
 * \ingroup group_pipelining
 */
enum vx_pipeling_enum_e
{
    VX_ENUM_ACCESS_TYPE     = 0x1C, /*!< \brief Data Object Access Type enumeration. */
};

/*! \brief The access type enumeration list.
 * \ingroup group_pipelining
 */
enum vx_access_type_e {
    /*! \brief The data object is virtual (intermediate data object with no data accessibility for the application)*/
    VX_ACCESS_TYPE_VIRTUAL = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_ENUM_ACCESS_TYPE) + 0x0,
    /*! \brief The data object data can only be accessed by the application using the following API's:
     *         <tt>\ref vxEnqueueReferenceHandle</tt>, <tt>\ref vxDequeueReferenceHandle</tt>,
     *         and <tt>\ref vxSwapImageHandle</tt> */
    VX_ACCESS_TYPE_HANDLE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_ENUM_ACCESS_TYPE) + 0x1,
    /*! \brief The data object data can only be accessed by the application using the vxCopy<Type>, vxMap<Type>, and vxUnMap<Type> API's */
    VX_ACCESS_TYPE_HOST = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_ENUM_ACCESS_TYPE) + 0x2,
};

/*! \brief The reference attributes list.
 * \ingroup group_pipelining
 */
enum vx_pipeling_reference_attribute_e {
    /*! \brief Used to set/query access type of the reference. Read-write.Use a <tt>\ref vx_enum</tt> parameter. Will contain a <tt>\ref vx_access_type_e</tt>.
     * \note This attribute can only be set/queried on the references to the following data types:<br>
		VX_TYPE_DELAY<br>
		VX_TYPE_LUT<br>
		VX_TYPE_DISTRIBUTION<br>
		VX_TYPE_PYRAMID<br>
		VX_TYPE_THRESHOLD<br>
		VX_TYPE_MATRIX<br>
		VX_TYPE_CONVOLUTION<br>
		VX_TYPE_SCALAR<br>
		VX_TYPE_ARRAY<br>
		VX_TYPE_IMAGE<br>
		VX_TYPE_REMAP<br>
		VX_TYPE_OBJECT_ARRAY<br>
		VX_TYPE_TENSOR
	   \note The default access type upon object creation is \ref VX_ACCESS_TYPE_VIRTUAL
	*/
    VX_REFERENCE_ACCESS_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REFERENCE) + 0x3,
};

/*! \brief Returns a Boolean to indicate if a call to <tt>\ref vxScheduleGraph</tt> allowed.
 * \param [in] graph The reference to the graph to check.
 * \return A <tt>\ref vx_bool</tt> value.
 * \retval vx_true_e The graph can be scheduled.
 * \retval vx_false_e A call to <tt>\ref vxWaitGraph</tt> is required before calling
 * <tt>\ref vxScheduleGraph</tt> again.
 * \ingroup group_pipelining
 */
VX_API_ENTRY vx_bool VX_API_CALL vxIsScheduleGraphAllowed(vx_graph graph);

/*! \brief Returns a Boolean to indicate if a call to <tt>\ref vxWaitGraph</tt> is required.
 * \param [in] graph The reference to the graph to check.
 * \return A <tt>\ref vx_bool</tt> value.
 * \retval vx_true_e <tt>\ref vxWaitGraph</tt> has been called less times than <tt>\ref vxScheduleGraph</tt>
 * \retval vx_false_e <tt>\ref vxWaitGraph</tt> has been called greater
 *         than or equal to the number of times than <tt>\ref vxScheduleGraph</tt>
 * \ingroup group_pipelining
 */
VX_API_ENTRY vx_bool VX_API_CALL vxIsWaitGraphRequired(vx_graph graph);


/*! \brief Enqueues a reference to a data object with a buffer handle.
 *
 * This function provides a new data handle (i.e. reference to buffer)
 * to be enqueued into a data object.
 *
 * This function essentially gives ownership of the memory referenced by
 * the \arg buf_handle to the framework.
 *
 * The memory referenced by the \arg buf_handle must have been allocated
 * consistently with the meta properties of the data object since the
 * import type, memory layout and dimensions are unchanged.
 *
 * The data object referenced by \arg data_ref should have been assigned
 * \ref VX_REFERENCE_ACCESS_TYPE = VX_ACCESS_TYPE_HANDLE prior to calling
 * <tt>\ref vxVerifyGraph<\tt> in order to call this function.
 *
 * The behavior of <tt>\ref vxEnqueueReferenceHandle</tt> when called
 * from a user node is undefined.
 *
 * \param [in] data_ref The reference to the data object in the graph.
 * \param [in] buf_handle The reference to the buffer handle.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors.
 * \retval VX_ERROR_INVALID_REFERENCE data_ref is not a valid data object
 * reference for this API.
 * \retval VX_FAILURE Handle could not be enqueued.
 * \ingroup group_pipelining
 */
VX_API_ENTRY vx_status VX_API_CALL vxEnqueueReadyHandle(vx_reference data_ref, vx_reference buf_handle);



/*! \brief Dequeues a buffer handle from a reference to a data object.
 *
 * This function returns a data handle (i.e. reference to buffer)
 * corresponding to a buffer after graph completion from a data object.
 *
 * This function essentially gives ownership of the memory referenced by
 * the \arg buf_handle to the application.
 *
 * The data object referenced by \arg data_ref should have been assigned
 * \ref VX_REFERENCE_ACCESS_TYPE = VX_ACCESS_TYPE_HANDLE prior to calling
 * <tt>\ref vxVerifyGraph<\tt> in order to call this function.
 *
 * The behavior of <tt>\ref vxDequeueReferenceHandle</tt> when called
 * from a user node is undefined.
 *
 * \param [in] data_ref The reference to the data object in the graph.
 * \param [in] buf_handle The reference to the buffer handle.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors.
 * \retval VX_ERROR_INVALID_REFERENCE data_ref is not a valid data object
 * reference for this API.
 * \retval VX_FAILURE No handle could be dequeued.
 * \ingroup group_pipelining
 */
VX_API_ENTRY vx_status VX_API_CALL vxDequeueDoneHandle(vx_reference data_ref, vx_reference *buf_handle);


/*! \brief Sets attributes on the reference object.
 * \param [in] ref The reference object for which an attribute is to be set.
 * \param [in] attribute The attribute to modify. Use a <tt>\ref vx_reference_attribute_e</tt> enumeration.
 * \param [in] ptr The pointer to the value to which to set the attribute.
 * \param [in] size The size of the data pointed to by \a ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_pipelining
 */
VX_API_ENTRY vx_status VX_API_CALL vxSetReferenceAttribute(vx_reference ref, vx_enum attribute, const void *ptr, vx_size size);

/*
 * EVENT
 */

typedef struct _vx_event_queue *vx_event_queue;

enum new_vx_enum_e {
    VX_ENUM_EVENT_TYPE         = 0x18,
};

/*! \brief Type of event that can be generated during system execution
 * \ingroup group_event
 */
enum vx_event_type_e {

    /* Reference consumed event.
     * When a data reference is consumed during a graph instance execution this event is generated
     * Its used to indicate that a given input is no longer used by the graph and can be
     * access and released by the application
     * Graph execution could be in progress for rest of the graph that does not use
     * this data reference
     */
    VX_EVENT_REF_CONSUMED = ((( VX_ID_KHRONOS ) << 20) | ( VX_ENUM_EVENT_TYPE << 12)) + 0x1,

    /* graph completion event, sent every time a graph execution completes
     * here complete refers to successful completion or abandoned completion
     */
    VX_EVENT_GRAPH_COMPLETED = ((( VX_ID_KHRONOS ) << 20) | ( VX_ENUM_EVENT_TYPE << 12)) + 0x2,

    /* node completion event, sent every time a node within a graph completes
     * here complete refers to successful completion or abandoned completion
     */
    VX_EVENT_NODE_COMPLETED = ((( VX_ID_KHRONOS ) << 20) | ( VX_ENUM_EVENT_TYPE << 12)) + 0x3,

    /*  event from the event queue which indicates that there was no space in the event queue to
     *  hold events generated for this event queue.
     *
     *  This indicates to the user that the rate of events being generated is higher than
     *  the rate of consumption of the events. Hence user needs to change the size of the event queue
     *  in order to allow buffering of more events or reduce the number of events
     *  that user is asking to be be generated
     *
     *  Every event queue MUST ensure that it can post atleast one overflow event
     *
     *  'ref' parameter in vx_event_t will hold the event queue handle
     */
    VX_EVENT_EVENT_QUEUE_OVERFLOW = ((( VX_ID_KHRONOS ) << 20) | ( VX_ENUM_EVENT_TYPE << 12)) + 0x4,

    /* user defined event sent by user outside of openVX framework */
    VX_EVENT_USER = ((( VX_ID_KHRONOS ) << 20) | ( VX_ENUM_EVENT_TYPE << 12)) + 0x4
};

/*! \brief Structure decribing an event
 * \ingroup group_event
 */
typedef struct _vx_event {

    vx_enum type; /*!< see \ref vx_event_type_e */
    vx_uint32 user_event_id; /*!< user defined event ID */
    void *user_event_parameter; /*!< user defined parameter value, can be used to pass additional parameters with an event */
    vx_reference ref; /*!< reference which generated this event, set to NULL for user events */

} vx_event_t;

/*
 * \brief Create event queue to receive events
 *
 * Note, event queue MUST be able to send a VX_EVENT_EVENT_QUEUE_OVERFLOW event
 * if events need to be dropped due to max_queue_depth being hit
 * and events still arriving at the event queue
 *
 * \param max_queue_depth [in] specifies the maximum events that can be queued
 *                             into the event queue witout extracting them
 *                             via vxWaitEvent or vxClearEvents or vxWaitEventConditional
 *
 */
VX_API_ENTRY vx_event_queue VX_API_CALL vxCreateEventQueue(vx_uint32 max_queue_depth);

VX_API_ENTRY vx_status VX_API_CALL vxReleaseEventQueue(vx_event_queue *event_q);

/*
 * \brief Wait for a single event
 *
 * \param event_q [in] event queue object
 * \param event [out] When any event arrives, the event information is listed in "event"
 * \param timeout [in] timeout specified in units of msecs, 0xFFFFFFFFu indicates wait for ever
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxWaitEvent(vx_event_queue event_q, vx_event_t *event, vx_uint32 timeout);

/*
 * \brief Wait for all events listed in event_cond_list to occur
 *
 *        The wait condition is of type "AND"
 *        If any other event occurs, it are extracted from the event queue internally
 *        and is not visible to the user.
 *
 * \param event_q [in] event queue object
 * \param event_cond_list [in] User provided list of event to wait on
 * \param num_event_cond_list [in] Num of events listed in event_cond_list
 * \param timeout [in] timeout specified in units of msecs, 0xFFFFFFFFu indicates wait for ever
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxWaitEventConditional(vx_event_queue event_q,
                vx_event_t *event_cond_list, vx_uint32 num_event_cond_list,
                vx_uint32 timeout);

/*
 * \brief Clear all pending event from the event queue
 *
 *        If any events are pending in the event, they are exctracted internally
 *        to make the event queue empty
 *
 * \param event_q [in] event queue object
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxClearEvents(vx_event_queue event_q);

/*
 * \brief Enable or disable events getting inserted into the event queue
 *
 * \param event_q [in] event queue object
 * \param enable [in] vx_true_e: enable events to be inserted into the event queue.
 *                    vx_false_e: drop all events targeted for this event queue.
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxEnableEvents(vx_event_queue event_q, vx_bool enable);

/*
 * \brief Send user event to an event queue
 *
 * \param event_q [in] event queue object
 * \param user_event_id [in] user event ID
 * \param user_event_parameter [in] user defined event parameter. NOT used by implemenation
 *                                   returned to user as part vx_event_t.user_event_parameter field
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxSendUserEvent(vx_event_queue event_q, vx_uint32 id, void *parameter);


/*
 * \brief Register an event to be generated
 *
 * \param ref [in] reference which will generate the event
 * \param type [in] type or condition on which event is generated
 * \param event_q [in] event queue to which the generated event is sent or inserted into
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxRegisterEvent(vx_reference ref, enum vx_event_type_e type, vx_event_queue event_q);

/*
 * \brief UnRegister a previously registered event
 *
 * \param ref [in] reference to which event was previously registered
 * \param type [in] type or condition on which event is registered
 * \param event_q [in] event queue to which the event is previoulsy registered
 *
 */
VX_API_ENTRY vx_status VX_API_CALL vxUnRegisterEvent(vx_reference ref, enum vx_event_type_e type, vx_event_queue event_q);

/*
 * STREAMING
 */

VX_API_ENTRY vx_status vxGraphStartEventTriggerMode(vx_graph graph, vx_enum event_type, vx_reference reference);

/* blocks until graph is gracefully stopped at logical boundary */

VX_API_ENTRY vx_status vxGraphStopEventTriggerMode(vx_graph graph);

/*! \brief Sets attributes on the reference object.
 * \param [in] ref The reference object for which an attribute is to be set.
 * \param [in] attribute The attribute to modify. Use a <tt>\ref vx_reference_attribute_e</tt> enumeration.
 * \param [in] ptr The pointer to the value to which to set the attribute.
 * \param [in] size The size of the data pointed to by \a ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_threshold
 */
VX_API_ENTRY vx_status VX_API_CALL vxSetReferenceAttribute(vx_reference ref, vx_enum attribute, const void *ptr, vx_size size);

#ifdef  __cplusplus
}
#endif

#endif
