
########################################################################

ifeq ($(BUILD_CPU_C6x_1),yes)
ifeq ($(TARGET_CPU),C66)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=c6x_1

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_c6x_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_C6x_2),yes)
ifeq ($(TARGET_CPU),C66)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=c6x_2

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_c6x_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_C7x_1),yes)
ifeq ($(TARGET_CPU),C71)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=c7x_1

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_c7x_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_MCU1_0),yes)
ifeq ($(TARGET_CPU),R5F)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=mcu1_0

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_r5f_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_MCU2_0),yes)
ifeq ($(TARGET_CPU),R5F)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=mcu2_0

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

ifeq ($(BUILD_MCU_BOARD_DEPENDENCIES),yes)
DEFS+=BUILD_MCU_BOARD_DEPENDENCIES
endif

ifeq ($(BUILD_ENABLE_ETHFW),yes)
DEFS+=ENABLE_ETHFW
endif

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_r5f_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_MCU2_1),yes)
ifeq ($(TARGET_CPU),R5F)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=mcu2_1

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_r5f_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_MCU3_0),yes)
ifeq ($(TARGET_CPU),R5F)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=mcu3_0

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_r5f_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_MCU3_1),yes)
ifeq ($(TARGET_CPU),R5F)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=mcu3_1

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_r5f_inc.mak

include $(FINALE)

endif
endif

########################################################################

ifeq ($(BUILD_CPU_MPU1),yes)
ifeq ($(TARGET_CPU),A72)
ifeq ($(TARGET_OS),SYSBIOS)

# CPU_ID must be set before include $(PRELUDE)
CPU_ID=mpu1

_MODULE=$(CPU_ID)
include $(PRELUDE)

TARGET      := app_rtos_common_$(CPU_ID)
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

DEFS+=APP_CFG_FILE=\"app_cfg_$(CPU_ID).h\"
DEFS+=CPU_$(CPU_ID)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

include $(VISION_APPS_PATH)/platform/$(SOC)/rtos/concerto_a72_inc.mak

include $(FINALE)

endif
endif
endif

########################################################################

