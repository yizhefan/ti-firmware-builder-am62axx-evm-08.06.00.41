#  Build Instructions {#BUILD_INSTRUCTIONS}

[TOC]

# IMPORTANT NOTES
- <b> Make sure additional components and network proxies are setup as mentioned in \ref ENVIRONMENT_SETUP before proceeding to building PSDK RTOS </b>
- <b> Make sure you dont skip any of the steps mentioned below (for the selected configuration )</b>
- ${PSDKR_PATH} refers to the path where Processor SDK RTOS (PSDK RTOS) is installed
- ${PSDKL_PATH} refers to the path where Processor SDK Linux (PSDK Linux) is installed
- All folders like, pdk, tiovx, vision_apps mentioned in the user guide are relative to ${PSDKR_PATH} unless explicitly stated otherwise.
- The build is tested on Ubuntu (x86_64) 18.04 system and may not work on earlier or later Ubuntu systems.
- 20GB of free space is required to install and build PSDK RTOS
- Make sure you have sudo access

# Quick Steps to build vision apps for selected configuration {#QUICK_SETUP_STEPS}

This section details the build steps for each of the following HLOS setup configurations.  Follow the steps
in the appropriate section.

- \ref QUICK_SETUP_STEPS_LINUX_TI_RTOS
- \ref QUICK_SETUP_STEPS_QNX_TI_RTOS
- \ref QUICK_SETUP_STEPS_PC_EMULATION
\if (DOCS_J721E || DOCS_J721S2)
- \ref QUICK_SETUP_STEPS_SAFERTOS
\endif

## Linux+RTOS mode {#QUICK_SETUP_STEPS_LINUX_TI_RTOS}

- Edit file tiovx/build_flags.mak and modify below variables
  \code
  BUILD_EMULATION_MODE=no
  BUILD_TARGET_MODE=yes
  BUILD_LINUX_A72=yes
  PROFILE=release
  \endcode
- Do below to build the source code using pre-built libraries provided in the SDK installer, with "N" being the number of parallel threads
  \code
  cd vision_apps
  make vision_apps -jN
  \endcode
- The RTOS, Linux executable are stored below
  \if DOCS_J721E
  \code
  vision_apps/out/$(TARGET_SOC)/A72/LINUX/$PROFILE
  vision_apps/out/$(TARGET_SOC)/R5F/$(RTOS)/$PROFILE
  vision_apps/out/$(TARGET_SOC)/C66/$(RTOS)/$PROFILE
  vision_apps/out/$(TARGET_SOC)/$(TARGET_C7X)/$(RTOS)/$PROFILE
  \endcode
  \else
  \code
  vision_apps/out/$(TARGET_SOC)/A72/LINUX/$PROFILE
  vision_apps/out/$(TARGET_SOC)/R5F/$(RTOS)/$PROFILE
  vision_apps/out/$(TARGET_SOC)/$(TARGET_C7X)/$(RTOS)/$PROFILE
  \endcode
  \endif


\if DOCS_J721E

### Additional steps required for HS devices in Linux+RTOS mode {#QUICK_SETUP_STEPS_LINUX_TI_RTOS_HS_DEVICES}

- If developing for HS devices, the following binaries will need to be built and installed to the SD card.  Please reference the developer note
  <a href="../../../psdk_rtos/docs/user_guide/developer_notes_psdkla.html">here</a> for details on how to do so.
  \code
  /media/$(USER)/BOOT/tispl.bin
  /media/$(USER)/BOOT/tiboot3.bin
  /media/$(USER)/BOOT/u-boot.img
  /media/$(USER)/BOOT/sysfw.itb
  /media/$(USER)/rootfs/boot/fitImage
  \endcode

\endif

## QNX+RTOS mode {#QUICK_SETUP_STEPS_QNX_TI_RTOS}

- Make sure that you have the QNX SDP 7.1 installed in your PC as mentioned in \ref ENVIRONMENT_SETUP_QNX
- Edit file tiovx/build_flags.mak and modify below variables
  \code
  BUILD_EMULATION_MODE=no
  BUILD_TARGET_MODE=yes
  BUILD_LINUX_A72=no
  BUILD_QNX_A72=yes
  PROFILE=release
  \endcode
- Edit the file vision_apps/vision_apps_tools_path.mak and modify below variables
  \code
  QNX_BASE=<QNX SDP installation path>
  \endcode
- Do below to build the source code using pre-built libraries provided in the SDK installer, with "N" being the number of parallel threads
  - Note: If using SBL as the bootloader, the BUILD_CPU_MCU1_0 should be set to "no" in vision_apps_build_flags.mak.  The boot app will be
          used as the MCU1-0 image in this case.  For more information, please refer to the app note <a href="../../../psdk_rtos/docs/user_guide/developer_notes_mcu1_0_sysfw.html">here</a>.
  \code
  cd vision_apps
  ./make_sdk.sh
  \endcode
