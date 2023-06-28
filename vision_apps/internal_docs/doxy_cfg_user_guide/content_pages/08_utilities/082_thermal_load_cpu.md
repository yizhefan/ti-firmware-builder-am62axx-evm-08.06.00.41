# Thermal Load Test (CPU) {#group_apps_utilities_app_thermal_load_cpu}

[TOC]

# Introduction

This application can be used to load C7x, C6x, R5F ranging from 1-99% for a specifed
time in seconds. The demo takes three input paramters. core, load in percentage, timer in seconds.

Core  | Value
------|------
C7x   | 2
MCU2  | 4
C6x   | 5
MCU3  | 6
MCU4  | 7

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2 / J784S4

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# Run the app as shown below: The below example loads C7x(Core - 2) at 80% for 5 seconds.
   \code
   cd /opt/vision_apps
   ./vx_app_load_test.out 2 80 5
   \endcode
-# Output will be sent to the terminal standard output.

# Sample Output

Shown below is a example input and its corresponding output

```
./vx_app_load_test.out 2 80 5

APP: Init ... !!!
MEM: Init ... !!!
MEM: Initialized DMA HEAP (fd=4) !!!
MEM: Init ... Done !!!
IPC: Init ... !!!
IPC: Init ... Done !!!
REMOTE_SERVICE: Init ... !!!
REMOTE_SERVICE: Init ... Done !!!
APP: Init ... Done !!!
core is 2 The load is 80 percent time is 5 seconds
118913.375297 s: REMOTE_SERVICE_TEST: Started load test for CPU c7x_1 load is 80 !!!
CPU:  c7x_1: TOTAL LOAD =  79.34 % ( HWI =   0. 3 %, SWI =   0. 4 % )
CPU:  c7x_1: TOTAL LOAD =  79.36 % ( HWI =   0. 3 %, SWI =   0. 4 % )
CPU:  c7x_1: TOTAL LOAD =  79.36 % ( HWI =   0. 3 %, SWI =   0. 4 % )
118919.378189 s: REMOTE_SERVICE_TEST: Stopped load test for CPU c7x_1 !!!
APP: Deinit ... !!!
REMOTE_SERVICE: Deinit ... !!!
REMOTE_SERVICE: Deinit ... Done !!!
IPC: Deinit ... !!!
IPC: DeInit ... Done !!!
MEM: Deinit ... !!!
MEM: Alloc's: 0 alloc's of 0 bytes
MEM: Free's : 0 free's  of 0 bytes
MEM: Open's : 0 allocs  of 0 bytes
MEM: Deinit ... Done !!!
APP: Deinit ... Done !!!
APP IPC TIOVX: Done !!!
```
