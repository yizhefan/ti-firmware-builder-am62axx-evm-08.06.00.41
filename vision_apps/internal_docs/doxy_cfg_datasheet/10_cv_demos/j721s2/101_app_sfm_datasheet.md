# Structure From Motion Application Datasheet {#group_apps_cv_demos_app_sfm_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   2.81 
mcu2_0    |   6. 0 
mcu2_1    |   4. 0 
 c7x_1    |  56. 0 
 c7x_2    |   1. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  MSC0    |  11.12 % ( 62 MP/s )
  MSC1    |   3.96 % ( 31 MP/s )
  DOF     |  13.90 % ( 15 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |    821 MB/s  |   4083 MB/s
WRITE BW |    295 MB/s  |   2966 MB/s
TOTAL BW |   1116 MB/s  |   7049 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.27 %
      REMOTE_SRV   |   0. 7 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0. 0 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0. 0 %
      TIVX_V1SC1   |   4.40 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   0. 0 %
      TIVX_CAPT1   |   0. 0 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.39 %
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
   DDR_LOCAL_MEM |   16777216 B |   16751616 B |  99 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 9 %
      REMOTE_SRV   |   0. 8 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_1   |   0. 0 %
        TIVX_SDE   |   0. 0 %
        TIVX_DOF   |   3. 4 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16248320 B |  96 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 4 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  55.49 %
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
   DDR_LOCAL_MEM |  268435456 B |  266863104 B |  99 %
          L3_MEM |    3964928 B |    2712624 B |  68 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |       7776 B |  47 %
 DDR_SCRATCH_MEM |  385875968 B |  385875968 B | 100 %

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

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16772608 B |  99 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  33551 |  33066 /  34067 |        103

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   29.80


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
  6       |    944


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
              scaler_node ( VPAC_MSC1)    |    913    |    887 /   1071   |        944
          GaussianPyramid ( VPAC_MSC1)    |   1570    |   1520 /   1945   |        944
         DenseOpticalFlow ( DMPAC_DOF)    |   4704    |   4685 /   5012   |        944
                 SFM_Node (  DSP_C7-1)    |  18614    |   2159 /  20036   |        944
              mosaic_node ( VPAC_MSC1)    |   1722    |   1607 /  23710   |        944
              DisplayNode (  DISPLAY1)    |   7745    |     57 /  16862   |        944
