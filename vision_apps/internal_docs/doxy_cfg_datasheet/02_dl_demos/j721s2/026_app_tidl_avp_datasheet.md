# Auto Valet Parking Application 1 Datasheet {#group_apps_dl_demos_app_tidl_avp_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |  11.11 
mcu2_0    |   7. 0 
mcu2_1    |   1. 0 
 c7x_1    |  50. 0 
 c7x_2    | 100. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  MSC0    |  13. 8 % ( 82 MP/s )
  MSC1    |   3.16 % ( 17 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   3103 MB/s  |  24064 MB/s
WRITE BW |   1730 MB/s  |  16746 MB/s
TOTAL BW |   4833 MB/s  |  40810 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.21 %
      REMOTE_SRV   |   0. 7 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0. 0 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0. 0 %
      TIVX_V1SC1   |   4.56 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   0. 0 %
      TIVX_CAPT1   |   0. 0 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   1.63 %
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
          IPC_RX   |   0. 0 %
      REMOTE_SRV   |   0. 5 %
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
          IPC_RX   |   0.32 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  48.66 %
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
   DDR_LOCAL_MEM |  268435456 B |  204916480 B |  76 %
          L3_MEM |    3964928 B |     950272 B |  23 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  382322965 B |  99 %

##CPU: c7x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.25 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  99.70 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16538112 B |  98 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  24379 |  14312 /  50241 |         97

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   41. 1


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 14       |   3690


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
               ScalerNode ( VPAC_MSC1)    |   2263    |   2141 /   2607   |       3690
            PCPreProcNode (     DSP-1)    |   1645    |   1519 /   2667   |       3690
               PCTIDLNode (  DSP_C7-1)    |   6721    |   6452 /   7286   |       3690
           PCPostProcNode (     DSP-1)    |   1160    |    841 /   1898   |       3690
            ODPreProcNode (     DSP-1)    |   1865    |   1623 /   3000   |       3690
               ODTIDLNode (  DSP_C7-1)    |   3845    |   3736 /   4818   |       3690
           ODPostProcNode (     DSP-1)    |   6099    |     17 /  13742   |       3690
       DrawDetectionsNode (     DSP-1)    |   3905    |    814 /   7686   |       3690
            ODPreProcNode (     DSP-1)    |   1831    |   1620 /   3032   |       3690
               ODTIDLNode (  DSP_C7-1)    |   3913    |   3729 /   4222   |       3690
           ODPostProcNode (     DSP-1)    |   8635    |     16 /  18073   |       3690
       DrawDetectionsNode (     DSP-1)    |   4335    |    821 /   7797   |       3690
               MosaicNode ( VPAC_MSC1)    |   2216    |   1948 /  24051   |       3690
              DisplayNode (  DISPLAY1)    |   8885    |     54 /  16882   |       3690
