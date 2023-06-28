# Camera based Image Classification Application {#group_apps_dl_demos_app_tidl_cam}

[TOC]

# Introduction

This application shows how to integrate a camera to a TIDL Node for image classification. The demo is designed to work with 2MP sensor and validated ONLY for D3's IMX390 2MP camera. The network chosen is MobileNet_v1. Raw pixels from sensor is captured via CSI2Rx and sent to VISS for image processing. The resulting image is written in NV12 format which is then sent to LDC for fish-eye rectification. The rectified image is scaled into 1/2 and 1/4 resolutions. The 1/2 resolution is sent to display and 1/4 resolution is center cropped and sent to TIDL node for inference.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode   | SoC
----------|--------------|-----------------|-----------------|----
Support   | NO           | YES             | YES             | J721e / J721S2 / J784S4

# Data flow

\if (DOCS_J721S2 || DOCS_J784S4)
Note: On J721S2 and J784S4, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\image html app_tidl_cam_data_flow.jpg

# Steps to run the application on J7 EVM

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample "app_tidl_cam.cfg" for image classification is provided under "/opt/vision_apps/" on the rootfs partition.
-# This demo is designed to work only with a 2MP sensor. Currently validated only on D3's IMX390 2MP sensor.
-# Run the app as shown below
   \code
   ./run_app_tidl_cam.sh
   \endcode

# Steps to run the application on PC Linux x86_64

 -# This demo cannot be run on PC as it requires D3's IMX390 2MP camera.

# Sample Output

Shown below is a example input and its corresponding output

\image html app_tidl_cam_output.jpg
