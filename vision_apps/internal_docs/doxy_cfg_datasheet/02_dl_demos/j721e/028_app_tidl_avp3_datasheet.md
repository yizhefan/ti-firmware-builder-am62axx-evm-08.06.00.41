# Auto Valet Parking Application 3 Datasheet {#group_apps_dl_demos_app_tidl_avp3_datasheet}

# Summary of CPU load

CPU      | TOTAL LOAD
----------|--------------
mpu1_0    |  11.42 
mcu2_0    |   8. 0 
mcu2_1    |   4. 0 
 c6x_1    |  46. 0 
 c6x_2    |  29. 0 
 c7x_1    |  47. 0 

# HWA performance statistics

HWA      | LOAD
----------|--------------
  MSC0    |  17.21 % ( 86 MP/s )
  MSC1    |   4.14 % ( 24 MP/s )
  DOF     |   8.94 % ( 9 MP/s )

# DDR performance statistics

DDR BW   | AVG          | PEAK
----------|--------------|-------
READ BW |   1727 MB/s  |  14138 MB/s
WRITE BW |    474 MB/s  |   3370 MB/s
TOTAL BW |   2201 MB/s  |  17508 MB/s

# Detailed CPU performance/memory statistics


##CPU: mcu2_0

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.12 %
      REMOTE_SRV   |   0. 9 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_0   |   0. 0 %
       TIVX_V1NF   |   0. 0 %
     TIVX_V1LDC1   |   0. 0 %
      TIVX_V1SC1   |   7.41 %
     TIVX_V1MSC2   |   0. 0 %
      TIVXVVISS1   |   0. 0 %
      TIVX_CAPT1   |   0. 0 %
      TIVX_CAPT2   |   0. 0 %
      TIVX_DISP1   |   0.58 %
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
          IPC_RX   |   0. 4 %
      REMOTE_SRV   |   0.11 %
       LOAD_TEST   |   0. 0 %
      TIVX_CPU_1   |   0. 0 %
        TIVX_SDE   |   0. 0 %
        TIVX_DOF   |   3.19 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   15887104 B |  94 %
          L3_MEM |     262144 B |     262144 B | 100 %

##CPU: c6x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0.13 %
      REMOTE_SRV   |   0. 1 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  45.60 %
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
          IPC_RX   |   0. 4 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
        TIVX_CPU   |  28.91 %
     IPC_TEST_RX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %
     IPC_TEST_TX   |   0. 0 %

###CPU Heap Table

HEAP   | Size  | Free | Unused
--------|-------|------|---------
   DDR_LOCAL_MEM |   16777216 B |   16692736 B |  99 %
          L2_MEM |     229376 B |     229376 B | 100 %
 DDR_SCRATCH_MEM |   50331648 B |   50331648 B | 100 %

##CPU: c7x_1

###Task Table

TASK          | TASK LOAD
--------------|-------
          IPC_RX   |   0. 2 %
      REMOTE_SRV   |   0. 0 %
       LOAD_TEST   |   0. 0 %
     TIVX_C71_P1   |  46. 9 %
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
   DDR_LOCAL_MEM |  268435456 B |  181043968 B |  67 %
          L3_MEM |    8159232 B |          0 B |   0 %
          L2_MEM |     458752 B |     458752 B | 100 %
          L1_MEM |      16384 B |          0 B |   0 %
 DDR_SCRATCH_MEM |  385875968 B |  385344256 B |  99 %

# Performance point statistics


##TOTAL Performance

PERF      | avg (usecs)  | min/max (usecs)  | number of executions
----------|----------|----------|----------
           TOTAL |  97970 |   7988 / 111144 |        326

##TOTAL FPS

PERF      | Frames per sec (FPS)
----------|----------
           TOTAL |   10.20


# GRAPH: Detailed Statistics


##Node Execution Table

Total Nodes      | Total executions
----------|--------------
 14       |    724


##Per Node Breakdown

NODE      | avg (usecs)      | min/max (usecs)      | Total Executions
----------|------------------|----------------------|------------
               ScalerNode ( VPAC_MSC1)    |   7351    |   7303 /   7662   |        724
              PreProcNode (     DSP-1)    |   1440    |   1434 /   1518   |        724
               ODTIDLNode (  DSP_C7-1)    |  12715    |  11991 /  13814   |        724
           ODPostProcNode (     DSP-2)    |   8558    |   1836 /  20203   |        724
       DrawDetectionsNode (     DSP-1)    |   3624    |   3258 /   4166   |        724
       DrawDetectionsNode (     DSP-1)    |   4632    |   3023 /   5401   |        724
          GaussianPyramid ( VPAC_MSC1)    |   4844    |   4595 /   5785   |        724
         DenseOpticalFlow ( DMPAC_DOF)    |   8940    |   8747 /   9780   |        724
             DofVisualize (     DSP-1)    |  26534    |  25285 /  27356   |        724
           DofPreProcNode (     DSP-1)    |  12464    |  12370 /  14032   |        724
               PCTIDLNode (  DSP_C7-1)    |  32222    |  31994 /  33150   |        724
           PCPostProcNode (     DSP-2)    |  18974    |  18814 /  21060   |        724
               MosaicNode ( VPAC_MSC1)    |   6555    |   6344 /  29358   |        724
              DisplayNode (  DISPLAY1)    |   8797    |     93 /  16922   |        724
