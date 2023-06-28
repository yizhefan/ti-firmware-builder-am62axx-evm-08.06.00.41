# Remote Log Enable {#group_apps_utilities_app_remote_log}

[TOC]

# Introduction

This application enables printing of the logs from the remote cores to
the terminal.  Each log is prefixed with the name of the remote core so it
is clear which core the log is coming from.  This is useful to be able
to see the errors and logs printed by the software running on the remote cores.

Please note that since this application never exits, it should either be
terminated with Cntrl^C, or run with a '&' after so that it can run in
the background and other processes can be initated from the terminal in
parallel.

In fact, this application with the '&' is invoked from the vision_apps_init.sh
script that is recommended to be run before most other apps.

# Description

This utility does the following:

  -# Enables the remote log reader for all enabled remote logs
  -# Starts a task which runs every 10ms to check the shared remote core log buffer and prints it to the terminal
  -# Runs forever in a while loop (never exits until Cntrl^C signal)

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2 / J784S4

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# Run the app as shown below
   \code
   cd /opt/vision_apps
   ./vx_app_arm_remote_log.out &
   \endcode
-# Output will be sent to the terminal standard output.

# Steps to run the application on J7 EVM (QNX + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# Run the app as shown below
   \code
   cd /ti_fs/vision_apps
   ./vx_app_arm_remote_log.out &
   \endcode
-# Output will be sent to the terminal standard output.

# Sample Output

Shown below is a example output from running this utility:

\if DOCS_J721E

