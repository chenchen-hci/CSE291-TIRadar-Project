/**
 *   @file  data_path.h
 *
 *   @brief
 *      Header for data_path
 *
 *  \par

 *  NOTE:
 * Copyright (c) 2018 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license
 * under copyrights and patents it now or hereafter owns or controls to make, have made, use,
 * import, offer to sell and sell ("Utilize") this software subject to the terms herein.
 *
 * With respect to the foregoing patent license, such license is granted  solely to the extent
 * that any such patent is necessary to Utilize the software alone.  The patent license shall
 * not apply to any combinations which include this software, other than combinations with
 * devices manufactured by or for TI (“TI Devices”). No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source code
 * license limitations below) in the documentation and/or other materials provided with the
 * distribution.
 *
 * Redistribution and use in binary form, without modification, are permitted provided that
 * the following conditions are met:
 *
 * No reverse engineering, decompilation, or disassembly of this software is permitted with
 * respect to any software provided in binary form. Any redistribution and use are licensed
 * by TI for use only with TI Devices. Nothing shall obligate TI to provide you with source
 * code for the software licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the source
 * code are permitted provided that the following conditions are met:
 *
 * Any redistribution and use of the source code, including any resulting derivative works,
 * are licensed by TI for use only with TI Devices.
 * Any redistribution and use of any object code compiled from the source code and any
 * resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be
 * used to endorse or promote products derived from this software without specific prior
 * written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S
 * LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef DATA_PATH_H
#define DATA_PATH_H

#include <ti/sysbios/knl/Semaphore.h>

#include <ti/common/sys_common.h>
#include <ti/common/mmwave_error.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/demo/io_interface/detected_obj.h>
#include <ti/demo/io_interface/mmw_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PI_ 3.14159265


#define BYTES_PER_SAMP_1D (2*sizeof(int16_t))  /*16 bit real, 16 bit imaginary => 4 bytes */
#define BYTES_PER_SAMP_2D (2*sizeof(int32_t))  /*32 bit real, 32 bit imaginary => 8 bytes */
#define BYTES_PER_SAMP_DET sizeof(uint16_t) /*pre-detection matrix is 16 bit unsigned =>2 bytes*/

//DETECTION (CFAR-CA) related parameters
#define MMW_MAX_OBJ_OUT 100
#define DET_THRESH_MULT 25
#define DET_THRESH_SHIFT 5 //DET_THRESH_MULT and DET_THRESH_SHIFT together define the CFAR-CA threshold
#define DET_GUARD_LEN 4 // this is the one sided guard lenght
#define DET_NOISE_LEN 16 //this is the one sided noise length

#define ONE_Q15 (1 << 15)
#define ONE_Q19 (1 << 19)
#define ONE_Q8 (1 << 8)

/* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS

/* 1D */
#define MMW_HWA_1D_ADCBUF_INP              (&gMmwHwaMemBuf[0])

/* 1D -ping */
#define MMW_HWA_DMA_TRIGGER_SOURCE_1D_PING  0
#define MMW_HWA_DMA_DEST_CHANNEL_1D_PING    0
#define MMW_HWA_1D_OUT_PING                (&gMmwHwaMemBuf[2])
#define MMW_EDMA_1D_PING_CH_ID              EDMA_TPCC0_REQ_HWACC_0
#define MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID  EDMA_SHADOW_LNK_PARAM_BASE_ID
#define MMW_EDMA_1D_PING_CHAIN_CH_ID        EDMA_TPCC0_REQ_FREE_0
#define MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 2)

/* 1D - pong */
#define MMW_HWA_DMA_TRIGGER_SOURCE_1D_PONG   1
#define MMW_HWA_DMA_DEST_CHANNEL_1D_PONG     1
#define MMW_HWA_1D_OUT_PONG                 (&gMmwHwaMemBuf[3])
#define MMW_EDMA_1D_PONG_CH_ID               EDMA_TPCC0_REQ_HWACC_1
#define MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID  (EDMA_SHADOW_LNK_PARAM_BASE_ID + 1)
#define MMW_EDMA_1D_PONG_CHAIN_CH_ID         EDMA_TPCC0_REQ_FREE_1
#define MMW_EDMA_1D_PONG_ONE_HOT_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 3)

