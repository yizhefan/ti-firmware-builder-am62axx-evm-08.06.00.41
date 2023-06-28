# Auto Valet Parking Application 4 {#group_apps_dl_demos_app_tidl_avp4}

# Introduction

This application demonstrates a typical auto valet parking use-case with realtime capture of four 2MP cameras @30fps. Live camera inputs are fed to VISS to perform RAW to NV12 conversion with required Auto Exposure and Auto White-balance (AEWB) correction algorithm running on Main R5F. This fish-eye image is sent to two pipelines one is a surround-view pipeline which uses the GPU to perform 3D surround-view and another is an analytics pipeline which uses TIDL to perform semantic segmentation for on all the 4 cameras. The VISS output is sent to LDC to perform fish-eye distortion and the to multi-scalar to reduce the effective resolution to 768x384 as required by the DL-network. The image is pre-processed on C66x_1 DSP and provided to the TIDL library running on C71x DSP for inference. The output of each of the 4 cameras is then post-processed on C66x_2 DSP where the chroma channels are color keyed based on the predicted classes per pixel. All the 4 post-processed outputs are then assembled on a 1920x1080 plane using SW mosaic (using MSC) and then submitted to 2MP display clocked @60fps. The same output is also sent to two instances of H.264 encoder doing I frame only GOP. On the A72 a image histogram of LDC output for all the four channels. The purpose of putting together this use-case is to serve as a reference example and also to study the effects of system traffic and DDR loading. 


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2

# Data flow

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\image html app_tidl_avp4_data_flow.png

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample config file "app_avp4.cfg" is provided under "/opt/vision_apps/" folder on the rootfs partition.
-# Edit config file to modify the fields as required
-# Run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./run_app_tidl_avp4.sh
   \endcode

# Steps to run the application on J7 EVM (QNX + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# A sample config file "app_avp4.cfg" is provided under "/ti_fs/vision_apps/" folder on the boot partition.
-# Edit config file to modify the fields as required
-# Run the app as shown below
   \code
   cd /ti_fs/vision_apps
   . ./vision_apps_init.sh
   ./run_app_tidl_avp4.sh
   \endcode

# Steps to run the application on Linux x86_64

As this application uses realtime camera inputs, build using host-emulation is not supported.

# Sample Output

Shown below is a example of 4 live cameras running semantic segmentation. 3D-surround view output is not displayed. Encoded bit-streams are also not written to media.

##Semantic Segmentation outputs for 4 live cameras

The GPU output is stored in memory and not sent to display

\image html app_tidl_avp4_output.jpg


