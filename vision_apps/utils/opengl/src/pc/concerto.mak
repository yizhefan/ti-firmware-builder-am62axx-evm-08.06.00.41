ifeq ($(TARGET_CPU),x86_64)

include $(PRELUDE)
TARGET      := app_utils_opengl
TARGETTYPE  := library
IDIRS       += $(VISION_APPS_PATH)
IDIRS       += $(GLM_PATH)/
CPPFLAGS    += --std=c++14 -D_HOST_EMULATION -pedantic -fPIC -w -c -g
CPPFLAGS    += -Wno-sign-compare
DEFS        += USE_X11

CPPSOURCES  := app_gl_egl_utils_pc.cpp

include $(FINALE)

endif

