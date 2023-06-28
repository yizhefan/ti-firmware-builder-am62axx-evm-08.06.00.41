#  Run Instructions {#RUN_INSTRUCTIONS}

[TOC]

# IMPORTANT NOTES
- <b> Make sure additional components and network proxies are setup as mentioned in \ref ENVIRONMENT_SETUP before proceeding to building PSDK RTOS </b>
- <b> Make sure you dont skip any of the steps mentioned below </b>
- ${PSDKR_PATH} refers to the path where Processor SDK RTOS (PSDK RTOS) is installed
- ${PSDKL_PATH} refers to the path where Processor SDK Linux (PSDK Linux) is installed
- All folders like, pdk, tiovx, vision_apps mentioned in the user guide are relative to ${PSDKR_PATH} unless explicitly stated otherwise.
- The build is tested on Ubuntu (x86_64) 18.04 system and may not work on earlier or later Ubuntu systems.
- 20GB of free space is required to install and build PSDK RTOS
- Make sure you have sudo access
- If a demo is enabled for both target and emulation mode, there will be 2 different configuration files, one for target execution and one for host
  emulation.
  - These can be found in the paths below:
    - Target Exeuction: apps/basic_demos/app_linux_fs_files/\<app_config>.cfg
    - Host Emulation:   apps/basic_demos/\<app_name>/\<app_config>.cfg
  - If one wants to modify the configuration for target execution, there are two options:
    - One can modify the config file on the SD card directly. However the config file will get overwritten with the original one when
      'make linux_fs_install_sd' is called.
    - One can modify the original file in ./apps/basic_demos/app_linux_fs_files/\<app_config>.cfg and then invoke 'make linux_fs_install_sd'

# Run vision apps on EVM in Linux+RTOS mode (via SD card boot) {#run_steps_linux}

\note make sure vision_apps is built for Linux+RTOS mode as mentioned in \ref BUILD_INSTRUCTIONS

## Step 1: Prepare SD card for boot (one time only) {#run_step1_linux}

\warning ALL contents of SD card will be lost when doing these steps.

\note Run below commands on host PC

\if DOCS_J721E
\note There are two versions of the J721e EVM, alpha, beta. The uboot detects the EVM version and chooses the right config for the EVM version.
\endif

- Insert SD card onto your PC SD card slot and format SD card in two partitions,
  - FAT32 partition: for boot loader, 64MB size recommended
  - ext4 partition: for root filesystem, rest of SD card, at least 32GB SD card recommended.

- You can use one of two options to format your SD card
  - <b>Option 1:</b> You can use the utility script **psdk_rtos/scripts/mk-linux-card.sh** to make the partitions
    - Use the command "df -h" to identify the device used by SD card
    - Unmount the SD card before running the script
      - Example, if your SD card is on device /dev/sdb having two partitions, do below to unmount them
        \code
        umount /dev/sdb1
        umount /dev/sdb2
        \endcode
      - Run the script as below to partition it, answer "y" to all questions asked when running the script
        \code
        cd ${PSDKR_PATH}
        sudo psdk_rtos/scripts/mk-linux-card.sh /dev/sdb
        \endcode
        \note Make sure to give the correct device name to this script, example /dev/sda is typically local harddisk,
              if you give this by mistake, your whole hard disk will be wiped off !!!
  - <b>Option 2:</b> you can use "gparted" utility (sudo apt install gparted) to use a GUI based interface to create the partitions.
    - Make sure you set the FAT32 partition flags as "boot", "lba"
    - Name the FAT32 partition as "BOOT" and the ext4 partition as "rootfs"

- After formatting, remove and insert the SD card for the freshly formatted partitions to get auto mounted on the host PC.

- Copy/untar filesystem and boot files to SD card by executing the script as shown below,
  \code
  cd ${PSDKR_PATH}
  psdk_rtos/scripts/install_to_sd_card.sh
  \endcode

- <b>DO NOT REMOVE SD card</b>, goto step 2 to complete rest of SD card installation

## Step 2: Copy test data to SD card (one time only) {#run_step_copy_test_data_linux}

\note Run below commands on host PC

