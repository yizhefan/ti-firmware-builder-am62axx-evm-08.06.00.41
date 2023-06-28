# Auto Valet Parking Application 3 Datasheet {#group_apps_dl_demos_app_tidl_avp4_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |  36.36 
mcu2_0    |  33. 0 
mcu2_1    |   8. 0 
 c6x_1    |   7. 0 
 c6x_2    |  45. 0 
 c7x_1    |  81. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  VISS    |  45.81 % ( 240 MP/s )
  LDC     |  39.71 % ( 235 MP/s )
  MSC0    |  60. 7 % ( 352 MP/s )
  MSC1    |  14.22 % ( 67 MP/s )
  DOF     |  27.22 % ( 33 MP/s )
  GPU     |  42.28 % ( 59 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   4279 MB/s  |   8735 MB/s
WRITE BW |   2380 MB/s  |   6133 MB/s
TOTAL BW |   6659 MB/s  |  14868 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.77 %
      REMOTE_SRV   |   0.11 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   2.56 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   4.23 %
      TIVX_V1SC1   |  10.21 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |  12.34 %
      TIVX_CAPT1   |   2.45 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.90 %
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
   DDR_LOCAL_MEM |   16777216 B |   14983936 B |  89 %
          L3_MEM |     262144 B |     195328 B |  74 %

##CPU: mcu2_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.20 %
      REMOTE_SRV   |   0.12 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_1   |   0. 0 %
        TIVX_SDE   |   0. 0 %
        TIVX_DOF   |   7.29 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   15591680 B |  92 %
          L3_MEM |     262144 B |     262144 B | 100 %

##CPU: c6x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 9 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |   6.61 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16748288 B |  99 %
          L2_MEM |     229376 B |          0 B |   0 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c6x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.10 %
      REMOTE_SRV   |   0. 3 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  44.50 %
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
          IPC_RX   |   0. 6 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  80. 4 %
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
   DDR_LOCAL_MEM |  268435456 B |  222420992 B |  82 %
          L3_MEM |    8159232 B |          0 B |   0 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  385344256 B |  99 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  35311 |  31200 /  40057 |        283

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   28.31


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 13       |    743


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
         DenseOpticalFlow ( DMPAC_DOF)    |   9885    |   9469 /  10785   |        743
             capture_node (  CAPTURE1)    |  22458    |  14251 /  63329   |        743
                viss_node (VPAC_VISS1)    |  19481    |  18314 /  20211   |        743
                aewb_node (    MCU2-0)    |   1109    |    143 /  32854   |        743
                 ldc_node ( VPAC_LDC1)    |  14509    |  13895 /  29455   |        743
              ImgHistNode (     A72-1)    |  21557    |  21183 /  24320   |        743
              scaler_node ( VPAC_MSC1)    |  21522    |  20757 /  22408   |        743
              PreProcNode (     DSP-1)    |   2089    |   1925 /   2443   |        743
                tidl_node (  DSP_C7-1)    |  28177    |  25687 /  30012   |        743
             PostProcNode (     DSP-2)    |  15697    |   9408 /  17409   |        743
              mosaic_node ( VPAC_MSC1)    |   6258    |   5060 /  33915   |        743
              DisplayNode (  DISPLAY1)    |   9829    |    119 /  29705   |        743
          OpenGL_SRV_Node (     A72-0)    |  15414    |  12135 /  43141   |        743