#define MMWDEMO_EDMA_TRANSFER_COMPLETION_CODE_1D_DONE   MMW_EDMA_1D_PONG_CHAIN_CH_ID

/* 2D */

/* 2D - ping */
#define MMW_HWA_DMA_TRIGGER_SOURCE_2D_PING   2
#define MMW_HWA_DMA_DEST_CHANNEL_2D_PONG     3
#define MMW_HWA_2D_INP_PING                 (&gMmwHwaMemBuf[0])
#define MMW_HWA_2D_OUT_PING                 (&gMmwHwaMemBuf[2])
#define MMW_HWA_DMA_DEST_CHANNEL_2D_PING     2
#define MMW_EDMA_2D_PING_CH_ID               EDMA_TPCC0_REQ_HWACC_2
#define MMW_EDMA_2D_PING_SHADOW_LINK_CH_ID1  (EDMA_SHADOW_LNK_PARAM_BASE_ID + 4)
#define MMW_EDMA_2D_PING_CHAIN_CH_ID1        EDMA_TPCC0_REQ_FREE_2
#define MMW_EDMA_2D_PING_SHADOW_LINK_CH_ID2  (EDMA_SHADOW_LNK_PARAM_BASE_ID + 5)
#define MMW_EDMA_2D_PING_CHAIN_CH_ID2        EDMA_TPCC0_REQ_FREE_3
#define MMW_EDMA_2D_PING_SHADOW_LINK_CH_ID3 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 6)
#define MMW_EDMA_2D_PING_CHAIN_CH_ID3        EDMA_TPCC0_REQ_FREE_4
#define MMW_EDMA_2D_PING_SHADOW_LINK_CH_ID4 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 7)

/* 2D - pong */
#define MMW_HWA_DMA_TRIGGER_SOURCE_2D_PONG   3
#define MMW_HWA_2D_INP_PONG                 (&gMmwHwaMemBuf[1])
#define MMW_HWA_2D_OUT_PONG                 (&gMmwHwaMemBuf[3])
#define MMW_EDMA_2D_PONG_CH_ID               EDMA_TPCC0_REQ_HWACC_3
#define MMW_EDMA_2D_PONG_SHADOW_LINK_CH_ID1 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 8)
#define MMW_EDMA_2D_PONG_CHAIN_CH_ID1        EDMA_TPCC0_REQ_FREE_5
#define MMW_EDMA_2D_PONG_SHADOW_LINK_CH_ID2  (EDMA_SHADOW_LNK_PARAM_BASE_ID + 9)
#define MMW_EDMA_2D_PONG_CHAIN_CH_ID2        EDMA_TPCC0_REQ_FREE_6
#define MMW_EDMA_2D_PONG_SHADOW_LINK_CH_ID3 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 10)
#define MMW_EDMA_2D_PONG_CHAIN_CH_ID3        EDMA_TPCC0_REQ_FREE_7
#define MMW_EDMA_2D_PONG_SHADOW_LINK_CH_ID4 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 11)

#define MMWDEMO_EDMA_TRANSFER_COMPLETION_CODE_2D_DONE    MMW_EDMA_2D_PONG_CHAIN_CH_ID2

