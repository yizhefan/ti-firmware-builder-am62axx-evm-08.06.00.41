# Camera Based Object Detection Application Datasheet {#group_apps_dl_demos_app_tidl_od_cam_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   1.21 
mcu2_0    |   9. 0 
mcu2_1    |   1. 0 
 c7x_1    |  35. 0 
 c7x_2    |  15. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  10.66 % ( 65 MP/s )
  LDC     |   9.21 % ( 64 MP/s )
  MSC0    |  18.10 % ( 128 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   2883 MB/s  |  13556 MB/s
WRITE BW |   1360 MB/s  |   8678 MB/s
TOTAL BW |   4243 MB/s  |  22234 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.30 %
      REMOTE_SRV   |   0. 6 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0.99 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0.63 %
      TIVX_V1SC1   |   2.13 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   2.77 %
      TIVX_CAPT1   |   0.48 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.44 %
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
   DDR_LOCAL_MEM |   16777216 B |   16048640 B |  95 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 8 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_1   |   0. 0 %
        TIVX_SDE   |   0. 0 %
        TIVX_DOF   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16773120 B |  99 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 5 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  34.13 %
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

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |  268435456 B |  239607552 B |  89 %
          L3_MEM |    3964928 B |     950272 B |  23 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  382257093 B |  99 %

##CPU: c7x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 7 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  14.30 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16772096 B |  99 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32397 |  32230 /  32598 |        308

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   30.86


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 10       |    773


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
             capture_node (  CAPTURE1)    |  32401    |  32276 /  63593   |        773
                viss_node (VPAC_VISS1)    |   3951    |   3922 /   4181   |        773
                aewb_node (    MCU2-0)    |    263    |     35 /   4651   |        773
                 ldc_node ( VPAC_LDC1)    |   3064    |   2998 /  15925   |        773
              scaler_node ( VPAC_MSC1)    |   4636    |   4588 /   5014   |        773
              PreProcNode (     DSP-1)    |   2904    |   2768 /   3262   |        773
                tidl_node (  DSP_C7-1)    |  10999    |  10941 /  11599   |        773
    DrawBoxDetectionsNode (     DSP-1)    |   1640    |   1591 /   2494   |        773
              mosaic_node ( VPAC_MSC1)    |   1601    |   1451 /  24826   |        773
              DisplayNode (  DISPLAY1)    |   8557    |     61 /  16876   |        773
