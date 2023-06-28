ifeq ($(BUILD_CPU_MPU1),yes)
ifeq ($(TARGET_CPU),A72)
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), QNX))

include $(PRELUDE)
TARGET      := app_rtos_qnx_mpu1_common
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/qnx
IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos/common
IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(FINALE)

endif
endif
endif
                                                   
