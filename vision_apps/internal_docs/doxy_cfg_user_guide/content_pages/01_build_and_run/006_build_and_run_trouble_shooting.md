#  Trouble shooting build and run errors {#BUILD_TROUBLE_SHOOTING}

[TOC]

# Trouble shooting build errors

## I see the error "ERROR: /home/<user>/ti-processor-sdk-rtos-${SOC}-evm-xx_xx_xx_xx/targetfs//usr/include not found !!!" when building vision_apps

vision_apps needs the PSDK Linux target filesystem to be installed as mentioned in \ref ENVIRONMENT_SETUP_STEP2 and \ref ADDITIONAL_DOWNLOADS
This tisdk-edgeai-image-${SOC}-evm-evm.tar.xz when untarr'ed at ${PSDKR_PATH}/targetfs will contain the Linux file system needed to build
in Linux+RTOS mode.

If you havent installed the target filesystem you will see a error like below,
\code
ERROR: /home/<user>/ti-processor-sdk-rtos-${SOC}-evm-xx_xx_xx_xx/targetfs//usr/include not found !!!
makerules/makefile_check_paths.mak:7: recipe for target 'sdk_check_paths' failed
make: *** [sdk_check_paths] Error 1
\endcode

## How do I install GCC tools for ARM A72 ? {#FAQ_BUILD_GCC}

Running the script mentioned in \ref ADDITIONAL_DOWNLOADS downloads and installs the requried GCC tools.

This needs proxy to be setup as mentioned in \ref PROXY_SETUP

