'''
* Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("ext1", Core.C66)

kernel = Kernel("channel_extract")

kernel.setParameter(Type.IMAGE, Direction.INPUT, ParamState.REQUIRED, "IN", do_map=False, do_unmap=False)
kernel.setParameter(Type.ENUM, Direction.INPUT, ParamState.REQUIRED, "CHANNEL")
kernel.setParameter(Type.IMAGE, Direction.OUTPUT, ParamState.REQUIRED, "OUT")

kernel.setTarget(Target.DSP1)
kernel.setTarget(Target.DSP2)

code.export(kernel)




kernel = Kernel("channel_combine")

kernel.setParameter(Type.IMAGE, Direction.INPUT, ParamState.REQUIRED, "SRC0")
kernel.setParameter(Type.IMAGE, Direction.INPUT, ParamState.REQUIRED, "SRC1")
kernel.setParameter(Type.IMAGE, Direction.INPUT, ParamState.OPTIONAL, "SRC2")
kernel.setParameter(Type.IMAGE, Direction.INPUT, ParamState.OPTIONAL, "SRC3")
kernel.setParameter(Type.IMAGE, Direction.OUTPUT, ParamState.REQUIRED, "DST", do_map_unmap_all_planes=True)

kernel.setTarget(Target.DSP1)
kernel.setTarget(Target.DSP2)

code.export(kernel)