/* CFAR */
#define MMW_HWA_DMA_TRIGGER_SOURCE_CFAR      4
#define MMW_HWA_DMA_DEST_CHANNEL_CFAR        4
#define MMW_HWA_CFAR_INP                    (&gMmwHwaMemBuf[0])
#define MMW_HWA_CFAR_OUT                    (&gMmwHwaMemBuf[2])
#define MMW_EDMA_CFAR_INP_CH_ID              EDMA_TPCC0_REQ_FREE_8
#define MMW_EDMA_CFAR_INP_SHADOW_LINK_CH_ID1 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 12)
#define MMW_EDMA_CFAR_INP_CHAIN_CH_ID        EDMA_TPCC0_REQ_FREE_9
#define MMW_EDMA_CFAR_INP_SHADOW_LINK_CH_ID2 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 13)
#define MMW_EDMA_CFAR_OUT_CH_ID              EDMA_TPCC0_REQ_HWACC_4
#define MMW_EDMA_CFAR_OUT_SHADOW_LINK_CH_ID1 (EDMA_SHADOW_LNK_PARAM_BASE_ID + 14)
#define MMW_EDMA_CFAR_OUT_CHAIN_CH_ID        EDMA_TPCC0_REQ_HWACC_4

#define MMWDEMO_EDMA_TRANSFER_COMPLETION_CODE_CFAR_DONE  MMW_EDMA_CFAR_OUT_CHAIN_CH_ID

/* Single range bin 2D FFT */
#define MMW_HWA_2DFFT_SINGLERBIN_INP              (&gMmwHwaMemBuf[2])
#define MMW_HWA_2DFFT_SINGLERBIN_OUT              (&gMmwHwaMemBuf[3])
#define MMW_HWA_DMA_TRIGGER_SINGLEBIN_2DFFT    5

#define MMW_EDMA_2DFFT_SINGLERBIN_CH_ID            EDMA_TPCC0_REQ_FREE_10
#define MMW_EDMA_2DFFT_SINGLERBIN_CHAIN_CH_ID      EDMA_TPCC0_REQ_FREE_11

#define MMW_EDMA_2DFFT_SINGLERBIN_SHADOW_LINK_CH_ID          (EDMA_SHADOW_LNK_PARAM_BASE_ID + 15)
#define MMW_EDMA_2DFFT_SINGLERBIN_SHADOW_LINK_CH_ID2          (EDMA_SHADOW_LNK_PARAM_BASE_ID + 16)

/* ANGLE */
#define MMW_HWA_ANGLE_AZIM_INP              (&gMmwHwaMemBuf[0])
#define MMW_HWA_ANGLE_ELEV_INP              (&gMmwHwaMemBuf[1])
#define MMW_HWA_ANGLE_AZIM_ABS_OUT          (&gMmwHwaMemBuf[2])
#define MMW_HWA_ANGLE_ELEV_CPLX_OUT         (&gMmwHwaMemBuf[3])
#define MMW_HWA_ANGLE_AZIM_CPLX_OUT         (&gMmwHwaMemBuf[1])

#define MMW_HWA_START_POS_PARAMSETS_1D       0
#define MMW_HWA_START_POS_PARAMSETS_2D      (MMW_HWA_START_POS_PARAMSETS_1D + HWAUTIL_NUM_PARAM_SETS_1D)
#define MMW_HWA_START_POS_PARAMSETS_CFAR    (MMW_HWA_START_POS_PARAMSETS_2D + HWAUTIL_NUM_PARAM_SETS_CFAR)
#define MMW_HWA_START_POS_PARAMSETS_ANGLE   (MMW_HWA_START_POS_PARAMSETS_CFAR + HWAUTIL_NUM_PARAM_SETS_CFAR)

#define MMW_HWA_WINDOWRAM_1D_OFFSET         0 //In samples

#define MMW_HWA_MAX_CFAR_DET_OBJ_LIST_SIZE (SOC_XWR14XX_MSS_HWA_MEM_SIZE/MMW_HWA_NUM_MEM_BUFS)/sizeof(cfarDetOutput_t)

/* FFT Window */
/*! Hanning window */
#define MMW_WIN_HANNING  0
/*! Blackman window */
#define MMW_WIN_BLACKMAN 1
/*! Rectangular window */
#define MMW_WIN_RECT     2

#define ONE_Q17 (1U << 17)

/*! @brief Azimuth FFT size */
#define MMW_NUM_ANGLE_BINS 64

/* number of range gates processed by the HWACC per iteration (during 2D FFT processing) */
#define MMW_NUM_RANGE_BINS_PER_TRANSFER 2

