# Camera based Semantic Segmentation Application {#group_apps_dl_demos_app_tidl_seg_cam}

[TOC]

# Introduction

This example shows how to write an OpenVx application which integrates the camera pipeline to TIDL based Semantic Segmentation pipeline.
The demo is validated on tiadsegNet_v2 network. Raw pixels from sensor is captured via CSI2Rx and sent to VISS for image processing. The resulting image is written in NV12 format which is then sent to LDC for fish-eye rectification. The rectified image is scaled using MSC to produce a DL resolution of 768x384 as required by the network. As a pre-processing step, the scaled NV12 resolution is converted to RGB format and padded as required on C66x_1 DSP. The pre-processed input is given to TIDL running on C7x DSP for inference. As a post-processing step, the TIDL output is read and each pixel is blended with a color map on the provided image on C66x_2 DSP. The final output is blended with system performance statistics and displayed on a 1920x1080 screen.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode   | SoC
----------|--------------|-----------------|-----------------|----
Support   | NO           | YES             | YES             | J721e / J721S2 / J784S4

# Data flow

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\if DOCS_J784S4
Note: On J784S4, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the second C7X+MMA.
\endif

\image html app_tidl_seg_cam_data_flow.jpg

# Steps to run the application on J7 EVM

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample "app_seg_cam.cfg" for Semantic Segmentation is provided under "/opt/vision_apps/" on the rootfs partition.
-# In case a different network is chosen, update the dl_size accordingly for network input resolution.
-# Run the app as shown below
   \code
   ./run_app_tidl_seg.sh
   \endcode
-# Select a sensor from a list of supported sensors
-# Choose if LDC should be enabled or not
-# Select the number of cameras to run the network

# Steps to run the application on PC Linux x86_64

 -# This application does not run on PC

# Sample Output

Shown below is a example for running semantic segmentation on 4 IMX390 2MP cameras

\image html app_tidl_seg_cam_output.jpg
