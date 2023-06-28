# 3D Surround View Application File Read {#group_apps_srv_demos_app_srv_fileio}

[TOC]

# Introduction

This application demonstrates the functionality of a Surroundview image generation.
The demo application generates a GPU LUT on the DSP that describes the Surroundview bowl surface.
This bowl surface is given to a node running the openGL rendering code to render a Surroundview image
to a file.

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode   | SoC
----------|--------------|-----------------|-----------------|----
Support   | NO           | YES             | YES             | J721e / J721S2 / J784S4

# Steps to run the application

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS
-# The input files come from "${VX_TEST_DATA_PATH}/test_data/psdkra/srv/applibTC_2mpix/"
-# In Linux+RTOS or QNX+RTOS mode
   -# The file "app_srv.cfg" will be copied from the folder "vision_apps/apps/basic_demos/app_linux_fs_files" to the /opt/vision_apps folder.
   -# VX_TEST_DATA_PATH environment variable is set as part of vision_apps_init.sh
   -# Run the app as shown below on the J7 EVM target
      \code
      cd /opt/vision_apps
      source ./vision_apps_init.sh
      ./vx_app_srv_fileio --cfg app_srv.cfg
      \endcode
-# The output SRV image will be output to a file called "srv_output_file.bin"
-# The output image is in RGB format and is either 1920x1080 or 1280x720 depending on what was specified in the config file
   It can be viewed with an image viewer that supports this file format, such as 7YUV

