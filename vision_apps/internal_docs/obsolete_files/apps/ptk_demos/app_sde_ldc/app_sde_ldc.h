 /*
 *******************************************************************************
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 *******************************************************************************
 */

#ifndef _APP_SDELDC_H_
#define _APP_SDELDC_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <sde_ldc_applib.h>


#ifdef __cplusplus
extern "C" {
#endif


void      SDELDCAPP_parseCfgFile(SDELDCAPP_Context *appCntxt, const char *cfg_file_name);

void      SDELDCAPP_setAllParams(SDELDCAPP_Context *appCntxt);

int32_t   SDELDCAPP_init(SDELDCAPP_Context *appCntxt);

void      SDELDCAPP_launchProcThreads(SDELDCAPP_Context *appCntxt);

void      SDELDCAPP_intSigHandler(SDELDCAPP_Context *appCntx, int sig);

void      SDELDCAPP_cleanupHdlr(SDELDCAPP_Context *appCntxt);

void      SDELDCAPP_createDispalyGraph(SDELDCAPP_Context *appCntxt);


int32_t   SDELDCAPP_init_LDC(SDELDCAPP_Context *appCntxt);

vx_status SDELDCAPP_setupPipeline(SDELDCAPP_Context * appCntxt);

void      SDELDCAPP_printStats(SDELDCAPP_Context * appCntxt);

void      SDELDCAPP_exportStats(SDELDCAPP_Context * appCntxt);

void      SDELDCAPP_waitGraph(SDELDCAPP_Context * appCntxt);

vx_status SDELDCAPP_getFreeParamRsrc(SDELDCAPP_Context       *appCntxt,
                                     SDELDCAPP_graphParams   **gpDesc);

vx_status SDELDCAPP_process(SDELDCAPP_Context * appCntxt, SDELDCAPP_graphParams * gpDesc);

int32_t   SDELDCAPP_processEvent(SDELDCAPP_Context * appCntxt, vx_event_t * event);

int32_t   SDELDCAPP_releaseParamRsrc(SDELDCAPP_Context  *appCntxt, uint32_t rsrcIndex);

int32_t   SDELDCAPP_getOutBuff(SDELDCAPP_Context *appCntxt, vx_image outputLeftImage, vx_image outputRightImage);
 
void      SDELDCAPP_releaseOutBuff(SDELDCAPP_Context * appCntxt);


#ifdef __cplusplus
}
#endif

#endif /* _APP_SDELDC_H_ */

