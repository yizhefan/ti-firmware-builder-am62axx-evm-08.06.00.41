 /*
 *******************************************************************************
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 *******************************************************************************
 */

#ifndef _APP_ESTOP_H_
#define _APP_ESTOP_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <app_estop_main.h>


#ifdef __cplusplus
extern "C" {
#endif


void      ESTOP_APP_parseCfgFile(ESTOP_APP_Context *appCntxt, const char *cfg_file_name);

void      ESTOP_APP_setAllParams(ESTOP_APP_Context *appCntxt);

void      ESTOP_APP_init(ESTOP_APP_Context *appCntxt);

void      ESTOP_APP_launchProcThreads(ESTOP_APP_Context *appCntxt);

void      ESTOP_APP_intSigHandler(ESTOP_APP_Context *appCntxt, int sig);

void      ESTOP_APP_cleanupHdlr(ESTOP_APP_Context *appCntxt);

vx_status ESTOP_APP_createDisplayGraph(ESTOP_APP_Context *appCntxt);

void      ESTOP_APP_detectionThread(ESTOP_APP_Context *appCntxt);

void      ESTOP_APP_createDisparityCCImage(ESTOP_APP_Context *appCntxt);

void      ESTOP_APP_overlay3DBB(ESTOP_APP_Context *appCntxt);


int32_t   ESTOP_APP_init_LDC(ESTOP_APP_Context *appCntxt);

int32_t   ESTOP_APP_init_SDE(ESTOP_APP_Context *appCntxt);

int32_t   ESTOP_APP_init_SS(ESTOP_APP_Context *appCntxt);

int32_t   ESTOP_APP_init_SS_Detection(ESTOP_APP_Context *appCntxt);

vx_status ESTOP_APP_setupPipeline(ESTOP_APP_Context * appCntxt);

vx_status ESTOP_APP_setupPipeline_SL(ESTOP_APP_Context * appCntxt);

vx_status ESTOP_APP_setupPipeline_ML(ESTOP_APP_Context * appCntxt);

void      ESTOP_APP_printStats(ESTOP_APP_Context * appCntxt);

void      ESTOP_APP_exportStats(ESTOP_APP_Context * appCntxt);

void      ESTOP_APP_waitGraph(ESTOP_APP_Context * appCntxt);


vx_status ESTOP_APP_popFreeInputDesc(ESTOP_APP_Context       *appCntxt,
                                     ESTOP_APP_graphParams   **gpDesc); 

vx_status ESTOP_APP_getDLRInputDesc(ESTOP_APP_Context       *appCntxt,
                                    ESTOP_APP_graphParams  **gpDesc);


vx_status ESTOP_APP_popDLRInputDesc(ESTOP_APP_Context       *appCntxt,
                                    ESTOP_APP_graphParams  **gpDesc);

vx_status ESTOP_APP_getOutputDesc(ESTOP_APP_Context       *appCntxt,
                                  ESTOP_APP_graphParams   *gpDesc);

vx_status ESTOP_APP_popOutputDesc(ESTOP_APP_Context       *appCntxt,
                                  ESTOP_APP_graphParams  **gpDesc);

void      ESTOP_APP_enqueOutputDesc(ESTOP_APP_Context      *appCntxt,
                                    ESTOP_APP_graphParams  *desc);

void      ESTOP_APP_enqueDLRInputDesc(ESTOP_APP_Context      *appCntxt,
                                      ESTOP_APP_graphParams  *desc);

void      ESTOP_APP_enqueInputDesc(ESTOP_APP_Context      *appCntxt,
                                   ESTOP_APP_graphParams  *desc);


vx_status ESTOP_APP_process(ESTOP_APP_Context * appCntxt, ESTOP_APP_graphParams * gpDesc);

vx_status ESTOP_APP_processEvent(ESTOP_APP_Context * appCntxt, vx_event_t * event);

vx_status ESTOP_APP_releaseParamRsrc(ESTOP_APP_Context  *appCntxt, uint32_t rsrcIndex);

vx_status ESTOP_APP_getOutBuff(ESTOP_APP_Context *appCntxt, vx_image rightRectImage,
                              vx_image ssOutImage, vx_user_data_object obsBB, vx_image disparity16);

vx_status ESTOP_APP_releaseOutBuff(ESTOP_APP_Context * appCntxt);

#ifdef __cplusplus
}
#endif

#endif /* _APP_ESTOP_H_ */

