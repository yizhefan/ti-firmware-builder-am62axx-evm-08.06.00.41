# Mosaic Application File Read {#group_apps_basic_demos_app_arm_mosaic}

[TOC]

# Introduction

This application demonstrates the functionality of a mosaic image generation using OpenGL.
It takes in an input image and generates a 2x2 mosaic of that image.

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode   | SoC
----------|--------------|-----------------|-----------------|----
Support   | NO           | YES             | YES             | J721e / J721S2 / J784S4

# Steps to run the application

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS
-# The input file comes from "${VX_TEST_DATA_PATH}/test_data/psdkra/app_opengl_mosaic/input1_file.bin"
-# In Linux+RTOS or QNX+RTOS mode
   -# VX_TEST_DATA_PATH environment variable is set as part of vision_apps_init.sh
   -# Run the app as shown below on the J7 EVM target
      \code
      cd /opt/vision_apps
      source ./vision_apps_init.sh
      ./vx_app_arm_opengl_mosaic.out
      \endcode
-# The output mosaic image will be output to a file called "mosaic_output_file.bin" at the location "/opt/vision_apps/test_data/output/".
-# The output image is in RGB format with dimensions 1920x1080.  It can be viewed with an image viewer that supports this file format,
   such as 7YUV.

