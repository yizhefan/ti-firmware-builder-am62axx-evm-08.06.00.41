ifeq ($(TARGET_CPU),A72)

include $(PRELUDE)

TARGET      := vx_app_srv_camera
TARGETTYPE  := exe
CSOURCES    := $(call all-c-files)

include $(VISION_APPS_PATH)/apps/concerto_a72_inc.mak

IDIRS += $(IMAGING_IDIRS)
IDIRS += $(VISION_APPS_APPLIBS_IDIRS)
IDIRS += $(VISION_APPS_SRV_IDIRS)

STATIC_LIBS += $(IMAGING_LIBS)
STATIC_LIBS += $(VISION_APPS_SRV_LIBS)

ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX))
CFLAGS      += -DEGL_NO_X11
SYS_SHARED_LIBS += gbm
endif

ifeq ($(TARGET_OS),QNX)
SYS_SHARED_LIBS += screen
endif

SYS_SHARED_LIBS += EGL
SYS_SHARED_LIBS += GLESv2

include $(FINALE)

endif
