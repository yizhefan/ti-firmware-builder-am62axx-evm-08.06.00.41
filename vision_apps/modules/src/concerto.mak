
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72 ))

include $(PRELUDE)
TARGET      := vx_app_modules
TARGETTYPE  := library

ifneq ($(SOC), am62a)
CSOURCES    := app_sensor_module.c
CSOURCES    += app_capture_module.c
CSOURCES    += app_aewb_module.c
CSOURCES    += app_display_module.c
endif

CSOURCES    += app_viss_module.c
CSOURCES    += app_ldc_module.c
CSOURCES    += app_scaler_module.c
CSOURCES    += app_tidl_module.c
CSOURCES    += app_img_mosaic_module.c
CSOURCES    += app_obj_arr_split_module.c

IDIRS       += $(IMAGING_IDIRS)
IDIRS       += $(VISION_APPS_PATH)/kernels/img_proc/include
IDIRS       += $(VISION_APPS_PATH)/kernels/fileio/include
IDIRS       += $(TIDL_PATH)/inc
IDIRS       += $(VISION_APPS_PATH)/modules/include

include $(FINALE)

endif