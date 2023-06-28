# Auto Valet Parking Application 2 Datasheet {#group_apps_dl_demos_app_tidl_avp2_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |   2.85 
mcu2_0    |   5. 0 
mcu2_1    |   1. 0 
 c7x_1    |  48. 0 
 c7x_2    |  52. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  MSC0    |  15.44 % ( 106 MP/s )
  MSC1    |   4.90 % ( 35 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   3206 MB/s  |  48864 MB/s
WRITE BW |   2017 MB/s  |  39650 MB/s
TOTAL BW |   5223 MB/s  |  88514 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.14 %
      REMOTE_SRV   |   0. 8 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0. 0 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0. 0 %
      TIVX_V1SC1   |   3.90 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   0. 0 %
      TIVX_CAPT1   |   0. 0 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   0.72 %
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
          IPC_RX   |   0. 3 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  47.18 %
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
   DDR_LOCAL_MEM |  268435456 B |  153566208 B |  57 %
          L3_MEM |    3964928 B |     950272 B |  23 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  381877665 B |  98 %

##CPU: c7x_2

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 6 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  51.68 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16690432 B |  99 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |      16384 B | 100 %
 DDR_SCRATCH_MEM |   67108864 B |   67108864 B | 100 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  66577 |  66121 /  67067 |        270

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   15. 2


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 10       |   1071


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
               ScalerNode ( VPAC_MSC1)    |   6392    |   6322 /   6615   |       1071
            ODPreProcNode (     DSP-1)    |   4841    |   4709 /   5234   |       1071
               PCTIDLNode (  DSP_C7-1)    |  19579    |  19454 /  20518   |       1071
           PCPostProcNode (     DSP-1)    |   2651    |   2562 /   3250   |       1071
               ODTIDLNode (  DSP_C7-1)    |  11713    |  11300 /  12126   |       1071
           ODPostProcNode (     DSP-1)    |  15544    |   9249 /  19560   |       1071
       DrawDetectionsNode (     DSP-1)    |   4485    |   3498 /   6093   |       1071
       DrawDetectionsNode (     DSP-1)    |   6199    |   4770 /   7191   |       1071
               MosaicNode ( VPAC_MSC1)    |   5237    |   5091 /  28310   |       1071
              DisplayNode (  DISPLAY1)    |   8826    |     56 /  16868   |       1071
