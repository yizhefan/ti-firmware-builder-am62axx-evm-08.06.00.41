/*
 *
 * Copyright (c) 2020 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _APP_LDC_MODULE
#define _APP_LDC_MODULE

/**
 * \defgroup group_vision_apps_modules_ldc LDC Node Module
 *
 * \brief This section contains module APIs for the TIOVX LDC node tivxVpacLdcNode
 *
 * \ingroup group_vision_apps_modules
 *
 * @{
 */

#include "app_modules.h"
#include "app_sensor_module.h"

/** \brief Default LDC table width.  Can be modified as needed for use case
 *
 */
#define LDC_TABLE_WIDTH     (1920)

/** \brief Default LDC table width.  Can be modified as needed for use case
 *
 */
#define LDC_TABLE_HEIGHT    (1080)

/** \brief Default LDC downscale factor.  Can be modified as needed for use case
 *
 */
#define LDC_DS_FACTOR       (2)

/** \brief Default LDC block width.  Can be modified as needed for use case
 *
 */
#define LDC_BLOCK_WIDTH     (64)

/** \brief Default LDC block height.  Can be modified as needed for use case
 *
 */
#define LDC_BLOCK_HEIGHT    (32)

/** \brief Default LDC pixel padding.  Can be modified as needed for use case
 *
 */
#define LDC_PIXEL_PAD       (1)

/** \brief LDC Module Data Structure
 *
 * Contains the data objects required to use tivxVpacLdcNode
 *
 */
typedef struct {
    /*! LDC node object */
    vx_node node;

    /*! LDC mesh image */
    vx_image mesh_img;

    /*! LDC node params structure to initialize config object */
    tivx_vpac_ldc_params_t params;

    /*! User data object for config parameter, used as node parameter of LDC node */
    vx_user_data_object config;

    /*! LDC mesh params structure to initialize mesh config object */
    tivx_vpac_ldc_mesh_params_t   mesh_params;

    /*! User data object for mesh config, used as node parameter of LDC node */
    vx_user_data_object mesh_config;

    /*! LDC region params structure to initialize region params config object */
    tivx_vpac_ldc_region_params_t region_params;

    /*! User data object for region config, used as node parameter of LDC node */
    vx_user_data_object region_config;

    /*! User data object for DCC config parameter, used as node parameter of LDC node */
    vx_user_data_object dcc_config;

    /*! LDC table width, set to \ref LDC_TABLE_WIDTH  */
    vx_uint32 table_width;

    /*! LDC table height, set to \ref LDC_TABLE_HEIGHT  */
    vx_uint32 table_height;

    /*! LDC downscale factor, set to \ref LDC_DS_FACTOR  */
    vx_uint32 ds_factor;

    /*! Output object array of LDC node  */
    vx_object_array output_arr;

    /*! Capture node graph parameter index */
    vx_int32 graph_parameter_index;

    /*! Flag to enable writing LDC output  */
    vx_int32 en_out_ldc_write;

    /*! Node used to write LDC output */
    vx_node write_node;

    /*! File path used to write LDC node output */
    vx_array file_path;

    /*! File path prefix used to write LDC node output */
    vx_array file_prefix;

    /*! User data object containing write cmd parameters */
    vx_user_data_object write_cmd;

    /*! Output file path for LDC node output */
    vx_char output_file_path[TIVX_FILEIO_FILE_PATH_LENGTH];

    /*! Name of LDC module */
    vx_char objName[APP_MODULES_MAX_OBJ_NAME_SIZE];
}LDCObj;

/** \brief LDC module init helper function
 *
 * This LDC init helper function will create all the data objects required to create the LDC
 * node
 *
 * \param [in]  context    OpenVX context which must be created using \ref vxCreateContext
 * \param [out] ldcObj     LDC Module object which gets populated with LDC node data objects
 * \param [in]  sensorObj  Sensor Module object used to initialize LDC data object parameters;
 *                         must be initialized prior to passing to this function
 * \param [in]  objName    String of the name of this object
 *
 */
vx_status app_init_ldc(vx_context context, LDCObj *ldcObj, SensorObj *sensorObj, char *objName);

/** \brief LDC module deinit helper function
 *
 * This LDC deinit helper function will release all the data objects created during the \ref app_init_ldc call
 *
 * \param [in,out] ldcObj  LDC Module object which contains LDC node data objects which are released in this function
 *
 */
void app_deinit_ldc(LDCObj *ldcObj);

/** \brief LDC module delete helper function
 *
 * This LDC delete helper function will delete the LDC node and write node that is created during the \ref app_create_graph_ldc call
 *
 * \param [in,out] ldcObj  LDC Module object which contains LDC node objects which are released in this function
 *
 */
void app_delete_ldc(LDCObj *ldcObj);

/** \brief LDC module create helper function
 *
 * This LDC create helper function will create the node using all the data objects created during the \ref app_init_ldc call.
 * Internally calls \ref app_create_graph_ldc_write_output if en_out_ldc_write is set
 *
 * \param [in]     graph      OpenVX graph that has been created using \ref vxCreateGraph and where the LDC node is created
 * \param [in,out] ldcObj     LDC Module object which contains LDC node and write node which are created in this function
 * \param [in]     input_arr  Input object array to LDC node.  Must be created separately using \ref vxCreateObjectArray
 *
 */
vx_status app_create_graph_ldc(vx_graph graph, LDCObj *ldcObj, vx_object_array input_arr);

/** \brief LDC module write output helper function
 *
 * This LDC create helper function will create the node for writing the LDC output
 *
 * \param [in]     graph   OpenVX graph
 * \param [in,out] ldcObj  LDC Module object which contains LDC node and write node which are created in this function
 *
 */
vx_status app_create_graph_ldc_write_output(vx_graph graph, LDCObj *ldcObj);


/** \brief LDC module write output helper function
 *
 * This LDC create helper function will create the node for writing the LDC output
 *
 * \param [in] ldcObj        LDC Module object which contains the write node used in this function
 * \param [in] start_frame   Starting frame to write
 * \param [in] num_frames    Total number of frames to write
 * \param [in] num_skip      Number of capture frames to skip writing
 *
 */
vx_status app_send_cmd_ldc_write_node(LDCObj *ldcObj, vx_uint32 start_frame, vx_uint32 num_frames, vx_uint32 num_skip);

/* @} */

#endif
