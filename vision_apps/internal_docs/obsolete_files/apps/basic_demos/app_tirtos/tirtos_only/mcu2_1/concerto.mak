ifeq ($(BUILD_APP_TIRTOS),yes)
ifeq ($(BUILD_CPU_MCU2_1),yes)
ifeq ($(TARGET_CPU),R5F)

include $(PRELUDE)

TARGET      := vx_app_tirtos_mcu2_1
TARGETTYPE  := exe
CSOURCES    := $(call all-c-files)

XDC_BLD_FILE = $($(_MODULE)_SDIR)/../../bios_cfg/config_r5f.bld
XDC_IDIRS    = $($(_MODULE)_SDIR)/../../bios_cfg/
XDC_CFG_FILE = $($(_MODULE)_SDIR)/mcu2_1.cfg
XDC_PLATFORM = "ti.platforms.cortexR:J7ES_MAIN"

LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker_mem_map.cmd
LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker.cmd

IDIRS+=$(VISION_APPS_PATH)/apps/basic_demos/app_tirtos/tirtos_only

LDIRS += $(PDK_PATH)/packages/ti/drv/ipc/lib/j721e/mcu2_1/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/csirx/lib/j721e/mcu2_1/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/dss/lib/j721e/mcu2_1/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/udma/lib/j721e/mcu2_1/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/sciclient/lib/j721e/mcu2_1/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/vhwa/lib/j721e/mcu2_1/$(TARGET_BUILD)/

include $($(_MODULE)_SDIR)/../../concerto_r5f_inc.mak

# CPU instance specific libraries
STATIC_LIBS += app_tirtos_common_mcu2_1
STATIC_LIBS += app_utils_hwa
STATIC_LIBS += app_utils_dss
STATIC_LIBS += app_utils_sensors

ADDITIONAL_STATIC_LIBS += csirx.aer5f
ADDITIONAL_STATIC_LIBS += dss.aer5f
ADDITIONAL_STATIC_LIBS += vhwa.aer5f

include $(FINALE)

endif
endif
endif