- The RTOS, QNX executables are stored below
  \code
  vision_apps/out/$(TARGET_SOC)/A72/QNX/$PROFILE
  vision_apps/out/$(TARGET_SOC)/R5F/$(RTOS)/$PROFILE
  vision_apps/out/$(TARGET_SOC)/C66/$(RTOS)/$PROFILE
  vision_apps/out/$(TARGET_SOC)/$(TARGET_C7X)/$(RTOS)/$PROFILE
  \endcode
\if (DOCS_J721E || DOCS_J721S2)
- Depending on whether you have a GP or HS device and are using SBL build, you will need to build using one of the following commands.
  Note: for J721E, by default, these files are built for J721E SR 1.1, but can be configured for J721E SR 1.0 by referencing \ref MAKEFILE_OPTIONS :
  - <b>Option 1:</b> If building for SBL boot mode for GP devices, do below to build the required boot images
    \code
    cd vision_apps
    make qnx_fs_create
    make sbl_bootimage
    \endcode
  - <b>Option 2:</b> If building for SBL boot mode for HS devices, do below to build the required boot images
    \code
    cd vision_apps
    make qnx_fs_create
    make sbl_bootimage_hs
    \endcode
\endif
\if (DOCS_J721S2 || DOCS_J784S4)
- If building for SBL boot mode, do below to build the required boot images
  \code
  cd vision_apps
  make qnx_fs_create
  make sbl_bootimage
  \endcode
\endif

## PC emulation mode {#QUICK_SETUP_STEPS_PC_EMULATION}

- Edit file tiovx/build_flags.mak and modify below variables
  \code
  BUILD_EMULATION_MODE=yes
  BUILD_TARGET_MODE=no
  \endcode

- Do below to build the source code,
  \code
  cd vision_apps
  ./make_sdk.sh
  \endcode
- The PC executables are stored below
  \code
  vision_apps/out/PC/x86_64/LINUX/$PROFILE
  \endcode

# Quick steps to clean vision apps generated files {#BUILD_CLEAN}

- To do a clean build of vision_apps do below,
  \code
  cd vision_apps
  make vision_apps_scrub
  \endcode

\if (DOCS_J721E || DOCS_J721S2)

## SafeRTOS Support {#QUICK_SETUP_STEPS_SAFERTOS}

- By default, the SDK builds the remote core images using FreeRTOS as the OS.
- For J721E and J721S2, SafeRTOS is also supported as an OS within the SDK.  An add-on package is required
  from WHIS in order to support this.  For more information on how to obtain the SafeRTOS package
  from WHIS as well as for more information on SafeRTOS, please see the PDK SafeRTOS documentation
  <a href="../../../pdk/docs/userguide/jacinto/modules/safertos.html">here</a>.
- Once the SafeRTOS package has been downloaded and installed, the SDK can be built for SafeRTOS.
  In order to do so, edit the file tiovx/build_flags.mak and modify below variables
  \code
  RTOS=SAFERTOS
  \endcode
- A limitation with SafeRTOS vs FreeRTOS is that the API for obtaining the core loading is not
  enabled.  Therefore, the performance statistics feature within the SDK is not supported with
  SafeRTOS.
- A limitation with SafeRTOS vs FreeRTOS is that when QNX is used as the HLOS for the A72,
  SPL+uboot must be used as the bootloader rather than SBL + the boot app.  The boot app does not
  currently support SafeRTOS.
\if DOCS_J721S2
- For J721S2, there is a known context save/restore issue in the C7X SafeRTOS port package resulting in
  issues seen on the C7X when using IPC, resulting in occasional undefined behavior.
\endif
- All other known issues with SafeRTOS + Vision Apps integration can be found in the vision apps
  release notes.

\endif

# Detailed steps to configure and build vision apps {#BUILD_DETAILED}

## Makefile and configuration options {#MAKEFILE_OPTIONS}

- Default makefile flag values are set to build the SDK properly.
  However below flags are recommended to be changed by users to speed up their build time,
  execution speed and/or configure what to build depending on their specific requirements.
- Edit below variables in **tiovx/build_flags.mak**. These flags will affect both tiovx as well as vision_apps

Makefile flag        | Valid values (default value in bold) | Description
---------------------|--------------------------------------|------------
PROFILE              | debug or release or **all**          | "debug" for debug only build, <br> "release" for release only build, <br> "all" for both debug and release builds
BUILD_TARGET_MODE    | **yes** or no                        | "yes" to build for SoC platform. In case you are building only for PC keep this to "no"
BUILD_QNX_A72        | yes or **no**                        | "yes" if you want to build for QNX on A72. In case you are building for RTOS only mode or Linux + RTOS mode, keep this to "no"
BUILD_LINUX_A72      | **yes** or no                        | "yes" if you want to build for ARM Linux on A72. In case you are building for RTOS only mode or QNX + RTOS mode, keep this to "no"
BUILD_EMULATION_MODE | **yes** or no                        | "yes" to build for PC emulation mode. In case you are building only for SoC keep this to "no"
BUILD_CT_*           | **yes** or no                        | "yes" to enable OpenVX conformance tests for specific group of tests (CT). Typically during development one would keep all to "no" and only enable the required tests in respective CT "test_main.h"