```
root@j7-evm:/opt/vision_apps# ./vx_app_arm_remote_log.out &
[MCU2_0]      4.421763 s: CIO: Init ... Done !!!
[MCU2_0]      4.421834 s: ### CPU Frequency <ORG = 1000000000 Hz>, <NEW = 1000000000 Hz>
[MCU2_0]      4.421876 s: APP: Init ... !!!
[MCU2_0]      4.421896 s: SCICLIENT: Init ... !!!
[MCU2_0]      4.422796 s: SCICLIENT: DMSC FW version [20.8.5--v2020.08b (Terrific Lla]
[MCU2_0]      4.422872 s: SCICLIENT: DMSC FW revision 0x14
[MCU2_0]      4.422902 s: SCICLIENT: DMSC FW ABI revision 3.1
[MCU2_0]      4.422928 s: SCICLIENT: Init ... Done !!!
[MCU2_0]      4.422948 s: UDMA: Init ... !!!
[MCU2_0]      4.431587 s: UDMA: Init ... Done !!!
[MCU2_0]      4.431643 s: MEM: Init ... !!!
[MCU2_0]      4.431679 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ d2400000 of size 8388608 bytes !!!
[MCU2_0]      4.431735 s: MEM: Created heap (L3_MEM, id=1, flags=0x00000000) @ 3600000 of size 131072 bytes !!!
[MCU2_0]      4.431778 s: MEM: Created heap (DDR_NON_CACHE_ME, id=5, flags=0x00000000) @ ce000000 of size 65536 bytes !!!
[MCU2_0]      4.431819 s: MEM: Init ... Done !!!
[MCU2_0]      4.431862 s: IPC: Init ... !!!
[MCU2_0]      4.431898 s: IPC: 6 CPUs participating in IPC !!!
[MCU2_0]      4.431937 s: IPC: Waiting for HLOS to be ready ... !!!
[MCU2_0]     19.499101 s: IPC: HLOS is ready !!!
[MCU2_0]     19.510435 s: IPC: Init ... Done !!!
[MCU2_0]     19.510500 s: APP: Syncing with 5 CPUs ... !!!
[MCU2_0]     19.996443 s: APP: Syncing with 5 CPUs ... Done !!!
[MCU2_0]     19.996690 s: REMOTE_SERVICE: Init ... !!!
[MCU2_0]     19.998497 s: REMOTE_SERVICE: Init ... Done !!!
[MCU2_0]     19.998577 s: ETHFW: Init ... !!!
[MCU2_0]     20.035247 s: CPSW_9G Test on MAIN NAVSS
[MCU2_0]     20.056187 s: PHY 16 is alive
[MCU2_0]     20.056248 s: PHY 17 is alive
[MCU2_0]     20.056275 s: PHY 18 is alive
[MCU2_0]     20.056296 s: PHY 19 is alive
[MCU2_0]     20.058018 s: ETHFW: Version   : 0.01.01
[MCU2_0]     20.058073 s: ETHFW: Build Date: Feb 15, 2021
[MCU2_0]     20.058101 s: ETHFW: Build Time: 21:43:43
[MCU2_0]     20.058123 s: ETHFW: Commit SHA: 1bec8c3b
[MCU2_0]     20.058148 s: ETHFW: Init ... DONE !!!
[MCU2_0]     20.058173 s: ETHFW: Remove server Init ... !!!
[MCU2_0]     20.059414 s: Remote demo device (core : mcu2_0) .....
[MCU2_0]     20.059479 s: ETHFW: Remove server Init ... DONE !!!
[MCU2_0]     20.080862 s: Host MAC address: 70:ff:76:1d:92:c2
[MCU2_0]     20.111982 s: FVID2: Init ... !!!
[MCU2_0]     20.112095 s: FVID2: Init ... Done !!!
[MCU2_0]     20.112144 s: DSS: Init ... !!!
[MCU2_0]     20.112166 s: DSS: Display type is eDP !!!
[MCU2_0]     20.112188 s: DSS: SoC init ... !!!
[MCU2_0]     20.112205 s: SCICLIENT: Sciclient_pmSetModuleState module=152 state=2
[MCU2_0]     20.113446 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.113484 s: SCICLIENT: Sciclient_pmSetModuleState module=297 state=2
[MCU2_0]     20.115199 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.115230 s: SCICLIENT: Sciclient_pmSetModuleState module=151 state=2
[MCU2_0]     20.116475 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.116517 s: SCICLIENT: Sciclient_pmSetModuleClkParent module=152 clk=9 parent=11
[MCU2_0]     20.117042 s: SCICLIENT: Sciclient_pmSetModuleClkParent success
[MCU2_0]     20.117080 s: SCICLIENT: Sciclient_pmSetModuleClkParent module=152 clk=13 parent=18
[MCU2_0]     20.117523 s: SCICLIENT: Sciclient_pmSetModuleClkParent success
[MCU2_0]     20.117551 s: SCICLIENT: Sciclient_pmSetModuleClkParent module=152 clk=1 parent=2
[MCU2_0]     20.117997 s: SCICLIENT: Sciclient_pmSetModuleClkParent success
[MCU2_0]     20.118028 s: SCICLIENT: Sciclient_pmSetModuleClkFreq module=152 clk=1 freq=148500000
[MCU2_0]     20.135291 s: SCICLIENT: Sciclient_pmSetModuleClkFreq success
[MCU2_0]     20.135323 s: SCICLIENT: Sciclient_pmModuleClkRequest module=152 clk=1 state=2 flag=0
[MCU2_0]     20.136349 s: SCICLIENT: Sciclient_pmModuleClkRequest success
[MCU2_0]     20.136380 s: DSS: SoC init ... Done !!!
[MCU2_0]     20.136400 s: DSS: Board init ... !!!
[MCU2_0]     20.136420 s: DSS: Turning on DP_PWR pin for eDP adapters ... !!!
[MCU2_0]     20.181992 s: DSS: Turning on DP_PWR pin for eDP adapters ... Done!!!
[MCU2_0]     20.182062 s: DSS: Board init ... Done !!!
[MCU2_0]     20.184652 s: Function:CpswProxyServer_attachExtHandlerCb,HostId:0,CpswType:6
[MCU2_0]     20.198924 s: DSS: Init ... Done !!!
[MCU2_0]     20.199067 s: VHWA: VPAC Init ... !!!
[MCU2_0]     20.199104 s: SCICLIENT: Sciclient_pmSetModuleState module=290 state=2
[MCU2_0]     20.199844 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.199874 s: VHWA: LDC Init ... !!!
[MCU2_0]     20.212762 s: VHWA: LDC Init ... Done !!!
[MCU2_0]     20.212817 s: VHWA: MSC Init ... !!!
[MCU2_0]     20.251256 s: VHWA: MSC Init ... Done !!!
[MCU2_0]     20.251309 s: VHWA: NF Init ... !!!
[MCU2_0]     20.257787 s: VHWA: NF Init ... Done !!!
[MCU2_0]     20.257840 s: VHWA: VISS Init ... !!!
[MCU2_0]     20.285926 s: VHWA: VISS Init ... Done !!!
[MCU2_0]     20.286009 s: VHWA: VPAC Init ... Done !!!
[MCU2_0]     20.286064 s:  VX_ZONE_INIT:Enabled
[MCU2_0]     20.286092 s:  VX_ZONE_ERROR:Enabled
[MCU2_0]     20.286115 s:  VX_ZONE_WARNING:Enabled
[MCU2_0]     20.287333 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target MCU2-0
[MCU2_0]     20.287668 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VPAC_NF
[MCU2_0]     20.288036 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VPAC_LDC1
[MCU2_0]     20.288354 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VPAC_MSC1
[MCU2_0]     20.288655 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VPAC_MSC2
[MCU2_0]     20.289004 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VPAC_VISS1
[MCU2_0]     20.289337 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE1
[MCU2_0]     20.289707 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE2
[MCU2_0]     20.290058 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target DISPLAY1
[MCU2_0]     20.290446 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target DISPLAY2
[MCU2_0]     20.290777 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CSITX
[MCU2_0]     20.291189 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE3
[MCU2_0]     20.291533 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE4
[MCU2_0]     20.291837 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE5
[MCU2_0]     20.292179 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE6
[MCU2_0]     20.292493 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE7
[MCU2_0]     20.292791 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target CAPTURE8
[MCU2_0]     20.292841 s:  VX_ZONE_INIT:[tivxInit:71] Initialization Done !!!
[MCU2_0]     20.292869 s: APP: OpenVX Target kernel init ... !!!
[MCU2_0]     20.306152 s: APP: OpenVX Target kernel init ... Done !!!
[MCU2_0]     20.306214 s: CSI2RX: Init ... !!!
[MCU2_0]     20.306241 s: SCICLIENT: Sciclient_pmSetModuleState module=25 state=2
[MCU2_0]     20.307015 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.307051 s: SCICLIENT: Sciclient_pmSetModuleState module=26 state=2
[MCU2_0]     20.308024 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.308054 s: SCICLIENT: Sciclient_pmSetModuleState module=27 state=2
[MCU2_0]     20.309492 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.309522 s: SCICLIENT: Sciclient_pmSetModuleState module=147 state=2
[MCU2_0]     20.310012 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.310042 s: SCICLIENT: Sciclient_pmSetModuleState module=148 state=2
[MCU2_0]     20.310450 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.313467 s: CSI2RX: Init ... Done !!!
[MCU2_0]     20.313517 s: CSI2TX: Init ... !!!
[MCU2_0]     20.313541 s: SCICLIENT: Sciclient_pmSetModuleState module=25 state=2
[MCU2_0]     20.314175 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.314208 s: SCICLIENT: Sciclient_pmSetModuleState module=28 state=2
[MCU2_0]     20.315635 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.315670 s: SCICLIENT: Sciclient_pmSetModuleState module=296 state=2
[MCU2_0]     20.316580 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     20.320253 s: CSI2TX: Init ... Done !!!
[MCU2_0]     20.320302 s: ISS: Init ... !!!
[MCU2_0]     20.320364 s: Found sensor IMX390-UB953_D3 at location 0
[MCU2_0]     20.320420 s: Found sensor AR0233-UB953_MARS at location 1
[MCU2_0]     20.320463 s: Found sensor AR0820-UB953_LI at location 2
[MCU2_0]     20.320505 s: Found sensor UB9xxx_RAW12_TESTPATTERN at location 3
[MCU2_0]     20.320549 s: Found sensor UB96x_UYVY_TESTPATTERN at location 4
[MCU2_0]     20.320591 s: Found sensor GW_AR0233_UYVY at location 5
[MCU2_0]     20.320615 s: IssSensor_Init ... Done !!!
[MCU2_0]     20.320685 s: vissRemoteServer_Init ... Done !!!
[MCU2_0]     20.320741 s: IttRemoteServer_Init ... Done !!!
[MCU2_0]     20.320769 s: UDMA Copy: Init ... !!!
[MCU2_0]     20.326980 s: UDMA Copy: Init ... Done !!!
[MCU2_0]     20.327034 s: APP: Init ... Done !!!
[MCU2_0]     20.327058 s: APP: Run ... !!!
[MCU2_0]     20.327077 s: IPC: Starting echo test ...
[MCU2_0]     20.329560 s: APP: Run ... Done !!!
[MCU2_0]     20.331202 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[.] C66X_1[P] C66X_2[.] C7X_1[.]
[MCU2_0]     20.331365 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[.] C66X_1[P] C66X_2[P] C7X_1[.]
[MCU2_0]     20.331490 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[P] C66X_1[P] C66X_2[P] C7X_1[.]
[MCU2_0]     20.331605 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[P] C66X_1[P] C66X_2[P] C7X_1[P]
[MCU2_0]     22.664088 s: Function:CpswProxyServer_registerMacHandlerCb,HostId:0,Handle:a21da9fc,CoreKey:38acb7e6, MacAddress:70:ff:76:1d:92:c1, FlowI0
[MCU2_0]     22.667186 s: Cpsw_ioctlInternal: CPSW: Registered MAC address.ALE entry:11, Policer Entry:0
[MCU2_1]      4.488894 s: CIO: Init ... Done !!!
[MCU2_1]      4.488968 s: ### CPU Frequency <ORG = 1000000000 Hz>, <NEW = 1000000000 Hz>
[MCU2_1]      4.489012 s: APP: Init ... !!!
[MCU2_1]      4.489034 s: SCICLIENT: Init ... !!!
[MCU2_1]      4.490050 s: SCICLIENT: DMSC FW version [20.8.5--v2020.08b (Terrific Lla]
[MCU2_1]      4.490100 s: SCICLIENT: DMSC FW revision 0x14
[MCU2_1]      4.490130 s: SCICLIENT: DMSC FW ABI revision 3.1
[MCU2_1]      4.490158 s: SCICLIENT: Init ... Done !!!
[MCU2_1]      4.490182 s: UDMA: Init ... !!!
[MCU2_1]      4.500424 s: UDMA: Init ... Done !!!
[MCU2_1]      4.500481 s: MEM: Init ... !!!
[MCU2_1]      4.500519 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ d2c00000 of size 16777216 bytes !!!
[MCU2_1]      4.500574 s: MEM: Created heap (L3_MEM, id=1, flags=0x00000001) @ 3620000 of size 131072 bytes !!!
[MCU2_1]      4.500623 s: MEM: Created heap (DDR_NON_CACHE_ME, id=5, flags=0x00000000) @ ce010000 of size 67043328 bytes !!!
[MCU2_1]      4.500668 s: MEM: Init ... Done !!!
[MCU2_1]      4.500690 s: IPC: Init ... !!!
[MCU2_1]      4.500721 s: IPC: 6 CPUs participating in IPC !!!
[MCU2_1]      4.500757 s: IPC: Waiting for HLOS to be ready ... !!!
[MCU2_1]     19.984322 s: IPC: HLOS is ready !!!
[MCU2_1]     19.996325 s: IPC: Init ... Done !!!
[MCU2_1]     19.996394 s: APP: Syncing with 5 CPUs ... !!!
[MCU2_1]     19.996440 s: APP: Syncing with 5 CPUs ... Done !!!
[MCU2_1]     19.996477 s: REMOTE_SERVICE: Init ... !!!
[MCU2_1]     19.998391 s: REMOTE_SERVICE: Init ... Done !!!
[MCU2_1]     19.998459 s: FVID2: Init ... !!!
[MCU2_1]     19.998549 s: FVID2: Init ... Done !!!
[MCU2_1]     19.998580 s: VHWA: DMPAC: Init ... !!!
[MCU2_1]     19.998604 s: SCICLIENT: Sciclient_pmSetModuleState module=48 state=2
[MCU2_1]     19.999876 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_1]     19.999924 s: SCICLIENT: Sciclient_pmSetModuleState module=305 state=2
[MCU2_1]     20.002532 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_1]     20.002569 s: VHWA: DOF Init ... !!!
[MCU2_1]     20.046066 s: VHWA: DOF Init ... Done !!!
[MCU2_1]     20.046126 s: VHWA: SDE Init ... !!!
[MCU2_1]     20.060159 s: VHWA: SDE Init ... Done !!!
[MCU2_1]     20.060224 s: VHWA: DMPAC: Init ... Done !!!
[MCU2_1]     20.060254 s: VHWA: Codec: Init ... !!!
[MCU2_1]     20.060275 s: VHWA: VDEC Init ... !!!
[MCU2_1]     20.076069 s: VHWA: VDEC Init ... Done !!!
[MCU2_1]     20.076131 s: VHWA: VENC Init ... !!!
[MCU2_1]     20.077360 s: MM_ENC_Init: No OCM RAM pool available, fallback to DDR mode for above mp params
[MCU2_1]     20.119919 s: VHWA: VENC Init ... Done !!!
[MCU2_1]     20.120006 s: VHWA: Init ... Done !!!
[MCU2_1]     20.120055 s:  VX_ZONE_INIT:Enabled
[MCU2_1]     20.120084 s:  VX_ZONE_ERROR:Enabled
[MCU2_1]     20.120106 s:  VX_ZONE_WARNING:Enabled
[MCU2_1]     20.121122 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target DMPAC_SDE
[MCU2_1]     20.121378 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target DMPAC_DOF
[MCU2_1]     20.121604 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VDEC1
[MCU2_1]     20.121836 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VDEC2
[MCU2_1]     20.122109 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VENC1
[MCU2_1]     20.122357 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:55] Added target VENC2
[MCU2_1]     20.122413 s:  VX_ZONE_INIT:[tivxInit:71] Initialization Done !!!
[MCU2_1]     20.122447 s: APP: OpenVX Target kernel init ... !!!
[MCU2_1]     20.122757 s: APP: OpenVX Target kernel init ... Done !!!
[MCU2_1]     20.122797 s: UDMA Copy: Init ... !!!
[MCU2_1]     20.142444 s: UDMA Copy: Init ... Done !!!
[MCU2_1]     20.142501 s: APP: Init ... Done !!!
[MCU2_1]     20.142530 s: APP: Run ... !!!
[MCU2_1]     20.142552 s: IPC: Starting echo test ...
[MCU2_1]     20.144791 s: APP: Run ... Done !!!
[MCU2_1]     20.146135 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[s] C66X_1[.] C66X_2[P] C7X_1[.]
[MCU2_1]     20.146353 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[s] C66X_1[P] C66X_2[P] C7X_1[.]
[MCU2_1]     20.146443 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[s] C66X_1[P] C66X_2[P] C7X_1[P]
[MCU2_1]     20.331015 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[s] C66X_1[P] C66X_2[P] C7X_1[P]
[C6x_1 ]      4.577342 s: CIO: Init ... Done !!!
[C6x_1 ]      4.577376 s: ### CPU Frequency <ORG = 1350000000 Hz>, <NEW = 1350000000 Hz>
[C6x_1 ]      4.577389 s: APP: Init ... !!!
[C6x_1 ]      4.577397 s: SCICLIENT: Init ... !!!
[C6x_1 ]      4.578382 s: SCICLIENT: DMSC FW version [20.8.5--v2020.08b (Terrific Lla]
[C6x_1 ]      4.578394 s: SCICLIENT: DMSC FW revision 0x14
[C6x_1 ]      4.578403 s: SCICLIENT: DMSC FW ABI revision 3.1
[C6x_1 ]      4.578413 s: SCICLIENT: Init ... Done !!!
[C6x_1 ]      4.578422 s: UDMA: Init ... !!!
[C6x_1 ]      4.591019 s: UDMA: Init ... Done !!!
[C6x_1 ]      4.591041 s: MEM: Init ... !!!
[C6x_1 ]      4.591054 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ d4000000 of size 16777216 bytes !!!
[C6x_1 ]      4.591072 s: MEM: Created heap (L2_MEM, id=2, flags=0x00000001) @ 800000 of size 229376 bytes !!!
[C6x_1 ]      4.591087 s: MEM: Created heap (DDR_SCRATCH_MEM, id=4, flags=0x00000001) @ d5000000 of size 50331648 bytes !!!
[C6x_1 ]      4.591103 s: MEM: Init ... Done !!!
[C6x_1 ]      4.591110 s: IPC: Init ... !!!
[C6x_1 ]      4.591124 s: IPC: 6 CPUs participating in IPC !!!
[C6x_1 ]      4.591137 s: IPC: Waiting for HLOS to be ready ... !!!
[C6x_1 ]     17.460380 s: IPC: HLOS is ready !!!
[C6x_1 ]     17.482882 s: IPC: Init ... Done !!!
[C6x_1 ]     17.482927 s: APP: Syncing with 5 CPUs ... !!!
[C6x_1 ]     19.996439 s: APP: Syncing with 5 CPUs ... Done !!!
[C6x_1 ]     19.996454 s: REMOTE_SERVICE: Init ... !!!
[C6x_1 ]     19.997368 s: REMOTE_SERVICE: Init ... Done !!!
[C6x_1 ]     19.997424 s:  VX_ZONE_INIT:Enabled
[C6x_1 ]     19.997437 s:  VX_ZONE_ERROR:Enabled
[C6x_1 ]     19.997447 s:  VX_ZONE_WARNING:Enabled
[C6x_1 ]     19.998489 s:  VX_ZONE_INIT:[tivxInit:71] Initialization Done !!!
[C6x_1 ]     19.998508 s: APP: OpenVX Target kernel init ... !!!
[C6x_1 ]     19.998812 s: APP: OpenVX Target kernel init ... Done !!!
[C6x_1 ]     19.998832 s: UDMA Copy: Init ... !!!
[C6x_1 ]     20.018118 s: UDMA Copy: Init ... Done !!!
[C6x_1 ]     20.018140 s: APP: Init ... Done !!!
[C6x_1 ]     20.023674 s: APP: Run ... !!!
[C6x_1 ]     20.023683 s: IPC: Starting echo test ...
[C6x_1 ]     20.024907 s: APP: Run ... Done !!!
[C6x_1 ]     20.025259 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C66X_1[s] C66X_2[x] C7X_1[P]
[C6x_1 ]     20.026315 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C66X_1[s] C66X_2[P] C7X_1[P]
[C6x_1 ]     20.145897 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[P] C66X_1[s] C66X_2[P] C7X_1[P]
[C6x_1 ]     20.330790 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C66X_1[s] C66X_2[P] C7X_1[P]
[C6x_2 ]      4.636058 s: CIO: Init ... Done !!!
[C6x_2 ]      4.636093 s: ### CPU Frequency <ORG = 1350000000 Hz>, <NEW = 1350000000 Hz>
[C6x_2 ]      4.636106 s: APP: Init ... !!!
[C6x_2 ]      4.636114 s: SCICLIENT: Init ... !!!
[C6x_2 ]      4.637131 s: SCICLIENT: DMSC FW version [20.8.5--v2020.08b (Terrific Lla]
[C6x_2 ]      4.637144 s: SCICLIENT: DMSC FW revision 0x14
[C6x_2 ]      4.637153 s: SCICLIENT: DMSC FW ABI revision 3.1
[C6x_2 ]      4.637163 s: SCICLIENT: Init ... Done !!!
[C6x_2 ]      4.637172 s: UDMA: Init ... !!!
[C6x_2 ]      4.650251 s: UDMA: Init ... Done !!!
[C6x_2 ]      4.650275 s: MEM: Init ... !!!
[C6x_2 ]      4.650288 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ d8000000 of size 16777216 bytes !!!
[C6x_2 ]      4.650305 s: MEM: Created heap (L2_MEM, id=2, flags=0x00000001) @ 800000 of size 229376 bytes !!!
[C6x_2 ]      4.650320 s: MEM: Created heap (DDR_SCRATCH_MEM, id=4, flags=0x00000001) @ d9000000 of size 50331648 bytes !!!
[C6x_2 ]      4.650337 s: MEM: Init ... Done !!!
[C6x_2 ]      4.650345 s: IPC: Init ... !!!
[C6x_2 ]      4.650359 s: IPC: 6 CPUs participating in IPC !!!
[C6x_2 ]      4.650373 s: IPC: Waiting for HLOS to be ready ... !!!
[C6x_2 ]     18.041433 s: IPC: HLOS is ready !!!
[C6x_2 ]     18.066393 s: IPC: Init ... Done !!!
[C6x_2 ]     18.066437 s: APP: Syncing with 5 CPUs ... !!!
[C6x_2 ]     19.996442 s: APP: Syncing with 5 CPUs ... Done !!!
[C6x_2 ]     19.996456 s: REMOTE_SERVICE: Init ... !!!
[C6x_2 ]     19.997385 s: REMOTE_SERVICE: Init ... Done !!!
[C6x_2 ]     19.997446 s:  VX_ZONE_INIT:Enabled
[C6x_2 ]     19.997462 s:  VX_ZONE_ERROR:Enabled
[C6x_2 ]     19.997473 s:  VX_ZONE_WARNING:Enabled
[C6x_2 ]     19.998495 s:  VX_ZONE_INIT:[tivxInit:71] Initialization Done !!!
[C6x_2 ]     19.998514 s: APP: OpenVX Target kernel init ... !!!
[C6x_2 ]     19.998826 s: APP: OpenVX Target kernel init ... Done !!!
[C6x_2 ]     19.998849 s: UDMA Copy: Init ... !!!
[C6x_2 ]     20.019176 s: UDMA Copy: Init ... Done !!!
[C6x_2 ]     20.019201 s: APP: Init ... Done !!!
[C6x_2 ]     20.024549 s: APP: Run ... !!!
[C6x_2 ]     20.024560 s: IPC: Starting echo test ...
[C6x_2 ]     20.025921 s: APP: Run ... Done !!!
[C6x_2 ]     20.026300 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C66X_1[.] C66X_2[s] C7X_1[P]
[C6x_2 ]     20.026335 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C66X_1[P] C66X_2[s] C7X_1[P]
[C6x_2 ]     20.145940 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[P] C66X_1[P] C66X_2[s] C7X_1[P]
[C6x_2 ]     20.330903 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C66X_1[P] C66X_2[s] C7X_1[P]
[C7x_1 ]      4.750986 s: CIO: Init ... Done !!!
[C7x_1 ]      4.751008 s: ### CPU Frequency <ORG = 1000000000 Hz>, <NEW = 1000000000 Hz>
[C7x_1 ]      4.751024 s: APP: Init ... !!!
[C7x_1 ]      4.751031 s: SCICLIENT: Init ... !!!
[C7x_1 ]      4.752069 s: SCICLIENT: DMSC FW version [20.8.5--v2020.08b (Terrific Lla]
[C7x_1 ]      4.752085 s: SCICLIENT: DMSC FW revision 0x14
[C7x_1 ]      4.752095 s: SCICLIENT: DMSC FW ABI revision 3.1
[C7x_1 ]      4.752106 s: SCICLIENT: Init ... Done !!!
[C7x_1 ]      4.752115 s: UDMA: Init ... !!!
[C7x_1 ]      4.760060 s: UDMA: Init ... Done !!!
[C7x_1 ]      4.760072 s: MEM: Init ... !!!
[C7x_1 ]      4.760083 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ dc000000 of size 268435456 bytes !!!
[C7x_1 ]      4.760104 s: MEM: Created heap (L3_MEM, id=1, flags=0x00000001) @ 70020000 of size 8159232 bytes !!!
[C7x_1 ]      4.760121 s: MEM: Created heap (L2_MEM, id=2, flags=0x00000001) @ 64800000 of size 491520 bytes !!!
[C7x_1 ]      4.760139 s: MEM: Created heap (L1_MEM, id=3, flags=0x00000001) @ 64e00000 of size 16384 bytes !!!
[C7x_1 ]      4.760156 s: MEM: Created heap (DDR_SCRATCH_MEM, id=4, flags=0x00000001) @ ec000000 of size 234881024 bytes !!!
[C7x_1 ]      4.760174 s: MEM: Init ... Done !!!
[C7x_1 ]      4.760181 s: IPC: Init ... !!!
[C7x_1 ]      4.760192 s: IPC: 6 CPUs participating in IPC !!!
[C7x_1 ]      4.760205 s: IPC: Waiting for HLOS to be ready ... !!!
[C7x_1 ]     18.755878 s: IPC: HLOS is ready !!!
[C7x_1 ]     18.766739 s: IPC: Init ... Done !!!
[C7x_1 ]     18.766755 s: APP: Syncing with 5 CPUs ... !!!
[C7x_1 ]     19.996442 s: APP: Syncing with 5 CPUs ... Done !!!
[C7x_1 ]     19.996459 s: REMOTE_SERVICE: Init ... !!!
[C7x_1 ]     19.996739 s: REMOTE_SERVICE: Init ... Done !!!
[C7x_1 ]     19.996763 s:  VX_ZONE_INIT:Enabled
[C7x_1 ]     19.996775 s:  VX_ZONE_ERROR:Enabled
[C7x_1 ]     19.996784 s:  VX_ZONE_WARNING:Enabled
[C7x_1 ]     19.997149 s:  VX_ZONE_INIT:[tivxInit:71] Initialization Done !!!
[C7x_1 ]     19.997166 s: APP: OpenVX Target kernel init ... !!!
[C7x_1 ]     19.997255 s: APP: OpenVX Target kernel init ... Done !!!
[C7x_1 ]     19.997271 s: APP: Init ... Done !!!
[C7x_1 ]     19.997281 s: APP: Run ... !!!
[C7x_1 ]     19.997289 s: IPC: Starting echo test ...
[C7x_1 ]     19.997767 s: APP: Run ... Done !!!
[C7x_1 ]     20.025275 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C66X_1[P] C66X_2[x] C7X_1[s]
[C7x_1 ]     20.026302 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C66X_1[P] C66X_2[P] C7X_1[s]
[C7x_1 ]     20.145954 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[P] C66X_1[P] C66X_2[P] C7X_1[s]
[C7x_1 ]     20.330929 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C66X_1[P] C66X_2[P] C7X_1[s]
```
\endif

