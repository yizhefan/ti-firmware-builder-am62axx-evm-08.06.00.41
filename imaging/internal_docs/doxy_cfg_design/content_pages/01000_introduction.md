# Introduction {#mainpage}

[TOC]

Imaging module includes TI's implementation of 

1. Reference IQ (Image Quality) enhancement algorithms 
2. Reference 2A (AutoExposure + AutoWhiteBalance) algorithms
3. Framework components. 
4. Reference driver for IMX390 image sensor
5. Reference driver for AR0233 image sensor
6. Reference driver for AR0820 image sensor
7. Reference driver for UB96x and UB97x RAW12 Test Pattern
8. Reference driver for UB96x and UB97x YUV422 (UYVY) Test Pattern
9. Reference driver for LI-AR0233-GW5200 YUV422 (UYVY) Camera


The deliverables included in this module enable camera based applications to leverage high quality features
offered by VISS Hardware Accelerator. Imaging module does not cover VISS drivers or the OpenVX kernels for VISS, 
only the algorithms which return the results to application so that VISS kernel/driver can update the registers for 
optimal pixel processing.

This document describes the design of imaging component.

# Functional Overview {#did_intro_overview}

1. Based on OpenVX version v1.1.

2. Supported Platforms 
   1. J721E EVMs
   2. PC emulation mode - Limited support. Sensor drivers and Auto Exposure are not supported in PC mode.

3. Current version includes
   1. AutoWhiteBalance + AutoExposure Algorithms
   2. AEWB node compliant with OpenVX 1.1
   3. Dynamic Camera Configuration Algorithm
   4. Tuning support for the following plugins
     3.1 Black Level Compensation (BLC)
     3.2 AutoWhiteBalance (AWB)
     3.3 Color Accuracy (RGB2RGB)
     3.4 Noise Filter (NSF4)
     3.5 Wide Dynamic Range (WDR)
     3.6 H3A statistics (H3A)
     3.7 Lens Distortion Correction (Mesh LDC)
     3.8 Edge Enhancement (EE)
     3.9 Lens Shading Correction (LSC)
     3.10 Color Filter Array Interpolation (CFAI)

     


# Assumptions and Constraints {#did_intro_assumptions}

1. Code is written using ANSI C language

2. Code uses stdint.h data types.

3. The application assumes below basic services are avaiable to it

   1. printf API which redirects logs to UART console or CCS console
   2. fopen, fclose, fseek, fread, fwrite stdio.h for file IO to a
      removeable media like SD card
   3. Memory allocation, OS services like task, semaphore and IPC
      services are assumed

# Features Not Supported {#did_intro_features_not_supported}

1. Tuning plugins other than mentioned in section 3.4
2. 12-bit YUV Output Tuning

# Revision History {#did_rev_history}

Revision | Date          | Author                 | Description
---------|---------------|------------------------|-----------------------------------------------------------------
0.5      | 18 Jan 2019 | Mayank Mangla | First draft
0.8      | 31 March 2019 | Mayank Mangla | Release documentation for 0.8 release
0.9      | 12 July 2019  | Mayank Mangla | Release documentation for 0.9 release
1.0      | 08 Oct 2019   | Mayank Mangla | Release documentation for 1.0 release
1.1      | 11 Nov 2020   | Mayank Mangla | Release documentation for 7.01 release
8.0      | 09 July 2021  | Mayank Mangla | Release documentation for 8.00 release