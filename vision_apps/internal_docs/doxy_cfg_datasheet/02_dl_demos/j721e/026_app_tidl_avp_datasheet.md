# Auto Valet Parking Application 1 Datasheet {#group_apps_dl_demos_app_tidl_avp_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   9.25 
mcu2_0    |  15. 0 
mcu2_1    |   1. 0 
 c6x_1    |  38. 0 
 c6x_2    |  80. 0 
 c7x_1    |  96. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  MSC0    |  26.56 % ( 149 MP/s )
  MSC1    |   6. 7 % ( 31 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   2838 MB/s  |  13073 MB/s
WRITE BW |    586 MB/s  |   3907 MB/s
TOTAL BW |   3424 MB/s  |  16980 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.67 %
      REMOTE_SRV   |   0.10 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0. 0 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0. 0 %
      TIVX_V1SC1   |  11.16 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   0. 0 %
      TIVX_CAPT1   |   0. 0 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   3.24 %
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
   DDR_LOCAL_MEM |   16777216 B |   16751872 B |  99 %
          L3_MEM |     262144 B |     261888 B |  99 %

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
          IPC_RX   |   0.63 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  37.22 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16752384 B |  99 %
          L2_MEM |     229376 B |          0 B |   0 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c6x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.51 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  79.41 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16540416 B |  98 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.21 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  94.82 %
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
   DDR_LOCAL_MEM |  268435456 B |  217746944 B |  81 %
          L3_MEM |    8159232 B |          0 B |   0 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  382212373 B |  99 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  16646 |  16164 /  17279 |         91

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   60. 7


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 14       |   6084


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
               ScalerNode ( VPAC_MSC1)    |   2557    |   2464 /   2947   |       6084
            PCPreProcNode (     DSP-1)    |    520    |    477 /    671   |       6084
               PCTIDLNode (  DSP_C7-1)    |   6367    |   5723 /   7236   |       6084
           PCPostProcNode (     DSP-2)    |   2422    |   2123 /   2997   |       6084
            ODPreProcNode (     DSP-1)    |    469    |    430 /    665   |       6084
               ODTIDLNode (  DSP_C7-1)    |   4431    |   4335 /   5451   |       6084
           ODPostProcNode (     DSP-2)    |   4368    |     60 /   9869   |       6084
       DrawDetectionsNode (     DSP-1)    |   2764    |    855 /   4929   |       6084
            ODPreProcNode (     DSP-1)    |    463    |    441 /    613   |       6084
               ODTIDLNode (  DSP_C7-1)    |   4887    |   4376 /   6215   |       6084
           ODPostProcNode (     DSP-2)    |   6178    |     71 /  13604   |       6084
       DrawDetectionsNode (     DSP-1)    |   2931    |    881 /   4909   |       6084
               MosaicNode ( VPAC_MSC1)    |   2529    |   2380 /  25369   |       6084
              DisplayNode (  DISPLAY1)    |  14856    |     87 /  16915   |       6084
