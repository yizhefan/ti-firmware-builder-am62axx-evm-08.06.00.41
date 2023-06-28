'''
* Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("hwa", "capture", "CUSTOM_KERNEL_PATH", include_filename="j7")

kernel = Kernel("capture")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED, "INPUT", ['tivx_capture_params_t'])
kernel.setParameter(Type.OBJECT_ARRAY, Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT", ['VX_DF_IMAGE_U8'])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


