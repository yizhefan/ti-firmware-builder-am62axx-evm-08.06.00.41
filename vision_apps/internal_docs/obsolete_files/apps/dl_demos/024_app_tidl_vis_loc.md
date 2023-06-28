# Visual Localization Application {#group_apps_valet_parking_app_visual_localization}

# Introduction

This application shows a demo of camera based visual localization algorithm. Currently
feature point detection and corresponding descriptor preperation is offline, and are read
from file by algorithm. Map data is read from file. Output of the algorithm is 3D pose of the
camera with respect to world co-ordinate system.

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | SoC
----------|--------------|-----------------|------
Support   | YES          | NO              | J721e

# Data flow

\image html vx_app_visual_localization_data_flow.jpg

# Steps to run the application

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS
-# Copy "app_vl.cfg" from the folder "vision_apps/apps/valet_parking/app_visual_localization/config" to the
   executable folder "vision_apps/out/PC/x86_64/LINUX/release/"
-# Edit config file to modify the fields as required
-# Run the app as shown below
   \code
   ./vx_app_visual_localization --cfg app_vl.cfg
   \endcode
-# View the output images in any image viewer

# Sample Output

Shown below is a example output from algorithm. Output image (1280x720) shows only translation parameter in X and Y direction.
Each frame execution plots one tiny rectangular spot in output image. (0,0) of output image is mapped to X=-27m, and Y = 60m
in 3D world co-ordinate system. Pixel per meter in horizontal (x) direction is 14.4, and vertical (y) direction is 3.8.

<table>
<tr>
<td>\image html vx_app_visual_localization_out.png "Output" </td>
</tr>
</table>


