# Camera Based Semantic Segmentation Application Datasheet {#group_apps_dl_demos_app_tidl_seg_cam_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   2.73 
mcu2_0    |  11. 0 
mcu2_1    |   1. 0 
 c6x_1    |   2. 0 
 c6x_2    |   7. 0 
 c7x_1    |  18. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  11.46 % ( 65 MP/s )
  LDC     |  10.26 % ( 64 MP/s )
  MSC0    |  18.75 % ( 114 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1239 MB/s  |   3541 MB/s
WRITE BW |    447 MB/s  |   1892 MB/s
TOTAL BW |   1686 MB/s  |   5433 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.34 %
      REMOTE_SRV   |   0.11 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0.92 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0.90 %
      TIVX_V1SC1   |   3. 8 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   3.12 %
      TIVX_CAPT1   |   0.77 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.73 %
      TIVX_DISP2   |   0. 0 %
      TIVX_CSITX   |   0. 0 %
      TIVX_CAPT3   |   0. 0 %
      TIVX_CAPT4   |   0. 0 %
      TIVX_CAPT5   |   0. 0 %
      TIVX_CAPT6   |   0. 0 %
      TIVX_CAPT7   |   0. 0 %
      TIVX_CAPT8   |   0. 0 %
     TIVX_DPM2M1   |   0. 0 %
     TIVX_DPM2M2   |   0. 0 %
     TIVX_DPM2M3   |   0. 0 %
     TIVX_DPM2M4   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16265728 B |  96 %
          L3_MEM |     262144 B |     245248 B |  93 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 9 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_1   |   0. 0 %
        TIVX_SDE   |   0. 0 %
        TIVX_DOF   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16773376 B |  99 %
          L3_MEM |     262144 B |     262144 B | 100 %

##CPU: c6x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 7 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   1.81 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16754432 B |  99 %
          L2_MEM |     229376 B |          0 B |   0 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c6x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 9 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   6.89 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16773376 B |  99 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 3 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  17.59 %
     TIVX_C71_P2   |   0. 0 %
     TIVX_C71_P3   |   0. 0 %
     TIVX_C71_P4   |   0. 0 %
     TIVX_C71_P5   |   0. 0 %
     TIVX_C71_P6   |   0. 0 %
     TIVX_C71_P7   |   0. 0 %
     TIVX_C71_P8   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |  268435456 B |  250812416 B |  93 %
          L3_MEM |    8159232 B |          0 B |   0 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  385344256 B |  99 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32398 |  32143 /  32659 |        309

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   30.86


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 10       |    774


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
             capture_node (  CAPTURE1)    |  32377    |  32244 /  63476   |        774
                viss_node (VPAC_VISS1)    |   4417    |   4373 /   4648   |        774
                aewb_node (    MCU2-0)    |    208    |     49 /   3940   |        774
                 ldc_node ( VPAC_LDC1)    |   3431    |   3363 /  20483   |        774
              scaler_node ( VPAC_MSC1)    |   5188    |   5127 /   5305   |        774
              PreProcNode (     DSP-1)    |    499    |    485 /   2760   |        774
                tidl_node (  DSP_C7-1)    |   5662    |   5641 /   6191   |        774
             PostProcNode (     DSP-2)    |   2135    |   2112 /   2249   |        774
              mosaic_node ( VPAC_MSC1)    |   1306    |   1165 /  25351   |        774
              DisplayNode (  DISPLAY1)    |   8540    |     84 /  16900   |        774