/* Maximum number of elevation objects that are stored for debugging*/
#define MMW_MAX_ELEV_OBJ_DEBUG 10

#define MMWDEMO_SPEED_OF_LIGHT_IN_METERS_PER_SEC (3.0e8)

/* CFAR tuning parameters */
#define MMW_HWA_NOISE_AVG_MODE             HWA_NOISE_AVG_MODE_CFAR_CASO
#define MMW_HWA_CFAR_THRESHOLD_SCALE       0x4b0
#define MMW_HWA_CFAR_WINDOW_LEN            8
#define MMW_HWA_CFAR_GUARD_LEN             4
#define MMW_HWA_CFAR_NOISE_DIVISION_RIGHT_SHIFT 3
#define MMW_HWA_CFAR_PEAK_GROUPING         HWA_FEATURE_BIT_DISABLE


/*! @brief Flag to enable/disable two peak detection in azimuth for same range and velocity */
#define MMWDEMO_AZIMUTH_TWO_PEAK_DETECTION_ENABLE 1
/*! @brief Threshold for two peak detection in azimuth for same range and velocity,
 *         if 2nd peak heigth > first peak height * this scale then declare
 *         2nd peak as detected. */
#define MMWDEMO_AZIMUTH_TWO_PEAK_THRESHOLD_SCALE  (0.5)

/* To enable negative frequency slope support, uncomment the following definition.
 * Note that this is experimental and not tested option */
/* #define MMW_ENABLE_NEGATIVE_FREQ_SLOPE */


/*!
 *  @brief      Data path mode
 *
 *  This enum defines if the data path chain is running standalone (such as in a unit test)
 *  or if it is running as part of a demo (integrated with ADC buffer)
 */
typedef enum
{
    /*!
      *  Data path running standalone
      */
    DATA_PATH_STANDALONE,
    /*!
      *  Data path integrated with ADC buffer
      */
    DATA_PATH_WITH_ADCBUF
} DataPath_mode;

/*!
 *  @brief      Data path chain selection
 *
 *  This enum defines the data path chain selection options for 2D FFT
 */
typedef enum DataPath_chain2DFftSel_e
{
    /*!
      *  Data path chain that runs separate 2D FFT and Log-mag Calcuation in the HWA.
      */
    DATA_PATH_CHAIN_SEPARATE_LOGMAG = 0U,
    /*!
      *  Data path chain that runs log-mag together with 2D FFT in the HWA.
      */
    DATA_PATH_CHAIN_COMBINED_LOGMAG
} DataPath_chain2DFftSel;

/*!
 *  @brief    Detected object parameters filled by HWA CFAR
 *
 */
typedef volatile struct cfarDetOutput
{
    uint32_t   noise;           /*!< Noise energy in CFAR cell */
    uint32_t   rangeIdx : 12;   /*!< Range index */
    uint32_t   dopplerIdx : 20; /*!< Doppler index */
} cfarDetOutput_t;

/*!
 *  @brief Timing information
 */
typedef struct MmwDemo_timingInfo
{
    /*! @brief number of processor cycles between frames excluding
           processing time to transmit output on UART */
    uint32_t interFrameProcCycles;

     /*! @brief number of processor cycles to transmit detected object information
           on the output UART port, which is not presently back-grounded. */
    uint32_t transmitOutputCycles;

    /*! @brief Inter frame processing end time */
    uint32_t interFrameProcessingEndTime;

    /*! @brief Inter frame processing end margin in number of cycles before
     * due time to start processing first chirp of the next frame */
    uint32_t interFrameProcessingEndMargin;

    /*! @brief CPU Load during active frame period - i.e. chirping */
    uint32_t activeFrameCPULoad;

    /*! @brief CPU Load during inter frame period - i.e. after chirps
     *  are done and before next frame starts */
    uint32_t interFrameCPULoad;

} MmwDemo_timingInfo_t;

/**
 * @brief
 *  Millimeter Wave Demo Data Path Information.
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path.
 */
