# Introduction {#mainpage}

[TOC]

"Vision Apps" integrates different components within Processor SDK RTOS
(PSDK RTOS) like SysBIOS, PDK, OpenVX, Algorithms to realize
system applications for vision processing like surround view,
park assist, front camera, CMS.

"Vision Apps" is component within Processor SDK RTOS as shown in below
figure

![](vision_apps_block_diagram.png "Vision Apps position within Processor SDK RTOS")

This document describes the design of Vision Apps compoment within
Processor SDK RTOS

# Functional Overview {#did_intro_overview}

1. "Vision Apps" is the applications layer of Processor SDK RTOS.
   Most of the applications in "Vision Apps" is built using the OpenVX
   middleware API. Some applications can be built indepedant of OpenVX APIs

2. "Vision Apps" can be built for Embedded Linux, RTOS as well
   as x86 Linux PC's. Not all applications will work on all operating
   systems

3. The application can be split into a "HOST" application logic and
   "target" side application nodes. "HOST" application resides on A72
   running Linux or RTOS. "target" side OpenVX kernels runs on RTOS.

4. Some OpenVX sub graphs are represented as "application library" or
   "applib". This is done to keep the application code modular and allow
   reuse of application library code across multiple applications

5. "Vision Apps" includes the BIOS, IPC, PDK configuration to build the
   final executable which runs on a CPU. The build system allows to
   build multiple CPU binaries with a single "make" invocation

6. "Vision Apps" also includes utility, or infrastructure code to
   interface "TIOVX" with Linux and RTOS. It also includes utilities
   for console IO, file IO, remote CPU logging, performance
   measurement. Its makes of PDK, RTOS or other for these and in most
   this utility API is a wrapper on top existing compoment APIs

# Assumptions and Constraints {#did_intro_assumptions}

1. Code is written using ANSI C language

2. Code uses stdint.h data types.

3. Below basic Misra-C rules are followed to allow Misra-C compatibility
   in future

   1. No multiple return in a function
   2. No continue in a loop
   3. No multiple break in a loop

5. The application assumes below basic services are avaiable to it

   1. printf API which redirects logs to UART console or CCS console
   2. fopen, fclose, fseek, fread, fwrite stdio.h for file IO to a
      removeable media like SD card

# Features Not Supported {#did_intro_features_not_supported}

1. Not applications will run on all OS's. The user guide
   will list the compatible OS's for a application


# Revision History {#did_rev_history}

Revision | Date          | Author             | Description
---------|---------------|--------------------|-----------------------------------------------------------------
0.1      | 24 July 2018  | Kedar C            | First draft, added top level design, DOF app design
