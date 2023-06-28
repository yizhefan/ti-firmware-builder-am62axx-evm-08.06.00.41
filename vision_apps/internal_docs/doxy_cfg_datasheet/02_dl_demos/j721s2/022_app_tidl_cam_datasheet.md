# Camera based Object Classification Application Datasheet {#group_apps_dl_demos_app_tidl_cam_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   3.57 
mcu2_0    |   9. 0 
mcu2_1    |   1. 0 
 c7x_1    |   4. 0 
 c7x_2    |   5. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  10.65 % ( 65 MP/s )
  LDC     |   9.15 % ( 64 MP/s )
  MSC0    |  18.14 % ( 128 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |    979 MB/s  |   5001 MB/s
WRITE BW |    402 MB/s  |   1641 MB/s
TOTAL BW |   1381 MB/s  |   6642 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.14 %
      REMOTE_SRV   |   0. 4 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0.97 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0.57 %
      TIVX_V1SC1   |   2.18 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   2.83 %
      TIVX_CAPT1   |   0.49 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.46 %
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
      REMOTE_SRV   |   0. 7 %
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
          IPC_RX   |   0. 4 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |   3.72 %
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
   DDR_LOCAL_MEM |  268435456 B |  254606592 B |  94 %
          L3_MEM |    3964928 B |     950272 B |  23 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  385344256 B |  99 %

##CPU: c7x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 7 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   4.74 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16697344 B |  99 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32397 |  32098 /  32762 |        309

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   30.86


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 10       |    776


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
             capture_node (  CAPTURE1)    |  32402    |  32338 /  63485   |        776
                viss_node (VPAC_VISS1)    |   3946    |   3923 /   4181   |        776
                aewb_node (    MCU2-0)    |    259    |     35 /   4567   |        776
                 ldc_node ( VPAC_LDC1)    |   3006    |   2997 /   3090   |        776
              scaler_node ( VPAC_MSC1)    |   4640    |   4618 /   4800   |        776
            pre_proc_node (     DSP-1)    |   1381    |   1347 /   1460   |        776
                tidl_node (  DSP_C7-1)    |   1152    |   1135 /   1382   |        776
           post_proc_node (     DSP-1)    |     62    |     60 /     75   |        776
              mosaic_node ( VPAC_MSC1)    |   1589    |   1445 /  24735   |        776
              DisplayNode (  DISPLAY1)    |   8581    |     59 /  16847   |        776
