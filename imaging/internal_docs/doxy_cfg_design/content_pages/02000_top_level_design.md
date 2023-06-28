# Top Level Design {#did_top_level_design}


# Top Level Directory Structure {#did_top_level_dir_structure}

Shown below is the directory structure of imaging::

    imaging
    ├── concerto                # makefile based build system
    ├── algos			# algorithms
    │   ├── ae		        # AutoExpsoure Algorithm Source
    ├── sensor_drv	        # Sensor Drivers
    │   ├──include		# Sensor Driver Interface files
    │   ├──src			# Sensor Driver Framework
    │      ├── imx390			                # IMX390 Sensor Driver
    │      ├── ar0233			                # AR0233 Sensor Driver
    │      ├── ar0820			                # AR0820 Sensor Driver
    │      ├── ub9xx_yuv_test_pattern			# UB96x and UB97x RAW12 Test Pattern Driver
    │      ├── ar0820			                # UB96x and UB97x YUV422 (UYVY) Test Pattern Driver
    │      ├── gw_ar0233_yuv                    # LI-AR0233-GW5200 YUV422 (UYVY) Camera Driver
    ├── docs                    # User documentation
    ├── kernels                 # OpenVX kernels (kernel wrappers)
    │   ├── arm                 # 
    │   ├── host                # 
    │   ├── include             # TI extention kernel APIs
    │   │   └── TI
    │   ├── ivision             # Kernels using TI ivision interface
    │   └── test                # 
    ├── lib                     # Pre built libraries
