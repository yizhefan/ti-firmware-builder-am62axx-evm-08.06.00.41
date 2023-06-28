# Copyright (C) 2011 Texas Insruments, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


ifeq ($(BUILD_DEBUG),1)
$(info TI_TOOLS_ROOT=$(TI_TOOLS_ROOT))
$(info TIARMCGT_LLVM_ROOT=$(TIARMCGT_LLVM_ROOT))
endif

# DEP_PROJECTS does not need to be set as the dependencies are contained in the build.

TIOVX_INC  = $(TIOVX_PATH)/include $(TIOVX_PATH)/kernels/include $(TIOVX_PATH)/utils/include
TIOVX_INC += $(TIOVX_CUSTOM_KERNEL_PATH)/include

SYSIDIRS := $(TIOVX_INC)

SYSLDIRS :=
SYSDEFS  :=

SYS_XDC_IDIRS = $(BIOS_PATH)/packages

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J7 J721S2 J784S4 AM62A))
    SYSDEFS +=
    ifeq ($(TARGET_FAMILY),ARM)
        ifeq ($(TARGET_CPU),A72)
            ifeq ($(TARGET_OS),SYSBIOS)
                SYSIDIRS += $(GCC_SYSBIOS_ARM_ROOT)/aarch64-elf/libc/usr/include/
                SYSLDIRS += $(GCC_SYSBIOS_ARM_ROOT)/aarch64-elf/libc/usr/lib/
            else ifeq ($(TARGET_OS),QNX)
                SYSIDIRS += $(PDK_QNX_PATH)/packages
                SYSIDIRS += $(GCC_QNX_ARM_ROOT)/../usr/include
                SYSLDIRS += $(GCC_QNX_ARM_ROOT)/../usr/lib
            else
                SYSIDIRS += $(LINUX_FS_PATH)/usr/include
                SYSLDIRS += $(LINUX_FS_PATH)/usr/lib
            endif
            INSTALL_LIB := /usr/lib
            INSTALL_BIN := /usr/bin
            INSTALL_INC := /usr/include
        else
            SYSIDIRS += $(TIARMCGT_LLVM_ROOT)/include
            SYSLDIRS += $(TIARMCGT_LLVM_ROOT)/lib
        endif
    else ifeq ($(TARGET_FAMILY),DSP)
        ifeq ($(TARGET_CPU),C66)
            SYSIDIRS += $(CGT6X_ROOT)/include
            SYSLDIRS += $(CGT6X_ROOT)/lib
        else
            SYSIDIRS += $(CGT7X_ROOT)/include
            SYSLDIRS += $(CGT7X_ROOT)/lib
        endif
    else ifeq ($(TARGET_FAMILY),EVE)
        SYSIDIRS += $(ARP32CGT_ROOT)/include
        SYSLDIRS += $(ARP32CGT_ROOT)/lib
    endif

    ifeq ($(TARGET_OS), $(filter $(TARGET_OS), FREERTOS SAFERTOS))
        SYSIDIRS += $(PDK_PATH)/packages
    endif

    ifeq ($(TARGET_CPU),C66)
        SYSIDIRS += $(MATHLIB_PATH)/packages
    endif

    SYSIDIRS += $(VXLIB_PATH)/packages
    #This is for utils/mem/include/linux/dma_buf...
    SYSIDIRS += $(VISION_APPS_PATH)
    #These 2 are for kernels/img_proc/include/TI/tivx_img_proc_kernels.h
    SYSIDIRS += $(TIDL_PATH)/inc
    SYSIDIRS += $(IVISION_PATH)
    SYSIDIRS += $(TIADALG_PATH)/include
    #This is for utils/iss/include/app_iss.h
    SYSIDIRS += $(IMAGING_PATH)
endif

ifeq ($(TARGET_PLATFORM),PC)
    SYSDEFS +=

    SYSIDIRS += $(GCC_WINDOWS_ROOT)/include
    SYSLDIRS += $(GCC_WINDOWS_ROOT)/lib
    SYSIDIRS += $(VXLIB_PATH)/packages
    SYSIDIRS += $(TIDL_PATH)/inc
    SYSIDIRS += $(IVISION_PATH)
    SYSIDIRS += $(TIADALG_PATH)/include
    SYSIDIRS += $(VISION_APPS_PATH)
    SYSIDIRS += $(IMAGING_PATH)
endif

