# Visual localization Application {#group_apps_dl_demos_app_tidl_vl}

[TOC]

# Introduction

The tasks involved in Autonomous/Automated Driving (AD) can be categorized in 4 modules of 1) perception, 2) localization 3) mapping and 4) planning and control. The localization module determines position of the vehicle i.e. 3D location and orientation and plays a critical role in safe and comfortable navigation. Localization is the process of estimating vehicle position in the paradigm of map-based navigation. When localization process uses only camera sensor it is referred to as Visual Localization. Visual localization is particularly valuable when other means of localization are unavailable e.g. GPS denied environments such as indoor locations. Also Visual Localization output can be fused with noisy Inertial Navigation System(INS) to obtain overall accurate vehicle positions. Along with self-driving cars other applications include robot navigation and augmented reality.

---

# Supported platforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | YES          | YES             |  YES          | J721e / J721S2 / J784S4


# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# A sample "app_vl.cfg" for Visual Localization is provided under "/opt/vision_apps/" on the rootfs partition.
-# Create a folder and keep all the .yuv files used for detection, there are a few images under "/opt/vision_apps/test_data/psdkra/app_visual_localization/inputs".
-# Run the app as shown below
   \code
   /opt/vision_apps>. ./vision_apps_init.sh
   /opt/vision_apps>./run_app_tidl_vl.sh
   \endcode

# Steps to run the application on J7 EVM (QNX + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# A sample "app_vl.cfg" for Visual Localization is provided under "/ti_fs/vision_apps/" on the boot partition.
-# Create a folder and keep all the .yuv files used for detection, there are a few images under "/ti_fs/vision_apps/test_data/psdkra/app_visual_localization/inputs".
-# Run the app as shown below
   \code
   /ti_fs/vision_apps>. ./vision_apps_init.sh
   /ti_fs/vision_apps>./run_app_tidl_vl.sh
   \endcode

# Steps to run the application on PC Linux x86_64

 -# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for PC emulation mode
 -# A sample "app_vl.cfg" for Visual Localization is provided under "${PSDKR_PATH}/vision_apps/apps/dl_demos/app_tidl_vl/config" folder.
 -# Create a folder and keep all the .yuv files used for detection, there are a few images under "/opt/vision_apps/psdkra/app_visual_localization/inputs".
 -# Create an output directory
    \code
    mkdir vl_out
    \endcode
 -# Run the app as shown below
    \code
    ./vx_app_tidl_vl --cfg ${PSDKR_PATH}/vision_apps/apps/dl_demos/app_tidl_vl/config/app_vl.cfg
    \endcode
 -# The output will be written in "vl_out" folder in .yuv (NV12) format.

---

# Visual Localization Algorithm Overview
The example Visual Localization (VL) scheme performs 6-Degrees of Freedom ego-localization using 3D sparse map of area of automated operation and a query image. Each point in the sparse map stores visual and location information of the scene key-point which can be used at localization time to match against the key-points in the query image. The Visual Localization process thus is built around camera pose estimation approach such as Perspective-n-Point (PnP) problem. PnP is the problem of estimating the pose of a calibrated camera given a set of n 3D points in the world and their corresponding 2D projections in the image. Considering the perspective transform as defined in equation below the problem then is to estimate the rotation and translation matrix [R|T] of the camera when provided with N numbers of world(Pw) - camera(Pc) point correspondences.

\image html RT.png

The [R|T] matrix encodes the camera pose in 6 degrees-of-freedom (DoF) which are made up of the rotation (roll, pitch, and yaw) and 3D translation of the camera with respect to the world.
The VL process thus involves detection and description of key-points points in the query image captured by the onboard camera of the vehicle, matching of the 2D image points with that of 3D landmark points from map to establish 2D-3D point correspondences, followed by camera-pose estimation using any of the SolvePnP solutions. This simplified process is depicted in the following Fig. Offline calibration of the camera sensors and vehicles frame of reference provides the extrinsic parameters that can be used to estimate the vehicle pose from the estimated camera pose.

\image html algo_block_diagram.png


---

# Deep learning based descriptor (DKAZE)
Key-point Descriptor plays critical role in ensuring high accuracy during Visual localization process. However descriptor computation can put quite a load on compute resources like CPU/DSPs. To address this issue we adopt Deep Learning based approach. We train deep neural net (DNN) to learn hand computed feature descriptor like KAZE in a supervised manner. We refer such descriptor as DKAZE. As DKAZE uses very similar network architecture as typical semantics segmentation networks, in actual deployment DKAZE can be considered as an additional decoder in multitask network (single encoder + multiple decoders topology).
If one needs to train DKAZE for her own dataset, PyTorch based training script can be provided at request.

