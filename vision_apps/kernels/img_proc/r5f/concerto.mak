
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 R5F))

include $(PRELUDE)
TARGET      := vx_target_kernels_img_proc_r5f
TARGETTYPE  := library

CSOURCES    := vx_kernels_img_proc_target.c

ifeq ($(TARGET_CPU), R5F)
  CSOURCES    += vx_img_mosaic_msc_target.c
else
  CSOURCES    += vx_img_mosaic_msc_target_sim.c
  IDIRS       += $(J7_C_MODELS_PATH)/include
  IDIRS       += $(TIOVX_PATH)/kernels_j7/hwa/include
endif

IDIRS       += $(VISION_APPS_PATH)/kernels/img_proc/include
IDIRS       += $(VISION_APPS_PATH)/kernels/img_proc/host
IDIRS       += $(TIADALG_PATH)/include
IDIRS       += $(IVISION_PATH)
IDIRS       += $(TIDL_PATH)/inc
IDIRS       += $(TIOVX_PATH)/kernels/ivision/include
IDIRS       += $(VXLIB_PATH)/packages

ifeq ($(TARGET_CPU), x86_64)
ifeq ($(SOC),am62a)
SKIPBUILD=1
endif
endif

include $(FINALE)

endif
