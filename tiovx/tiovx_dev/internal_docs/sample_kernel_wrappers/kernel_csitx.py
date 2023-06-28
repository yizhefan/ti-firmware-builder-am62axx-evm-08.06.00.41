'''
* Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("hwa", "csitx", "CUSTOM_KERNEL_PATH", include_filename="j7")

kernel = Kernel("csitx")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED, "CONFIGURATION", ['tivx_csitx_params_t'])
kernel.setParameter(Type.OBJECT_ARRAY, Direction.INPUT, ParamState.REQUIRED, "INPUT", ['VX_DF_IMAGE_RGBX', 'VX_DF_IMAGE_UYVY','VX_DF_IMAGE_YUYV','VX_DF_IMAGE_U16'])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)