\image html dkaze.jpg


---
<div id="dataset"></div>

# Dataset creation using Carla Simulator
Along with the out-of-box example, dataset generated using Carla Simulator 0.9.9.4 (https://carla.org/) is provided. Dataset contains the following,

1. Camera images
2. Ground truth position (6 Degree Of Freedom)
3. Ground truth depth
4. Camera intrinsic (Focal length and camera center)

The snapshot of one of the captured images is shown below.

\image html 0000000304.png


We also make Python script to capture dataset using Carla available on request.



---

# Localization Accuracy

The following is the X-Z plot of the localization position estimates of vehicles with ground truth vehicle positions. It is evident that visual localization output is very close to ground truth locations. Also it is able to handle sharp turn without any issue.

\image html map_X_Z_lat_lon.jpg

Here is the objective comparison with GT positions,
use-case|Average error
--------|-------------
10 fps  |16.3 cm
30 fps  |10.6 cm

---
<div id="DataFlow"></div>

# Visual Localization Data Flow

The following conceptual block diagram indicates where various components of visualization location pipeline get mapped to various HW resources available on SOC.

\image html resources_block_diagram.png

In this demo, we read file based NV12 inputs of resolution 768x384 from SD card and pass it to a resize MSC. As the input resolution is matching with resolution required by DKAZE network, the input and output resolution of resizer stage is the same. This is then passed to C66x_1 for pre-processing where NV12 image is converted to RGB/BGR planar image along with necessary padding as required by TIDL. The DKAZE features are then obtained from TIDL running on C7x and is passed to C66x_2 for pose calculation. The result is then sent to another OpenVx node running on the same C66x_2 core for visualization where the position of the car is indicated by a blue pixel. The intensity of the pixel changes as the ego vehicle moves. This information is overlayed on the top view of the map and passed to SW mosaic node for overlay with front camera image on a 1920x1080 NV12 plane. This plane is submitted to DSS for display.

\if DOCS_J721S2
Note: On J721S2, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the standalone C7X.
\endif

\if DOCS_J784S4
Note: On J784S4, the algorithms running on C66_1 and C66_2 have been recompiled and are running on the second C7X+MMA.
\endif

\image html app_tidl_vl_data_flow.png

---

# Sparse 3D Map
During localization deployment per Kilometer MAP size is one of the critical factors to ensure real time operations. We adopt sparse 3D format for the Map storage, which does not need camera images, resulting in more compact MAP size. With out-of-box visual localization example, the off-line created sparse 3D map is provided. One can create her own off-line map in the following format,

```
#MapParams: MapVersion  FeatureType FeatureParams_1 FeatureParams_2 MapRange_xmin   MapRange_xmax   MapRange_ymin   MapRange_ymax   MapRange_zmin   MapRange_zmax   MapVoxelSize_x  MapVoxelSize_y  MapVoxelSize_z  MapRealDimensions_x MapRealDimensions_y MapRealDimensions_z MapVoxelDimensions_x    MapVoxelDimensions_y    MapVoxelDimensions_z    ImageScalingFactor_x,   ImageScalingFactor_y

#List of voxels
#[X,Y,Z,P]: X_index Y_index Z_index Number_Points_in_Voxel
#pt-1
location_x  location_y  location_z  Feature-descriptor-64-elements
#pt-2
location_x  location_y  location_z  Feature-descriptor-64-elements
#pt-3
location_x  location_y  location_z  Feature-descriptor-64-elements
.......
.......
.......
#pt-Number_Points_in_Voxel
location_x  location_y  location_z  Feature-descriptor-64-elements


```

An example map file,
```
#MapParams: 2 4 3 4 -250 250 -5 5 -250 250 3 3 3 501 11 501 167 4 167 1. 1.


#[X,Y,Z,P]: 0 0 0 0

#[X,Y,Z,P]: 0 0 1 0

#[X,Y,Z,P]: 0 0 2 0

#[X,Y,Z,P]: 1 3 50 2

-247.407654 1.428528 -100.674500 122. 123. 41. 45. 121. 118. 59. 75. 128. 119. 61. 74. 123. 122. 43. 42. 127. 128. 69. 68. 130. 148. 78. 117. 126. 152. 83. 114. 127. 135. 69. 64. 135. 122. 72. 48. 138. 132. 85. 84. 111. 128. 88. 86. 116. 124. 72. 55. 130. 120. 42. 32. 131. 107. 56. 62. 119. 108. 59. 58. 121.
-247.216385 1.640076 -101.193047 122. 123. 44. 46. 127. 118. 57. 77. 128. 116. 61. 75. 129. 123. 39. 46. 126. 133. 68. 76. 134. 156. 76. 140. 121. 154. 78. 132. 128. 133. 61. 75. 135. 120. 70. 54. 144. 109. 88. 93. 111. 106. 92. 92. 124. 118. 66. 56. 130. 123. 41. 38. 131. 119. 55. 67. 124. 122. 58. 69. 126.
#[X,Y,Z,P]: 1 3 51 0

#[X,Y,Z,P]: 1 3 52 2

-247.520508 1.129715 -95.023788 125. 123. 14. 15. 128. 118. 20. 22. 126. 119. 23. 24. 123. 122. 15. 11. 127. 141. 23. 52. 126. 150. 34. 86. 130. 151. 34. 89. 119. 140. 24. 54. 135. 167. 28. 105. 133. 190. 31. 198. 122. 187. 34. 199. 121. 161. 27. 112. 128. 133. 13. 45. 128. 131. 8. 83. 124. 126. 9. 82. 126.
-248.440674 1.048920 -94.369522 124. 123. 9. 16. 127. 125. 18. 22. 128. 119. 23. 24. 124. 130. 16. 16. 124. 143. 19. 44. 120. 147. 27. 71. 137. 144. 33. 73. 128. 142. 26. 46. 134. 167. 19. 109. 126. 209. 23. 200. 129. 206. 25. 198. 124. 161. 19. 106. 129. 124. 12. 52. 129. 118. 9. 91. 124. 116. 9. 88. 126.


.....................
.....................
.....................
```

---

# Optimized Building Blocks for your own Visual localization pipeline
As an out-of-box example end-to-end optimized visual localization pipeline SW is provided. However one can build her own pipeline by either replacing few compute blocks of her own choice or create visual localization pipeline from scratch but accelerate compute heavy blocks by utilizing available optimized blocks. The following are the  optimized compute blocks are available as part of TIADALG component package.

<b> 1. Two Way Descriptor Matching  -</b>

Name of the API:  tiadalg_select_top_feature()

This API can be used to carry out two way matching between 2 sets of descriptors. Typically descriptors sample form the sparse 3D map and descriptors computed using current frame need to be matched to find N best matching pairs. This API does two directional matching to avoid wrong matches resulting from feature less regions like flat surfaces.  It supports unsigned 16 and unsigned 8 bit descriptors datatype. Length of descriptor is configurable. The current example exercises 8 bit unsigned type descriptor with 64 elements per descriptor.

<b> 2. Sparse Up-sampling -</b>

Name of the API:  tiadalg_sparse_upsampling()

DKAZE module generates the descriptors at 1/4th image resolution for optimal memory usage. However Visual localization pipeline needs it at full resolution. To do up-sampling process in most optimal manner this API does up-sampling only on the sparse points selected by key point detector using nearest neighbor resizing. It also applies  7x7 filtering on the resized data.

<b> 3.  Recursive NMS -</b>

Name of the API:  tiadalg_image_recursive_nms()

DKAZE module produces key point score plane buffer at original image resolution. It does scores plane buffer thresholding, and then 3x3 NMS is performed on thresholded score buffer. This is helpful getting relevant interest points in the scenario where scores are saturated in clusters. These compute blocks executes on C66x DSP as part of visual localization OpenVX node.

<b> 4. Perspective N Point Pose estimation, a.k.a. SolvePnP -</b>

Name of the API:  tiadalg_solve_pnp()

After establishing 2D-3D correspondences using, two way descriptor matching API shown above, one can use perspective N point API to find 6 DOF camera pose.

More details about these API interfaces and compute performance details are provided in user guide and data sheet of TIADALG component.

</a>

---

# Sample Output
The following picture shows trajectory of estimated positions generated using visual localization (thick blue line) on the top-view image. The thin white line indicates ground truth trajectory.

\image html app_tidl_vl_output.png

---
