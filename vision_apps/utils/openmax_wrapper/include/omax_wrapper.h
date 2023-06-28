/*
 *  Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TI_OMAX_WRAPPER_H_
#define _TI_OMAX_WRAPPER_H_

#include <utils/codec_wrapper/include/codec_wrapper.h>

/**
 * \brief 
 * 
 * Launches the OMXPipeline described by user defined parameters.
 * 
 * \param [in]      prm         Init parameters
 */
int32_t appOMXInit(app_codec_wrapper_params_t *prm);

/**
 * \brief 
 * 
 * Initializes the Encode elements to be able to push buffers to the OMXPipeline.
 * Wraps the given allocated memory as OMXBuffers to push. 
 * The input parameters (in_*) must be properly initialised.
 * 
 * \param [in]      data_ptr        memory that the OMXBuffers will map to
 */
int32_t appOMXEncInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES]);

/**
 * \brief 
 * 
 * Initializes the Decode elements to be able to pull buffers from the OMXPipeline.
 * Registers the data_pointers that will map to the bufferpool of pulled OMXBuffers.
 * The output parameters (out_*) must be properly initialised.
 * 
 * \param [in]      data_ptr        pointers to hold the pulled buffers
 */
int32_t appOMXDecInit(void* (*data_ptr)[CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES]);

/**
 * \brief 
 * 
 * Starts the OMXPipeline that was launched previously.
 */
int32_t appOMXStart();

/**
 * \brief 
 * 
 * Push a buffer from the bufferpool to the Encode element.
 * 
 * \param [in]     idx             the buffer id, from the bufferpool, to be pushed to the OMXPipeline
 */
int32_t appOMXEnqAppEnc(uint8_t idx);

/**
 * \brief 
 * 
 * Wait for a previously pushed buffer to be consumed by the OMXPipeline.
 * 
 * \param [in]     idx             the buffer id, from the bufferpool, to wait on
 */
int32_t appOMXDeqAppEnc(uint8_t idx);

/**
 * \brief 
 * 
 * Push EOS (End Of Stream) to the Encode element.
 * Signals the elements to stop processing buffers after this.
 */
int32_t appOMXEnqEosAppEnc();

/**
 * \brief 
 * 
 * Pull a buffer from the Decode element into the bufferpool.
 * 
 * \param [in]     idx             the slot into which the pulled data is stored  
 *                                 in the pulled_data_ptr array, while maintaining 
 *                                 a reference to the pulled OMXBuffer
 */
int32_t appOMXDeqAppDec(uint8_t idx);

/**
 * \brief 
 * 
 * Release a previously pulled OMXBuffer back to GStreamer and cleanup.
 * 
 * \param [in]     idx             the slot from which the buffer is released 
 *                                 back to OMXreamer. The data pointer for this
 *                                 slot is no longer valid
 */
int32_t appOMXEnqAppDec(uint8_t idx);

/**
 * \brief 
 * 
 * Stops the OMXPipeline that was in playing state.
 * If there is no Decode element, waits to recieve EOS before cleaning up.
 */
int32_t appOMXStop(); 

/**
 * \brief 
 * 
 * Unmaps all buffers that were mapped and destroys all GStreamer objects.
 */
void    appOMXDeInit();


/**
 * \brief 
 * 
 * Prints the current count of the numbers of buffers pushed/pulled to/from the OMXPipeline.
 * Not multi-thread safe.
 */
void appOMXPrintStats();

/* @} */

#endif /* _TI_OMAX_WRAPPER_H_ */

