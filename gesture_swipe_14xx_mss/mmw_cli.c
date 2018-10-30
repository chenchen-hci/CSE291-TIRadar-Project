/*
 *   @file  cli.c
 *
 *   @brief
 *      Mmw (Milli-meter wave) DEMO CLI Implementation
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/drivers/uart/UART.h>
#include <ti/utils/cli/cli.h>
#include <ti/control/mmwavelink/mmwavelink.h>

/* Demo Include Files */
#include "mmw.h"

extern uint32_t log2Approx(uint32_t x);

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/* CLI Command Functions */
static int32_t MmwDemo_CLICfarCfg (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLISensorStart (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLISensorStop (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLIGuiMonSel (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLIPeakGroupingCfg (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLIMultiObjBeamForming (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLICalibDcRangeSig (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLIClutterRemoval (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLICompRangeBiasAndRxChanPhaseCfg (int32_t argc, char* argv[]);
static int32_t MmwDemo_CLIMeasureRangeBiasAndRxChanPhaseCfg (int32_t argc, char* argv[]);

/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/

extern MmwDemo_MCB    gMmwMCB;

/**************************************************************************
 **************************** MMW CLI Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for gui monitoring configuration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIGuiMonSel (int32_t argc, char* argv[])
{
    MmwDemo_GuiMonSel   guiMonSel;

    /* Sanity Check: Minimum argument check */
    if (argc != 7)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the ADC Output configuration: */
    memset ((void *)&guiMonSel, 0, sizeof(MmwDemo_GuiMonSel));

    /* Populate configuration: */
    guiMonSel.detectedObjects           = atoi (argv[1]);
    guiMonSel.logMagRange               = atoi (argv[2]);
    guiMonSel.noiseProfile              = atoi (argv[3]);
    guiMonSel.rangeAzimuthHeatMap       = atoi (argv[4]);
    guiMonSel.rangeDopplerHeatMap       = atoi (argv[5]);
    guiMonSel.statsInfo                     = atoi (argv[6]);

    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.cliCfg.guiMonSel, (void *)&guiMonSel, sizeof(MmwDemo_GuiMonSel));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for CFAR configuration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLICfarCfg (int32_t argc, char* argv[])
{
    MmwDemo_CfarCfg   cfarCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 8)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the ADC Output configuration: */
    memset ((void *)&cfarCfg, 0, sizeof(MmwDemo_CfarCfg));

    /* Populate configuration: */
    //procDirection           = (uint8_t) atoi (argv[1]);
    cfarCfg.averageMode     = (uint8_t) atoi (argv[2]);
    cfarCfg.winLen          = (uint8_t) atoi (argv[3]);
    cfarCfg.guardLen        = (uint8_t) atoi (argv[4]);
    cfarCfg.noiseDivShift   = (uint8_t) atoi (argv[5]);
    cfarCfg.cyclicMode      = (uint8_t) atoi (argv[6]);
    cfarCfg.thresholdScale  = (uint16_t) atoi (argv[7]);

    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.dataPathObj.cliCfg->cfarCfg, (void *)&cfarCfg, sizeof(MmwDemo_CfarCfg));
    return 0;
}


/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for Peak grouping configuration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIPeakGroupingCfg (int32_t argc, char* argv[])
{
    MmwDemo_PeakGroupingCfg peakGroupingCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 6)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the ADC Output configuration: */
    memset ((void *)&peakGroupingCfg, 0, sizeof(MmwDemo_PeakGroupingCfg));

    /* Populate configuration: */
    peakGroupingCfg.scheme               = (uint8_t) atoi (argv[1]);
    peakGroupingCfg.inRangeDirectionEn   = (uint8_t) atoi (argv[2]);
    peakGroupingCfg.inDopplerDirectionEn = (uint8_t) atoi (argv[3]);
    peakGroupingCfg.minRangeIndex    = (uint16_t) atoi (argv[4]);
    peakGroupingCfg.maxRangeIndex    = (uint16_t) atoi (argv[5]);

    if (peakGroupingCfg.scheme != 1 && peakGroupingCfg.scheme != 2)
    {
        CLI_write ("Error: Invalid peak grouping scheme\n");
        return -1;
    }

    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.dataPathObj.cliCfg->peakGroupingCfg, (void *)&peakGroupingCfg,
        sizeof(MmwDemo_PeakGroupingCfg));

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for multi object beam forming configuration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIMultiObjBeamForming (int32_t argc, char* argv[])
{
    MmwDemo_MultiObjBeamFormingCfg cfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 3)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize configuration: */
    memset ((void *)&cfg, 0, sizeof(MmwDemo_PeakGroupingCfg));

    /* Populate configuration: */
    cfg.enabled                     = (uint8_t) atoi (argv[1]);
    cfg.multiPeakThrsScal   = (float) atof (argv[2]);

    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.dataPathObj.cliCfg->multiObjBeamFormingCfg, (void *)&cfg,
        sizeof(MmwDemo_MultiObjBeamFormingCfg));

    return 0;
}


/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for DC range calibration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLICalibDcRangeSig (int32_t argc, char* argv[])
{
    MmwDemo_CalibDcRangeSigCfg cfg;
    uint32_t log2NumAvgChirps;

    /* Sanity Check: Minimum argument check */
    if (argc != 5)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the ADC Output configuration: */
    memset ((void *)&cfg, 0, sizeof(MmwDemo_CalibDcRangeSigCfg));

    /* Populate configuration: */
    cfg.enabled          = (uint16_t) atoi (argv[1]);
    cfg.negativeBinIdx   = (int16_t)  atoi (argv[2]);
    cfg.positiveBinIdx   = (int16_t)  atoi (argv[3]);
    cfg.numAvgChirps     = (uint16_t)  atoi (argv[4]);

    if (cfg.negativeBinIdx > 0)
    {
        CLI_write ("Error: Invalid negative bin index\n");
        return -1;
    }
    if ((cfg.positiveBinIdx - cfg.negativeBinIdx + 1) > DC_RANGE_SIGNATURE_COMP_MAX_BIN_SIZE)
    {
        CLI_write ("Error: Number of bins exceeds the limit\n");
        return -1;
    }
    log2NumAvgChirps = (uint32_t) log2Approx (cfg.numAvgChirps);
    if (cfg.numAvgChirps != (1 << log2NumAvgChirps))
    {
        CLI_write ("Error: Number of averaged chirps is not power of two\n");
        return -1;
    }

    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.dataPathObj.cliCfg->calibDcRangeSigCfg, (void *)&cfg,
        sizeof(MmwDemo_CalibDcRangeSigCfg));
    gMmwMCB.dataPathObj.dcRangeSigCalibCntr = 0;
    gMmwMCB.dataPathObj.log2NumAvgChirps = log2NumAvgChirps;

    return 0;
}

/**
 *  @b Description
 *  @n
 *      Clutter removal Configuration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIClutterRemoval (int32_t argc, char* argv[])
{
    MmwDemo_ClutterRemovalCfg cfg;


    /* Sanity Check: Minimum argument check */
    if (argc != 2)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize configuration for clutter removal */
    memset ((void *)&cfg, 0, sizeof(MmwDemo_ClutterRemovalCfg));

    /* Populate configuration: */
    cfg.enabled = (uint16_t) atoi (argv[1]);


    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.dataPathObj.cliCfg->clutterRemovalCfg,
           (void *)&cfg, sizeof(MmwDemo_ClutterRemovalCfg));

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the ADCBUF configuration
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIADCBufCfg (int32_t argc, char* argv[])
{
    MmwDemo_ADCBufCfg    adcBufCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 5)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the ADC Output configuration: */
    memset ((void *)&adcBufCfg, 0, sizeof(MmwDemo_ADCBufCfg));

    /* Populate configuration: */
    adcBufCfg.adcFmt          = (uint8_t) atoi (argv[1]);
    adcBufCfg.iqSwapSel       = (uint8_t) atoi (argv[2]);
    adcBufCfg.chInterleave    = (uint8_t) atoi (argv[3]);
    adcBufCfg.chirpThreshold  = (uint8_t) atoi (argv[4]);

    /* Save Configuration to use later */
    memcpy((void *)&gMmwMCB.dataPathObj.cliCfg->adcBufCfg, (void *)&adcBufCfg,
        sizeof(MmwDemo_ADCBufCfg));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the sensor start command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLISensorStart (int32_t argc, char* argv[])
{

    bool doReconfig = true;
    if (argc==2)
    {
        doReconfig = (bool) atoi (argv[1]);
    }
    /* Notify the sensor management module to start the sensor */
    MmwDemo_notifySensorStart (doReconfig);
    /* Pend for completion */
    return (MmwDemo_waitSensorStartComplete());
}


