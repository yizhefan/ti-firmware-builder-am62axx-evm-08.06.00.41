DEFS+=CPU_mcu2_0

ifeq ($(RTOS),SYSBIOS)
	XDC_BLD_FILE = $($(_MODULE)_SDIR)/../bios_cfg/config_r5f.bld
	XDC_IDIRS    = $($(_MODULE)_SDIR)/../bios_cfg/;$(BIOS_PATH)/packages/ti/posix/ccs
	XDC_CFG_FILE = $($(_MODULE)_SDIR)/mcu2_0.cfg
	XDC_PLATFORM = "ti.platforms.cortexR:J7ES_MAIN"
	LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker.cmd
endif
ifeq ($(RTOS),FREERTOS)
	CSOURCES += $(SOC)_mpu_cfg.c
	LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/$(SOC)_linker_freertos.cmd
endif

ifeq ($(RTOS),SAFERTOS)
	CSOURCES += $(SOC)_safertos_mpu_cfg.c
	LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/$(SOC)_linker_safertos.cmd
endif

LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker_mem_map.cmd

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

ifeq ($(RTOS),SAFERTOS)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT_r5f)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/config
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/kernel/include_api
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/kernel/include_prv
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_r5f)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_r5f)/$(SAFERTOS_COMPILER_EXT_r5f)
endif

LDIRS += $(PDK_PATH)/packages/ti/drv/ipc/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/udma/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/sciclient/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/

LDIRS += $(PDK_PATH)/packages/ti/drv/csirx/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/csitx/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/dss/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/vhwa/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/

ifeq ($(RTOS),FREERTOS)
    LDIRS += $(PDK_PATH)/packages/ti/kernel/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
endif

ifeq ($(RTOS),SAFERTOS)
	LDIRS += $(PDK_PATH)/packages/ti/kernel/safertos/lib/$(SOC)/mcu2_0/$(TARGET_BUILD)/
endif

include $($(_MODULE)_SDIR)/../concerto_r5f_inc.mak

# CPU instance specific libraries
STATIC_LIBS += app_rtos_common_mcu2_0
ifeq ($(RTOS), $(filter $(RTOS), FREERTOS SAFERTOS))
	STATIC_LIBS += app_rtos
endif
STATIC_LIBS += app_utils_hwa
STATIC_LIBS += app_utils_dss
STATIC_LIBS += app_utils_sciclient

ADDITIONAL_STATIC_LIBS += csirx.aer5f
ADDITIONAL_STATIC_LIBS += csitx.aer5f
ADDITIONAL_STATIC_LIBS += dss.aer5f
ADDITIONAL_STATIC_LIBS += vhwa.aer5f

ADDITIONAL_STATIC_LIBS += pm_lib.aer5f
ADDITIONAL_STATIC_LIBS += sciclient.aer5f

DEFS        += $(RTOS)
