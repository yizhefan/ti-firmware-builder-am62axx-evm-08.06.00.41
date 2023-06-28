ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72))
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX))

include $(PRELUDE)
TARGET      := vx_app_sfm_fisheye
TARGETTYPE  := exe
CSOURCES    := app_sfm_main.c
CSOURCES    += applib_support.c
CSOURCES    += ./config_data/lens_lut.c

include $(VISION_APPS_PATH)/apps/ptk_demos/concerto_inc.mak

CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-result
CFLAGS += -Wno-maybe-uninitialized

include $(FINALE)

endif #ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX))
endif #ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72))