typedef struct MmwDemo_DataPathObj_t
{
    /*! Pointer to cliCfg */
    MmwDemo_CliCfg_t *cliCfg;

    /*! @brief Pointer to cli config common to all subframes*/
    MmwDemo_CliCommonCfg_t *cliCommonCfg;

    /*! @brief   Number of receive channels */
    uint32_t numRxAntennas;

    /*! @brief   ADCBUF handle. */
    ADCBuf_Handle adcbufHandle;

    /*! @brief   Handle of the EDMA driver. */
    EDMA_Handle edmaHandle;

    /*! @brief   EDMA error Information when there are errors like missing events */
    EDMA_errorInfo_t  EDMA_errorInfo;

    /*! @brief EDMA transfer controller error information. */
    EDMA_transferControllerErrorInfo_t EDMA_transferControllerErrorInfo;

    /*! @brief Semaphore handle for 1D EDMA completion. */
    Semaphore_Handle EDMA_1Ddone_semHandle;

    /*! @brief Semaphore handle for 2D EDMA completion. */
    Semaphore_Handle EDMA_2Ddone_semHandle;

    /*! @brief Semaphore handle for CFAR EDMA completion. */
    Semaphore_Handle EDMA_CFARdone_semHandle;

    /*! @brief Handle to hardware accelerator driver. */
    HWA_Handle  hwaHandle;

    /*! @brief Semaphore handle for Hardware accelerator completion. */
    Semaphore_Handle HWA_done_semHandle;

    /*! @brief Hardware accelerator Completion Isr count for debug purposes. */
    uint32_t hwaDoneIsrCounter;

    /*! @brief Frame counter incremented in frame start interrupt handler*/
    uint32_t frameStartIntCounter;

    /*! @brief Semaphore handle for Frame start indication. */
    Semaphore_Handle frameStart_semHandle;

    /*! @brief Number of CFAR detections by Hardware accelerator for debug purposes. */
    uint32_t numHwaCfarDetections;

    /*! @brief Number of detected objects. */
    uint32_t numObjOut;

    /*! @brief output object array */
    MmwDemo_detectedObj objOut[MMW_MAX_OBJ_OUT];

    /*! @brief noise energy */
    uint32_t noiseEnergy;

    /*! @brief Pointer to Radar Cube memory in L3 RAM */
    uint32_t *radarCube;

    /*! @brief Pointer to range-doppler log magnitude matrix memory in L3 RAM */
    uint16_t *rangeDopplerLogMagMatrix;

    /*! @brief pointer to CFAR detection output in L3 RAM */
    cfarDetOutput_t *cfarDetectionOut;

    /*! @brief valid Profile index */
    uint32_t validProfileIdx;

    /*! @brief number of transmit antennas */
    uint32_t numTxAntennas;

    /*! @brief number of virtual antennas */
    uint32_t numVirtualAntennas;

    /*! @brief number of virtual azimuth antennas */
    uint32_t numVirtualAntAzim;

    /*! @brief number of virtual elevation antennas */
    uint32_t numVirtualAntElev;

    /*! @brief number of ADC samples */
    uint32_t numAdcSamples;

    /*! @brief number of range bins */
    uint32_t numRangeBins;

    /*! @brief number of chirps per frame */
    uint32_t numChirpsPerFrame;

    /*! @brief number of angle bins */
    uint32_t numAngleBins;

    /*! @brief number of doppler bins */
    uint32_t numDopplerBins;

    /*! @brief number of range bins per transfer */
    uint32_t numRangeBinsPerTransfer;

    /*! @brief range resolution in meters */
    float rangeResolution;

    /*! @brief Q format of the output x/y/z coordinates */
    uint32_t xyzOutputQFormat;

    /*! @brief Timing information */
    MmwDemo_timingInfo_t timingInfo;

	/*! @brief Data path mode */
	DataPath_mode    dataPathMode;

	/*! @brief Detected objects azimuth index for debugging */
	uint8_t detObj2dAzimIdx[MMW_MAX_OBJ_OUT];
    
	/*! @brief Detected object elevation angle for debugging */
    float detObjElevationAngle[MMW_MAX_ELEV_OBJ_DEBUG];

    /*! @brief  Used for checking that inter frame processing finshed on time */
    int32_t interFrameProcToken;


    /*! @brief Pointer to DC range signature compensation buffer */
    cmplx32ImRe_t *dcRangeSigMean;

    /*! @brief DC range signature calibration counter */
    uint32_t dcRangeSigCalibCntr;

    /*! @brief DC range signature calibration forced disable counter */
    uint32_t dcRangeForcedDisableCntr;


    /*! @brief log2 of number of averaged chirps */
    uint32_t log2NumAvgChirps;

    /*! @brief data path chain selection */
    DataPath_chain2DFftSel datapathChainSel;

    /*! @brief Rx channel gain/phase offset compensation coefficients */
    MmwDemo_compRxChannelBiasCfg_t compRxChanCfg;

} MmwDemo_DataPathObj;

