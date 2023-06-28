# Thermal Load Test (VISS) {#group_apps_utilities_app_thermal_load_viss}

[TOC]

# Introduction

This application can be used to load VISS Hardware Accelerator. Load ranging from 1-100% for a specifed
time in seconds. The demo takes two input paramters. Load in percentage, time in seconds.

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2 / J784S4

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# Run the app as shown below: The below example loads 90% for 10 Seconds
   \code
   cd /opt/vision_apps
   ./vx_app_load_viss.out 90 10
   \endcode
-# Output will be sent to the terminal standard output.

# Sample Output

Shown below is a example input and its corresponding output

```
./vx_app_load_viss.out 100 10
The number of args is 3
The arguments are 100 and 10
APP: Init ... !!!
MEM: Init ... !!!
MEM: Initialized DMA HEAP (fd=4) !!!
MEM: Init ... Done !!!
IPC: Init ... !!!
IPC: Init ... Done !!!
REMOTE_SERVICE: Init ... !!!
REMOTE_SERVICE: Init ... Done !!!
APP: Init ... Done !!!
120051.775793 s:  VX_ZONE_INIT:Enabled
120051.775832 s:  VX_ZONE_ERROR:Enabled
120051.775837 s:  VX_ZONE_WARNING:Enabled
120051.778101 s:  VX_ZONE_INIT:[tivxInit:71] Initialization Done !!!
120051.778260 s:  VX_ZONE_INIT:[tivxHostInit:48] Initialization Done for HOST !!!
Processing time for Resolution 1936x1096 is Time 9742.646ms
120061.642072 s:  VX_ZONE_INIT:[tivxHostDeInit:56] De-Initialization Done for HOST !!!
120061.646387 s:  VX_ZONE_INIT:[tivxDeInit:111] De-Initialization Done !!!
APP: Deinit ... !!!
REMOTE_SERVICE: Deinit ... !!!
REMOTE_SERVICE: Deinit ... Done !!!
IPC: Deinit ... !!!
IPC: DeInit ... Done !!!
MEM: Deinit ... !!!
MEM: Alloc's: 8 alloc's of 11841687 bytes
MEM: Free's : 8 free's  of 11841687 bytes
MEM: Open's : 0 allocs  of 0 bytes
MEM: Deinit ... Done !!!
APP: Deinit ... Done !!!
```
