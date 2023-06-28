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
 * licensed and provided to you in appCntxtect code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any appCntxtect code compiled from the source code
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

#include <signal.h>

#include <app_semseg_cnn.h>
#include <app_ptk_demo_profile.h>

static SEMSEG_CNN_APP_Context gSemSegAppContext{};

static void SEMSEG_CNN_APP_appShowUsage(char *argv[])
{
    PTK_printf("\n");
    PTK_printf(" Semantic Segmentation Network Demo - (c) Texas Instruments 2020\n");
    PTK_printf(" =====================================================================\n");
    PTK_printf("\n");
    PTK_printf("Please refer to demo guide for prerequisites before running this demo\n");
    PTK_printf("\n");
    PTK_printf(" Usage,\n");
    PTK_printf("  %s --cfg <config file>\n", argv[0]);
    PTK_printf("\n");

    return;
} 

static char menu[] = {
    "\n"
    "\n =========================================="
    "\n Demo : Sematic Segmentation  Network      "
    "\n =========================================="
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};


static void SEMSEG_CNN_APP_parseCmdLineArgs(SEMSEG_CNN_APP_Context *appCntxt, int argc, char *argv[])
{
    int i;

    if (argc == 1)
    {
        SEMSEG_CNN_APP_appShowUsage(argv);
        exit(0);
    }

    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--cfg") == 0)
        {
            i++;
            if (i >= argc)
            {
                SEMSEG_CNN_APP_appShowUsage(argv);
            }

            SEMSEG_CNN_APP_parseCfgFile(appCntxt, argv[i]);

            break;
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            SEMSEG_CNN_APP_appShowUsage(argv);
            exit(0);
        }
    }

    return;
} 

static void sigHandler(int32_t sig)
{
    SEMSEG_CNN_APP_intSigHandler(&gSemSegAppContext);
}

int main(int argc, char **argv)
{
    SEMSEG_CNN_APP_Context *appCntxt = &gSemSegAppContext;
    PTK_CRT                 ptkConfig;
    int32_t                 status;

    //tivx_set_debug_zone(VX_ZONE_INFO);

    /* Register the signal handler. */
    signal(SIGINT, sigHandler);

    /* Initialize PTK library. */
    ptkConfig.exit   = exit;
    ptkConfig.printf = printf;
    ptkConfig.time   = NULL;

    PTK_init(&ptkConfig);

    /* parse command line */
    SEMSEG_CNN_APP_parseCmdLineArgs(appCntxt, argc, argv);

    /* Initialize the Application context */
    status = SEMSEG_CNN_APP_init(appCntxt);

    if (status < 0)
    {
        PTK_printf("[%s:%d] SEMSEG_CNN_APP_init() failed.\n",
                   __FUNCTION__, __LINE__);

        return status;
    }

    /* Launch processing threads. */
    SEMSEG_CNN_APP_launchProcThreads(appCntxt);

    if (appCntxt->is_interactive)
    {
        uint32_t done = 0;

        appPerfStatsResetAll();

        while (!done)
        {
            char ch;

            PTK_printf(menu);
            ch = getchar();
            PTK_printf("\n");

            switch (ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    SEMSEG_CNN_APPLIB_printStats(appCntxt->sscnnHdl);

                    ptkdemo_print_proctime();
                    ptkdemo_reset_proctime();
                    break;

                case 'e':
                    {
                        FILE *fp = NULL;
                        char *name = SEMSEG_CNN_PERF_OUT_FILE;

                        fp = appPerfStatsExportOpenFile(".", name);

                        if (fp != NULL)
                        {
                            SEMSEG_CNN_APPLIB_exportStats(appCntxt->sscnnHdl,
                                                          fp,
                                                          true);

                            appPerfStatsExportCloseFile(fp);
                        }
                        else
                        {
                            PTK_printf("Could not open [%s] for exporting "
                                       "performance data\n", name);
                        }
                    }

                    break;

                case 'x':
                    done = 1;
                    break;
            } // switch(ch)

            /* Consume the newline character. */
            if (ch != '\n')
            {
                getchar();
            }

        } // while (!done)

    } // if (appCntxt->is_interactive)

    SEMSEG_CNN_APP_cleanupHdlr(appCntxt);

    PTK_printf("\nDEMO FINISHED!\n");

    return 0;
}
