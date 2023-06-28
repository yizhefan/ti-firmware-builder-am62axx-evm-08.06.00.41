ifneq ($(TARGET_PLATFORM),PC)
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX QNX))
ifeq ($(TARGET_CPU),A72)

include $(PRELUDE)
TARGET      := app_utils_opengl
TARGETTYPE  := library

ifeq ($(TARGET_OS),LINUX)
IDIRS       += $(LINUX_FS_PATH)/usr/include
IDIRS       += $(LINUX_FS_PATH)/usr/include/drm

CSOURCES    := app_gl_egl_utils_linux.c

CFLAGS      += -DEGL_NO_X11

endif

ifeq ($(TARGET_OS),QNX)
IDIRS       += $(QNX_TARGET)/usr/include

CSOURCES    := app_gl_egl_utils_qnx.c
endif

include $(FINALE)

endif
endif
endif

