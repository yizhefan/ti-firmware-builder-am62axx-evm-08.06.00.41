ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 C71 C7120 C7504))

include $(PRELUDE)
TARGET      := vx_app_c7x_target_kernel
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
CPPSOURCES  := $(call all-cpp-files)
ifeq ($(TARGET_CPU), x86_64)
IDIRS       += $(CGT7X_ROOT)/host_emulation/include/C7100
CFLAGS += --std=c++14 -D_HOST_EMULATION -pedantic -fPIC -w -c -g
CFLAGS += -Wno-sign-compare
endif

include $(FINALE)

endif
