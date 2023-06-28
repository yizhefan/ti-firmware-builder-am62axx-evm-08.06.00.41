# Vision Apps Introduction {#mainpage}

[TOC]

# Introduction

"Vision Apps" integrates different components within
Processor SDK RTOS (PSDK RTOS) like Free RTOS,
PDK, OpenVX, Algorithms to realize system applications for vision processing
like surround view, auto park assist.

"Vision Apps" is a component within Processor SDK RTOS as shown in below figure

\image html psdk_rtos_block_diagram.png "Processor SDK RTOS Block Diagram" width=90%

# Directory structure

The main modules in Vision Apps are listed below

Vision Apps Modules                         | Description
--------------------------------------------|------------
platform/${SOC}/rtos                        | Application demo executables for Free RTOS on DSP's and R5F's
platform/${SOC}/linux                       | Linux-specific library for common app init functionality
platform/${SOC}/qnx                         | QNX-specific library for common app init functionality
apps/basic_demos/                           | Application demo executables for different demos on Linux A72
apps/dl_demos/                              | ^
apps/cv_demos/                              | ^
apps/srv_demos/                             | ^
kernels/                                    | OpenVX kernels/node used to make various applications
utils                                       | Additional utility functions used on the SoC
tools                                       | Additional utility tools and scripts used on PC (x86_64) side
concerto                                    | Makefile build infrastructure
docs                                        | User documentation
out                                         | Build generated files and executables