/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the sensor stop command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLISensorStop (int32_t argc, char* argv[])
{
    /* Notify the sensor management module to stop the sensor */
    MmwDemo_notifySensorStop ();
    /* Pend for completion */
    MmwDemo_waitSensorStopComplete();

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for compensation of range bias and channel phase offsets
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLICompRangeBiasAndRxChanPhaseCfg (int32_t argc, char* argv[])
{
    MmwDemo_compRxChannelBiasCfg_t   cfg;
    int32_t Re, Im;
    int32_t argInd;
    int32_t i;

    /* Sanity Check: Minimum argument check */
    if (argc != (1+1+SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL*2))
    {
        CLI_write ("Error: Invalid usage of the CLI command, argc = %d, actual = %d \n", argc, (1+1+SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL*2));
        return -1;
    }

    /* Initialize configuration: */
    memset ((void *)&cfg, 0, sizeof(MmwDemo_compRxChannelBiasCfg_t));

    /* Populate configuration: */
    cfg.rangeBias          = (float) atof (argv[1]);

    argInd = 2;
    for (i=0; i < SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL; i++)
    {
        Re = (int32_t) (atof (argv[argInd++]) * 32768.);
        Re = MMWDEMO_SATURATE_HIGH(Re);
        Re = MMWDEMO_SATURATE_LOW(Re);
        cfg.rxChPhaseComp[i].real = (int16_t) Re;

        Im = (int32_t) (atof (argv[argInd++]) * 32768.);
        Im = MMWDEMO_SATURATE_HIGH(Im);
        Im = MMWDEMO_SATURATE_LOW(Im);
        cfg.rxChPhaseComp[i].imag = (int16_t) Im;

    }
    /* Save Configuration to use later */
    memcpy((void *) &gMmwMCB.cliCommonCfg.compRxChanCfg, &cfg, sizeof(MmwDemo_compRxChannelBiasCfg_t));

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for measurement configuration of range bias
 *      and channel phase offsets
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIMeasureRangeBiasAndRxChanPhaseCfg (int32_t argc, char* argv[])
{
    MmwDemo_measureRxChannelBiasCfg_t   cfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 4)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize configuration: */
    memset ((void *)&cfg, 0, sizeof(MmwDemo_measureRxChannelBiasCfg_t));

    /* Populate configuration: */
    cfg.enabled          = (uint8_t) atoi (argv[1]);
    cfg.targetDistance   = (float) atof (argv[2]);
    cfg.searchWinSize   = (float) atof (argv[3]);

    /* Save Configuration to use later */
    memcpy((void *) &gMmwMCB.cliCommonCfg.measureRxChanCfg, &cfg, sizeof(MmwDemo_measureRxChannelBiasCfg_t));

    return 0;
}


/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for enabling CQ monitor
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIChirpQualityRxSatMonCfg (int32_t argc, char* argv[])
{
    rlRxSatMonConf_t      cqSatMonCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 6)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize configuration: */
    memset ((void *)&cqSatMonCfg, 0, sizeof(rlRxSatMonConf_t));

    /* Populate configuration: */
    cqSatMonCfg.profileIndx                 = (uint8_t) atoi (argv[1]);

    if(cqSatMonCfg.profileIndx >= RL_MAX_PROFILES_CNT)
        return -1;

    cqSatMonCfg.satMonSel                   = (uint8_t) atoi (argv[2]);
    cqSatMonCfg.primarySliceDuration        = (uint16_t) atoi (argv[3]);
    cqSatMonCfg.numSlices                   = (uint16_t) atoi (argv[4]);
    cqSatMonCfg.rxChannelMask               = (uint8_t) atoi (argv[5]);

    /* Save Configuration to use later */
    memcpy((void *) &gMmwMCB.cliCommonCfg.cqSatMonCfg[cqSatMonCfg.profileIndx],
                   &cqSatMonCfg,
                   sizeof(rlRxSatMonConf_t));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for enabling CQ monitor
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIChirpQualitySigImgMonCfg (int32_t argc, char* argv[])
{
    rlSigImgMonConf_t      cqSigImgMonCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 4)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize configuration: */
    memset ((void *)&cqSigImgMonCfg, 0, sizeof(rlSigImgMonConf_t));

    /* Populate configuration: */
    cqSigImgMonCfg.profileIndx              = (uint8_t) atoi (argv[1]);

    if(cqSigImgMonCfg.profileIndx >= RL_MAX_PROFILES_CNT)
        return -1;

    cqSigImgMonCfg.numSlices        = (uint16_t) atoi (argv[2]);
    cqSigImgMonCfg.timeSliceNumSamples = (uint16_t) atoi (argv[3]);

    /* Save Configuration to use later */
    memcpy((void *) &gMmwMCB.cliCommonCfg.cqSigImgMonCfg[cqSigImgMonCfg.profileIndx],
            &cqSigImgMonCfg,
            sizeof(rlSigImgMonConf_t));

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for enabling analog monitors
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_CLIAnalogMonitorCfg (int32_t argc, char* argv[])
{
    /* Sanity Check: Minimum argument check */
    if (argc != 3)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Save Configuration to use later */
    gMmwMCB.cliCommonCfg.anaMonCfg.rxSatMonEn = atoi (argv[1]);
    gMmwMCB.cliCommonCfg.anaMonCfg.sigImgMonEn = atoi (argv[2]);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Execution Task
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_CLIInit (void)
{
    CLI_Cfg     cliCfg;
    uint32_t    cnt = 0;
    char        demoBanner[256];

    /* Create Demo Banner to be printed out by CLI */
    sprintf(&demoBanner[0],
                       "******************************************\n" \
                       "xWR14xx MMW Demo %02d.%02d.%02d.%02d\n"  \
                       "******************************************\n",
                       MMWAVE_SDK_VERSION_MAJOR,
                       MMWAVE_SDK_VERSION_MINOR,
                       MMWAVE_SDK_VERSION_BUGFIX,
                       MMWAVE_SDK_VERSION_BUILD
            );


    /* Initialize the CLI configuration: */
    memset ((void *)&cliCfg, 0, sizeof(CLI_Cfg));

    /* Populate the CLI configuration: */
    cliCfg.cliPrompt                    = "mmwDemo:/>";
    cliCfg.cliBanner                    = demoBanner;
    cliCfg.cliUartHandle                = gMmwMCB.commandUartHandle;
    cliCfg.taskPriority                 = 3;
    cliCfg.enableMMWaveExtension        = 1U;
    cliCfg.usePolledMode                = true;
    cliCfg.mmWaveHandle                 = gMmwMCB.ctrlHandle;
    cliCfg.tableEntry[cnt].cmd            = "sensorStart";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "[doReconfig(optional, default:enabled)]";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLISensorStart;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "sensorStop";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "No arguments";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLISensorStop;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "guiMonitor";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<detectedObjects> <logMagRange> <noiseProfile> <rangeAzimuthHeatMap> <rangeDopplerHeatMap> <statsInfo>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIGuiMonSel;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "cfarCfg";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<procDirection> <averageMode> <winLen> <guardLen> <noiseDiv> <cyclicMode> <thresholdScale>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLICfarCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "peakGrouping";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<groupingMode> <rangeDimEn> <dopplerDimEn> <startRangeIdx> <endRangeIdx>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIPeakGroupingCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "multiObjBeamForming";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<enabled> <threshold>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIMultiObjBeamForming;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "calibDcRangeSig";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<enabled> <negativeBinIdx> <positiveBinIdx> <numAvgFrames>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLICalibDcRangeSig;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "clutterRemoval";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<enabled>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIClutterRemoval;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "adcbufCfg";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<adcOutputFmt> <SampleSwap> <ChanInterleave> <ChirpThreshold>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIADCBufCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "compRangeBiasAndRxChanPhase";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<rangeBias> <Re00> <Im00> <Re01> <Im01> <Re02> <Im02> <Re03> <Im03> <Re10> <Im10> <Re11> <Im11> <Re12> <Im12> <Re13> <Im13> ";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLICompRangeBiasAndRxChanPhaseCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "measureRangeBiasAndRxChanPhase";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<enabled> <targetDistance> <searchWin>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIMeasureRangeBiasAndRxChanPhaseCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "CQRxSatMonitor";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<profile> <satMonSel> <priSliceDuration> <numSlices> <rxChanMask>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIChirpQualityRxSatMonCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "CQSigImgMonitor";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<profile> <numSlices> <numSamplePerSlice>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIChirpQualitySigImgMonCfg;
    cnt++;

    cliCfg.tableEntry[cnt].cmd            = "analogMonitor";
#if 0
    cliCfg.tableEntry[cnt].helpString     = "<rxSaturation> <sigImgBand>";
#else
    cliCfg.tableEntry[cnt].helpString     = NULL;
#endif
    cliCfg.tableEntry[cnt].cmdHandlerFxn  = MmwDemo_CLIAnalogMonitorCfg;
    
    /* Open the CLI: */
    if (CLI_open (&cliCfg) < 0)
    {
        //System_printf ("Error: Unable to open the CLI\n");
        MmwDemo_debugAssert (0);
        return;
    }
    return;
}

