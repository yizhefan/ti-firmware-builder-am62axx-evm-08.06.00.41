'''
* Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("hwa", "display", "CUSTOM_KERNEL_PATH", include_filename="j7")

kernel = Kernel("display")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED, "INPUT", ['tivx_display_params_t'])
kernel.setParameter(Type.IMAGE, Direction.INPUT, ParamState.REQUIRED, "INPUT", ['VX_DF_IMAGE_RGB', 'VX_DF_IMAGE_RGBX', 'VX_DF_IMAGE_UYVY','VX_DF_IMAGE_NV12'])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)

