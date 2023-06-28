
ifeq ($(BUILD_APP_TIRTOS),yes)
ifeq ($(BUILD_CPU_C7x_1),yes)
ifeq ($(TARGET_CPU),C71)

include $(PRELUDE)

TARGET      := vx_app_tirtos_c7x_1
TARGETTYPE  := exe
CSOURCES    := $(call all-c-files)

XDC_BLD_FILE = $($(_MODULE)_SDIR)/../../bios_cfg/config_c71.bld
XDC_IDIRS    = $($(_MODULE)_SDIR)/../../bios_cfg/
XDC_CFG_FILE = $($(_MODULE)_SDIR)/c7x_1.cfg
XDC_PLATFORM = "ti.platforms.tms320C7x:J7ES"

IDIRS+=$(VISION_APPS_PATH)/apps/basic_demos/app_tirtos/tirtos_only

LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker_mem_map.cmd
LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker.cmd


include $($(_MODULE)_SDIR)/../../concerto_c7x_inc.mak

# CPU instance specific libraries
STATIC_LIBS += app_tirtos_common_c7x_1

DEFS+=SOC_J721E

include $(FINALE)

endif
endif 
endif
