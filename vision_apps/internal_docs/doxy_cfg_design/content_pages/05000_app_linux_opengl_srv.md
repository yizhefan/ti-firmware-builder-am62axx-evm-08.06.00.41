# Design Topic : Surround View

[TOC]

# Requirements Addressed {#did_app_srv_requirements}

- https://jira.itg.ti.com/browse/ADASVISION-1811

# Introduction {#did_app_srv_intro}

## Purpose {#did_app_srv_purpose}

The Surround View application is used to show the capabilities of
J7 SoC to implement Surround View algorithms.

## Short Application Description {#did_app_srv_short_desc}

- Two graphs are created in this application
   - The first generates the GPU LUT
   - The second generates a SRV image
- In PC Emulation mode, the output image is rendered to the screen

## Input and Output format {#did_app_srv_io_format}

- The application takes in a cfg file containing the values for the
  following:
   - A flag for whether the images are 1MP or 2MP
   - The bowl parameters
   - The view parameters
- Four BMP input images that are read by the application
- An output SRV image

# Directory Structure {#did_app_srv_dir_structure}

SRV application is at below path in vision apps::

    ├── apps                              # Applications
    │   ├── basic_demos                   # Application Group
    │   │   ├── app_linux_opengl_srv      # SRV application
    │   │   │   └── config                # Default configuration file

# Diagrams {#did_app_srv_diagrams}

## OpenVX Graph {#did_app_srv_openvx_graph}

Below application diagram shows two OpenVX graphs that comprise the SRV application.
The first graph is run as updates to the bowl shape are needed.  The two nodes that
comprise this graph are connected via the srv_bowl_lut_gen applib.
The second graph runs at the desired frame rate.

![](app_srv_data_flow.png)

# Resource usage {#did_app_srv_resource_usage}
1. Generate3DBowl and Generate3DLUT are run on DSP
2. OpenGL node is run on GPU

# Error handling {#did_app_srv_error_handling}
1. Input parameters specified in configuration file are validated by the application.
   In case of any error application either exits or uses a default
2. The SRV nodes have validate callbacks to verify the inputs and outputs before
   running the graph.

# Interface {#did_app_srv_interface}

Following configuration options will be supported in the configuration file::

    # If 2MP is used, set to 1, otherwise set to 0
    is2MP 1

    # Bowl offset
    offsetXleft    -400
    offsetXright   400
    offsetYfront   -450
    offsetYback    450

    # view parameters
    camx 0
    camy 0
    camz 480
    targetx 0
    targety 0
    targetz 0
    anglex 0
    angley 0
    anglez 0

# Design Analysis and Resolution (DAR) {#did_app_srv_dar}

## Design Decision : xyz {#did_app_srv_dar_01}


### Design Criteria: xyz {#did_app_srv_dar_01_criteria}


### Design Alternative: xyz {#did_app_srv_dar_01_alt_01}


### Design Alternative: xyz {#did_app_srv_dar_01_alt_02}


### Final Decision {#did_app_srv_dar_01_decision}


