# Application Design : Dense Optical Flow

[TOC]

# Requirements Addressed {#did_app_dof_requirements}

- https://jira.itg.ti.com/browse/ADASVISION-1791
- https://jira.itg.ti.com/browse/ADASVISION-1826

# Introduction {#did_app_dof_intro}

## Purpose {#did_app_dof_purpose}

Dense Optical Flow application is used show the capabilities of
DOF HWA.

## Short Application Description {#did_app_dof_short_desc}

- Process a sequence of frames via the DOF HWA
- Output the DOF flow vector and confidence measure in a format which can be viewed by the user

## Input and Output format {#did_app_dof_io_format}

- This application reads input as a sequence of .png files and feeds as grayscale 8b images to DOF TIOVX node
- Gaussian pyramid TIOVX kernel is used to generate image pyramid of DOF
- This application outputs flow vector as a RGB image and confidence measure as a grayscale image
  using DofVisualize TIOVX node
- PNG Rd/Wr utility is from TIOVX is used to read/write files
- The input file sequence, output file sequence, input width, height and other properties are specified
  in a configuration file

# Directory Structure {#did_app_dof_dir_structure}

DOF application is at below path in vision apps::

    ├── apps                              # Applications
    │   ├── basic_demos                   # Application Group
    │   │   ├── app_dof                   # DOF application
    │   │   │   └── config                # Default configuration file

# Diagrams {#did_app_dof_diagrams}

## Sequence Diagram {#did_app_dof_sequence_diagram}

![](/app_dof_sequence_diagram.png)

## Component Interaction {#did_app_dof_component_interaction}

![](app_dof_component_interaction.png)

## OpenVX Graph {#did_app_dof_openvx_graph}

Below graph is generated after running the DOF app.
Design document is updated after application was tested to include this.

![](app_dof_data_flow.jpg)

# Resource usage {#did_app_dof_resource_usage}
1. DOF node will run on DOF HWA target
2. DOF visualization will run on either OpenVX HOST
3. Guassiun Pyramid will run using scalar HWA

# Error handling {#did_app_dof_error_handling}
1. Input parameters specified in configuration file are validated by the application.
   In case of any error application either exits or uses a default
2. If input or output files are not present application will exit or continue
   gracefully, i.e it will not crash and lock up the system

# Interface {#did_app_dof_interface}

Following configuration options will be supported in the configuration file::

    # location of input *.png files
    input_file_path   /ti/j7//test_data/database/sequence0001/camera002/data

    # filename prefix, input file name is constructed as <filename prefix>ddddd.png,
    # e.g. tidof_00000.png, tidof_00001.png, ...
    input_file_prefix 00000

    # location of output *.png files
    output_file_path  ./dof_out/

    # filename prefix, output file name is constructed as
    # <filename prefix>_flov_ddddd.png, <filename prefix>_conf_ddddd.png, <filename prefix>_raw_ddddd.png
    # e.g. tidof_flov_00000.png, tidof_conf_00000.png, ...
    out_file_prefix   00000

    # first file number to pick as input
    start_seq         1

    # last file number to pick as input
    end_seq           10

    # input data width in pixels
    width             1280

    # input data height in lines
    height            720

    # dof levels, valid range: 2..5
    dof_levels        5

    # enable saving of intermediate outputs to file
    #save_intermediate_output

    # enable temporal predicton flow vector, valid values are 0 or 1
    enable_temporal_predicton_flow_vector 1

# Design Analysis and Resolution (DAR) {#did_app_dof_dar}

## Design Decision : none {#did_app_dof_dar_01}
na

### Design Criteria: none {#did_app_dof_dar_01_criteria}
na

### Design Alternative: none {#did_app_dof_dar_01_alt_01}
na

### Design Alternative: none {#did_app_dof_dar_01_alt_02}
na

### Final Decision {#did_app_dof_dar_01_decision}
na
