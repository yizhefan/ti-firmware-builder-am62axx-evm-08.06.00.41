# Single Camera Application Datasheet {#group_apps_basic_demos_app_single_cam_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   0.50 
mcu2_0    |  24. 0 
mcu2_1    |   1. 0 
 c6x_1    |   1. 0 
 c6x_2    |   1. 0 
 c7x_1    |   1. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  22.28 % ( 127 MP/s )
  LDC     |  20. 9 % ( 124 MP/s )
  MSC0    |  30.91 % ( 186 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1126 MB/s  |   2979 MB/s
WRITE BW |    838 MB/s  |   1966 MB/s
TOTAL BW |   1964 MB/s  |   4945 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.23 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   8.67 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   1.73 %
      TIVX_V1SC1   |   2.13 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   6. 2 %
      TIVX_CAPT1   |   0. 0 %
      TIVX_CAPT2   |   1.73 %
      TIVX_DISP1   |   3.30 %
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
   DDR_LOCAL_MEM |   16777216 B |   16262400 B |  96 %
          L3_MEM |     262144 B |     245248 B |  93 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 1 %
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
           TOTAL |  16648 |  16549 /  16776 |        601

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   60. 6


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
  6       |   1581


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
                  node_96 (  CAPTURE2)    |  11866    |    677 /  28983   |       1581
          VISS_Processing (VPAC_VISS1)    |   4430    |   4377 /   4909   |       1581
               2A_AlgNode (    MCU2-0)    |   1383    |   1246 /   3881   |       1581
                 node_112 ( VPAC_LDC1)    |   3396    |   3372 /   3835   |       1581
                 node_114 ( VPAC_MSC1)    |   5171    |   5124 /   6090   |       1581
                 node_116 (  DISPLAY1)    |  16573    |     80 /  25300   |       1581