\if DOCS_J721S2
```
[MCU2_0]      4.226248 s: CIO: Init ... Done !!!
[MCU2_0]      4.226299 s: ### CPU Frequency = 1000000000 Hz
[MCU2_0]      4.226332 s: APP: Init ... !!!
[MCU2_0]      4.226353 s: SCICLIENT: Init ... !!!
[MCU2_0]      4.226484 s: SCICLIENT: DMSC FW version [22.1.1--v2022.01 (Terrific Llam]
[MCU2_0]      4.226518 s: SCICLIENT: DMSC FW revision 0x16  
[MCU2_0]      4.226548 s: SCICLIENT: DMSC FW ABI revision 3.1
[MCU2_0]      4.226581 s: SCICLIENT: Init ... Done !!!
[MCU2_0]      4.226604 s: UDMA: Init ... !!!
[MCU2_0]      4.227540 s: UDMA: Init ... Done !!!
[MCU2_0]      4.227579 s: UDMA: Init ... !!!
[MCU2_0]      4.228106 s: UDMA: Init for CSITX/CSIRX ... Done !!!
[MCU2_0]      4.228146 s: MEM: Init ... !!!
[MCU2_0]      4.228181 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ db000000 of size 16777216 bytes !!!
[MCU2_0]      4.228244 s: MEM: Init ... Done !!!
[MCU2_0]      4.228267 s: IPC: Init ... !!!
[MCU2_0]      4.228311 s: IPC: 5 CPUs participating in IPC !!!
[MCU2_0]      4.228347 s: IPC: Waiting for HLOS to be ready ... !!!
[MCU2_0]     12.560729 s: IPC: HLOS is ready !!!
[MCU2_0]     12.570212 s: IPC: Init ... Done !!!
[MCU2_0]     12.570255 s: APP: Syncing with 4 CPUs ... !!!
[MCU2_0]     12.694631 s: APP: Syncing with 4 CPUs ... Done !!!
[MCU2_0]     12.694667 s: REMOTE_SERVICE: Init ... !!!
[MCU2_0]     12.696146 s: REMOTE_SERVICE: Init ... Done !!!
[MCU2_0]     12.696183 s: FVID2: Init ... !!!
[MCU2_0]     12.696239 s: FVID2: Init ... Done !!!
[MCU2_0]     12.696262 s: SCICLIENT: Sciclient_pmSetModuleState module=219 state=2
[MCU2_0]     12.696365 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.696407 s: DSS: Init ... !!!
[MCU2_0]     12.696430 s: DSS: Display type is eDP !!!
[MCU2_0]     12.696453 s: DSS: M2M Path is enabled !!!
[MCU2_0]     12.696476 s: DSS: SoC init ... !!!
[MCU2_0]     12.696496 s: SCICLIENT: Sciclient_pmSetModuleState module=158 state=0
[MCU2_0]     12.696560 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.696587 s: SCICLIENT: Sciclient_pmSetModuleState module=365 state=2
[MCU2_0]     12.696774 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.696800 s: SCICLIENT: Sciclient_pmSetModuleState module=156 state=2
[MCU2_0]     12.696962 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.696988 s: SCICLIENT: Sciclient_pmSetModuleState module=365 state=2
[MCU2_0]     12.697058 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.697084 s: SCICLIENT: Sciclient_pmSetModuleState module=156 state=2
[MCU2_0]     12.697148 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.697173 s: SCICLIENT: Sciclient_pmSetModuleState module=158 state=0
[MCU2_0]     12.697294 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.697320 s: SCICLIENT: Sciclient_pmSetModuleClkFreq module=158 clk=36 freq=148500000
[MCU2_0]     12.698066 s: SCICLIENT: Sciclient_pmSetModuleClkFreq success
[MCU2_0]     12.698092 s: SCICLIENT: Sciclient_pmModuleClkRequest module=158 clk=36 state=2 flag=2
[MCU2_0]     12.698224 s: SCICLIENT: Sciclient_pmModuleClkRequest success
[MCU2_0]     12.698251 s: SCICLIENT: Sciclient_pmSetModuleState module=158 state=2
[MCU2_0]     12.698396 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.698422 s: DSS: SoC init ... Done !!!
[MCU2_0]     12.698445 s: DSS: Board init ... !!!
[MCU2_0]     12.698465 s: DSS: Board init ... Done !!!
[MCU2_0]     12.752307 s: DSS: Init ... Done !!!
[MCU2_0]     12.752359 s: VHWA: VPAC Init ... !!!
[MCU2_0]     12.752384 s: SCICLIENT: Sciclient_pmSetModuleState module=361 state=2
[MCU2_0]     12.752552 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.752583 s: VHWA: LDC Init ... !!!
[MCU2_0]     12.754171 s: VHWA: LDC Init ... Done !!!
[MCU2_0]     12.754211 s: VHWA: MSC Init ... !!!
[MCU2_0]     12.760265 s: VHWA: MSC Init ... Done !!!
[MCU2_0]     12.760299 s: VHWA: NF Init ... !!!
[MCU2_0]     12.761083 s: VHWA: NF Init ... Done !!!
[MCU2_0]     12.761113 s: VHWA: VISS Init ... !!!
[MCU2_0]     12.765881 s: VHWA: VISS Init ... Done !!!
[MCU2_0]     12.765912 s: VHWA: VPAC Init ... Done !!!
[MCU2_0]     12.765949 s:  VX_ZONE_INIT:Enabled
[MCU2_0]     12.765976 s:  VX_ZONE_ERROR:Enabled
[MCU2_0]     12.766001 s:  VX_ZONE_WARNING:Enabled
[MCU2_0]     12.766721 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target IPU1-0 
[MCU2_0]     12.766939 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target VPAC_NF 
[MCU2_0]     12.767139 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target VPAC_LDC1 
[MCU2_0]     12.767333 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target VPAC_MSC1 
[MCU2_0]     12.767519 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target VPAC_MSC2 
[MCU2_0]     12.767783 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target VPAC_VISS1 
[MCU2_0]     12.767995 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE1 
[MCU2_0]     12.768263 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE2 
[MCU2_0]     12.768481 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DISPLAY1 
[MCU2_0]     12.768687 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DISPLAY2 
[MCU2_0]     12.768882 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CSITX 
[MCU2_0]     12.769084 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE3 
[MCU2_0]     12.769281 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE4 
[MCU2_0]     12.769478 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE5 
[MCU2_0]     12.769680 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE6 
[MCU2_0]     12.769887 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE7 
[MCU2_0]     12.770107 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target CAPTURE8 
[MCU2_0]     12.770309 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DSS_M2M1 
[MCU2_0]     12.770506 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DSS_M2M2 
[MCU2_0]     12.770703 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DSS_M2M3 
[MCU2_0]     12.770903 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DSS_M2M4 
[MCU2_0]     12.770945 s:  VX_ZONE_INIT:[tivxInitLocal:130] Initialization Done !!!
[MCU2_0]     12.770975 s: APP: OpenVX Target kernel init ... !!!
[MCU2_0]     12.797213 s: APP: OpenVX Target kernel init ... Done !!!
[MCU2_0]     12.797251 s: CSI2RX: Init ... !!!
[MCU2_0]     12.797275 s: SCICLIENT: Sciclient_pmSetModuleState module=136 state=2
[MCU2_0]     12.797366 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.797396 s: SCICLIENT: Sciclient_pmSetModuleState module=38 state=2
[MCU2_0]     12.797496 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.797522 s: SCICLIENT: Sciclient_pmSetModuleState module=39 state=2
[MCU2_0]     12.797609 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.797635 s: SCICLIENT: Sciclient_pmSetModuleState module=152 state=2
[MCU2_0]     12.797707 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.797734 s: SCICLIENT: Sciclient_pmSetModuleState module=153 state=2
[MCU2_0]     12.797801 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.798357 s: CSI2RX: Init ... Done !!!
[MCU2_0]     12.798395 s: CSI2TX: Init ... !!!
[MCU2_0]     12.798417 s: SCICLIENT: Sciclient_pmSetModuleState module=136 state=2
[MCU2_0]     12.798491 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.798519 s: SCICLIENT: Sciclient_pmSetModuleState module=40 state=2
[MCU2_0]     12.798618 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.798644 s: SCICLIENT: Sciclient_pmSetModuleState module=41 state=2
[MCU2_0]     12.798739 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.798766 s: SCICLIENT: Sciclient_pmSetModuleState module=363 state=2
[MCU2_0]     12.798850 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_0]     12.799241 s: CSI2TX: Init ... Done !!!
[MCU2_0]     12.799284 s: ISS: Init ... !!!
[MCU2_0]     12.799317 s: IssSensor_Init ... Done !!!
[MCU2_0]     12.799378 s: vissRemoteServer_Init ... Done !!!
[MCU2_0]     12.799429 s: IttRemoteServer_Init ... Done !!!
[MCU2_0]     12.799456 s: UDMA Copy: Init ... !!!
[MCU2_0]     12.800421 s: UDMA Copy: Init ... Done !!!
[MCU2_0]     12.800487 s: APP: Init ... Done !!!
[MCU2_0]     12.800518 s: APP: Run ... !!!
[MCU2_0]     12.800539 s: IPC: Starting echo test ...
[MCU2_0]     12.802804 s: APP: Run ... Done !!!
[MCU2_0]     12.803549 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[.] C7X_1[P] C7X_2[.] 
[MCU2_0]     12.803626 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[.] C7X_1[P] C7X_2[P] 
[MCU2_0]     12.803695 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[P] C7X_1[P] C7X_2[P] 
[MCU2_1]      4.220156 s: CIO: Init ... Done !!!
[MCU2_1]      4.220205 s: ### CPU Frequency = 1000000000 Hz
[MCU2_1]      4.220237 s: APP: Init ... !!!
[MCU2_1]      4.220257 s: SCICLIENT: Init ... !!!
[MCU2_1]      4.220382 s: SCICLIENT: DMSC FW version [22.1.1--v2022.01 (Terrific Llam]
[MCU2_1]      4.220414 s: SCICLIENT: DMSC FW revision 0x16  
[MCU2_1]      4.220440 s: SCICLIENT: DMSC FW ABI revision 3.1
[MCU2_1]      4.220471 s: SCICLIENT: Init ... Done !!!
[MCU2_1]      4.220493 s: UDMA: Init ... !!!
[MCU2_1]      4.221640 s: UDMA: Init ... Done !!!
[MCU2_1]      4.221673 s: MEM: Init ... !!!
[MCU2_1]      4.221707 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ dc000000 of size 16777216 bytes !!!
[MCU2_1]      4.221770 s: MEM: Init ... Done !!!
[MCU2_1]      4.221792 s: IPC: Init ... !!!
[MCU2_1]      4.221838 s: IPC: 5 CPUs participating in IPC !!!
[MCU2_1]      4.221872 s: IPC: Waiting for HLOS to be ready ... !!!
[MCU2_1]     12.685207 s: IPC: HLOS is ready !!!
[MCU2_1]     12.694558 s: IPC: Init ... Done !!!
[MCU2_1]     12.694597 s: APP: Syncing with 4 CPUs ... !!!
[MCU2_1]     12.694631 s: APP: Syncing with 4 CPUs ... Done !!!
[MCU2_1]     12.694660 s: REMOTE_SERVICE: Init ... !!!
[MCU2_1]     12.696054 s: REMOTE_SERVICE: Init ... Done !!!
[MCU2_1]     12.696095 s: FVID2: Init ... !!!
[MCU2_1]     12.696152 s: FVID2: Init ... Done !!!
[MCU2_1]     12.696176 s: VHWA: DMPAC: Init ... !!!
[MCU2_1]     12.696198 s: SCICLIENT: Sciclient_pmSetModuleState module=58 state=2
[MCU2_1]     12.696322 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_1]     12.696350 s: SCICLIENT: Sciclient_pmSetModuleState module=62 state=2
[MCU2_1]     12.696433 s: SCICLIENT: Sciclient_pmSetModuleState success
[MCU2_1]     12.696459 s: VHWA: DOF Init ... !!!
[MCU2_1]     12.700485 s: VHWA: DOF Init ... Done !!!
[MCU2_1]     12.700525 s: VHWA: SDE Init ... !!!
[MCU2_1]     12.701648 s: VHWA: SDE Init ... Done !!!
[MCU2_1]     12.701680 s: VHWA: DMPAC: Init ... Done !!!
[MCU2_1]     12.701717 s:  VX_ZONE_INIT:Enabled
[MCU2_1]     12.701740 s:  VX_ZONE_ERROR:Enabled
[MCU2_1]     12.701765 s:  VX_ZONE_WARNING:Enabled
[MCU2_1]     12.702453 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DMPAC_SDE 
[MCU2_1]     12.702634 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target DMPAC_DOF 
[MCU2_1]     12.702823 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:54] Added target IPU1-1 
[MCU2_1]     12.702864 s:  VX_ZONE_INIT:[tivxInitLocal:130] Initialization Done !!!
[MCU2_1]     12.702893 s: APP: OpenVX Target kernel init ... !!!
[MCU2_1]     12.703137 s: APP: OpenVX Target kernel init ... Done !!!
[MCU2_1]     12.703167 s: UDMA Copy: Init ... !!!
[MCU2_1]     12.704111 s: UDMA Copy: Init ... Done !!!
[MCU2_1]     12.704158 s: APP: Init ... Done !!!
[MCU2_1]     12.704182 s: APP: Run ... !!!
[MCU2_1]     12.704203 s: IPC: Starting echo test ...
[MCU2_1]     12.706273 s: APP: Run ... Done !!!
[MCU2_1]     12.706880 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[s] C7X_1[P] C7X_2[.] 
[MCU2_1]     12.706954 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[s] C7X_1[P] C7X_2[P] 
[MCU2_1]     12.803470 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[s] C7X_1[P] C7X_2[P] 
[C7x_1 ]      4.367809 s: CIO: Init ... Done !!!
[C7x_1 ]      4.367825 s: ### CPU Frequency = 1000000000 Hz
[C7x_1 ]      4.367836 s: APP: Init ... !!!
[C7x_1 ]      4.367845 s: SCICLIENT: Init ... !!!
[C7x_1 ]      4.367954 s: SCICLIENT: DMSC FW version [22.1.1--v2022.01 (Terrific Llam]
[C7x_1 ]      4.367969 s: SCICLIENT: DMSC FW revision 0x16  
[C7x_1 ]      4.367979 s: SCICLIENT: DMSC FW ABI revision 3.1
[C7x_1 ]      4.367990 s: SCICLIENT: Init ... Done !!!
[C7x_1 ]      4.367999 s: UDMA: Init ... !!!
[C7x_1 ]      4.368943 s: UDMA: Init ... Done !!!
[C7x_1 ]      4.368955 s: MEM: Init ... !!!
[C7x_1 ]      4.368966 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ 100000000 of size 268435456 bytes !!!
[C7x_1 ]      4.368986 s: MEM: Created heap (L3_MEM, id=1, flags=0x00000001) @ 70020000 of size 3964928 bytes !!!
[C7x_1 ]      4.369005 s: MEM: Created heap (L2_MEM, id=2, flags=0x00000001) @ 64800000 of size 458752 bytes !!!
[C7x_1 ]      4.369022 s: MEM: Created heap (L1_MEM, id=3, flags=0x00000001) @ 64e00000 of size 16384 bytes !!!
[C7x_1 ]      4.369039 s: MEM: Created heap (DDR_SCRATCH_MEM, id=4, flags=0x00000001) @ e3000000 of size 419430400 bytes !!!
[C7x_1 ]      4.369058 s: MEM: Init ... Done !!!
[C7x_1 ]      4.369066 s: IPC: Init ... !!!
[C7x_1 ]      4.369081 s: IPC: 5 CPUs participating in IPC !!!
[C7x_1 ]      4.369095 s: IPC: Waiting for HLOS to be ready ... !!!
[C7x_1 ]     10.818443 s: IPC: HLOS is ready !!!
[C7x_1 ]     10.820401 s: IPC: Init ... Done !!!
[C7x_1 ]     10.820416 s: APP: Syncing with 4 CPUs ... !!!
[C7x_1 ]     12.694633 s: APP: Syncing with 4 CPUs ... Done !!!
[C7x_1 ]     12.694653 s: REMOTE_SERVICE: Init ... !!!
[C7x_1 ]     12.694799 s: REMOTE_SERVICE: Init ... Done !!!
[C7x_1 ]     12.694822 s:  VX_ZONE_INIT:Enabled
[C7x_1 ]     12.694833 s:  VX_ZONE_ERROR:Enabled
[C7x_1 ]     12.694872 s:  VX_ZONE_WARNING:Enabled
[C7x_1 ]     12.695098 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1 
[C7x_1 ]     12.695164 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_2 
[C7x_1 ]     12.695223 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_3 
[C7x_1 ]     12.695283 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_4 
[C7x_1 ]     12.695347 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_5 
[C7x_1 ]     12.695409 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_6 
[C7x_1 ]     12.695472 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_7 
[C7x_1 ]     12.695531 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP_C7-1_PRI_8 
[C7x_1 ]     12.695555 s:  VX_ZONE_INIT:[tivxInitLocal:130] Initialization Done !!!
[C7x_1 ]     12.695568 s: APP: OpenVX Target kernel init ... !!!
[C7x_1 ]     12.695647 s: APP: OpenVX Target kernel init ... Done !!!
[C7x_1 ]     12.695659 s: APP: Init ... Done !!!
[C7x_1 ]     12.695668 s: APP: Run ... !!!
[C7x_1 ]     12.695677 s: IPC: Starting echo test ...
[C7x_1 ]     12.695816 s: APP: Run ... Done !!!
[C7x_1 ]     12.696217 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C7X_1[s] C7X_2[P] 
[C7x_1 ]     12.706770 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[P] C7X_1[s] C7X_2[P] 
[C7x_1 ]     12.803408 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C7X_1[s] C7X_2[P] 
[C7x_2 ]      4.423608 s: CIO: Init ... Done !!!
[C7x_2 ]      4.423624 s: ### CPU Frequency = 1000000000 Hz
[C7x_2 ]      4.423636 s: APP: Init ... !!!
[C7x_2 ]      4.423644 s: SCICLIENT: Init ... !!!
[C7x_2 ]      4.423753 s: SCICLIENT: DMSC FW version [22.1.1--v2022.01 (Terrific Llam]
[C7x_2 ]      4.423768 s: SCICLIENT: DMSC FW revision 0x16  
[C7x_2 ]      4.423778 s: SCICLIENT: DMSC FW ABI revision 3.1
[C7x_2 ]      4.423790 s: SCICLIENT: Init ... Done !!!
[C7x_2 ]      4.423799 s: UDMA: Init ... !!!
[C7x_2 ]      4.424731 s: UDMA: Init ... Done !!!
[C7x_2 ]      4.424745 s: MEM: Init ... !!!
[C7x_2 ]      4.424756 s: MEM: Created heap (DDR_SHARED_MEM, id=0, flags=0x00000004) @ de000000 of size 16777216 bytes !!!
[C7x_2 ]      4.424777 s: MEM: Created heap (L2_MEM, id=2, flags=0x00000001) @ 65800000 of size 458752 bytes !!!
[C7x_2 ]      4.424797 s: MEM: Created heap (L1_MEM, id=3, flags=0x00000001) @ 65e00000 of size 16384 bytes !!!
[C7x_2 ]      4.424814 s: MEM: Created heap (DDR_SCRATCH_MEM, id=4, flags=0x00000001) @ df000000 of size 67108864 bytes !!!
[C7x_2 ]      4.424834 s: MEM: Init ... Done !!!
[C7x_2 ]      4.424842 s: IPC: Init ... !!!
[C7x_2 ]      4.424857 s: IPC: 5 CPUs participating in IPC !!!
[C7x_2 ]      4.424871 s: IPC: Waiting for HLOS to be ready ... !!!
[C7x_2 ]     11.572452 s: IPC: HLOS is ready !!!
[C7x_2 ]     11.574250 s: IPC: Init ... Done !!!
[C7x_2 ]     11.574265 s: APP: Syncing with 4 CPUs ... !!!
[C7x_2 ]     12.694634 s: APP: Syncing with 4 CPUs ... Done !!!
[C7x_2 ]     12.694653 s: REMOTE_SERVICE: Init ... !!!
[C7x_2 ]     12.694800 s: REMOTE_SERVICE: Init ... Done !!!
[C7x_2 ]     12.694825 s:  VX_ZONE_INIT:Enabled
[C7x_2 ]     12.694836 s:  VX_ZONE_ERROR:Enabled
[C7x_2 ]     12.694845 s:  VX_ZONE_WARNING:Enabled
[C7x_2 ]     12.695362 s:  VX_ZONE_INIT:[tivxPlatformCreateTargetId:59] Added target DSP-1 
[C7x_2 ]     12.695385 s:  VX_ZONE_INIT:[tivxInitLocal:130] Initialization Done !!!
[C7x_2 ]     12.695399 s: APP: OpenVX Target kernel init ... !!!
[C7x_2 ]     12.695733 s: APP: OpenVX Target kernel init ... Done !!!
[C7x_2 ]     12.695751 s: APP: Init ... Done !!!
[C7x_2 ]     12.695762 s: APP: Run ... !!!
[C7x_2 ]     12.695770 s: IPC: Starting echo test ...
[C7x_2 ]     12.695964 s: APP: Run ... Done !!!
[C7x_2 ]     12.696223 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[x] C7X_1[P] C7X_2[s] 
[C7x_2 ]     12.706785 s: IPC: Echo status: mpu1_0[x] mcu2_0[x] mcu2_1[P] C7X_1[P] C7X_2[s] 
[C7x_2 ]     12.803423 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C7X_1[P] C7X_2[s]
```

\endif
