'''
* Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
'''

from tiovx import *

code = KernelExportCode(Module.PARK_ASSIST, "c7x", "CUSTOM_APPLICATION_PATH")

# Triangulation Node
code.setCoreDirectory("c7x")
kernel = Kernel("triangulation")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION", ['tivx_triangulation_params_t'])
kernel.setParameter(Type.ARRAY,            Direction.INPUT,  ParamState.REQUIRED, "INPUT_TRACK",   ['tivx_triangulation_track_t'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_POSE",    ['tivx_triangulation_pose_t'])
kernel.setParameter(Type.ARRAY,            Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT_POINT",  ['PTK_Point'])

kernel.setTarget(Target.DSP1)
code.export(kernel)

# DOF to Tracke Node
code.setCoreDirectory("c7x")
kernel = Kernel("dof_to_tracks")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION",   ['tivx_dof_to_tracks_params_t'])
kernel.setParameter(Type.IMAGE, Direction.INPUT,  ParamState.REQUIRED,            "INPUT_DOF_FIELD", ['VX_DF_IMAGE_U32'])
kernel.setParameter(Type.LUT,   Direction.INPUT,  ParamState.OPTIONAL,            "INPUT_D2U_LUT",   ['VX_TYPE_FLOAT32'])
kernel.setParameter(Type.ARRAY,            Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT_TRACKS",   ['tivx_triangulation_track_t'])

kernel.setTarget(Target.DSP1)
code.export(kernel)

# SFM OGMAP Node
code.setCoreDirectory("arm")
kernel = Kernel("sfm_ogmap")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION", ['PTK_Alg_SfmOgmapParams'])
kernel.setParameter(Type.ARRAY,            Direction.INPUT,  ParamState.REQUIRED, "INPUT_POINT",   ['PTK_Point'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_POSE",    ['PTK_RigidTransform'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_MAP",     ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT_MAP",    ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Lidar OGMAP Node
code.setCoreDirectory("arm")
kernel = Kernel("lidar_ogmap")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "configuration", ['PTK_Alg_LidarOgmapParams'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "point_cloud",   ['PTK_PointCloud'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.REQUIRED, "input_map",     ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.REQUIRED, "output_map",    ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Radar OGMAP Node
code.setCoreDirectory("arm")
kernel = Kernel("radar_ogmap")

kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION",  ['PTK_Alg_RadarOgmapParams'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "sensor_config",  ['PTK_Alg_RadarSensorConfig'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "radar_obj_data", ['PTK_Alg_RadarDetOutput'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "imu_data",       ['PTK_INS_Record'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "ecew_w",         ['PTK_RigidTransform_d'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "INPUT_MAP",      ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT_MAP",     ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Fused OGMAP Node
code.setCoreDirectory("arm")
kernel = Kernel("fused_ogmap")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "CONFIGURATION", ['tivx_fused_ogmap_config_t'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "ogmap_camera",  ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "ogmap_radar",   ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "ogmap_lidar",   ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "timestamps",    ['tivx_fused_ogmap_time_t'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT, ParamState.REQUIRED,  "input_map",     ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.REQUIRED, "output_map",    ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Free Space Detection Node
code.setCoreDirectory("arm")
kernel = Kernel("fsd")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "configuration", ['PTK_Alg_FsdConfig'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "time",          ['VX_TYPE_UINT64'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "map_in",        ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.REQUIRED, "map_out",       ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Parking Free Space Detection Node
code.setCoreDirectory("arm")
kernel = Kernel("parking_freespace_detection")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION",                 ['PTK_Alg_PfsdParams'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_MAP",                     ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_TIMESTAMP",               ['uint64_t'])
kernel.setParameter(Type.ARRAY,            Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT_PARKING_FREESPACE_DESC", ['PTK_Alg_PfsdDesc'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.OPTIONAL, "OUTPUT_MAP",                    ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Parking Spot Mapping Node
code.setCoreDirectory("arm")
kernel = Kernel("ps_mapping")

kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "CONFIGURATION",                 ['tivx_ps_mapping_config_t'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "DATAPTR",                       ['tivx_ps_mapping_input_t'])
kernel.setParameter(Type.LUT,              Direction.INPUT,  ParamState.REQUIRED, "D2U_LUT",                       ['VX_TYPE_FLOAT32'])
kernel.setParameter(Type.LUT,              Direction.INPUT,  ParamState.REQUIRED, "PROJMAT",                       ['VX_TYPE_FLOAT64'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_POSE",                    ['tivx_ps_mapping_pose_t'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.INPUT,  ParamState.REQUIRED, "INPUT_MAP",                     ['PTK_Map'])
kernel.setParameter(Type.USER_DATA_OBJECT, Direction.OUTPUT, ParamState.REQUIRED, "OUTPUT_MAP",                    ['PTK_Map'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

# Radar Tracker Node
code.setCoreDirectory("arm")
kernel = Kernel("radar_gtrack")

kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "configuration",  ['PTK_Alg_RadarGTrackParams'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "sensor_config",  ['PTK_Alg_RadarSensorConfig'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "radar_obj_data", ['PTK_Alg_RadarDetOutput'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "imu_data",       ['PTK_INS_Record'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.INPUT,  ParamState.REQUIRED, "ecew_w",         ['PTK_RigidTransform_d'])
kernel.setParameter(Type.USER_DATA_OBJECT,  Direction.OUTPUT, ParamState.REQUIRED, "track_info",     ['PTK_Alg_RadarGTrackTargetInfo'])

kernel.setTarget(Target.A15_0)
code.export(kernel)

