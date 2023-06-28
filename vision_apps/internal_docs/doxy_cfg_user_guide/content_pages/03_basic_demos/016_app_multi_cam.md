# Multi Camera VPAC Application {#group_apps_basic_demos_app_multi_cam}

[TOC]

# Introduction

This application demonstrates the use of VISS node and TI's 2A (AE & AWB) algorithm in multi-channel mode. The dataflow is

1. Configure image sensors using sensor driver f/w APIs 
2. Acquire RAW images from camera sensors
3. Process the images on VISS
4. Run AE (Auto Exposure) and AWB (AutoWhiteBalance) algorithms, using H3A output from VISS
5. Run LDC on the output of VISS
6. Scale LDC output using MSC and arrange in a mosaic
7. Display the mosaic using Display Sub-System

Any number of cameras from 1-8 can be selected by the user, subject to constraints of sensor driver. 
The application assumes that all cameras are identical.



# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2 / J784S4


# Data flow (RAW12 input)

\image html app_multi_cam_data_flow.png

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# Connect four D3-IMX390 RCM sensor boards on cam0, cam1, cam2 and cam3 input ports of the Fusion1 board.
-# Use the is_interactive flag to run the demo in an interactive mode which allows the user to print performance characteristics on the UART console.
-# Run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./run_app_multi_cam.sh
   \endcode
-# The processed output for one of the captured channel is displayed through DSS on HDMI or eDP display - Display interface selection through compile time setting in the file platform/$(SOC)/rtos/common/app_cfg_mcu2_0.h

# Steps to run the application on J7 EVM (QNX + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# Connect four D3-IMX390 RCM sensor boards on cam0, cam1, cam2 and cam3 input ports of the Fusion1 board.
-# Use the is_interactive flag to run the demo in an interactive mode which allows the user to print performance characteristics on the UART console.
-# Run the app as shown below
   \code
   cd /ti_fs/vision_apps
   . ./vision_apps_init.sh
   ./run_app_multi_cam.sh
   \endcode
-# The processed output for one of the captured channel is displayed through DSS on HDMI or eDP display - Display interface selection through compile time setting in the file platform/$(SOC)/rtos/common/app_cfg_mcu2_0.h

