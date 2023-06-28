# Top Level Design {#did_top_level_design}

[TOC]

# Requirements Addressed {#did_top_level_requirements}

- https://jira.itg.ti.com/browse/ADASVISION-1827

# Top Level Directory Structure {#did_top_level_dir_structure}

Shown below is the directory structure of vision_apps::


    vision_apps                           # Root folder for Vision Apps
    ├── applibs                           # Application libraries
    │   ├── fused_ogmap_applib            # Specific Application library
    │   ├── lidar_ogmap_applib
    │   ├── surround_radar_ogmap_applib
    │   └── surround_sfm_ogmap_applib
    ├── apps                              # Applications
    │   ├── basic_demos                   # Application Group
    │   │   ├── app_c7x_kernel            # Specific Application
    │   │   │   ├── c7x                   # Application specific sub-folder
    │   │   │   └── config                # Default Application config file
    │   │   ├── app_dof
    │   │   │   └── config
    │   │   ├── app_ldc
    │   │   │   └── config
    │   │   ├── app_ldc_dof
    │   │   │   └── config
    │   │   └── app_stereo
    │   │       └── config
    │   │   └── app_tidl
    │   │       └── config
    │   └── valet_parking
    │       ├── app_fused_ogmap
    │       │   └── config
    │       ├── app_lidar_ogmap
    │       │   └── config
    │       ├── app_sfm_fisheye
    │       │   ├── config
    │       │   └── config_data
    │       ├── app_surround_radar_ogmap
    │       │   └── config
    │       └── app_valet_parking
    │           └── config
    ├── concerto                            # Makefile based build system common files
    │   └── compilers                       # Compiler specific common options
    ├── docs                                # User documentation
    │   ├── relnotes_archive
    │   └── user_guide
    ├── kernels                             # OpenVX kernel wrapper for applications/application libraries
    │   ├── lidar                           # Kernel library or kernel group
    │   │   ├── arm                         # Target side kernel wrappers for "arm" target
    │   │   ├── host                        # Host side kernel wrappers
    │   │   ├── include                     # Kernel user interface
    │   │   │   └── TI
    │   │   └── test                        # Kernel unit test
    │   ├── park_assist
    │   │   ├── arm
    │   │   ├── c7x
    │   │   ├── common
    │   │   ├── host
    │   │   ├── include
    │   │   │   └── TI
    │   │   └── test
    │   └── stereo
    │       ├── arm
    │       ├── c7x
    │       ├── host
    │       └── include
    │           └── TI
    ├── platform                            # SoC-specific memory map and common libraries
    │   ├── j721e                           # J721E-specific memory map and common libraries
    │   │   ├── rtos                        # J721E RTOS firmware and memory map files
    │   │   ├── linux                       # J721E Linux common library
    │   │   └── qnx                         # J721E QNX common library
    └── utils                                   # utilities used by apps, applibs, kernels
        └── perception                          # Specific utility
            ├── alg_parking_freespace_detection # Utility defined sub-folders
            │   ├── include
            │   └── src
            ├── alg_sfm_ogmap
            │   ├── include
            │   └── src
            ├── calmat_utils
            └── TI                              # User interface to the utility

Below table has more information about some important folders

<table>
<tr>
    <th>
    Folder
    </td>
    <th>
    Description
    </th>
</tr>
<tr>
    <td>
    applib
    </td>
    <td>
    Application library which contains API and
    implementation of OpenVX sub-graph or
    sub-application. Multiple "applibs"
    can be used together to form a larger application.
    Each application library can define its own interface.
    There is no specific rule for application
    library interface. Not all applications will have a
    corresponding "applib"
    </td>
<tr>
    <td>
    apps
    </tr>
    <td>
       Applications contain final executable binaries code.
       Applications are grouped into further folders for modularity.
       For example, "basic_demos" some simple "unit level"
       demos to show various syste/SoC features.
       On a x86 PC and Embedded Linux each sub-folder
       within the group of application is a executable by itself.
       On a RTOS application, each group of folders is a application
       by itself. Ex, in RTOS, there will be a single binary
       called "basic_demos" which in turn will include multiple
       applications.
       This is done to avoid having a large monolithic binary containing
       all applications on RTOS.
    </td>
</tr>
<tr>
    <td>
    kernels
    </td>
    <td>
       This folder contains OpenVX kernel wrappers used
       by apps or applibs. Kernels are further grouped into
       folders. Each folder is a OpenVX kernel library.
    </td>
</tr>
<tr>
    <td>
     utils
    </td>
    <td>
       This folder contains other modules used to interface
       vision_apps with other Processor SDK RTOS 
       components.
    </td>
</tr>
<tr>
    <td>
     concerto
    </td>
    <td>
     Makefile based build system. "Concerto" is just a name.
    </td>
</tr>
<tr>
    <td>
     docs
    </td>
    <td>
     User documentation like user guide, API guide
    </td>
</tr>
</table>

# Top Level Component Interaction {#did_top_level_component_interaction}


![](top_level_component_interaction.png "Top Level Component Interaction")

- A "app" consists of zero or more "applibs"
- A "applib" makes a subgraph using "kernels"
- "apps", "applibs", "kernels" make use of "utils"
- A "applib" can be non-OpenVX library which performs some logical reusuable (sub)application function
- A "app" can make OpenVX graph using "kernels" directly
- A "app" can be non-OpenVX application
