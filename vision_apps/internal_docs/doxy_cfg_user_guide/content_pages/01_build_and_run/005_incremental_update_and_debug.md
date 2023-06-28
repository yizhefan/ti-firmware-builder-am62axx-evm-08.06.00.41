#  Incremental Update and Debug {#INCREMENTAL_UPDATE_AND_DEBUG}

[TOC]

# Steps for incremental update and debug {#incremental_update_steps}

The following steps are to be done, when you want to incrementally update the code and run the updated binaries.

## Step 1 : Change Code {#incremental_update_change_code}

- Edit/Update the code as needed.

## Step 2 : Recompile & Build the code  {#incremental_update_recompile_build}

- Recompile and build the code after making the changes
  \code
  cd ${PSDKR_PATH}/vision_apps
  ./make_sdk.sh
  \endcode
  \note This builds the SDK in release mode.

  OR
  \code
  cd ${PSDKR_PATH}/vision_apps
  PROFILE=debug ./make_sdk.sh
  \endcode
  \note This builds the SDK in debug mode, in case you want to use gcc (linux) or CCS to debug the programs.

  \note "make_sdk.sh" rebuilds not just the linux/QNX programs but also the remote cores. So if code is changed on those remote cores, make_sdk.sh is needed to rebuild it.

  \warning Make sure that there are no compiler/linker issues, after doing this step.

  \note To run in PC Emulation mode, now you can follow the steps mentioned in \ref build_instruction_sec4
  \note To run on EVM, you can follow the steps given below.

## Step 3a : Copy executable files to SD card (Linux+RTOS mode) {#incremental_update_copy_files}

\note make sure you have already prepared the sd card and copied the test data as mentioned in \ref run_steps_linux

- Insert the SD card onto your host PC SD card slot, (if not already inserted).
- Do below to copy vision apps binaries to SD card
  \code
  cd ${PSDKR_PATH}/vision_apps
  make linux_fs_install_sd
  \endcode
  \note This loads the release versions of all the files

  OR
  \code
  cd ${PSDKR_PATH}/vision_apps
  make linux_fs_install_sd PROFILE=debug
  \endcode
  \note This loads the debug versions of all the files in case you want to use gcc (linux) or CCS to debug the programs

  OR
  \code
  cd ${PSDKR_PATH}/vision_apps
  make linux_fs_install_sd HS=1
  \endcode
  \note This loads the release versions of all the files, and installs the signed firmware binaries as required for an HS platorm
- Eject and remove SD card from PC and insert in EVM

## Step 3a : Copy executable files to SD card (QNX+RTOS mode) {#incremental_update_copy_files_qnx}

\note make sure you have already prepared the sd card and copied the test data as mentioned in \ref run_steps_qnx

- Insert the SD card onto your host PC SD card slot, (if not already inserted).
- Do below to copy vision apps binaries to SD card
  \code
  cd ${PSDKR_PATH}/vision_apps
  make qnx_fs_install_sd
  \endcode
  \note This loads the release versions of all the files

  OR
  \code
  cd ${PSDKR_PATH}/vision_apps
  make qnx_fs_install_sd PROFILE=debug
  \endcode
  \note This loads the debug versions of all the files in case you want to use CCS to debug the programs
- Eject and remove SD card from PC and insert in EVM

## Step 4a: Run on EVM (Linux+RTOS mode) {#incremental_update_run_on_evm_linux}

- To run the updated binaries on EVM , you can follow the steps mentioned in \ref run_step_run_on_evm_linux

## Step 4b: Run on EVM (QNX+RTOS mode) {#incremental_update_run_on_evm_qnx}

- To run the updated binaries on EVM , you can follow the steps mentioned in \ref run_step_run_on_evm_qnx
