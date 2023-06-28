#
# Utility makefile to build PDK libaries and related components
#
# Edit this file to suit your specific build needs
#

ifeq ($(PROFILE), $(filter $(PROFILE),release all))
PDK_BUILD_PROFILE_LIST_ALL+=release
endif
ifeq ($(PROFILE), $(filter $(PROFILE),debug all))
PDK_BUILD_PROFILE_LIST_ALL+=debug
endif

# vision_apps does not use A72 SysBIOS
#ifeq ($(BUILD_CPU_MPU1),yes)
#PDK_CORE_LIST_ALL+=mpu1_0
#endif
ifeq ($(BUILD_CPU_MCU1_0),yes)
PDK_CORE_LIST_ALL+=mcu1_0
endif
ifeq ($(BUILD_CPU_MCU1_1),yes)
PDK_CORE_LIST_ALL+=mcu1_1
endif
ifeq ($(BUILD_CPU_MCU2_0),yes)
PDK_CORE_LIST_ALL+=mcu2_0
endif
ifeq ($(BUILD_CPU_MCU2_1),yes)
PDK_CORE_LIST_ALL+=mcu2_1
endif
ifeq ($(BUILD_CPU_MCU3_0),yes)
PDK_CORE_LIST_ALL+=mcu3_0
endif
ifeq ($(BUILD_CPU_MCU3_1),yes)
PDK_CORE_LIST_ALL+=mcu3_1
endif
ifeq ($(BUILD_CPU_MCU4_0),yes)
PDK_CORE_LIST_ALL+=mcu4_0
endif
ifeq ($(BUILD_CPU_MCU4_1),yes)
PDK_CORE_LIST_ALL+=mcu4_1
endif
ifeq ($(BUILD_CPU_C6x_1),yes)
PDK_CORE_LIST_ALL+=c66xdsp_1
endif
ifeq ($(BUILD_CPU_C6x_2),yes)
PDK_CORE_LIST_ALL+=c66xdsp_2
endif
ifeq ($(BUILD_CPU_C7x_1),yes)
PDK_CORE_LIST_ALL+=c7x_1
endif
ifeq ($(BUILD_CPU_C7x_2),yes)
PDK_CORE_LIST_ALL+=c7x_2
endif
ifeq ($(BUILD_CPU_C7x_3),yes)
PDK_CORE_LIST_ALL+=c7x_3
endif
ifeq ($(BUILD_CPU_C7x_4),yes)
PDK_CORE_LIST_ALL+=c7x_4
endif

pdk_build:
	$(MAKE) -C $(PDK_PATH)/packages/ti/build BOARD=$(BUILD_PDK_BOARD) custom_target BUILD_PROFILE_LIST_ALL="$(PDK_BUILD_PROFILE_LIST_ALL)" CORE_LIST_ALL="$(PDK_CORE_LIST_ALL)" BUILD_TARGET_LIST_ALL="$(PDK_BUILD_TARGET_LIST_ALL)" -s

pdk: pdk_emu
ifeq ($(BUILD_TARGET_MODE),yes)
	$(MAKE) pdk_build -s PDK_BUILD_TARGET_LIST_ALL="pdk_libs pdk_app_libs"
endif

pdk_emu:
ifeq ($(SOC), am62a)
ifeq ($(BUILD_EMULATION_MODE),yes)
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl osal_nonos sciclient dmautils SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=release
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl osal_nonos sciclient dmautils SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=debug
endif
else
ifeq ($(BUILD_EMULATION_MODE),yes)
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl osal_nonos sciclient udma dmautils SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=release
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl osal_nonos sciclient udma dmautils SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=debug
endif
endif

pdk_clean:
	$(MAKE) pdk_build PDK_BUILD_TARGET_LIST_ALL="pdk_libs_clean pdk_app_libs_clean"

pdk_emu_clean:
ifeq ($(SOC), am62a)
ifeq ($(BUILD_EMULATION_MODE),yes)
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl_clean osal_nonos_clean sciclient_clean dmautils_clean SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=release
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl_clean osal_nonos_clean sciclient_clean dmautils_clean SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=debug
endif
else
ifeq ($(BUILD_EMULATION_MODE),yes)
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl_clean osal_nonos_clean sciclient_clean udma_clean dmautils_clean SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=release
	$(MAKE) -C $(PDK_PATH)/packages/ti/build csl_clean osal_nonos_clean sciclient_clean udma_clean dmautils_clean SOC=$(SOC) BOARD=$(SOC)_hostemu CORE=c7x-hostemu -s BUILD_PROFILE=debug
endif
endif

pdk_scrub:
	$(MAKE) -C $(PDK_PATH)/packages/ti/build allclean
	rm -rf $(PDK_PATH)/packages/ti/binary
	rm -rf $(PDK_PATH)/packages/ti/boot/sbl/binary

.PHONY: pdk pdk_clean pdk_build
