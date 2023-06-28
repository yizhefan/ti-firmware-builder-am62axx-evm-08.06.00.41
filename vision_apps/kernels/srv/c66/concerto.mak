ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 C66))


include $(PRELUDE)
TARGET      := vx_target_kernels_srv_c66
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/include
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/host
IDIRS       += $(VISION_APPS_PATH)
IDIRS       += $(VXLIB_PATH)/packages
DEFS += CORE_DSP

ifeq ($(BUILD_BAM),yes)
DEFS += BUILD_BAM
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64))
DEFS += _HOST_BUILD _TMS320C6600 TMS320C66X HOST_EMULATION
IDIRS       += $(VXLIB_PATH)/packages/ti/vxlib/src/common/c6xsim
CFLAGS += -Wno-maybe-uninitialized
endif

include $(FINALE)

endif

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721S2 J784S4))

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C7120))

include $(PRELUDE)
TARGET      := vx_target_kernels_srv_c66
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/include
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/host
IDIRS       += $(VISION_APPS_PATH)
IDIRS       += $(VXLIB_PATH)/packages
DEFS        += CORE_DSP

ifeq ($(BUILD_BAM),yes)
DEFS += BUILD_BAM
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C7120))
DEFS += C6X_MIGRATION _TMS320C6600
endif

include $(FINALE)

endif

endif
