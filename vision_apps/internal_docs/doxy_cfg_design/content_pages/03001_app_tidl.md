# Application Design : TI Deep Learning

[TOC]

# Requirements Addressed {#did_app_tidl_requirements}

- https://jira.itg.ti.com/browse/ADASVISION-1795
- https://jira.itg.ti.com/browse/ADASVISION-1796

# Introduction {#did_app_tidl_intro}

## Purpose {#did_app_tidl_purpose}
Sample application to show Deep Learning capabilities of TIDL libarary

## Short Application Description {#did_app_tidl_short_desc}
The application performs image classification and object detection using TI Deep Learning libarary.
Prerequisites are
- Pre-trained, quantized network which is result of TIDL Graph Import tool
- Resized image in bmp-format which is result of tidl_image_resize.py python script

## Input and Output format {#did_app_tidl_io_format}
- Accepts a pre-trainied network run through TIDL import tool
- Accepts a pre-processed input (raw format)
- For image classification output class is printed on console
- For object detection output image is written

# Directory Structure {#did_app_tidl_dir_structure}

TIDL application is at below path in vision apps::

    ├── apps                              # Applications
    │   ├── basic_demos                   # Application Group
    │   │   ├── app_tidl                  # TIDL application
    │   │   │   └── config                # Default configuration file


# Diagrams {#did_app_tidl_diagrams}

## Sequence Diagram {#did_app_tidl_sequence_diagram}

![](app_tidl_sequence_diagram.png)

## Component Interaction {#did_app_tidl_component_interaction}

![](app_tidl_component_interaction.png)

## OpenVX Graph {#did_app_tidl_openvx_graph}

Below graph is generated after running the TIDL app.
Design document is updated after application was tested to include this.

![](app_tidl_data_flow.jpg)

# Resource usage {#did_app_tidl_resource_usage}
1. TIDL node will run on C7x + MMA target
2. Image classification output will be printed on OpenVx Host console

# Error handling {#did_app_tidl_error_handling}
1. Input parameters specified in configuration file are validated by the application.
   In case of any error application either exits or uses a default
2. If input or output files are not present application will exit or continue
   gracefully, i.e it will not crash and lock up the system

# Interface {#did_app_tidl_interface}
Following configuration options will be supported in the configuration file::

    # location of conifg
    tidl_config_file_path   <network_path>/config.bin

    # location of network
    tidl_network_file_path  <network_path>/network.bin

    # location of input
    input_file_path         <input_path>/input.bmp

    # location of output (for object detection only)
    output_file_path        <output_path>/output.bmp

    # Mode of operation 0: image classification, 1: object detection
    operation_mode     0


# Design Analysis and Resolution (DAR) {#did_app_tidl_dar}
NA

## Design Decision : None {#did_app_tidl_dar_01}
NA

### Design Criteria: None {#did_app_tidl_dar_01_criteria}
NA

### Design Alternative: None {#did_app_tidl_dar_01_alt_01}
NA

### Design Alternative: None {#did_app_tidl_dar_01_alt_02}
NA

### Final Decision {#did_app_tidl_dar_01_decision}
NA
