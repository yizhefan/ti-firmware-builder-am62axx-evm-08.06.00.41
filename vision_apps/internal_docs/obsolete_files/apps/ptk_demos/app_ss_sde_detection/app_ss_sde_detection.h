 /*
 *******************************************************************************
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 *******************************************************************************
 */

#ifndef _APP_SS_SDE_DETECTION_H_
#define _APP_SS_SDE_DETECTION_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <app_ss_sde_detection_main.h>


#ifdef __cplusplus
extern "C" {
#endif


void SS_DETECT_APP_parseCfgFile(SS_DETECT_APP_Context *appCntxt, const char *cfg_file_name);

void SS_DETECT_APP_setAllParams(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_init(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_launchProcThreads(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_intSigHandler(SS_DETECT_APP_Context *appCntxt, int sig);

void SS_DETECT_APP_cleanupHdlr(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_createDispalyGraph(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_detectionThread(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_createDisparityCCImage(SS_DETECT_APP_Context *appCntxt);

void SS_DETECT_APP_overlay3DBB(SS_DETECT_APP_Context *appCntxt);




#ifdef __cplusplus
}
#endif

#endif /* _APP_SS_SDE_DETECTION_H_ */

