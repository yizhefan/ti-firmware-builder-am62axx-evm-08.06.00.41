# Dense Optical Flow Application {#group_apps_basic_demos_app_dof}

[TOC]

# Introduction

This application shows a demo of the TI Dense Optical Flow (DOF) hardware
accelerator (HWA). The demo takes as input a sequence of input 8b gray scale
images in .png or .bmp file format. Each frame is run through dense optical flow OpenVX
node. The flow vectors and confidence scores computed by DOF node are mapped based on
a color palatte and output to files in .png or .bmp format, or to display if
DISPLAY_OPTION is enabled.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | YES          | YES             |  YES          | J721e / J721S2 / J784S4

# Data flow

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

![](vx_app_dof_data_flow.png "Data flow")

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample "app_dof.cfg" for Dense Optical Flow is provided under "/opt/vision_apps/" on the rootfs partition. Update paths and other fields if necessary.
-# Run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./run_app_dof.sh
   \endcode
-# Output will be sent to display.

# Steps to run the application on J7 EVM (QNX + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# A sample "app_dof.cfg" for Dense Optical Flow is provided under "/ti_fs/vision_apps/" on the boot partition. Update paths and other fields if necessary.
-# Run the app as shown below
   \code
   cd /ti_fs/vision_apps
   . ./vision_apps_init.sh
   ./run_app_dof.sh
   \endcode
-# Output will be sent to display.

# Steps to run the application on PC Linux x86_64

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for PC emulation mode
-# A sample "app.cfg" for Dense Optical Flow is provided under "${PSDKR_PATH}/vision_apps/apps/basic_demos/app_dof/config" folder.Update paths and other fields if necessary.
-# Run the app as shown below
   \code
   cd ${PSDKR_PATH}/vision_apps/out/PC/x86_64/LINUX/$PROFILE/
   ./vx_app_dense_optical_flow --cfg ${PSDKR_PATH}/vision_apps/apps/basic_demos/app_dof/config/app.cfg
   \endcode
-# Output will be generated as .bmp files in the "output_file_path" folder specified in app.cfg
-# View the output images in any image viewer

# Sample Output

Shown below is a example input and its corresponding output

<table>
<tr>
<td>![](vx_app_dof_input.png "Input" )</td>
<td>![](vx_app_dof_output2.png "Output DOF Flow Vectors") </td>
<td>![](vx_app_dof_output1.png "Output DOF Confidence Map, White is high confidence, Dark is low confidence") </td>
</tr>
</table>


Shown below is a sample display on J7 EVM

\image html vx_app_dof_evm_display.jpg
