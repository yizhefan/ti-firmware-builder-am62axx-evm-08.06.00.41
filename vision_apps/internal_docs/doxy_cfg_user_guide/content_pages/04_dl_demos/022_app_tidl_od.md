# Object Detection Application {#group_apps_dl_demos_app_tidl_od}

[TOC]

# Introduction

This example shows how to write an OpenVx application which runs pre-processing, post-processing nodes on C66x DSP and runs TIDL node on C7x DSP for Object Detection.
The demo is validated on Pelee network available at, https://github.com/Robert-JunWang/Pelee. The trained network is imported using TIDL import tool and raw NV12 input files of resolution 1024x512 is read from SD card or USB 3.0 typec-pen-drive or PCIe memory. The input files are scaled using VPAC multi-scaler MSC to produce a DL resolution of 1024x512 as required by the Pelee network. As a pre-processing step, the scaled NV12 resolution is converted to RGB format and padded as required on C66x_1 DSP. The pre-processed input is given to TIDL running on C7x DSP for inference. As a post-processing step, the TIDL output is read and top detections are rendered as 2D boxes on the provided image on C66x_2 DSP. The final ouput is blended with system performance statistics and displayed on a 1920x1080 screen.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode   | SoC
----------|--------------|-----------------|-----------------|----
Support   | YES          | YES             | YES             | J721e / J721S2 / J784S4

# Data flow

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\if DOCS_J784S4
Note: On J784S4, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the second C7X+MMA.
\endif

\image html app_tidl_od_data_flow.jpg

# Steps to run the application on J7 EVM

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample "app_od.cfg" for Object detection is provided under "/opt/vision_apps/" on the rootfs partition.
-# Create a folder and keep all the .yuv files used for detection, there are a few images under "/opt/vision_apps/test_data/psdkra/tidl_demo_images".
-# In case a different network is choosen, update the dl_size accordingly for network input resolution.
-# Run the app as shown below
   \code
   ./run_app_tidl_od.sh
   \endcode

# Steps to run the application on PC Linux x86_64

 -# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for PC emulation mode
 -# A sample "app_od.cfg" for Object detection is provided under "${PSDKR_PATH}/vision_apps/apps/dl_demos/app_tidl_od/config" folder.
 -# Create a folder and keep all the .yuv files used for detection, there are a few images under "/opt/vision_apps/test_data/psdkra/tidl_demo_images".
 -# In case a different network is chosen, update the dl_size accordingly for network input resolution.
 -# Create an output directory
    \code
    mkdir app_tidl_od_out
    \endcode
 -# Run the app as shown below
    \code
    ./vx_app_tidl_od --cfg ${PSDKR_PATH}/vision_apps/apps/dl_demos/app_tidl_od/config/app_od.cfg
    \endcode
 -# The output will be written in "app_tidl_od_out" folder in .yuv (NV12) format.

# Sample Output

Shown below is a example input and its corresponding output

\image html app_tidl_od_demo.png
