'''
* Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("hwa", "display_m2m", "CUSTOM_KERNEL_PATH", include_filename="j7")


code.setCoreDirectory("display_m2m")
kernel = Kernel("display_m2m")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "CONFIGURATION", ['tivx_display_m2m_params_t'])
kernel.setParameter(Type.IMAGE,            Direction.INPUT, ParamState.REQUIRED,  "INPUT",         ['VX_DF_IMAGE_RGB', 'VX_DF_IMAGE_RGBX', 'VX_DF_IMAGE_UYVY','VX_DF_IMAGE_NV12'])
kernel.setParameter(Type.IMAGE,            Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT",        ['VX_DF_IMAGE_RGB', 'VX_DF_IMAGE_RGBX', 'VX_DF_IMAGE_UYVY','VX_DF_IMAGE_NV12'])
kernel.setParameterRelationship(["INPUT", "OUTPUT"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


