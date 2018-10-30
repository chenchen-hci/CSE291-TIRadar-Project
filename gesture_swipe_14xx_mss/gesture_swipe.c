/**
 *   @file  gesture_swipe.c
 *
 *   @brief
 *      Gesture detection for swipe
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
 
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include "data_path.h"
#include "gesture_swipe.h"


//CAUTION: should be equal to numDopplerBins
/*the implementaion of Gesture_findNumDetections needs to be changed to
directly use the variable numDopplerBins.(Need to remove 2d array based implementation*/


volatile uint16_t dbgVolatile=0;
void dbgDummyFunc()
{
	dbgVolatile=1;
	dbgVolatile=2;
}

void Gesture_findNumDetections(uint16_t numRangeBins, uint16_t numDopplerBins,uint16_t *preDetMatrix,uint16_t detThresh,float * gestureMetrics, uint16_t *maxIndices )
{
	float averagedispvel=0,averagedisprange=0;//CAUTION: Need to check if there is a possibility of overflow
	
	//float range_res=0.046875,vel_res=0.0380479;


	//tempPtr=preDetMatrixPtr+numDopplerBins*NUM_INITIAL_RANGE_BINS_SKIP+NUM_INITIAL_DOPPLER_BINS_SKIP;
	uint16_t wt,maxVal=0,maxRangeIdx=0,maxDopplerIdx=0;
	uint16_t numDetections=0,numDetectionsNeg=0,numDetectionsPos=0;
	int dopplerAvg=0,rangeAvg=0,wtSum=0;
	int dopplerAvgNeg=0,rangeAvgNeg=0,wtSumNeg=0;//CAUTION: Need to check if there is a possibility of overflow
	int dopplerAvgPos=0,rangeAvgPos=0,wtSumPos=0;
	short i,j;
	uint16_t valatindx = 0;


	//tempPtr=preDetMatrixPtr+numDopplerBins*NUM_INITIAL_RANGE_BINS_SKIP+NUM_INITIAL_DOPPLER_BINS_SKIP;
	//Computing statistics for positive dopplers
	for(i=NUM_INITIAL_RANGE_BINS_SKIP;i<MAX_RANGE_BIN_USED ;i++)
	{

		for(j=NUM_INITIAL_DOPPLER_BINS_SKIP+1;j<MAX_DOPPLER_BIN_USED_ONE_SIDED+1;j++)//+1 to include the zero doppler bin
		{
			valatindx = preDetMatrix[i*numDopplerBins+j];
			if((valatindx>detThresh))
			{
				numDetectionsPos++;
				wt=valatindx;
				dopplerAvgPos+=wt*j;
				rangeAvgPos+=(wt+4000)*i;
				wtSumPos+=wt;
			}

			if(valatindx>maxVal)
			{
				maxVal=valatindx;
				maxRangeIdx=i;
				maxDopplerIdx=j;
			}
		}
	}

	//Computing statistics for negative dopplers
	for(i=NUM_INITIAL_RANGE_BINS_SKIP;i<MAX_RANGE_BIN_USED ;i++)
	{
		for(j=numDopplerBins-MAX_DOPPLER_BIN_USED_ONE_SIDED;j<numDopplerBins-NUM_INITIAL_DOPPLER_BINS_SKIP;j++)
		{
			valatindx = preDetMatrix[i*numDopplerBins+j];
			if((valatindx>detThresh))
			{
				numDetectionsNeg++;
				wt=valatindx;
				dopplerAvgNeg+=wt*(j-numDopplerBins);
				rangeAvgNeg+=(wt+4000)*i;
				wtSumNeg+=wt;
			}

			if(valatindx>maxVal)
			{
				maxVal=valatindx;
				maxRangeIdx=i;
				maxDopplerIdx=j;
			}


		}

	}

	numDetections=numDetectionsPos+numDetectionsNeg;
	wtSum=wtSumPos+wtSumNeg;
	dopplerAvg=dopplerAvgPos+dopplerAvgNeg;
	rangeAvg=rangeAvgPos+rangeAvgNeg;

	if(wtSum>0)
	{
		dopplerAvg=dopplerAvg/wtSum;
		rangeAvg=rangeAvg/wtSum;
	}

	for(i=NUM_INITIAL_RANGE_BINS_SKIP;i<MAX_RANGE_BIN_USED ;i++)
	{
		for(j=NUM_INITIAL_DOPPLER_BINS_SKIP+1;j<MAX_DOPPLER_BIN_USED_ONE_SIDED+1;j++)//+1 to include the zero doppler bin
		{
				valatindx = preDetMatrix[i*numDopplerBins+j];
				wt=valatindx;
				averagedispvel+=(j-dopplerAvg)*(j-dopplerAvg)*wt;
				averagedisprange+=(i-rangeAvg)*(i-rangeAvg)*(wt);

		}
	}

	for(i=NUM_INITIAL_RANGE_BINS_SKIP;i<MAX_RANGE_BIN_USED ;i++)
	{
		for(j=numDopplerBins-MAX_DOPPLER_BIN_USED_ONE_SIDED;j<numDopplerBins-NUM_INITIAL_DOPPLER_BINS_SKIP;j++)
		{

				valatindx = preDetMatrix[i*numDopplerBins+j];
				wt=valatindx;
				averagedispvel+=(j-numDopplerBins-dopplerAvg)*(j-numDopplerBins-dopplerAvg)*wt;
				averagedisprange+=(i-rangeAvg)*(i-rangeAvg)*(wt);


		}

	}

	if(wtSum>0)
	{
		averagedisprange = sqrt(averagedisprange/wtSum);
		averagedispvel = sqrt(averagedispvel/wtSum);
	}
	
	
	//storing the range and doppler index corresponding to the maximum in the predetection matrix
	maxIndices[0]=maxRangeIdx;
	maxIndices[1]=maxDopplerIdx;


	gestureMetrics[0]=11;
	gestureMetrics[1]=numDetections;
	gestureMetrics[2]=dopplerAvg;
	gestureMetrics[3]=rangeAvg;
	gestureMetrics[4]=wtSum;//reduce the value by dividing by 500 to fit it in 16-bit
	gestureMetrics[9]=averagedisprange;
	gestureMetrics[10]=averagedispvel;

	if(numDetections>120)
	{
		dbgDummyFunc();
	}



}
extern mmwHwaBuf_t gMmwHwaMemBuf[MMW_HWA_NUM_MEM_BUFS];
short Gesture_angleEst(MmwDemo_DataPathObj *obj,uint16_t *maxIndices)
{
    uint16_t rangeIdx, dopplerIdx,numDopplerBins,numVirtualAntAzim,numVirtualAntElev,numVirtualAnt,j;
    uint16_t maxValAngleFFT,tempValAngleFFT,*azimFFTAbsPtr,numAngleBins;
    short maxIdxAngleFFT;
    uint32_t *srcPtr,*dstPtrAzim,*fftBuffptr;

	numDopplerBins=obj->numDopplerBins;
	numVirtualAntAzim=obj->numVirtualAntAzim;
	numVirtualAntElev=obj->numVirtualAntElev;
	numVirtualAnt=numVirtualAntAzim+numVirtualAntElev;
	numAngleBins=obj->numAngleBins;
	rangeIdx= maxIndices[0];
	dopplerIdx=maxIndices[1];
	fftBuffptr=obj->radarCube;
	dstPtrAzim=(uint32_t*) MMW_HWA_ANGLE_AZIM_INP;

            srcPtr = (uint32_t*) &fftBuffptr[(numVirtualAnt * numDopplerBins * rangeIdx + numVirtualAnt * dopplerIdx)];

            /* transfer data corresponding to azimuth virtual antennas (corresponding to chirp of antenna Tx0) */
            for(j = 0; j < numVirtualAntAzim; j++)
            {
                *dstPtrAzim++ = *srcPtr++;
            }



            /* Angle estimation - Azimuth FFT */
            MmwDemo_configAngleEstimation_HWA(obj, 1);
            /* Ensure R4F writes to HWA memory is flushed out */
            MEM_BARRIER();
            MmwDemo_dataPathTriggerAngleEstimation(obj);
            MmwDemo_dataPathWaitAngleEstimation(obj);
            /* Azimuth FFT done!*/

			/*Extract index of peak value */
			azimFFTAbsPtr=(uint16_t *)MMW_HWA_ANGLE_AZIM_ABS_OUT;
			maxValAngleFFT=0;
            for(j=0; j < numAngleBins; j++)
	        {
	            tempValAngleFFT = *azimFFTAbsPtr++;
	            if(tempValAngleFFT > maxValAngleFFT)
	            {
	                maxValAngleFFT = tempValAngleFFT;
	                maxIdxAngleFFT = j;
	            }
	        }


	   	if(maxIdxAngleFFT > (numAngleBins/2 -1))
	    {
	        maxIdxAngleFFT-=numAngleBins;
	    }

		return(maxIdxAngleFFT);
}



