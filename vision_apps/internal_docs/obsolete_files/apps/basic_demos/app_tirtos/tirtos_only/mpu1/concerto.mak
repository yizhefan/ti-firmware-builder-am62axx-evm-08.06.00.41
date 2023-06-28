ifeq ($(BUILD_APP_TIRTOS),yes)
ifeq ($(BUILD_CPU_MPU1),yes)
ifeq ($(TARGET_CPU),A72)
ifeq ($(TARGET_OS),SYSBIOS)

include $(PRELUDE)

TARGET      := vx_app_tirtos_mpu1
TARGETTYPE  := exe
CSOURCES    := $(call all-c-files)

XDC_BLD_FILE = $($(_MODULE)_SDIR)/../../bios_cfg/config_a72.bld
XDC_IDIRS    = $($(_MODULE)_SDIR)/../../bios_cfg/
XDC_CFG_FILE = $($(_MODULE)_SDIR)/mpu1.cfg
XDC_PLATFORM = "ti.platforms.cortexA:J7ES"

IDIRS+=$(VISION_APPS_PATH)/apps/basic_demos/app_tirtos/tirtos_only

LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker_mem_map.cmd
LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker.cmd

include $($(_MODULE)_SDIR)/../../concerto_a72_inc.mak

LDIRS += $(PDK_PATH)/packages/ti/drv/ipc/lib/j721e/mpu1_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/udma/lib/j721e/mpu1_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/sciclient/lib/j721e/mpu1_0/$(TARGET_BUILD)/
LDIRS += $(PDK_PATH)/packages/ti/drv/udma/lib/j721e/mpu1_0/$(TARGET_BUILD)/



# CPU instance specific libraries
STATIC_LIBS += app_tirtos_common_mpu1


include $(FINALE)

endif
endif
endif
endif
