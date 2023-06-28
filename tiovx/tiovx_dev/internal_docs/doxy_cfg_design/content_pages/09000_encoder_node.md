# Design Topic : Video Encoder Node {#did_encoder_node}

[TOC]

# Requirements Addressed {#did_encoder_requirements}

- https://jira.itg.ti.com/browse/TIOVX-706
- https://jira.itg.ti.com/browse/TIOVX-884

# Introduction {#did_encoder_intro}

Purpose of the video encoder node is to encode YUV420 images into H.264 bitstream.  An example use case is to encode YUV images into H.264 bitstream on analytics box and transfer via ethernet to infotainment box for display purpose.

# Directory Structure {#did_encoder_dir_structure}

    kernels_j7/hwa/venc/vx_kernels_hwa_target.c   # venc code location
    kernels_j7/hwa/venc/vx_video_encoder_target.c   # venc code location
    kernels_j7/hwa/host/vx_video_encoder_host.c   # venc code location
    kernels_j7/hwa/test/test_video_encoder.c   # venc conformance test code location

# Diagrams {#did_encoder_diagrams}

## Component Interaction {#did_encoder_component_interaction}

![](venc_interface.png "Video Encoder Node Interface")

# Resource usage {#did_encoder_resource_usage}

# Error handling {#did_encoder_error_handling}

- The input configuration must be of type tivx_video_encoder_params_t.  Any other user data object types return an error
  during vxVerifyGraph.

- The input image to the video encoder node must be of type NV12.  Any other input image types return an error
  during vxVerifyGraph.

- The output bitstream user data object must be of type tivx_video_bitstream_t.  Any other user data object types return an error
  during vxVerifyGraph.

- The bitstream format must be TIVX_BITSTREAM_FORMAT_H264 within the tivx_video_bitstream_t output user data object.  Any other 
  user data object types return an error during vxVerifyGraph.

# Interface {#did_encoder_interface}

## Video Encoder API's {#did_encoder_interface_apis}

- The video encoder API's are defined in kernels_j7/include/TI/j7_video_encoder.h.

- The video encoder node takes as an input a user data object of type tivx_video_encoder_params_t.
  This data structure can be found in kernels_j7/include/TI/j7_video_encoder.h.  This
  data structure defines the parameters needed to initialize the video encoder interface.

- The video encoder node takes as input an image of type NV12.

- The video encoder node takes as output a user data object of type tivx_video_bitstream_t.

## Video Encoder Node Implementation {#did_encoder_interface_implementation}

- Upon kernel create, the video encoder node instance will instantiate an encoder channel in the driver.  Additionally, it will configure the driver channel with the user-provided configuration, such as the resolution and image format.
  
- Within the kernel process, the framework-provided buffers are mapped into the hardware MMU.  Additionally, the driver state machine is moved into encoding mode upon which it can receive buffers for encoding.  Once the buffers are encoded by the driver, the node gets a notification upon which the process call is completed.

## Video Encoder Node Testing {#did_encoder_interface_testing}

- The video encoder node is tested using test cases found at kernels_j7/hwa/test/test_video_encoder.c. 
  The test cases provided include 3 scenarios: 1) node creation, 2) single channel encoding, 3) multi-channel encoding.  The single channel encoding test case exercises NV12, 720p encoding.  The multi channel encoding test case exercises NV12 720p + 1080p encoding.  The output is dumped to SD card and verified visually on PC.

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_08 | 24 Feb  2020   | Sunita Nadampalli       | First draft

