# Design Topic : Capture Robustness {#did_capture_robustness}

[TOC]

# Requirements Addressed {#did_capture_robustness_requirements}

- https://jira.itg.ti.com/browse/TIOVX-805
- https://jira.itg.ti.com/browse/TIOVX-940

# Design Discussions {#did_capture_robustness_design_discussions}

- https://confluence.itg.ti.com/x/9aISEg

# Introduction {#did_capture_robustness_intro}

- In production application, it is possible that a camera may fail.  If this occurs within the exising
  implementation of TIOVX capture node, the application will hang and not allow further frames to be
  processed by downstream nodes.  This requirement states that in the case that a camera fails, the
  application must not halt.  Instead, the capture node would output a dummy frame, the contents of
  which are based on what is supplied by the application.  In addition, the capture node will notify
  the application of the failure in case it must take a specific action.  Finally, in the case that
  the camera is reconnected, the processing will continue.

# Directory Structure {#did_capture_robustness_dir_structure}

    tiovx_dev/kernels_j7/hwa/capture/vx_capture_target.c
    tiovx_dev/kernels_j7/hwa/host/vx_capture_host.c
    tiovx_dev/kernels_j7/include/TI/j7_capture.h

# Resource usage {#did_capture_robustness_resource_usage}

- Once the application provides a reference to use as the black frame, the capture node will allocate
  object descriptors equal to TIVX_CAPTURE_MAX_NUM_BUFS times the number of channels used in the capture
  node.

# Error handling {#did_capture_robustness_error_handling}

- In the case that a camera fails, the capture node will send a node error event to the application.  This
  error event must be registered by the application during create time by using the vxRegisterEvent API. The
  capture node also prints that camera that failed.  The application can also query the capture node using the
  TIVX_CAPTURE_GET_STATISTICS control command in order to query which returns which cameras are active.
- Before passing the reference to the node control callback, the capture node host side performs a verification
  of the reference to ensure it has the same properties as the reference used as the output of the capture node.
- In the case that a user supplies a timeout value but does not send a frame to use in the case of
  failure, the capture node will default to waiting forever for a frame to receive as it does not
  have a reference to use as output.  It will also print a warning stating that the frame has not been
  received
- In the case that multiple error frames have been passed, it will only use the first frame rather than
  allocating additional descriptors.

# Interface {#did_capture_robustness_interface}

## Capture Robustness API's {#did_capture_robustness_interface_apis}

- The tivx_capture_params_t structure used as the user data object structure of the capture node was modified
  to include both a "timeoutInitial" and "timeout" parameters.  The "timeoutInitial" is the initial threshold
  for timing out in the case that a frame does not come through.  Once a frame has already been dropped, the
  "timeout" value is used.  Note, the capture node gets a log of which cameras are active and waits the "timeout"
  length of time to receive only the active channels.  However, if a frame still comes through from the inactive
  channel, then the capture node will pass this along and mark the channel as active for future iterations.
- The tivxCaptureRegisterErrorFrame API is used by the application to interface with the capture node for passing
  error frames.  The application must create a single frame that will be used in the case of a failing camera
  then pass this frame along with the capture node to this API.  Internally, this API will send the frame to the
  capture node via a control command.  Inside the control callback of the capture node, the capture node will
  create object descriptors equal to the max buffer depth times the number of cameras used in the application.
  Each of these descriptors will point to the memory of the passed buffer.  These descriptors are put into a
  queue locally and dequeued to be used whenever a frame has failed.
- In the case that a timeout is set but the error frame has not been passed via the tivxCaptureRegisterErrorFrame,
  the capture node will default to waiting forever as it does not have a frame to pass along in case the capture
  node fails.

## Capture Robustness Implementation {#did_capture_robustness_interface_implementation}

- By default when calling the tivx_capture_params_init to initialize the capture params, the timeout and
  timeoutInitial are set to "TIVX_EVENT_TIMEOUT_WAIT_FOREVER".  The application can set these values to a timeout
  corresponding to the frame rate that is expected from the imaging parameters.  In order to set the timeout to be
  used by the application, the application must also create a frame and pass to the node using tivxCaptureAllocErrorFrame.
  Otherwise, the capture node will default to "TIVX_EVENT_TIMEOUT_WAIT_FOREVER".
- If the conditions above are set to appropriately use the application timeout value, the application will wait either
  until it gets a frame from each channel, or until the time specified by the "timeoutInitial".  If a frame has not
  been received until the capture node reaches "timeoutInitial", then the capture node will begin sending a frame allocated
  as a part of the tivxCaptureAllocErrorFrame.  It will also flag this channel as "inactive" using the "timeout" value
  instead to wait on a frame.  The capture node will then wait for all active channels to return before exiting the node.
  If a previously inactive channel becomes available, it will send the new frame then mark it as "active".
- Once the application has send a frame over the tivxCaptureRegisterErrorFrame API to the capture node, the capture node
  will allocate buffers equal to the number of cameras times the max buffer depth as error frames.  These are placed in a
  queue and dequeued once a channel has been deemed inactive.

## Capture Robustness Testing {#did_capture_robustness_interface_testing}

- This change to the capture node has several test cases to validate the functionality.  These are described below:
   - Capture only: The "capture-only" test cases consist of a graph with a single 4 channel capture node performing raw capture.
     Each of these graphs begin with all channels active and running for a certain number of iterations.  One of the channels
     is then disabled and the test case verifies that an error is returned from the capture node.  It then dequeues remaining
     channels.  This test case has test vectors that disable all 4 channels in different test cases.
   - Capture-display: The "capture-display" test case consists of a graph with 4 channel capture node connected to a display
     node.  The display node cycles through all 4 channels to display.  Four of the test vectors start with all 4 channels
     active then disable each of the 4 channels as each test vector.  The test continues to run, displaying the error frame for
     the failing channel.  Other vectors include not starting any of the cameras upon the start of graph processing and ensuring
     black frames are passed to display.  The cameras are then started after a specified number of iterations then continue to
     ensure that frames are sent to display.  Finally, other combinations of cameras are disabled after first enabling all 4,
     including the 2 cameras at a time and all 4 cameras being stopped.
   - Multi camera app: The multi camera app was modified to include a config parameter for enabling the error frame usage.
     In this test case, when that flag is used, the multi cam app is started then one of the cameras is hot unplugged.  The app
     then continues to display at 30 fps, while the unplugged camera generates a black frame on the output.

# Design Analysis and Resolution (DAR) {#did_capture_robustness_dar}

## Design Decision : Timeout {#did_capture_robustness_dar_01}

- A decision needed to be made on whether or not to always time out or to keep a flag of active vs inactive channels

### Design Criteria: Achieve best performance {#did_capture_robustness_dar_01_criteria}

- The main design criteria is to maximize performance by not dropping any frames from capture.

### Design Alternative: Always use time out {#did_capture_robustness_dar_01_alt_01}

- One alternative is to wait the "timeout" amount of time always without regard to whether the channel is active or not.

### Design Alternative: Keep a flag of active vs inactive channels {#did_capture_robustness_dar_01_alt_02}

- Another alternative is to track the active vs inactive channels and only wait for the active channels.

### Final Decision {#did_capture_robustness_dar_01_decision}

- Track the active channels and wait for only the active channels.  This method does not require unnecessary wait time
  in the event that a channel has been disconnected as well as allowing for recovery in the case that the camera became
  available.

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_09  | 1 May  2020   | Lucas W            | First draft