After installation you should see GCC compilers at below path
    \code
    ${PSDKR_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu
    ${PSDKR_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf
    ${PSDKR_PATH}/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf
    \endcode

\note If this step is unsucessful then you will see compile errors when compiling A72 files.

You can also manually download and install these packages by refering to the download links in setup_psdk_rtos.sh.

If you installed these packages at a different path then modify below variables in tiovx/psdkra_tools_path.mak to point to your install folder

\code
GCC_SYSBIOS_ARM_ROOT ?= $(PSDK_PATH)/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf
GCC_LINUX_ARM_ROOT ?= $(PSDK_PATH)/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu
\endcode


## What does setup_psdk_rtos.sh do and can I skip executing this ?

It is HIGHLY recommended to NOT skip executing setup_psdk_rtos.sh as mentioned in \ref ADDITIONAL_DOWNLOADS.

However some steps within this are optional,
depending on your use-case and can be skipped as mentioned below.

The script setup_psdk_rtos.sh does the below,

- Installs packages on your local linux machine using "apt-get install". These are required to build PSDK RTOS esp in PC emulation mode.
  This needs sudo permission to install the packages.
  \note In case you don't want to install packages with sudo permission pass below argument to this script to skip this step.
      \code
      --skip_sudo
      \endcode
  \note This step can be skipped. Make sure to keep the flag BUILD_EMULATION_MODE=no in tiovx/build_flags.mak.
        And if any tool is not found during build, install it separately using "apt-get install"

- It extracts PSDK Linux target filesystem to the folder ${PSDKR_PATH}/targetfs. This is needed for some files in PSDK RTOS to compile and link
  - Make sure you have installed PSDK Linux to get the filesystem tar ball (see \ref ENVIRONMENT_SETUP_STEP2)

  \note This step should NOT be skipped

- It extracts PSDK Linux boot files to a folder ${PSDKR_PATH}/bootfs. This is a temporary staging area to hold the prebuilt linux SPL, uboot, and other
  boot related files before they are copied to SD card
  \note This step should NOT be skipped

- Downloads and installs GCC compilers for ARM64, ARM32 for Linux and ARM64 for RTOS (see \ref FAQ_BUILD_GCC)
  \note This step should NOT be skipped

- Installs "glm", "glew" tools for PC Linux. These are needed for PC emulation mode ONLY.
  \note You can skip this step. Make sure to keep the flag BUILD_EMULATION_MODE=no in tiovx/build_flags.mak

- Installs pip3 for python3. This is needed to run PyTIOVX and PyPSDK_RTOS_TOOLS code generation tools.
  \note You can skip this step. It does not affect code compile and run.


# Trouble shooting run time errors

## I dont see any print on UART terminal ?

- Make sure the processor board is connected tightly on the EVM. A loose connection between processor board and base board will result in SoC boot failing.
- Make sure the correct UART port is used
- Make sure UART settings on PC are correct
- Make sure the boot pin selection is correct
- Make sure the SD card has the boot files

Refer \ref run_steps_linux for EVM setup.
Refer \ref run_step1_linux for SD card setup.

## What is the purpose of different files in bootfs partition ?

File |  Purpose
-----|---------------
tiboot3.bin |  R5F SPL, first stage bootloader, ROM boots this file on MCU R5F (mcu1_0)
\if DOCS_J721E
sysfw.itb  | DMSC firmware, R5F SPL boots DMSC FW when it starts
\endif
tispl.bin | A72 SPL which will the load uboot
u-boot.img | A72 uboot
uenv.txt | additional configuration given to uboot. Largely this specifies the dtbo's to apply when booting the Linux kernel
uenv.txt.psdkra | dtbo's that PSDK RTOS applies when it boots linux

- When steps in \ref run_step1_linux are followed the boot files are copied from ${PSDKR_PATH}/bootfs to SD card bootfs partition.
- When **make linux_fs_install_sd** is called,
  - Default uenv.txt is replaced by vision_apps/apps/basic_demos/app_linux_fs_files/uenv.txt

## What are the filesystem changes done by vision apps on top of default PSDK Linux filesystem

- See makefile target **linux_fs_install_sd** in vision_apps/makerules/makefile_linux_arm.mak for exact filesystem changes that are done
- In summary
  - /lib/firmware/${SOC}-evm-*-fw are changed to point to vision_apps firmwares or executables for C6x, C7x and R5F
  - uenv.txt and sysfw.itb are replaced in bootfs partition to use files specific to vision_apps
  - uenv.txt applies the dtbo **k3-${SOC}-vision-apps.dtbo**
\if DOCS_J721E
    - These dtbo adjust the memory map based on vision_apps requirements
    - k3-${SOC}-vision-apps.dtbo
      - disables display on A72 so that R5F can control display
      - disables i2c1 on A72 which is used for HDMI display control by R5F
      - disables i2c6 on A72 which is used for CSI2RX sensor control by R5F
      - enables DMA Buf contigous memory allocator and reserves a heap for DMA Buf
      - reserves memory for shared memory between different CPUs
\endif
  - Updates etc/security/limits.conf to increase limit of max open files in a process. This is need when using DMA Buf since in DMA Buf every memory alloc is
    a file handle. And the default file open limit is too small and max number of open DMA Buf alloc's is limited in this case.
  - When executed with HS=1 (Necessary when working with HS devices), 
    - signed versions of the vision_apps firmwares or executables are also installed
    - /lib/firmware/${SOC}-evm-*-fw-sec are installed to point to the above

## What is DMA heap and why I dont see CMEM or ION anymore ?

- DMA heap is the contiguous memory allocator used in Linux starting PSDK RTOS v7.0.0.
- It replaces ION as the contiguous memory allocator.
- See vision_apps/apps/basic_demos/app_mem for simple application which uses the DMA heap memory allocator.

## How do I know the remote cores like C6x, C7x, R5F booted correctly ?

Here we assume you are able to reach the Linux login prompt on the EVM and want to know state of remote cores like C6x, C7x, R5F

- Login at the login prompt as shown below
  \code
  Arago 2019.09 ${SOC}-evm ttyS2

  ${SOC}-evm login: root
  \endcode

- Do below to see logs from remote cores
  \code
  root@${SOC}-evm:/opt/vision_apps# dmesg | grep rpmsg
  \endcode

  You should see something like below
  \code
    [   11.645925] virtio_rpmsg_bus virtio0: rpmsg host is online
    [   11.674968] virtio_rpmsg_bus virtio0: creating channel rpmsg_chrdev addr 0xd
    [   11.895276] virtio_rpmsg_bus virtio1: rpmsg host is online
    [   11.926545] virtio_rpmsg_bus virtio1: creating channel rpmsg_chrdev addr 0xd
    [   12.358941] virtio_rpmsg_bus virtio2: rpmsg host is online
    [   12.360858] virtio_rpmsg_bus virtio2: creating channel rpmsg_chrdev addr 0xd
    [   12.564245] virtio_rpmsg_bus virtio3: rpmsg host is online
    [   12.594490] virtio_rpmsg_bus virtio3: creating channel ti.ipc4.ping-pong addr 0xd
    [   12.602105] virtio_rpmsg_bus virtio3: creating channel rpmsg_chrdev addr 0xe
    [   13.023025] virtio_rpmsg_bus virtio4: rpmsg host is online
    [   13.051430] virtio_rpmsg_bus virtio4: creating channel rpmsg_chrdev addr 0xd
    [   13.267107] virtio_rpmsg_bus virtio5: rpmsg host is online
    [   13.272167] virtio_rpmsg_bus virtio5: creating channel rpmsg_chrdev addr 0xd
    [   13.280834] virtio_rpmsg_bus virtio5: creating channel rpmsg_chrdev addr 0x15
    [   13.301423] virtio_rpmsg_bus virtio2: creating channel rpmsg_chrdev addr 0x15
    [   13.312954] virtio_rpmsg_bus virtio2: creating channel ti.ipc4.ping-pong addr 0xe
    [   13.324875] virtio_rpmsg_bus virtio0: creating channel rpmsg_chrdev addr 0x15
    [   13.336735] virtio_rpmsg_bus virtio0: creating channel ti.ipc4.ping-pong addr 0xe
    [   13.355257] virtio_rpmsg_bus virtio1: creating channel rpmsg_chrdev addr 0x15
    [   13.362563] virtio_rpmsg_bus virtio1: creating channel ti.ipc4.ping-pong addr 0xe
    [   13.372567] virtio_rpmsg_bus virtio4: creating channel rpmsg_chrdev addr 0x15
    [   13.384308] virtio_rpmsg_bus virtio4: creating channel ti.ethfw.notifyservice addr 0x1e
    [   13.398527] virtio_rpmsg_bus virtio4: creating channel rpmsg-kdrv addr 0x1a
    [   13.407891] virtio_rpmsg_bus virtio5: creating channel ti.ipc4.ping-pong addr 0xe
    [   13.418470] rpmsg-kdrv-eth-switch rpmsg-kdrv-2-mpu_1_0_ethswitch-device-0: Device info: permissions: 07FFFFFF uart_id: 2
    [   13.434450] rpmsg-kdrv-eth-switch rpmsg-kdrv-2-mpu_1_0_ethswitch-device-0: FW ver 0.1 (rev 1)  8/Apr/2021 SHA:
    [   13.446521] virtio_rpmsg_bus virtio4: creating channel ti.ipc4.ping-pong addr 0xe
  \endcode

- **virtio_rpmsg_bus virtioN: rpmsg host is online** means a remote core was booted, by default in vision apps
  you should see virtio0 to virtio3 "online" representing C6x-1, C6x-2, mcu2-1, c7x-1

- Below lines for each virtioN indicate that they were able to initialize themselves and established IPC with linux
  \code
    [   14.094299] virtio_rpmsg_bus virtio3: creating channel rpmsg_chrdev addr 0xd
    [   14.113023] virtio_rpmsg_bus virtio3: creating channel rpmsg_chrdev addr 0x15
    [   14.206737] virtio_rpmsg_bus virtio3: creating channel ti.ipc4.ping-pong addr 0xe
  \endcode
  So if you see "host in online" but not the "creating channel" then the CPU was booted but when it was initializing it failed somewhere.

- Do below to see logs from remote cores on Linux A72
  \code
  cd /opt/vision_apps
  source ./vision_apps_init.sh
  \endcode

  - vision_apps_init.sh runs a process called **vx_app_linux_arm_remote_log.out** in the background.
    - This process continuously monitors a shared memory and logs strings from remote cores to the linux terminal
    - Make sure this script is invoked only once after EVM power ON.
  - You should see something as shown in these logs <a href="../logs/vision_app_init_log.txt" target="_blank">[TXT]</a>
  - The main lines to look for are shown below
    \code
    [MCU2_0]      0.084681 s: IPC: Echo status: mpu1_0[x] mcu2_0[s] mcu2_1[P] C66X_1[P] C66X_2[P] C7X_1[P]
    [MCU2_1]     24.589628 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[s] C66X_1[P] C66X_2[P] C7X_1[P]
    [C6x_1 ]      0.842620 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C66X_1[s] C66X_2[P] C7X_1[P]
    [C6x_2 ]      0.792484 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C66X_1[P] C66X_2[s] C7X_1[P]
    [C7x_1 ]     24.589506 s: IPC: Echo status: mpu1_0[x] mcu2_0[P] mcu2_1[P] C66X_1[P] C66X_2[P] C7X_1[s]
    \endcode
  - The "P" next to each CPU's log indicates that for example **[C7x_1 ]** was able to talk to **mcu2_0[P] C66X_1[P] C66X_2[P]**
  - You will always see **mpu1_0[x]** so ignore this.
  - If you dont see the "P" then IPC with that CPU has failed for some reason

- You can also run a sample unit level IPC test to confirm linux is able to talk to all CPUs by running below test on the EVM.
  \code
  cd /opt/vision_apps
  ./vx_app_linux_arm_ipc.out
  \endcode
  You should see something as shown in these logs <a href="../logs/vx_app_linux_arm_ipc_log.txt" target="_blank">[TXT]</a>

  - If you see any failures or the application hangs then IPC with the remote core has failed and for some reason the remote core did not initialize as expected.

## How can I confirm the memory carve outs for various CPUs and remote cores are applied correctly by Linux ?

You should see something like below early in the boot. Compare this with your dts/dtsi/dtso file.

\code
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a0000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-dma-memory@a0000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a0100000, size 15 MiB
[    0.000000] OF: reserved mem: initialized node vision_apps-r5f-memory@a0100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a1000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-dma-memory@a1000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a1100000, size 15 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-memory@a1100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a2000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-dma-memory@a2000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a2100000, size 31 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-memory@a2100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a4000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-dma-memory@a4000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a4100000, size 31 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-memory@a4100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a6000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-dma-memory@a6000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a6100000, size 15 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-memory@a6100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a7000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-dma-memory@a7000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a7100000, size 15 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-r5f-memory@a7100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a8000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-c66-dma-memory@a8000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a8100000, size 15 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-c66-memory@a8100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a9000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-c66-dma-memory@a9000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000a9100000, size 15 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-c66-memory@a9100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000aa000000, size 1 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-c71-dma-memory@aa000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000aa100000, size 79 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-c71-memory@aa100000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000b2000000, size 96 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-dma-memory@b2000000, compatible id shared-dma-pool
[    0.000000] OF: reserved mem: initialized node vision_apps_shared-memories, compatible id dma-heap-carveout
[    0.000000] Reserved memory: created DMA memory pool at 0x00000000d8000000, size 576 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-core-heap-memory-lo@d8000000, compatible id shared-dma-pool
[    0.000000] Reserved memory: created DMA memory pool at 0x0000000880000000, size 1024 MiB
[    0.000000] OF: reserved mem: initialized node vision-apps-core-heap-memory-hi@880000000, compatible id shared-dma-pool
\endcode

\if DOCS_J721E

## Why do I see the below error logs ?

If you are using an infotainment daughter card, you may see logs as below.

\code
[   23.912367] ${SOC}-evm-audio sound@0: ASoC: failed to init link CPB pcm3168a DAC: -517
[   23.925730] ${SOC}-evm-audio sound@0: devm_snd_soc_register_card() failed: -517
\endcode

In order to avoid these logs, you can add the k3-${SOC}-common-proc-board-infotainment.dtbo to the "name_overlays"
in the uenv.txt file on the BOOT partition as shown below.

\code
name_overlays=k3-${SOC}-common-proc-board-infotainment.dtbo k3-${SOC}-vision-apps.dtbo
\endcode

\endif

## Where can I find sample logs for different applications ?

When something does not work as expected, sometimes it helps to see sample working logs and compare against the failing system.
Sample logs from a run of vision apps is located here <a href="../logs/" target="_blank">[FOLDER]</a>.

\note All logs of all demos may not be present here.
