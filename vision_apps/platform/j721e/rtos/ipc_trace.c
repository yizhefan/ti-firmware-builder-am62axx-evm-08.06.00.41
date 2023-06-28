/*
 *  Copyright (c) Texas Instruments Incorporated 2020
 *  All rights reserved.
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

/**
 *  \file ipc_trace.c
 *
 *  \brief IPC  functions used for trace
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "ipc_trace.h"

char Ipc_traceBuffer[IPC_TRACE_BUFFER_MAX_SIZE]
__attribute__ ((section(".tracebuf")))
;

static uint32_t gTraceBufIndex = 0U;

int32_t Ipc_Trace_printf(const char *format, ...)
{
    char buffer[IPC_TRACE_MAX_LINE_LENGTH];
    va_list args = {0};
    uint8_t i = 0;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    for (i = 0; i < strlen(buffer); i++)
    {
        Ipc_traceBuffer[gTraceBufIndex++] = buffer[i];

        if (gTraceBufIndex == IPC_TRACE_BUFFER_MAX_SIZE) {
            gTraceBufIndex = 0;
        }
    }
    return 0;
}
