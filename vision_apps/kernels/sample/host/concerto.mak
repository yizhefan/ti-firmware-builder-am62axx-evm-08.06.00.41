
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), A72))
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX QNX))

include $(PRELUDE)
TARGET      := vx_kernels_sample
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VISION_APPS_PATH)/kernels/sample/include

include $(FINALE)

endif
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64))

include $(PRELUDE)
TARGET      := vx_kernels_sample
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VISION_APPS_PATH)/kernels/sample/include

include $(FINALE)

endif
