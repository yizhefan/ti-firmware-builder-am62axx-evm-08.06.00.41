# Multi camera application (8Ch) Datasheet {#group_apps_basic_demos_app_multi_cam_8ch_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   0.25 
mcu1_0    |   1. 0 
mcu2_0    |  33. 0 
mcu2_1    |   1. 0 
 c7x_1    |   0. 0 
 c7x_2    |   1. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  80.62 % ( 524 MP/s )
  MSC0    |  59.12 % ( 524 MP/s )
  MSC1    |  59.27 % ( 524 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1065 MB/s  |   4775 MB/s
WRITE BW |    996 MB/s  |   3815 MB/s
TOTAL BW |   2061 MB/s  |   8590 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.11 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   6. 9 %
         TIVX_NF   |   0. 0 %
       TIVX_LDC1   |   0. 0 %
       TIVX_MSC1   |   6.93 %
       TIVX_MSC2   |   0. 0 %
      TIVX_VISS1   |  16.43 %
      TIVX_CAPT1   |   3.27 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.53 %
      TIVX_DISP2   |   0. 0 %
      TIVX_CSITX   |   0. 0 %
      TIVX_CAPT3   |   0. 0 %
      TIVX_CAPT4   |   0. 0 %
      TIVX_CAPT5   |   0. 0 %
      TIVX_CAPT6   |   0. 0 %
      TIVX_CAPT7   |   0. 0 %
      TIVX_CAPT8   |   0. 0 %
     TIVX_DISP_M   |   0. 0 %
     TIVX_DISP_M   |   0. 0 %
     TIVX_DISP_M   |   0. 0 %
     TIVX_DISP_M   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
  DDR_SHARED_MEM |   16777216 B |   13485568 B |  80 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
        TIVX_SDE   |   0. 0 %
        TIVX_DOF   |   0. 0 %
      TIVX_CPU_1   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
  DDR_SHARED_MEM |   16777216 B |   16773120 B |  99 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     TIVX_CPU_PR   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
  DDR_SHARED_MEM |  268435456 B |  268435200 B |  99 %
          L3_MEM |    3964928 B |    3964928 B | 100 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |  419430400 B |  419430400 B | 100 %

##CPU: c7x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   0. 0 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
  DDR_SHARED_MEM |   16777216 B |   16776960 B |  99 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32398 |  31064 /  33856 |        309

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   30.86


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
  5       |    882


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
             capture_node (  CAPTURE1)    |  32387    |  30883 /  63776   |        882
                viss_node (VPAC_VISS1)    |  30403    |  27990 /  30620   |        882
                aewb_node (    IPU1-0)    |   2737    |    165 /   6398   |        882
              mosaic_node ( VPAC_MSC1)    |  20541    |  20070 /  49353   |        882
              DisplayNode (  DISPLAY1)    |   8291    |     62 /  16841   |        882
