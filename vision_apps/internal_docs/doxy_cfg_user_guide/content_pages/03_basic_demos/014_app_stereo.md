# Stereo Disparity Application {#group_apps_basic_demos_app_stereo}

[TOC]

# Introduction

This application shows a demo of the TI Stereo Disparity Engine (SDE) hardware
accelerator (HWA). The demo takes as input a sequence of input 8b or 16b gray scale
left+right pair images in .bmp file format for 8b and raw .bin file format for 16b. Each pair of frames
is run through Stereo Disparity OpenVX node.
The stereo disparity map computed by SDE node are mapped based on
a color palatte and output to files in .bmp format.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode   | SoC
----------|--------------|-----------------|-----------------|----
Support   | YES          | YES             | YES             | J721e / J721S2 / J784S4

# Data flow

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\image html vx_app_stereo_data_flow.png

# Steps to run the application on J7 EVM

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample "app_stereo.cfg" for Stereo Depth with 8b input is provided under "/opt/vision_apps/" on the rootfs partition. Update paths and other fields if necessary.
-# A sample "app_stereo_16bit.cfg" for Stereo Depth with 16b input is provided under "/opt/vision_apps/" on the rootfs partition. Update paths and other fields if necessary.
-# For 8b input, run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./run_app_stereo.sh
   \endcode
-# For 16b input, run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./run_app_stereo_16bit.sh
   \endcode
-# Output will be sent to display.

# Steps to run the application on PC Linux x86_64

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for PC emulation mode
-# A sample "app.cfg" for Stereo Depth with 8b input is provided under "${PSDKR_PATH}/vision_apps/apps/basic_demos/app_stereo/config" folder.Update paths and other fields if necessary.
-# A sample "app_16bit.cfg" for Stereo Depth with 16b input is provided under "${PSDKR_PATH}/vision_apps/apps/basic_demos/app_stereo/config" folder.Update paths and other fields if necessary.
-# For 8b input, run the app as shown below
   \code
   cd ${PSDKR_PATH}/vision_apps/out/PC/x86_64/LINUX/$PROFILE/
   ./vx_app_stereo_depth --cfg ${PSDKR_PATH}/vision_apps/apps/basic_demos/app_stereo/config/app.cfg
   \endcode
-# For 16b input, run the app as shown below
   \code
   cd ${PSDKR_PATH}/vision_apps/out/PC/x86_64/LINUX/$PROFILE/
   ./vx_app_stereo_depth --cfg ${PSDKR_PATH}/vision_apps/apps/basic_demos/app_stereo/config/app_16bit.cfg
   \endcode
-# Output will be generated as .bmp files in the "output_file_path" folder specified in app.cfg or app_16bit.cfg.
-# View the output images in any image viewer

# Sample Output

Shown below is a example input and its corresponding output

<table>
<tr>
<td>\image html vx_app_stereo_left_in.png "Left Input" </td>
<td>\image html vx_app_stereo_right_in.png "Right Input" </td>
<td>\image html vx_app_stereo_disparity_out.png "Output Stereo Disparity Map" </td>
<td>\image html vx_app_stereo_histo_out.png "Output Disparity Histogram Map" </td>
</tr>
</table>

Shown below is a sample display on J7 EVM

\image html vx_app_stereo_evm_display.jpg

