# Auto Valet Parking Application {#group_apps_dl_demos_app_tidl_avp}

# Introduction

This application shows a demo of auto valet parking application which runs multiple deep learning algorithms such as parking spot detection, vehicle detection and semantic segmentation on multiple input channels such as front, right and left cameras. The demo takes as input a sequence of nv12 images of resolution 1280x720 pixels coming from different input channels. The input is resized using a multi-scalar hardware (MSC) to 512x512 resolution which is provided to object-detection network for doing parking spot detection and vehicle detection. Also resized to 768x384 resolution which is provided to pixel classification networks such as semantic segmentation. The inputs are pre-processed on C66 DSP before providing to TIDL running on C7x DSP. The output of TIDL is then post-processed on another C66 DSP which draws rectangles around objects and blends semantic segmentation output. User can select a sub-set of outputs for display which is rendered using DSS driver.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | YES          | YES             |  YES          | J721e / J721S2 / J784S4

# Data flow

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\if DOCS_J784S4
Note: On J784S4, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the second C7X+MMA.
\endif

\image html app_tidl_avp_dataflow.jpg

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample config file "app_avp.cfg" is provided under "/opt/vision_apps/" folder on the rootfs partition.
-# Edit config file to modify the fields as required
-# Run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./run_app_tidl_avp.sh
   \endcode

# Steps to run the application on J7 EVM (QNX +RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# A sample config file "app_avp.cfg" is provided under "/ti_fs/vision_apps/" folder on the boot partition.
-# Edit config file to modify the fields as required
-# Run the app as shown below
   \code
   cd /ti_fs/vision_apps
   . ./vision_apps_init.sh
   ./run_app_tidl_avp.sh
   \endcode

# Steps to run the application on PC Linux x86_64

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for PC emulation mode
-# A sample config file "app_avp.cfg" is provided under "${PSDKR_PATH}/vision_apps/apps/dl_demos/app_tidl_avp/config" folder
-# Edit config file to modify the fields as required
-# Create an output directory
   \code
   mkdir app_tidl_avp_out
   \endcode
-# Run the app as shown below
   \code
   ./vx_app_tidl_avp --cfg ${PSDKR_PATH}/vision_apps/apps/dl_demos/app_tidl_avp/config/app_avp.cfg
   \endcode
-# The output will be written in "app_tidl_avp_out" folder in nv12 format.
-# Use open source tools like YUView to see the output.

# Sample Output

Shown below is a example input and its corresponding output

\image html app_tidl_avp_output.jpg
