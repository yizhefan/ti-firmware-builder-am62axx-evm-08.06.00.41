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
#ifndef ITT_SERVER_REMOTE_H_
#define ITT_SERVER_REMOTE_H_

#include "TI/tivx_mutex.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*******************************************************************************
 *  Include files
 *******************************************************************************
 */

/*******************************************************************************
 *  Defines
 *******************************************************************************
 */
#ifndef x86_64

#define AEWB_SERVER_REMOTE_SERVICE_NAME   "com.ti.aewb_server"

#endif

/**
 *  \brief RPC Commands application can send to AEWB
 * \ingroup group_vision_function_imaging
 */

typedef enum
{
    AEWB_CMD_GET_2A_PARAMS = 0,
    AEWB_CMD_SET_2A_PARAMS,
    AEWB_CMD_MAX,
    AEWB_CMD_FORCE32BITS          = 0x7FFFFFFF
}AEWB_COMMAND;

/*Enusre that this is same as SET_DCC_PARAMS in VISS and other nodes*/
#define ISS_CMD_SET_DCC_PARAMS                   (0x30000000u)


/*******************************************************************************
 *  \brief 2A Parameters, Used by the DCC to control 2A parameters
 *         It could be used to enable/disable AE/AWB or used to fix
 *         output of AE/AWB
 *******************************************************************************
 */
typedef struct {
    uint32_t                aeMode;
    /**< AE Mode, used to enable/disable AE.
         It could be either AUTO (0) or manual (1),
         Auto mode is default and runs the AE algorithms
         Manual mode uses below AE parameters value as AE output */
    uint32_t                digitalGain;
    /**< Digital Gain for manual AE mode */
    uint32_t                analogGain;
    /**< Analog Gain for manual AE mode */
    uint32_t                expTime;
    /**< Exposure time for manual AE mode */
    uint32_t                awbMode;
    /**< AWB Mode, used to enable/disable AWB.
         It could be either AUTO (0) or manual (1),
         Auto mode is default and runs the AWB algorithms
         Manual mode uses below AWB parameters value as AE output */
    uint32_t                rGain;
    /**< Red color gain for manual AWB mode*/
    uint32_t                gGain;
    /**< Green color gain for manual AWB mode*/
    uint32_t                bGain;
    /**< Blue color gain for manual AWB mode*/

    uint32_t                colorTemp;
    /**< Color Temperature for manual AWB mode*/
    uint32_t                awb_valid;
    /**< AWB valid*/
    uint32_t                awb_converged;
    /**< AWB converged*/
} AlgItt_IssAewb2AParams;

/**
 *******************************************************************************
 *   \brief Structure containing control parameters for 2A State Information
 *******************************************************************************
*/

/**
 *******************************************************************************
 *   \brief Structure containing control parameters
 *******************************************************************************
*/
typedef struct
{
    uint32_t                   size;
    /**< Size of the structure */

    uint32_t                   controlCmd;
    /**< Plugin defined control command */

    /**< Mutex to protect read/write 2A state parameters */
} AlgItt_ControlParams;


typedef struct
{
    AlgItt_ControlParams     baseClassControl;
    /**< Base class control params */
    AlgItt_IssAewb2AParams   aewb2APrms;
    /**< 2A Parameters */
} AlgItt_IssAewb2AControlParams;


int32_t AewbServer_RemoteServiceHandler(char *service_name, uint32_t cmd,
    void *prm, uint32_t prm_size, uint32_t flags);


 /**
 *******************************************************************************
 *
 * \brief Function to initialize remote ITT server.
 * \return 0 in case of success
 *         error otherwise
 *
 * \ingroup group_vision_function_imaging
 *******************************************************************************
 */
int32_t IttRemoteServer_Init();

 /**
 *******************************************************************************
 *
 * \brief Function to de-initialize remote ITT server.
 * \return 0 in case of success
 *         error otherwise
 *
 * \ingroup group_vision_function_imaging
 *******************************************************************************
 */
int32_t IttRemoteServer_DeInit();


#endif /* End of ITT_SERVER_REMOTE_H_*/