- Untar the demo input files located in the **psdk_rtos_ti_data_set_xx_xx_xx.tar.gz** and **psdk_rtos_ti_data_set_xx_xx_xx_{SOC}.tar.gz** files
  to the SD card at below folder.
  You can find these files at the same location the PSDK RTOS installer is hosted.
  \code
  cd /media/$USER/rootfs/
  mkdir -p opt/vision_apps
  cd opt/vision_apps
  tar --strip-components=1 -xf ${path/to/file}/psdk_rtos_ti_data_set_xx_xx_xx.tar.gz
  tar --strip-components=1 -xf ${path/to/file}/psdk_rtos_ti_data_set_xx_xx_xx_{SOC}.tar.gz
  sync
  \endcode

  \note This test data is required for most of the vision apps demos, so this step is mandatory when running vision apps demos

- After untar you should see a folder "test_data" in folder opt/vision_apps in the SD card. i.e folder in SD card should look like
  \code
  tree -L 1 -d /media/$USER/rootfs/opt/vision_apps/test_data

    /media/$USER/rootfs/opt/vision_apps/test_data
    ├── harriscorners
    ├── output
    ├── psdkra
    └── tivx
  \endcode

\if DOCS_J721E

- <b>DO NOT REMOVE SD card</b>, goto step 3 to complete rest of SD card installation

\endif

## Step 3: Copy executable files to SD card (first time and each time you want to run updated applications) {#run_step_copy_files_linux}

\note Run below commands on host PC

- Insert the formatted SD card onto your host PC SD card slot, (if not already inserted).
- Do below to copy vision apps binaries to SD card
  \code
  cd ${PSDKR_PATH}/vision_apps
  make linux_fs_install_sd
  \endcode
  OR if using an HS device,
  \code
  cd ${PSDKR_PATH}/vision_apps
  make linux_fs_install_sd HS=1
  \endcode
- This also copies a file uEnv.txt to boot partition to select the .dtbo's which is required to run vision apps demos.
- Eject and remove SD card from PC and insert in EVM

## Step 4: Run on EVM {#run_step_run_on_evm_linux}

\if DOCS_J721E
- Setup the EVM as shown on this page <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721e.html">[HTML]</a>
\endif
\if DOCS_J721S2
- Setup the EVM as shown on this page <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721s2.html">[HTML]</a>
\endif
\if DOCS_J784S4
- Setup the EVM as shown on this page <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j784s4.html">[HTML]</a>
\endif
  - Connect UART/USB cable and setup UART terminal
  - Connect daughter card for camera, display as required
  - Connect power supply
  - Connect HDMI and/or DP display
  - Select SD card boot mode on EVM
  - Insert SD card
- Power on the EVM
- First time only: The uboot environment may need to be cleared prior to running with the prebuilt filesystem.  The uboot environment can be cleared by first pressing enter to stop the boot process at uboot and come to uboot prompt.  The below commands can be entered to clear the uboot environment to default and save the changes.
  \code
  env default –a –f
  saveenv
  \endcode
- Power cycle the EVM
- You should see bootloader prints on the UART terminal and then bootloader will boot linux kernel and you should see login prompt as below
  \code
  ${SOC}-evm login:
  \endcode
- On the EVM, Login using below user id, no password
  \code
  root
  \endcode
- On the EVM, do below steps to init the environment for the demos
  \code
  cd /opt/vision_apps
  source ./vision_apps_init.sh
  \endcode
