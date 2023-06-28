# Copyright (C) 2013 Texas Instruments
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

ifndef CGT6X_ROOT
$(error You must define CGT6X_ROOT!)
endif

# check for the supported CPU types for this compiler
ifeq ($(filter $(TARGET_FAMILY),DSP),)
$(error TARGET_FAMILY $(TARGET_FAMILY) is not supported by this compiler)
endif

# check for the support OS types for this compiler
ifeq ($(filter $(TARGET_OS),SYSBIOS FREERTOS NO_OS LINUX SAFERTOS),)
$(error TARGET_OS $(TARGET_OS) is not supported by this compiler)
endif

ifeq ($(TARGET_OS),LINUX)
$(if $(C6X_UCLINUX_ROOT),,$(error C6X_UCLINUX_ROOT must be defined to support LINUX on C6x targets)
endif

CC=$(CGT6X_ROOT)/bin/cl6x
CP=$(CGT6X_ROOT)/bin/cl6x
AS=$(CGT6X_ROOT)/bin/cl6x
AR=$(CGT6X_ROOT)/bin/ar6x
LD=$(CGT6X_ROOT)/bin/cl6x

ifdef LOGFILE
LOGGING:=&>$(LOGFILE)
else
LOGGING:=
endif

OBJ_EXT=obj
LIB_PRE=
LIB_EXT=lib
ifeq ($(strip $($(_MODULE)_TYPE)),library)
	BIN_PRE=
	BIN_EXT=.$(LIB_EXT)
else ifeq ($(strip $($(_MODULE)_TYPE)),exe)
	BIN_PRE=
	BIN_EXT=.out
endif

$(_MODULE)_BIN  := $(TDIR)/$(BIN_PRE)$($(_MODULE)_TARGET)$(BIN_EXT)

#If linker file does not exist inside the module directory
#then expect it to be generated by some other module in the output target directory
$(_MODULE)_DEPS := $(foreach linkf,$(LINKER_FILES),$(if $(wildcard $(SDIR)/$(linkf)),$(SDIR)/$(linkf),$(TDIR)/$(linkf)))
$(_MODULE)_OBJS := $(ASSEMBLY:%.asm=$(ODIR)/%.obj) $(CPPSOURCES:%.cpp=$(ODIR)/%.obj) $(CSOURCES:%.c=$(ODIR)/%.obj)
# Redefine the local static libs and shared libs with REAL paths and pre/post-fixes
$(_MODULE)_STATIC_LIBS := $(foreach lib,$(STATIC_LIBS),$(TDIR)/$(LIB_PRE)$(lib).$(LIB_EXT))
$(_MODULE)_SHARED_LIBS := $(foreach lib,$(SHARED_LIBS),$(TDIR)/$(LIB_PRE)$(lib).$(LIB_EXT))
$(_MODULE)_COPT := --abi=eabi --gcc
ifeq ($(TARGET_OS),SYSBIOS)
	$(_MODULE)_XDC_TARGET := ti.targets.elf.C66
endif
$(_MODULE)_IDIRS += $(CGT6X_ROOT)/include
$(_MODULE)_LDIRS += $(CGT6X_ROOT)/lib

# Ensure the compilation rules syntactically match the rules generated by the compiler
$(_MODULE)_OBJS := $(call PATH_CONV,$($(_MODULE)_OBJS))

ifeq ($(TARGET_BUILD),debug)
$(_MODULE)_COPT += -g -D_DEBUG_=1
else ifneq ($(filter $(TARGET_BUILD),release production),)
$(_MODULE)_COPT += --opt_level=3 --gen_opt_info=2 -DNDEBUG
endif

ifeq ($(TARGET_CPU),C64T)
	$(_MODULE)_COPT += --silicon_version=tesla
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C64T
	endif
else ifeq ($(TARGET_CPU),C64P)
	$(_MODULE)_COPT += --silicon_version=6400+
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C64P
	endif
else ifeq ($(TARGET_CPU),C64)
	$(_MODULE)_COPT += --silicon_version=6400
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C64
	endif
else ifeq ($(TARGET_CPU),C66)
	$(_MODULE)_COPT += --silicon_version=6600
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C66
	endif
else ifeq ($(TARGET_CPU),C674)
	$(_MODULE)_COPT += --silicon_version=6740
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C674
	endif
else ifeq ($(TARGET_CPU),C67)
	$(_MODULE)_COPT += --silicon_version=6700
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C67
	endif
else ifeq ($(TARGET_CPU),C67P)
	$(_MODULE)_COPT += --silicon_version=6700+
	ifeq ($(TARGET_OS),SYSBIOS)
		$(_MODULE)_COPT += -D=xdc_target_name=C67P
	endif
endif

ifneq ($(MISRA_RULES),) # If module specifies rules,
$(_MODULE)_MISRA := --check_misra=$(MISRA_RULES)
else ifeq ($(CHECK_MISRA),1) # else, check the environment variable
$(_MODULE)_MISRA := --check_misra=required
endif

ifeq ($(KEEP_ASM),1)
$(_MODULE)_COPT += --keep_asm
endif

ifeq ($(TREAT_WARNINGS_AS_ERROR),1)
$(_MODULE)_COPT += --emit_warnings_as_errors
endif

ifeq ($(DEBUG_PIPELINE),1)
$(_MODULE)_COPT += --debug_software_pipeline
endif

ifeq ($(TARGET_OS),LINUX)
$(_MODULE)_COPT += --linux
$(_MODULE)_IDIRS += $(C6X_UCLINUX_ROOT)/libc/usr/include
$(_MODULE)_DEFS += __GNUC_MINOR__=3 __extensions__ __nothrow__ \
	__STDC__="unsigned int" __WINT_TYPE__="unsigned int" \
	__WCHAR_TYPE__="unsigned int" __gnuc_va_list=va_list
endif

ifeq ($(TARGET_OS),SYSBIOS)
	$(_MODULE)_DEFS += xdc_target_name__=C66 xdc_target_types__=ti/targets/elf/std.h
endif
$(_MODULE)_CGT_ROOT = $(CGT6X_ROOT)

$(_MODULE)_MAP      := $($(_MODULE)_BIN).map
$(_MODULE)_INCLUDES := $(foreach inc,$($(_MODULE)_IDIRS),-I="$(basename $(inc))") $(foreach inc,$($(_MODULE)_SYSIDIRS),-I="$(basename $(inc))")
$(_MODULE)_DEFINES  := $(foreach def,$($(_MODULE)_DEFS),-D=$(def))
$(_MODULE)_LIBRARIES:= $(foreach ldir,$($(_MODULE)_LDIRS),--search_path="$(ldir)") $(foreach ldir,$($(_MODULE)_SYSLDIRS),--search_path="$(ldir)") $(foreach lib,$(STATIC_LIBS),--library=$(LIB_PRE)$(lib).$(LIB_EXT)) $(foreach lib,$(ADDITIONAL_STATIC_LIBS),--library=$(lib)) $(foreach lib,$(SYS_STATIC_LIBS),--library=$(LIB_PRE)$(lib).$(LIB_EXT)) $(foreach linkerf,$(LINKER_FILES),--library=$(linkerf))
$(_MODULE)_AFLAGS   := $($(_MODULE)_INCLUDES)
$(_MODULE)_CFLAGS   := $($(_MODULE)_INCLUDES) $($(_MODULE)_DEFINES) $($(_MODULE)_COPT) $(CFLAGS)
$(_MODULE)_LDFLAGS  := $($(_MODULE)_CFLAGS) -z --warn_sections --reread_libs --zero_init=on --rom_model $($(_MODULE)_LINKER_CMD_FILES) -mv7R5 --diag_suppress=10063

###################################################
# COMMANDS
###################################################

$(_MODULE)_LINK_LIB   = $(call PATH_CONV,$(AR) ru2 $($(_MODULE)_BIN) $($(_MODULE)_OBJS) $($(_MODULE)_STATIC_LIBS))
$(_MODULE)_LINK_EXE   = $(call PATH_CONV,$(LD) $($(_MODULE)_LDFLAGS) $($(_MODULE)_OBJS) $($(_MODULE)_LIBRARIES) --output_file=$($(_MODULE)_BIN) --map_file=$($(_MODULE)_MAP))

###################################################
# MACROS FOR COMPILING
###################################################

define $(_MODULE)_BUILD
build:: $($(_MODULE)_BIN)
endef

define $(_MODULE)_COMPILE_TOOLS
$(call PATH_CONV,$(ODIR)$(PATH_SEP)%.$(OBJ_EXT)): $(SDIR)/%.c
	@echo [C6X] Compiling C $$(notdir $$<)
	$(Q)$$(call PATH_CONV,$(CC) $($(_MODULE)_CFLAGS) --preproc_dependency=$(ODIR)/$$*.dep --preproc_with_compile -fr=$$(dir $$@) -fs=$$(dir $$@) -ft=$$(dir $$@) -eo=.$(OBJ_EXT) -fc=$$< $(LOGGING)) $($(_MODULE)_MISRA)

$(call PATH_CONV,$(ODIR)$(PATH_SEP)%.$(OBJ_EXT)): $(SDIR)/%.cpp
	@echo [C6X] Compiling C++ $$(notdir $$<)
	$(Q)$$(call PATH_CONV,$(CP) $($(_MODULE)_CFLAGS) --preproc_dependency=$(ODIR)/$$*.dep --preproc_with_compile -fr=$$(dir $$@) -fs=$$(dir $$@) -ft=$$(dir $$@) -eo=.$(OBJ_EXT) -fp=$$< $(LOGGING))

$(call PATH_CONV,$(ODIR)$(PATH_SEP)%.$(OBJ_EXT)): $(SDIR)/%.asm
	@echo [C6X] Assembling $$(notdir $$<)
	$(Q)$$(call PATH_CONV,$(AS) $($(_MODULE)_AFLAGS) --preproc_dependency=$(ODIR)/$$*.dep --preproc_with_compile -fr=$$(dir $$@) -ft=$$(dir $$@) -eo=.$(OBJ_EXT) -fa=$$< $(LOGGING))

endef
