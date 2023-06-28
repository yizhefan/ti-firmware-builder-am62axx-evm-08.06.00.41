ifeq ($(TARGET_CPU),C66)

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos/common
IDIRS+=$(VISION_APPS_PATH)/kernels/img_proc/include
IDIRS+=$(VISION_APPS_PATH)/kernels/fileio/include
IDIRS+=$(VISION_APPS_PATH)/kernels/srv/include
IDIRS+=$(VISION_APPS_PATH)/kernels/park_assist/include
IDIRS+=$(PTK_PATH)/include
IDIRS+=$(VISION_APPS_PATH)/kernels/stereo/include
IDIRS+=$(IMAGING_PATH)/kernels/include

ifeq ($(RTOS),SYSBIOS)
	LDIRS += $(PDK_PATH)/packages/ti/osal/lib/tirtos/$(SOC)/c66/$(TARGET_BUILD)/
endif
ifeq ($(RTOS),FREERTOS)
	LDIRS += $(PDK_PATH)/packages/ti/osal/lib/freertos/$(SOC)/c66/$(TARGET_BUILD)/
endif
ifeq ($(RTOS),SAFERTOS)
	LDIRS += $(PDK_PATH)/packages/ti/osal/lib/safertos/$(SOC)/c66/$(TARGET_BUILD)/
endif
LDIRS += $(PDK_PATH)/packages/ti/csl/lib/$(SOC)/c66/$(TARGET_BUILD)/
LDIRS += $(TIOVX_PATH)/lib/$(TARGET_PLATFORM)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS += $(PTK_PATH)/lib/$(TARGET_PLATFORM)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS += $(VXLIB_PATH)/lib/$(TARGET_PLATFORM)/$(TARGET_CPU)/NO_OS/$(TARGET_BUILD)
LDIRS += $(TIADALG_PATH)/lib/$(TARGET_CPU)/$(TARGET_BUILD)
LDIRS += $(MATHLIB_PATH)/packages/ti/mathlib/lib

STATIC_LIBS += app_utils_mem
STATIC_LIBS += app_utils_console_io
STATIC_LIBS += app_utils_ipc
STATIC_LIBS += app_utils_remote_service
STATIC_LIBS += app_utils_udma
STATIC_LIBS += app_utils_sciclient
STATIC_LIBS += app_utils_misc
STATIC_LIBS += app_utils_perf_stats
STATIC_LIBS += vx_target_kernels_img_proc_c66
STATIC_LIBS += vx_kernels_common
STATIC_LIBS += vx_target_kernels_stereo

PTK_LIBS =
PTK_LIBS += ptk_algos
PTK_LIBS += ptk_base

SYS_STATIC_LIBS += $(PTK_LIBS)

TIOVX_LIBS =
TIOVX_LIBS += vx_framework vx_platform_psdk_j7_rtos vx_kernels_target_utils
TIOVX_LIBS += vx_target_kernels_openvx_core
TIOVX_LIBS += vx_target_kernels_tutorial
TIOVX_LIBS += vx_target_kernels_dsp
TIOVX_LIBS += vx_target_kernels_source_sink
TIOVX_LIBS += vx_target_kernels_ivision_common
TIOVX_LIBS += vx_target_kernels_j7_arm

STATIC_LIBS += vx_target_kernels_srv_c66
STATIC_LIBS += vx_target_kernels_stereo

SYS_STATIC_LIBS += $(TIOVX_LIBS)

ADDITIONAL_STATIC_LIBS += vxlib_C66.lib

ADDITIONAL_STATIC_LIBS += ti.osal.ae66
ADDITIONAL_STATIC_LIBS += ipc.ae66
ADDITIONAL_STATIC_LIBS += sciclient.ae66
ADDITIONAL_STATIC_LIBS += udma.ae66
ADDITIONAL_STATIC_LIBS += mathlib.ae66

ifeq ($(RTOS),FREERTOS)
	ADDITIONAL_STATIC_LIBS += ti.kernel.freertos.ae66
endif
ifeq ($(RTOS),SAFERTOS)
	ADDITIONAL_STATIC_LIBS += ti.kernel.safertos.ae66
endif

ADDITIONAL_STATIC_LIBS += ti.csl.ae66

ifeq ($(RTOS), $(filter $(RTOS), FREERTOS SAFERTOS))
	ADDITIONAL_STATIC_LIBS += ti.csl.intc.ae66
endif

ADDITIONAL_STATIC_LIBS += libtiadalg_fisheye_transformation.a
ADDITIONAL_STATIC_LIBS += libtiadalg_image_preprocessing.a
ADDITIONAL_STATIC_LIBS += libtiadalg_dof_plane_seperation.a
ADDITIONAL_STATIC_LIBS += libtiadalg_select_top_feature.a
ADDITIONAL_STATIC_LIBS += libtiadalg_sparse_upsampling.a
ADDITIONAL_STATIC_LIBS += libtiadalg_visual_localization.a
ADDITIONAL_STATIC_LIBS += libtiadalg_solve_pnp.a
ADDITIONAL_STATIC_LIBS += libtiadalg_image_color_blending.a
ADDITIONAL_STATIC_LIBS += libtiadalg_image_recursive_nms.a
ADDITIONAL_STATIC_LIBS += libc.a

SYS_STATIC_LIBS += rts6600_elf

endif
