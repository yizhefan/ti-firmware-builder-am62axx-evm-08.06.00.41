ifeq ($(TARGET_CPU),x86_64)


include $(PRELUDE)

TARGET      := vx_app_ldc
TARGETTYPE  := exe

CSOURCES    := $(call all-c-files)

include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak

include $(FINALE)

endif
