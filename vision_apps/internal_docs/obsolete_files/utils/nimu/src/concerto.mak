ifeq ($(TARGET_PLATFORM),J7)
ifeq ($(TARGET_OS),SYSBIOS)
ifeq ($(TARGET_CPU),A72)

include $(PRELUDE)
TARGET      := app_nimu
TARGETTYPE  := library

CSOURCES    := nimu_osal.c app_nimu.c

DEFS+=SOC_J721E

SKIPBUILD=1

include $(FINALE)

endif
endif
endif