- Now run one of the below scripts/applications to run the demos
  \code
  ./run_app_tidl.sh                     - Image classification demo (needs display)
  ./run_app_tidl_avp2.sh                - Auto valet parking demo (needs display)
  ./run_app_dof.sh                      - Dense optical flow demo (needs display)
  ./run_app_stereo.sh                   - Stereo disparity demo (needs display)
  ./run_app_c7x.sh                      - C7x sample kernel demo
  ./run_app_srv.sh                      - 3D SRV 4 camera demo (needs display, Fusion1 board, 4x IMX390 camera)
  ./run_app_single_cam.sh               - Single camera + VISS + Display demo (needs display, Fusion1 board, 1x IMX390 or compatible camera's)
  ./run_app_multi_cam.sh                - Multi (4x) camera + VISS + Display demo (needs display, Fusion1 board, 4x IMX390 or compatible camera's)
  ./vx_app_arm_opengl_mosaic.out        - OpenGL + OpenVX test
  ./vx_app_linux_arm_ipc.out            - inter processor communication test
  ./vx_app_linux_arm_mem.out            - memory allocation test
  ./vx_app_tutorial.out                 - TI OpenVX tutorial
  ./vx_app_conformance.out              - TI OpenVX conformance tests
  \endcode
- Once the demo runs, you should see output on the display
  - Display can be eDP or HDMI as selected via \ref MAKEFILE_OPTIONS
- Type 'x' to exit the demo and run another demo

## Sample logs

- Sample logs during EVM boot can be found here <a href="../logs/boot_log.txt" target="_blank">[TXT]</a>
- Sample logs during vision_apps_init.sh can be found here <a href="../logs/vision_app_init_log.txt" target="_blank">[TXT]</a>

# Run vision apps on EVM in Linux+RTOS mode (via NFS boot - For Debug only) {#run_steps_linux_nfs}

\note make sure vision_apps is built for Linux+RTOS mode as mentioned in \ref BUILD_INSTRUCTIONS
\note make sure that you have sd card set up at least once as mentioned above

- Setup NFS according PSDK Linux documentation
  - This is done by executing the ./setup.sh script in the folder "ti-processor-sdk-linux-<board-name>-<ver-num>"
    - Use all default options with two possible exceptions.
      - If you are connected to a VPN, setup.sh will sometimes detect two IP addresses and present both at once as the default option. Be sure to only enter the local IP address (likely begins with 192.168.x.x)
      - The serial port that you would like to use is usually not /dev/ttyS0, choose /dev/ttyUSB0 if only one EVM is connected to the host Linux machine.
    - Exit the script without connecting to the EVM (it will try to auto-connect at the end of the script)
  - After running setup.sh, the following should be on your host Linux machine:
    1. A target folder named "ti-processor-sdk-linux-<board-name>-<ver-num>/targetNFS" (or something similar if you changed the file path in ./setup.sh) populated with the root filesystem.
    2. The folder above listed in /etc/exports
    3. A folder named /tftpboot populated with kernels and dtb files.
    4. A file in "ti-processor-sdk-linux-<board-name>-<ver-num>/bin/setupBoard.minicom"

- Insert SD Card (for NFS, the boot partition in SD card is still required)
  - Do not power on EVM yet.

- Run the command from "ti-processor-sdk-linux-<board-name>-<ver-num>"
  \code
      sudo minicom -S ./bin/setupBoard.minicom
  \endcode

- Power on the EVM
  - The minicom script will populate u-boot with new environment variables
  - Once the script begins to load the kernel, interrupt the process with Ctrl-C
    - Note: it is okay if the kernel finishes loading - you can also just power cycle the board and stop the boot process at the u-boot prompt by pressing any key. Previously declared u-boot parameters have been saved with "saveenv" command.

- Once you have a u-boot terminal again ( "=>" ), enter the following commands:
  - Note: the first command is the only one which requires manually input data (Linux host machine IP address). All of the following commands can be copy-pasted directly into the u-boot prompt.
\if DOCS_J721E
   \code
      setenv serverip_to_set <your_host_ip_address>
      setenv serverip ${serverip_to_set}
      setenv init_net 'run args_all args_net; setenv autoload no;dhcp; setenv serverip ${serverip_to_set};rproc init; run boot_rprocs_mmc; rproc list;'
      setenv rproc_fw_binaries '2 /lib/firmware/j7-main-r5f0_0-fw 3 /lib/firmware/j7-main-r5f0_1-fw 6 /lib/firmware/j7-c66_0-fw 7 /lib/firmware/j7-c66_1-fw 8 /lib/firmware/j7-c71_0-fw'
      setenv rproc_load_and_boot_one 'if nfs $loadaddr ${serverip}:${nfs_root}/${rproc_fw};then if rproc load ${rproc_id} ${loadaddr} ${filesize}; then rproc start ${rproc_id};fi;fi'
      setenv overlay_files 'k3-j721e-vision-apps.dtbo'
      setenv overlayaddr ${dtboaddr}
      setenv args_net 'setenv bootargs console=${console} ${optargs} rootfstype=nfs root=/dev/nfs rw nfsroot=${serverip}:${nfs_root},${nfs_options} ip=dhcp'
      setenv setup_net 'setenv autoload no; dhcp; setenv serverip ${serverip_to_set}'
      saveenv
      boot
  \endcode
\endif
\if DOCS_J721S2
   \code
      setenv serverip_to_set <your_host_ip_address>
      setenv serverip ${serverip_to_set}
      setenv init_net 'run args_all args_net; setenv autoload no;dhcp; setenv serverip ${serverip_to_set};rproc init; run boot_rprocs_mmc; rproc list;'
      setenv rproc_fw_binaries '2 /lib/firmware/j721s2-main-r5f0_0-fw 3 /lib/firmware/j721s2-main-r5f0_1-fw 6 /lib/firmware/j721s2-c71_0-fw 7 /lib/firmware/j721s2-c71_1-fw'
      setenv rproc_load_and_boot_one 'if nfs $loadaddr ${serverip}:${nfs_root}/${rproc_fw};then if rproc load ${rproc_id} ${loadaddr} ${filesize}; then rproc start ${rproc_id};fi;fi'
      setenv overlay_files 'k3-j721s2-vision-apps.dtbo'
      setenv overlayaddr ${dtboaddr}
      setenv args_net 'setenv bootargs console=${console} ${optargs} rootfstype=nfs root=/dev/nfs rw nfsroot=${serverip}:${nfs_root},${nfs_options} ip=dhcp'
      setenv setup_net 'setenv autoload no; dhcp; setenv serverip ${serverip_to_set}'
      saveenv
      boot
  \endcode
\endif
\if DOCS_J784S4
   \code
      setenv serverip_to_set <your_host_ip_address>
      setenv serverip ${serverip_to_set}
      setenv init_net 'run args_all args_net; setenv autoload no;dhcp; setenv serverip ${serverip_to_set};rproc init; run boot_rprocs_mmc; rproc list;'
      setenv rproc_fw_binaries '2 /lib/firmware/j784s4-main-r5f0_0-fw 3 /lib/firmware/j784s4-main-r5f0_1-fw 6 /lib/firmware/j784s4-c71_0-fw 7 /lib/firmware/j784s4-c71_1-fw'
      setenv rproc_load_and_boot_one 'if nfs $loadaddr ${serverip}:${nfs_root}/${rproc_fw};then if rproc load ${rproc_id} ${loadaddr} ${filesize}; then rproc start ${rproc_id};fi;fi'
      setenv overlay_files 'k3-j784s4-vision-apps.dtbo'
      setenv overlayaddr ${dtboaddr}
      setenv args_net 'setenv bootargs console=${console} ${optargs} rootfstype=nfs root=/dev/nfs rw nfsroot=${serverip}:${nfs_root},${nfs_options} ip=dhcp'
      setenv setup_net 'setenv autoload no; dhcp; setenv serverip ${serverip_to_set}'
      saveenv
      boot
  \endcode
\endif

- To change between nfs and mmc boot:

    - For NFS boot:
    \code
        setenv rproc_load_and_boot_one 'if nfs $loadaddr ${serverip}:${rootpath}${rproc_fw};then if rproc load ${rproc_id} ${loadaddr} ${filesize}; then rproc start ${rproc_id};fi;fi'
        setenv boot nfs
    \endcode

    - For MMC boot:
    \code
        setenv rproc_load_and_boot_one 'if load mmc ${bootpart} $loadaddr ${rproc_fw}; then if rproc load ${rproc_id} ${loadaddr} ${filesize}; then rproc start ${rproc_id};fi;fi'
        setenv boot mmc
    \endcode

# Run vision apps on EVM in QNX+RTOS mode (via SD card boot) {#run_steps_qnx}

\note make sure vision_apps is built for QNX+RTOS mode as mentioned in \ref BUILD_INSTRUCTIONS

## Step 1: Prepare SD card for boot (one time only) {#run_step1_qnx}

\warning ALL contents of SD card will be lost when doing these steps.

\note Run below commands on host PC
\note There are two versions of the J721e EVM, alpha, beta. The uboot detects the EVM version and chooses the right config for the EVM version.

- Insert SD card onto your PC SD card slot and format SD card in two partitions,
  - ext4 partition: for remote core firmwares, 128MB size recommended
  - FAT32 partition: for boot files, QNX filesystem and applications, rest of SD card, at least 32GB SD card recommended.

- You can use one of two options to format your SD card
  - <b>Option 1:</b> You can use the utility script **psdk_rtos/scripts/mk-qnx-card.sh** to make the partitions
    - Use the command "df -h" to identify the device used by SD card
    - Unmount the SD card before running the script
      - Example, if your SD card is on device /dev/sdb having two partitions, do below to unmount them
        \code
        umount /dev/sdb1
        umount /dev/sdb2
        \endcode
      - Run the script as below to partition it, answer "y" to all questions asked when running the script
        \code
        cd ${PSDKR_PATH}
        sudo psdk_rtos/scripts/mk-qnx-card.sh --device /dev/sdb
        \endcode
        \note Make sure to give the correct device name to this script, example /dev/sda is typically local harddisk,
              if you give this by mistake, your whole hard disk will be wiped off !!!
  - <b>Option 2:</b> you can use "gparted" utility (sudo apt install gparted) to use a GUI based interface to create the partitions.
    - Make sure you set the FAT32 partition flags as "boot", "lba"
    - Name the FAT32 partition as "boot" and the ext4 partition as "rootfs"

- After formatting, remove and insert the SD card for the freshly formatted partitions to get auto mounted on the host PC.

- Copy the QNX filesystem and binaries to SD card by executing the command in vision_apps folder,
  \code
  make qnx_fs_create_sd
  \endcode

  \note Above command builds the QNX BSP+filesystem and copies it to the SD card

- <b>DO NOT REMOVE SD card</b>, goto step 2 to complete rest of SD card installation

## Step 2a: Copy executable files to SD card for boot using SPL+Uboot(first time and each time you want to run updated applications) {#run_step_copy_files_qnx_a}

\note Run below commands on host PC

- Insert the formatted SD card onto your host PC SD card slot, (if not already inserted).
- Do below to copy the binaries to SD card
  \code
  cd ${PSDKR_PATH}/vision_apps
  make qnx_fs_install_sd
  \endcode
- <b>DO NOT REMOVE SD card</b>, goto step 3 to complete rest of SD card installation

## Step 2b: Copy executable files to SD card for boot using MMCSD+SBL(first time and each time you want to run updated applications) {#run_step_copy_files_qnx_b}

\note Run below commands on host PC

- Ensure all steps have been followed from the page \ref QUICK_SETUP_STEPS_QNX_TI_RTOS.
- Insert the formatted SD card onto your host PC SD card slot, (if not already inserted).
- Depending on whether you have a GP or HS device, you will need to use one of the following commands to copy to the SD card:
  - <b>Option 1:</b> If using a GP device, do below to copy the binaries to SD card
    \code
    cd ${PSDKR_PATH}/vision_apps
    make qnx_fs_install_sd_sbl
    \endcode
  - <b>Option 2:</b> If using an HS device, do below to copy the binaries to SD card
    \code
    cd ${PSDKR_PATH}/vision_apps
    make qnx_fs_install_sd_sbl_hs
    \endcode
- <b>DO NOT REMOVE SD card</b>, goto step 3 to complete rest of SD card installation

## Step 2c: Copy executable files to SD card for boot using OSPI+SBL(first time and each time you want to run updated applications) {#run_step_copy_files_qnx_c}

\note Run below commands on host PC

- Ensure all steps have been followed from the page \ref QUICK_SETUP_STEPS_QNX_TI_RTOS.
- Insert the formatted SD card onto your host PC SD card slot, (if not already inserted). This is for copying the vision apps binaries only.
- To flash boot binaries to the OSPI flash, you need to have Uniflash flashprogrammer installed in the host PC. If you don't have this, install it from <a href="http://www.ti.com/tool/UNIFLASH">[UNIFLASH]</a>
- Make sure that the MCU UART is connected to the host PC. Note the port name of the second COM PORT of the MCU UART. You can find this using "dmesg | grep ttyUSB" in linux.
  For example let's assume it was '/dev/ttyUSB1'
- Edit file vision_apps/makerules/makefile_sbl,mk for below variables based on your system environment
  \code
  UNIFLASH_DIR=${HOME}/ti/$(UNIFLASH_VERSION)  # this is the default install dir
  UNIFLASH_COM_PORT=/dev/ttyUSB1 # this must be the second MCU_UART port on the EVM
  \endcode
\if DOCS_J721E
- Ensure your EVM is in UART boot mode.  Please refer to the following link for how to set the EVM to UART boot mode <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721e.html">[HTML]</a>.
\endif
\if DOCS_J721S2
- Ensure your EVM is in UART boot mode.  Please refer to the following link for how to set the EVM to UART boot mode <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721s2.html">[HTML]</a>.
\endif
\if DOCS_J784S4
- Ensure your EVM is in UART boot mode.  Please refer to the following link for how to set the EVM to UART boot mode <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j784s4.html">[HTML]</a>.
\endif
- Flash boot files, vision_apps binaries for R5F, C6x, C7x and QNX kernel, rootfs to OSPI flash. This also copies QNX A72 applications to SD card.
  \note Make sure EVM is in UART boot mode and make sure MCU UART terminal is not open since UNIFLASH needs to use this UART device
  \code
  cd ${PSDKR_PATH}/vision_apps
  make qnx_fs_install_ospi
  \endcode
- Connect the UART cable back to the standard UART connection from the MCU UART connection
\if DOCS_J721E
- Modify the EVM switch settings to OSPI boot mode.  Please refer to the following link for how to set the EVM to OSPI boot mode <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721e.html">[HTML]</a>.
\endif
\if DOCS_J721S2
- Modify the EVM switch settings to OSPI boot mode.  Please refer to the following link for how to set the EVM to OSPI boot mode <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721s2.html">[HTML]</a>.
\endif
\if DOCS_J784S4
- Modify the EVM switch settings to OSPI boot mode.  Please refer to the following link for how to set the EVM to OSPI boot mode <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j784s4.html">[HTML]</a>.
\endif
- <b>DO NOT REMOVE SD card</b>, goto step 3 to complete rest of SD card installation

## Step 3: Copy test data to SD card (one time only) {#run_step_copy_test_data_qnx}

\note Run below commands on host PC

- Untar the demo input files located in the **psdk_rtos_ti_data_set_xx_xx_xx.tar.gz** and **psdk_rtos_ti_data_set_xx_xx_xx_{SOC}.tar.gz**
  files to the SD card at below folder.
  You can find this file at the same location the PSDK RTOS installer is hosted.
  \code
  cd /media/$USER/qnxfs/
  mkdir -p vision_apps
  cd vision_apps
  tar --strip-components=1 -xf ${path/to/file}/psdk_rtos_ti_data_set_xx_xx_xx.tar.gz
  tar --strip-components=1 -xf ${path/to/file}/psdk_rtos_ti_data_set_xx_xx_xx_{SOC}.tar.gz
  sync
  \endcode

  \note This test data is required for most of the vision apps demos, so this step is mandatory when running vision apps demos

- After untar you should see a folder "test_data" in folder vision_apps in the SD card. i.e folder in SD card should look like
  \code
  tree -L 1 -d /media/$USER/qnxfs/vision_apps/test_data

    /media/$USER/qnxfs/vision_apps/test_data
    ├── harriscorners
    ├── output
    ├── psdkra
    └── tivx
  \endcode

## Step 4: Run on EVM {#run_step_run_on_evm_qnx}

\if DOCS_J721E
- Setup the EVM as shown on this page <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721e.html">[HTML]</a>
\endif
\if DOCS_J721S2
- Setup the EVM as shown on this page <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j721s2.html">[HTML]</a>
\endif
\if DOCS_J784S4
- Setup the EVM as shown on this page <a href="../../../psdk_rtos/docs/user_guide/evm_setup_j784s4.html">[HTML]</a>
\endif
  - Connect UART/USB cable and setup UART terminal
  - Connect daughter card for camera, display as required
  - Connect power supply
  - Connect HDMI and/or DP display
  - If you're using SPL+Uboot or SBL-MMCSD boot, select SD card boot mode on EVM. If you're using the SBL-OSPI boot, select the OSPI boot mode on the EVM.
  - Insert SD card
- Power on the EVM
- You should see bootloader prints on the UART terminal. This can be the MCU UART terminal if the boot is MMCSD or OSPI based, or the MAIN UART Terminal if SPL+Uboot based.
- The bootloader will boot QNX kernel and you should see QNX prompt in the first port of MAIN UART as below
  \code
  J7EVM@QNX#
  \endcode
- On the EVM, do below steps to init the environment for the demos
  \code
  cd /ti_fs/vision_apps
  . ./vision_apps_init.sh (source)
  \endcode
- Now run one of the below scripts/applications to run the demos
  \code
  ./run_app_tidl.sh                     - Image classification demo (needs display)
  ./run_app_tidl_avp2.sh                - Auto valet parking demo (needs display)
  ./run_app_dof.sh                      - Dense optical flow demo (needs display)
  ./run_app_c7x.sh                      - C7x sample kernel demo
  ./run_app_srv.sh                      - 3D SRV 4 camera demo (needs display, Fusion1 board, 4x IMX390 camera)
  ./run_app_single_cam.sh               - Single camera + VISS + Display demo (needs display, Fusion1 board, 1x IMX390 or compatible camera's)
  ./run_app_multi_cam.sh                - Multi (4x) camera + VISS + Display demo (needs display, Fusion1 board, 4x IMX390 or compatible camera's)
  ./vx_app_arm_opengl_mosaic.out        - OpenGL + OpenVX test
  ./vx_app_linux_arm_ipc.out            - inter processor communication test
  ./vx_app_linux_arm_mem.out            - memory allocation test
  ./vx_app_tutorial.out                 - TI OpenVX tutorial
  ./vx_app_conformance.out              - TI OpenVX conformance tests
  \endcode
- Once the demo runs, you should see output on the display
  - Display can be eDP or HDMI as selected via \ref MAKEFILE_OPTIONS
- Type 'x' to exit the demo and run another demo

# Run vision apps on PC in PC emulation mode {#run_in_pc_mode}

## Running the TIOVX demos in PC emulation mode {#build_instruction_sec3}
- Set environment variable below to specify the path of the test data. This is used by both
  the conformance test and tutorial executables,
  \code
  export VX_TEST_DATA_PATH=../../../../../conformance_tests/test_data
  \endcode
- Do below execute the TIOVX tutorials
  \code
  cd tiovx/out/PC/x86_64/LINUX/$PROFILE
  ./vx_tutorial_exe
  \endcode
- Do below execute the TIOVX conformance tests
  \code
  cd tiovx/out/PC/x86_64/LINUX/$PROFILE
  ./vx_conformance_tests_exe
  \endcode

\note Output .bmp/.png files generated by the tutorial and conformance tests should be present in the folder **tiovx/conformance_tests/test_data**

## Running the vision apps demos in PC emulation mode {#build_instruction_sec4}

- Most of the demos take a config (.cfg) file as input which is used to specify various inputs and outputs to the demos
  - A sample .cfg file is provide in each respective demo folder (**vision_apps/apps/[demo type]_demos/[demo name]/config**)
- TI provides reference test input for the demos
  - Untar the file **psdk_rtos_ti_data_set_xx_xx_xx.tar.gz** to the folder "/ti/j7/workarea"
  - Untar the file **psdk_rtos_ti_data_set_xx_xx_xx_{SOC}.tar.gz** to the folder "/ti/j7/workarea"
  - If test data is NOT installed at "/ti/j7/workarea", then modify the demo .cfg files to point to the input data
- To run the demo copy the demo .cfg files to vision_apps/out/PC/x86_64/LINUX/$PROFILE
- Run the specific demo by using below command
  \code
  cd vision_apps/out/PC/x86_64/LINUX/$PROFILE
  <demo executable name> --cfg <cfg file>
  \endcode
- Refer to detailed demo pages for details on running each individual demo

