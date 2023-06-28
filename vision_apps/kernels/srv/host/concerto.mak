ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72))

include $(PRELUDE)
TARGET      := vx_kernels_srv
TARGETTYPE  := library

CSOURCES    := tivx_srv_node_api.c vx_generate_3dbowl_host.c vx_generate_gpulut_host.c vx_kernels_srv_host.c vx_point_detect_host.c vx_pose_estimation_host.c

ifeq ($(TARGET_CPU), A72)
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX QNX))
CFLAGS      += -DGL_ES
CSOURCES    += vx_gl_srv_host.c

ifeq ($(TARGET_OS), LINUX)
IDIRS       += $(LINUX_FS_PATH)/usr/include
IDIRS       += $(LINUX_FS_PATH)/usr/include/drm
CFLAGS      += -DEGL_NO_X11
endif

ifeq ($(TARGET_OS), QNX)
IDIRS       += $(QNX_TARGET)/usr/include
endif

endif
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64))
CFLAGS      += -DGL_ES -DSTANDALONE
CSOURCES    += vx_gl_srv_host.c
endif

IDIRS       += $(VISION_APPS_PATH)/kernels/srv/include
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(VISION_APPS_PATH)/kernels/srv/gpu/3dsrv

include $(FINALE)

endif
