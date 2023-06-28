# Design Topic : Capture Node {#did_capture_node}

[TOC]

# Requirements Addressed {#did_capture_requirements}

- https://jira.itg.ti.com/browse/TIOVX-74

# Introduction {#did_capture_intro}

Purpose of the capture node is to provide a node whereby a user can connect a
capture source to an OpenVX graph.  This provides the ability to create an
OpenVX graph where streaming is enabled from a capture source to a display
node.

# Directory Structure {#did_capture_dir_structure}

    kernels_j7/hwa/capture/vx_kernels_hwa_target.c   # capture code location
    kernels_j7/hwa/host/vx_capture_host.c   # capture code location
    kernels_j7/hwa/test/test_capture.c   # capture code location

# Diagrams {#did_capture_diagrams}

## Sequence Diagram {#did_capture_sequence_diagram}

![](capture_pipeup.png "Capture Pipeup and Steady States")

## Component Interaction {#did_capture_component_interaction}

![](capture_interface.png "Capture Node Interface")

# Resource usage {#did_capture_resource_usage}

# Error handling {#did_capture_error_handling}

- Presently, the exemplar of the object array must be of type vx_image. The 
  validate function will return error in the case that the exemplars are of
  another type.

- Presently, the capture node only supports RGBX formatted images as elements of
  the object array.  The validate function will return error in the case that the
  images are in any other format.

# Interface {#did_capture_interface}

## Capture API's {#did_capture_interface_apis}

- The capture API's are defined in kernels_j7/include/TI/j7_nodes.h.

- The capture node takes as an input a user data object of type tivx_capture_params_t.
  This data structure can be found in kernels_j7/include/TI/j7_kernels.h.  This
  data structure defines the parameters needed to initialize the capture source.

- The capture node takes as an output an object array with exemplar type vx_image. The
  use of an object array allows for multiple channels to be used with a single node by
  specifying the number of elements of the object array to be equivalent to the number
  of capture channels.

## Capture Node Implementation {#did_capture_interface_implementation}

- The implementation of the capture node began with an extension to the methodology in
  which the framework supplies a node with buffers.  This was due to a requirement from
  the capture node to require multiple buffers before it returns any buffers.  Therefore,
  two stages of buffer management were created, called "Pipe-up" and "Steady-State".
  During the pipe-up state, the framework supplies a buffer to the node without expecting
  a buffer in return.  During steady-state, the framework supplies a buffer to the node
  and expects a buffer in return.  By creating an API that can be called during kernel
  registration, the capture node can specify the pipe-up depth required. The framework
  in turn will supply the capture node with the specified number of buffers before
  entering steady-state.

- Upon kernel create, the capture node will create the FVID2 driver instance and allocate
  memory needed for its internal operation.

- The capture node maintains local queues to track the buffers given during the pipe-up
  state.  Once the pipe-up number of buffers has been hit, the capture node will pend
  until a buffer is returned by the FVID2 driver.  A callback is registered to post a
  semaphore denoting a captured buffer has been received.  Once the semaphore has been
  posted, the capture node will return the buffer received from the FVID2 driver.

- The capture node supports multiple channels by maintaining local queues to track
  whether or not a captured buffer has been received from each channel.

## Capture Node Testing {#did_capture_interface_testing}

- The capture node is tested using test cases found at kernels_j7/hwa/test/test_capture.c
  and kernels_j7/hwa/test/test_capture_display.c. The first test case is a unit test case
  for the capture node that uses pipelining for enqueue-ing and dequeue-ing object arrays to
  the capture node.  The second test connects a capture node to a display node to stream
  data from the capture node.

# Design Analysis and Resolution (DAR) {#did_capture_dar}

## Design Decision : Maintaining capture state information {#did_capture_dar_01}

- The major design decision in designing the capture node framework enhancement was
  how to maintain state information.

### Design Criteria: Place the burden of state tracking on the framework {#did_capture_dar_01_criteria}

- The guiding design criteria for the capture node framework enhancement was to place
  the burden on the framework rather on the kernel or application.  The idea was that
  a customer will be developing new kernels and applications and so to alleviate their
  burden of development, we wanted the framework to have the intelligence of state
  tracking built-in.

### Design Alternative: xyz {#did_capture_dar_01_alt_01}

- Have the framework maintain state tracking.

### Design Alternative: xyz {#did_capture_dar_01_alt_02}

- Have the kernel maintain state tracking.

### Final Decision {#did_capture_dar_01_decision}

- Have the framework maintain state tracking, due to design criteria given.

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_04 | 24 Oct  2019  | Lucas W            | First draft
