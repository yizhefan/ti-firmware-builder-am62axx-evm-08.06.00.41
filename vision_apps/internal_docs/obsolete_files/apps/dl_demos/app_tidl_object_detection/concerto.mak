ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 A72))

include $(PRELUDE)

TARGET      := vx_app_tidl_object_detection
CSOURCES    := main.c

ifeq ($(HOST_COMPILER),GCC_LINUX)
CFLAGS += -Wno-unused-function
endif

ifeq ($(TARGET_CPU),x86_64)

TARGETTYPE  := exe

CSOURCES    += main_x86.c

include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak

IDIRS       += $(VISION_APPS_KERNELS_IDIRS)

STATIC_LIBS += $(VISION_APPS_KERNELS_LIBS)
STATIC_LIBS += $(TIADALG_LIBS)

endif

ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX QNX))
ifeq ($(TARGET_CPU),A72)

TARGETTYPE  := exe

CSOURCES    += main_linux_arm.c

include $(VISION_APPS_PATH)/apps/concerto_a72_inc.mak

IDIRS       += $(VISION_APPS_KERNELS_IDIRS)

STATIC_LIBS += $(VISION_APPS_KERNELS_LIBS)

SKIPBUILD=1

endif
endif

include $(FINALE)

endif
