
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), A72 ))
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX QNX))

include $(PRELUDE)
TARGET      := vx_target_kernels_srv_gpu
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
CFLAGS      += -DGL_ES
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/include
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/gpu
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/gpu/3dsrv
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/host
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(VISION_APPS_PATH)

ifeq ($(TARGET_OS), LINUX)
IDIRS       += $(LINUX_FS_PATH)/usr/include
IDIRS       += $(LINUX_FS_PATH)/usr/include/drm
CFLAGS      += -DEGL_NO_X11
endif

ifeq ($(TARGET_OS), QNX)
IDIRS       += $(QNX_TARGET)/usr/include
endif

ifeq ($(TARGET_CPU), A72)
DEFS += CORE_A72
endif

include $(FINALE)

endif
endif

ifeq ($(TARGET_CPU),x86_64)

include $(PRELUDE)
TARGET      := vx_target_kernels_srv_gpu
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/include
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/gpu
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/gpu/3dsrv
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/host
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(VISION_APPS_PATH)
CFLAGS      += -DGL_ES -DSTANDALONE

DEFS += _HOST_BUILD _TMS320C6600 TMS320C66X HOST_EMULATION

include $(FINALE)

endif
