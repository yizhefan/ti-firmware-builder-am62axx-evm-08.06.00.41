ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), A72))
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX QNX))

include $(PRELUDE)

TARGET      := vx_app_tidl_avp4
TARGETTYPE  := exe

CSOURCES    := main.c
CSOURCES    += main_linux_arm.c
CSOURCES    += app_pre_proc_module.c
CSOURCES    += app_post_proc_module.c
CSOURCES    += app_srv_module.c
CSOURCES    += app_img_hist_module.c
CSOURCES    += app_dof_pyramid_module.c
CSOURCES    += app_dof_proc_module.c

ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX))
CFLAGS      += -DEGL_NO_X11
SYS_SHARED_LIBS += gbm
endif

ifeq ($(TARGET_OS),QNX)
SYS_SHARED_LIBS += screen
endif

include $(VISION_APPS_PATH)/apps/concerto_a72_inc.mak

STATIC_LIBS += $(IMAGING_LIBS)
STATIC_LIBS += $(VISION_APPS_KERNELS_LIBS)
STATIC_LIBS += $(VISION_APPS_MODULES_LIBS)
STATIC_LIBS += $(VISION_APPS_SRV_LIBS)

SYS_SHARED_LIBS += EGL
SYS_SHARED_LIBS += GLESv2

IDIRS += $(IMAGING_IDIRS)
IDIRS += $(VISION_APPS_KERNELS_IDIRS)
IDIRS += $(VISION_APPS_MODULES_IDIRS)
IDIRS += $(VISION_APPS_APPLIBS_IDIRS)
IDIRS += $(VISION_APPS_SRV_IDIRS)

include $(FINALE)

endif
endif
