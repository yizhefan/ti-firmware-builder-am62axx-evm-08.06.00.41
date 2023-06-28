ifeq ($(TARGET_CPU),x86_64)

include $(PRELUDE)

TARGET      := vx_app_ldc_dof
TARGETTYPE  := exe

CSOURCES    := $(call all-c-files)

include $(VISION_APPS_PATH)/apps/ptk_demos/concerto_inc.mak

IDIRS += $(PTK_PATH)/include

STATIC_LIBS += $(PTK_LIBS)

include $(FINALE)

endif
