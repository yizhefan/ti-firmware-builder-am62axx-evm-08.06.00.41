# Multi camera application (4Ch) Datasheet {#group_apps_basic_demos_app_multi_cam_4ch_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   0.50 
mcu2_0    |  25. 0 
mcu2_1    |   1. 0 
 c6x_1    |   0. 0 
 c6x_2    |   1. 0 
 c7x_1    |   1. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  48.61 % ( 259 MP/s )
  LDC     |  42.13 % ( 252 MP/s )
  MSC0    |  31.87 % ( 253 MP/s )
  MSC1    |  31.93 % ( 253 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1863 MB/s  |   3607 MB/s
WRITE BW |   1417 MB/s  |   1927 MB/s
TOTAL BW |   3280 MB/s  |   5534 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 7 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   2.31 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   3.14 %
      TIVX_V1SC1   |   5. 6 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |  11.90 %
      TIVX_CAPT1   |   2.20 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.78 %
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
   DDR_LOCAL_MEM |   16777216 B |   14992128 B |  89 %
          L3_MEM |     262144 B |     195328 B |  74 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 0 %
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
   DDR_LOCAL_MEM |   16777216 B |   16773376 B |  99 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c6x_2

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
   DDR_LOCAL_MEM |   16777216 B |   16773376 B |  99 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |   0. 0 %
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
   DDR_LOCAL_MEM |  268435456 B |  268435200 B |  99 %
          L3_MEM |    8159232 B |    8159232 B | 100 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |  385875968 B |  385875968 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32823 |  31472 / 113788 |        305

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   30.46


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
  6       |    819


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
             capture_node (  CAPTURE1)    |  32517    |  31334 / 113687   |        819
                viss_node (VPAC_VISS1)    |  18907    |  17966 /  19152   |        819
                aewb_node (    MCU2-0)    |    671    |    132 /   5137   |        819
                 ldc_node ( VPAC_LDC1)    |  14049    |  13657 /  23024   |        819
              mosaic_node ( VPAC_MSC1)    |  11425    |  11013 /  39133   |        819
              DisplayNode (  DISPLAY1)    |   8525    |     80 /  16925   |        819
