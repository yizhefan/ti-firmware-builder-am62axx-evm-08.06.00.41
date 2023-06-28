# Camera based Object Classification Application Datasheet {#group_apps_dl_demos_app_tidl_cam_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   1. 5 
mcu2_0    |   9. 0 
mcu2_1    |   1. 0 
mcu3_0    |   1. 0 
mcu3_1    |   1. 0 
mcu4_0    |   1. 0 
mcu4_1    |   1. 0 
 c7x_1    |   5. 0 
 c7x_2    |   7. 0 
 c7x_3    |   1. 0 
 c7x_4    |   1. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  10.65 % ( 65 MP/s )
  LDC     |   9.17 % ( 64 MP/s )
  MSC0    |  18.52 % ( 128 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1059 MB/s  |  13754 MB/s
WRITE BW |    416 MB/s  |   5963 MB/s
TOTAL BW |   1475 MB/s  |  19717 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.14 %
      REMOTE_SRV   |   0. 8 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0.93 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0.59 %
      TIVX_V1SC1   |   2.83 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   2.84 %
      TIVX_CAPT1   |   0.52 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   0.63 %
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
   DDR_LOCAL_MEM |   16777216 B |   16044032 B |  95 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 6 %
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
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16776960 B |  99 %

##CPU: mcu3_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 3 %
       LOAD_TEST   |   0. 0 %
     TIVX_MCU3_0   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |    8388608 B |    8388352 B |  99 %

##CPU: mcu3_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
     TIVX_MCU3_1   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |    8388608 B |    8388352 B |  99 %

##CPU: mcu4_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 6 %
       LOAD_TEST   |   0. 0 %
     TIVX_MCU4_0   |   0. 0 %
       TIVX_V2NF   |   0. 0 %
      TIVXV2LDC1   |   0. 0 %
      TIVXV2MSC1   |   0. 0 %
      TIVXV2MSC2   |   0. 0 %
     TIVXV2VISS1   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |    8388608 B |    8384512 B |  99 %

##CPU: mcu4_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
     TIVX_MCU4_1   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |    8388608 B |    8388352 B |  99 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 5 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |   4.51 %
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
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   67108864 B |   61611264 B |  91 %
          L3_MEM |    3145728 B |     131072 B |   4 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |   67108864 B |   66584320 B |  99 %
 DDR_NON_CACHE_M |   67108864 B |   58778496 B |  87 %
 DDR_SCRATCH_NON |   67108864 B |   67102592 B |  99 %

##CPU: c7x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.11 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C72_P1   |   6.57 %
     TIVX_C72_P2   |   0. 0 %
     TIVX_C72_P3   |   0. 0 %
     TIVX_C72_P4   |   0. 0 %
     TIVX_C72_P5   |   0. 0 %
     TIVX_C72_P6   |   0. 0 %
     TIVX_C72_P7   |   0. 0 %
     TIVX_C72_P8   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   67108864 B |   67028992 B |  99 %
          L3_MEM |    3145728 B |    3145728 B | 100 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %
 DDR_NON_CACHE_M |   67108864 B |   67108864 B | 100 %
 DDR_SCRATCH_NON |   67108864 B |   67108864 B | 100 %

##CPU: c7x_3

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C73_P1   |   0. 0 %
     TIVX_C73_P2   |   0. 0 %
     TIVX_C73_P3   |   0. 0 %
     TIVX_C73_P4   |   0. 0 %
     TIVX_C73_P5   |   0. 0 %
     TIVX_C73_P6   |   0. 0 %
     TIVX_C73_P7   |   0. 0 %
     TIVX_C73_P8   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   67108864 B |   67104256 B |  99 %
          L3_MEM |    3145728 B |    3145728 B | 100 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %
 DDR_NON_CACHE_M |   67108864 B |   67108864 B | 100 %
 DDR_SCRATCH_NON |   67108864 B |   67108864 B | 100 %

##CPU: c7x_4

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C74_P1   |   0. 0 %
     TIVX_C74_P2   |   0. 0 %
     TIVX_C74_P3   |   0. 0 %
     TIVX_C74_P4   |   0. 0 %
     TIVX_C74_P5   |   0. 0 %
     TIVX_C74_P6   |   0. 0 %
     TIVX_C74_P7   |   0. 0 %
     TIVX_C74_P8   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   67108864 B |   67104256 B |  99 %
          L3_MEM |    3145728 B |    3145728 B | 100 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %
 DDR_NON_CACHE_M |   67108864 B |   67108864 B | 100 %
 DDR_SCRATCH_NON |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32398 |  31965 /  32863 |        371

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   30.86


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 10       |    831


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
             capture_node (  CAPTURE1)    |  32400    |  32332 /  63519   |        831
                viss_node (VPAC_VISS1)    |   3962    |   3931 /   4229   |        831
                aewb_node (    MCU2-0)    |    251    |     37 /   4422   |        831
                 ldc_node ( VPAC_LDC1)    |   3009    |   2998 /   3126   |        831
              scaler_node ( VPAC_MSC1)    |   4654    |   4624 /   4908   |        831
            pre_proc_node (  DSP_C7-2)    |   1947    |   1935 /   2046   |        831
                tidl_node (  DSP_C7-1)    |   1397    |   1379 /   1612   |        831
           post_proc_node (  DSP_C7-2)    |     64    |     62 /     79   |        831
              mosaic_node ( VPAC_MSC1)    |   1786    |   1595 /  27010   |        831
              DisplayNode (  DISPLAY1)    |   8519    |     66 /  16850   |        831
