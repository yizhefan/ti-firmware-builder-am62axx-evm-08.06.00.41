'''
* Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode("hwa", "vpac_nf", "CUSTOM_KERNEL_PATH", include_filename="j7")


code.setCoreDirectory("vpac_nf")
kernel = Kernel("vpac_nf_generic")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "CONFIGURATION", ['tivx_vpac_nf_common_params_t'])
kernel.setParameter(Type.IMAGE,            Direction.INPUT, ParamState.REQUIRED,  "INPUT",         ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.CONVOLUTION,      Direction.INPUT, ParamState.REQUIRED,  "CONV")
kernel.setParameter(Type.IMAGE,            Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT",        ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameterRelationship(["INPUT", "OUTPUT"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


kernel = Kernel("vpac_nf_bilateral")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "CONFIGURATION", ['tivx_vpac_nf_bilateral_params_t'])
kernel.setParameter(Type.IMAGE,            Direction.INPUT, ParamState.REQUIRED,  "INPUT",         ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "SIGMAS",        ['tivx_vpac_nf_bilateral_sigmas_t'])
kernel.setParameter(Type.IMAGE,            Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT",        ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameterRelationship(["INPUT", "OUTPUT"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


code.setCoreDirectory("dmpac_sde")
kernel = Kernel("dmpac_sde")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED, "CONFIGURATION",        ['tivx_dmpac_sde_params_t'])
kernel.setParameter(Type.IMAGE,            Direction.INPUT, ParamState.REQUIRED, "LEFT",                 ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,            Direction.INPUT, ParamState.REQUIRED, "RIGHT",                ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,            Direction.OUTPUT, ParamState.REQUIRED,"OUTPUT",               ['VX_DF_IMAGE_S16'])
kernel.setParameter(Type.DISTRIBUTION, Direction.OUTPUT, ParamState.OPTIONAL,    "CONFIDENCE_HISTOGRAM")
kernel.setParameterRelationship(["LEFT", "RIGHT", "OUTPUT"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])
kernel.setParameterRelationship(["LEFT", "RIGHT"], [Attribute.Image.FORMAT])
kernel.setTarget(Target.IPU1_0)
code.export(kernel)


code.setCoreDirectory("vpac_ldc")
kernel = Kernel("vpac_ldc")

kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION",     ['tivx_vpac_ldc_params_t'])
kernel.setParameter(Type.MATRIX,            Direction.INPUT,  ParamState.OPTIONAL, "WARP_MATRIX",       ['VX_TYPE_INT16', 'VX_TYPE_FLOAT32'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.OPTIONAL, "REGION_PRMS",       ['tivx_vpac_ldc_region_params_t', 'tivx_vpac_ldc_multi_region_params_t'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.OPTIONAL, "MESH_PRMS",         ['tivx_vpac_ldc_mesh_params_t'])
kernel.setParameter(Type.IMAGE,             Direction.INPUT,  ParamState.OPTIONAL, "MESH_IMG",          ['VX_DF_IMAGE_U32'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.OPTIONAL, "DCC_DB",            ['dcc_ldc'])
kernel.setParameter(Type.IMAGE,             Direction.INPUT,  ParamState.REQUIRED, "IN_IMG",            ['VX_DF_IMAGE_NV12', 'TIVX_DF_IMAGE_NV12_P12', 'VX_DF_IMAGE_UYVY', 'VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.REQUIRED, "OUT0_IMG",          ['VX_DF_IMAGE_NV12', 'TIVX_DF_IMAGE_NV12_P12', 'VX_DF_IMAGE_UYVY', 'VX_DF_IMAGE_YUYV', 'VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT1_IMG",          ['VX_DF_IMAGE_NV12', 'VX_DF_IMAGE_UYVY', 'VX_DF_IMAGE_YUYV', 'VX_DF_IMAGE_U8'])

kernel.setParameterRelationship(["OUT0_IMG", "OUT1_IMG"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


'''
code.setCoreDirectory("vpac_msc")
kernel = Kernel("vpac_msc")

kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "IN_CONFIG", ['tivx_vpac_msc_input_params_t'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "OUT_CONFIG", ['tivx_vpac_msc_output_params_t'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "COEFIFICENTS", ['tivx_vpac_msc_coefficients_t'])
kernel.setParameter(Type.IMAGE,             Direction.INPUT,  ParamState.OPTIONAL, "IN_IMAGE_0",   ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.INPUT,  ParamState.OPTIONAL, "IN_IMAGE_1",   ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_0",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_1",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_2",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_3",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_4",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_5",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_6",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_7",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_8",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "OUT_IMAGE_9",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)
'''

code.setCoreDirectory("dmpac_dof")
kernel = Kernel("dmpac_dof")

# DOF configuration,
#
# Dervied parameters,
# Number of levels derived from INPUT_CURRENT, number of levels in INPUT_CURRENT and INPUT_REF must match
# SOF enable dervied from presence of SPARSE_OF_MAP
# Histogram output enable derived form presense of CONFIDENCE_HISTOGRAM
# Temporal perdictor enable dervied from presense of FLOW_VECTOR_IN_TEMPORAL
#
# Parameters preset within the kernel,
# Confidence measure calculation related parameters (CSCFGR, TREE[x]_*)
#
# NOT supported by TIOVX kernel,
# PSA (signature) generation/checking for safety checks
#
# NOT supported by C model,
# PSA
#
# Direct user input parameters,
# Search range V ( Range [0, 63]    recommended = 48, 48 )
# Search range H ( Range [0, 191]   recommended = 191, 191 )
# Flow post filtering (0 = Disabled,  1 = enabled [recommended])
# Motion smoothness factor (Range [0, 31]   recommended = 24)
# Direction (0 = Motion neutral 1 = Forward motion [recommended], 2 = Reverse motion )
#
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION", ['tivx_dmpac_dof_params_t'])

kernel.setParameter(Type.IMAGE,  Direction.INPUT,  ParamState.OPTIONAL, "INPUT_CURRENT_BASE", ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,  Direction.INPUT,  ParamState.OPTIONAL, "INPUT_REFERENCE_BASE", ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])

# pyramid of type VX_SCALE_PYRAMID_HALF, image dataformat of uint8 or packed 12b
kernel.setParameter(Type.PYRAMID,  Direction.INPUT,  ParamState.REQUIRED, "INPUT_CURRENT", ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])

# pyramid of type VX_SCALE_PYRAMID_HALF, image dataformat of uint8 or packed 12b
kernel.setParameter(Type.PYRAMID,  Direction.INPUT,  ParamState.REQUIRED, "INPUT_REFERENCE", ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])

# uint32 image, 32b per pixel
kernel.setParameter(Type.IMAGE,  Direction.INPUT,  ParamState.OPTIONAL, "FLOW_VECTOR_IN", ['VX_DF_IMAGE_U16', 'VX_DF_IMAGE_U32'])

# variable (frame to frame) width and height of output, when NULL SOF is disabled
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.OPTIONAL, "SPARSE_OF_CONFIG", ['tivx_dmpac_dof_sof_params_t'])

# uint8 image, 1b mask per pixel, object width = real width/8, when NULL SOF is disabled
kernel.setParameter(Type.IMAGE,  Direction.INPUT,  ParamState.OPTIONAL, "SPARSE_OF_MAP", ['VX_DF_IMAGE_U8'])

# uint32 image, 32b per pixel
kernel.setParameter(Type.IMAGE,  Direction.OUTPUT, ParamState.REQUIRED, "FLOW_VECTOR_OUT", ['VX_DF_IMAGE_U16', 'VX_DF_IMAGE_U32'])

# histogram of having bins=16, offset=0, range=16, when NULL histogram is NOT output
kernel.setParameter(Type.DISTRIBUTION,  Direction.OUTPUT, ParamState.OPTIONAL, "CONFIDENCE_HISTOGRAM")

kernel.setParameterRelationship(["INPUT_CURRENT", "INPUT_REFERENCE"], [Attribute.Pyramid.LEVELS, Attribute.Pyramid.SCALE, Attribute.Pyramid.WIDTH, Attribute.Pyramid.HEIGHT, Attribute.Pyramid.FORMAT])
kernel.setParameterRelationship(["INPUT_CURRENT", "FLOW_VECTOR_IN", "SPARSE_OF_MAP", "FLOW_VECTOR_OUT"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])
kernel.setParameterRelationship(["INPUT_CURRENT_BASE", "INPUT_REFERENCE_BASE"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT, Attribute.Image.FORMAT])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


code.setCoreDirectory("arm")
kernel = Kernel("dof_visualize")

#uint32 image, 32b per pixel
kernel.setParameter(Type.IMAGE,  Direction.INPUT,  ParamState.REQUIRED, "FLOW_VECTOR", ['VX_DF_IMAGE_U32'])

#uint32 scalar
kernel.setParameter(Type.UINT32,  Direction.INPUT,  ParamState.OPTIONAL, "CONFIDENCE_THRESHOLD", ['VX_TYPE_UINT32'])

# RGB image
kernel.setParameter(Type.IMAGE,  Direction.OUTPUT, ParamState.REQUIRED, "FLOW_VECTOR_RGB", ['VX_DF_IMAGE_RGB'])

# uint8 image
kernel.setParameter(Type.IMAGE,  Direction.OUTPUT, ParamState.REQUIRED, "CONFIDENCE_IMAGE", ['VX_DF_IMAGE_U8'])

kernel.setParameterRelationship(["FLOW_VECTOR", "FLOW_VECTOR_RGB", "CONFIDENCE_IMAGE"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)



code.setCoreDirectory("vpac_viss")
kernel = Kernel("vpac_viss")

kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT, ParamState.REQUIRED, "CONFIGURATION", ['tivx_vpac_viss_params_t'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT, ParamState.REQUIRED, "AE_AWB_RESULT", ['tivx_ae_awb_params_t'])
kernel.setParameter(Type.RAW_IMAGE,         Direction.INPUT, ParamState.REQUIRED, "RAW")  # Combine all exposures in single element
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "Y12",       ['VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "UV12_C1",   ['VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "Y8_R8_C2",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "UV8_G8_C3", ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])
kernel.setParameter(Type.IMAGE,             Direction.OUTPUT, ParamState.OPTIONAL, "S8_B8_C4",  ['VX_DF_IMAGE_U8', 'VX_DF_IMAGE_U16', 'TIVX_DF_IMAGE_P12'])

kernel.setParameter(Type.DISTRIBUTION,      Direction.OUTPUT, ParamState.OPTIONAL, "HISTOGRAM") # 256 bins x 32bit container
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.OUTPUT, ParamState.OPTIONAL, "H3A_AEW_AF", ['tivx_h3a_data_t']) # We can give some utility api to calc blob size
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT, ParamState.OPTIONAL, "DCC_PARAM", ['dcc_viss'])

kernel.setParameterRelationship(["RAW", "Y12", "Y8_R8_C2", "S8_B8_C4"], [Attribute.Image.WIDTH, Attribute.Image.HEIGHT])
kernel.setParameterRelationship(["RAW", "UV12_C1", "UV8_G8_C3"], [Attribute.Image.WIDTH])

kernel.setTarget(Target.IPU1_0)
code.export(kernel)


