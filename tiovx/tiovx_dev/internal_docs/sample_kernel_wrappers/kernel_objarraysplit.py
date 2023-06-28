'''
* Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("hwa", Core.A72, "CUSTOM_KERNEL_PATH", include_filename="j7")
# module name = srv

kernel = Kernel("obj_array_split") # Kernel name

########################################
# Define I/O for the Node

kernel.setParameter(Type.OBJECT_ARRAY,      Direction.INPUT, ParamState.REQUIRED, "IN")

kernel.setParameter(Type.OBJECT_ARRAY,      Direction.OUTPUT, ParamState.REQUIRED, "OUT0")
kernel.setParameter(Type.OBJECT_ARRAY,      Direction.OUTPUT, ParamState.REQUIRED, "OUT1")
kernel.setParameter(Type.OBJECT_ARRAY,      Direction.OUTPUT, ParamState.OPTIONAL, "OUT2")
kernel.setParameter(Type.OBJECT_ARRAY,      Direction.OUTPUT, ParamState.OPTIONAL, "OUT3")

kernel.setTarget(Target.A72_0)

code.export(kernel)
code.exportDiagram(kernel)