- Edit below variables in **vision_apps/vision_apps_build_flags.mak**,

Makefile flag           | Valid values (default value in bold) | Description
------------------------|--------------------------------------|------------
BUILD_CPU_<cpu name>    | **yes** or no                        | "yes" if you want that CPU in your build, else keep it to "no". <br> **Note**, BUILD_CPU_MPU1, BUILD_CPU_MCU2_0 MUST be kept as "yes"<br> **Note**, If building for MCU1_0, please refer to the developer note <a href="../../../psdk_rtos/docs/user_guide/developer_notes_mcu1_0_sysfw.html">here</a>. <br> **Note**, if switching between building with BUILD_CPU_MCU1_0 turned on and off, you will need to re-install the bootfs and targetfs to the SD card as described here \ref run_step1_linux.
BUILD_APP_RTOS_LINUX    | **yes** or no                        | "yes" to build Linux+RTOS mode binaries for selected CPUs. Keep to "no" if you are not interested in Linux+RTOS binaries. <br> By default this is set to BUILD_LINUX_A72
BUILD_APP_RTOS_QNX      | yes or **no**                        | "yes" to build QNX+RTOS mode binaries for selected CPUs. Keep to "no" if you are not interested in QNX+RTOS binaries. <br> By default this is set to BUILD_QNX_A72
\if DOCS_J721E
HS_SR                   | **1_1** or 1_0                       | 1_1 to build SBL boot binaries for J7ES SR 1.1 or 1_0 to build for J7ES SR 1.0
\endif

- Edit **vision_apps/platform/${SOC}/rtos/common/app_cfg*.h** for additional build time configuration,
  <br> **Note**, additional \#define's are defined in these files, but only ones which typical users need to customize are listed below.

File              | \#define                  | Description
------------------|---------------------------|------------
app_cfg.h         | ENABLE_IPC_<cpu name>     | \#undef these to disable a CPU from the system, \#define to enable in the system <BR> **Note**, this must match the BUILD_CPU_<cpu name> vision_apps/vision_apps_build_flags.mak
\if DOCS_J721E
app_cfg_mcu2_0.h  | ENABLE_DSS_HDMI           | \#define this to enable HDMI display via Infotainment daughter card, make sure to \#undef ENABLE_DSS_EDP and ENABLE_DSS_DSI as well; ethernet firmware must also be disabled by following these instructions \ref ETHFW_HOWTO_DISABLE
app_cfg_mcu2_0.h  | ENABLE_DSS_EDP            | \#define this to enable eDP display via base EVM, make sure to \#undef ENABLE_DSS_HDMI and ENABLE_DSS_DSI as well.  Issues have been reported when ethernet firmware is enabled and using certain eDP displays or using an eDP to HDMI adaptor.  If issues are observed with an eDP display, try disabling ethernet firmware.  Instructions can be found here \ref ETHFW_HOWTO_DISABLE
\endif
\if (DOCS_J721S2 || DOCS_J784S4)
app_cfg_mcu2_0.h  | ENABLE_DSS_EDP            | \#define this to enable eDP display via base EVM, make sure to \#undef ENABLE_DSS_DSI as well.
\endif
app_cfg_mcu2_0.h  | ENABLE_DSS_DSI            | \#define this to enable DSI display, make sure to \#undef ENABLE_DSS_EDP and ENABLE_DSS_HDMI as well
\if DOCS_J721E
app_cfg_mcu2_0.h  | ENABLE_DSS                | \#undef this to disable display (both HDMI and eDP), if you don't have display panel or daughter card for display
\endif
\if (DOCS_J721S2 || DOCS_J784S4)
app_cfg_mcu2_0.h  | ENABLE_DSS                | \#undef this to disable display, if you don't have display panel
\endif

## Build vision apps without prebuilt libraries {#BUILD_SOURCE}
- Do below to see values of make configuration variables
  \code
  cd vision_apps
  make sdk_show_config
  \endcode
- Do below to see additional make targets to do a granular SDK build
  \code
  cd vision_apps
  make sdk_help
  \endcode
- Do below to build the full PSDK RTOS, with "N" being the number of parallel threads
  \code
  cd vision_apps
  make sdk -jN
  \endcode
- This command does same as above, except it also traps all build errors and displays them at end of log (since parallel build otherwise may not display all errors at the end).
  \code
  cd vision_apps
  ./make_sdk.sh
  \endcode
- Do below to clean the full PSDK RTOS
  \code
  cd vision_apps
  make sdk_scrub
  \endcode

