ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), A72))
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX))

include $(PRELUDE)
TARGET      := vx_app_estop
TARGETTYPE  := exe

CPPSOURCES  := $(call all-cpp-files)
CPPFLAGS    := --std=c++11

include $(VISION_APPS_PATH)/apps/ptk_demos/concerto_inc.mak

IDIRS       += $(VISION_APPS_KERNELS_IDIRS)
STATIC_LIBS += $(VISION_APPS_KERNELS_LIBS)
STATIC_LIBS += $(TIADALG_LIBS)

SKIPBUILD=1

include $(FINALE)

endif #ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX))
endif #ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72))