/*! simple memory pool structure for flexible partitioning of L3 RAM */
typedef struct {
    /*! base address of memory */
    uint8_t *base;

    /*! size of memory in bytes */
    uint32_t size;

    /*! index into the pool to track where is next free */
    uint32_t indx;
} MmwDemoMemPool_t;


/*! Partiion HWA memory into 4 equal parts - M0,M1,M2,M3. */
#define MMW_HWA_NUM_MEM_BUFS 4
typedef struct {
    uint8_t buf[SOC_XWR14XX_MSS_HWA_MEM_SIZE/MMW_HWA_NUM_MEM_BUFS];
} mmwHwaBuf_t;

void MmwDemo_hwaInit(MmwDemo_DataPathObj *obj);
void MmwDemo_edmaInit(MmwDemo_DataPathObj *obj);
void MmwDemo_hwaOpen(MmwDemo_DataPathObj *obj, SOC_Handle socHandle);
void MmwDemo_edmaOpen(MmwDemo_DataPathObj *obj);
void MmwDemo_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);
void MmwDemo_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);

void MmwDemo_memPoolReset(MmwDemoMemPool_t *pool);
uint8_t *MmwDemo_memPoolAlloc(MmwDemoMemPool_t *pool, uint32_t size);

void MmwDemo_dataPathObjInit(MmwDemo_DataPathObj *obj,
                             MmwDemo_CliCfg_t *cliCfg,
                             MmwDemo_CliCommonCfg_t *cliCommonCfg);
void MmwDemo_dataPathCfgBuffers(MmwDemo_DataPathObj *obj, MmwDemoMemPool_t *pool);

void MmwDemo_dataPathConfigCommon(MmwDemo_DataPathObj *obj);
void MmwDemo_dcRangeSignatureCompensation(MmwDemo_DataPathObj *obj);
void MmwDemo_process2D(MmwDemo_DataPathObj *obj);
void MmwDemo_processCfar(MmwDemo_DataPathObj *obj, uint16_t *numDetectedObjects);
void MmwDemo_processAngle(MmwDemo_DataPathObj *obj);


void MmwDemo_config1D_HWA(MmwDemo_DataPathObj *obj);
void MmwDemo_dataPathTrigger1D(MmwDemo_DataPathObj *obj);
void MmwDemo_dataPathWait1D(MmwDemo_DataPathObj *obj);

void MmwDemo_configAngleEstimation_HWA(MmwDemo_DataPathObj *obj, uint32_t numObjOut);
void MmwDemo_dataPathTriggerAngleEstimation(MmwDemo_DataPathObj *obj);
void MmwDemo_dataPathWaitAngleEstimation(MmwDemo_DataPathObj *obj);

void MmwDemo_edmaClose(MmwDemo_DataPathObj *obj);
void MmwDemo_dataPathDeleteSemaphore(MmwDemo_DataPathObj *obj);
void MmwDemo_hwaClose(MmwDemo_DataPathObj *obj);


uint32_t MmwDemo_pow2roundup (uint32_t x);
#ifdef __cplusplus
}
#endif

#endif /* DATA_PATH_H */

