# Camera based Object Classification Application Datasheet {#group_apps_dl_demos_app_tidl_cam_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   0. 0 
mcu2_0    |  11. 0 
mcu2_1    |   1. 0 
 c6x_1    |   5. 0 
 c6x_2    |   1. 0 
 c7x_1    |   5. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  11.44 % ( 65 MP/s )
  LDC     |  10.24 % ( 64 MP/s )
  MSC0    |  20.48 % ( 128 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1115 MB/s  |   5000 MB/s
WRITE BW |    404 MB/s  |   1408 MB/s
TOTAL BW |   1519 MB/s  |   6408 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.21 %
      REMOTE_SRV   |   0. 9 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0.97 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0.92 %
      TIVX_V1SC1   |   2.99 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   3. 8 %
      TIVX_CAPT1   |   0.75 %
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
          IPC_RX   |   0. 6 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   4.58 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16735744 B |  99 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c6x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 3 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   0.53 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16735744 B |  99 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 3 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |   4.31 %
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
   DDR_LOCAL_MEM |  268435456 B |  249448704 B |  92 %
          L3_MEM |    8159232 B |          0 B |   0 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  385344256 B |  99 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  32396 |  31827 /  32821 |        308

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
             capture_node (  CAPTURE1)    |  32378    |  32268 /  63455   |        773
                viss_node (VPAC_VISS1)    |   4406    |   4374 /   4584   |        773
                aewb_node (    MCU2-0)    |    206    |     48 /   3862   |        773
                 ldc_node ( VPAC_LDC1)    |   3373    |   3359 /   3471   |        773
              scaler_node ( VPAC_MSC1)    |   5202    |   5182 /   5445   |        773
            pre_proc_node (     DSP-1)    |   1414    |   1396 /   1505   |        773
                tidl_node (  DSP_C7-1)    |   1350    |   1339 /   2776   |        773
           post_proc_node (     DSP-2)    |    105    |    103 /    147   |        773
              mosaic_node ( VPAC_MSC1)    |   1854    |   1705 /  25829   |        773
              DisplayNode (  DISPLAY1)    |   8550    |     88 /  16896   |        773
