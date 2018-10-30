/**
 *   @file  HandCraftedclassifier.c
 *
 *   @brief
 *      Handcrafted classifier for gesture swipe recognition
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define inputveclen 420
#define num_logs 250
#define nnlayerlen 100
#define outputveclen 2
#define lengesture 20
#define num_features 7

int usefulframes_forgesture[lengesture],usefulwtrangeframes_forgesture[lengesture],steepfallidxs[lengesture],steepriseidxs[lengesture],numdetectptspeak_idx[lengesture];

float findmin(float a,float b) {
	return ((a) < (b) ? (a) : (b));
}

float findmax(float a,float b) {
	return ((a) > (b) ? (a) : (b));
}

int getmaxidx_inarray(int left_idx,int right_idx,float input[]) {
	int i,maxelement_idx;
	float max_element;
	max_element = input[left_idx];
	maxelement_idx = left_idx;
	for(i=left_idx;i<=right_idx;i++) {
		if(max_element<input[i]) {
			max_element = input[i];
			maxelement_idx = i;
		}
	}
	return (maxelement_idx);
}

int getminidx_inarray(int left_idx,int right_idx,float input[]) {
	int i,minelement_idx;
	float min_element;
	min_element = input[left_idx];
	minelement_idx = left_idx;
	for(i=left_idx;i<=right_idx;i++) {
		if(min_element>input[i]) {
			min_element = input[i];
			minelement_idx = i;
		}
	}
	return (minelement_idx);
}

int Handmadeclassifier_prediction(float wt_range[],float wt_doppler[],float range_disp[],float vel_disp[],float instenergy[],float numdetectpts[],float angle[]) {
	int i,len_forsteepfall=0,len_forsteeprise=0,len_squaresdetected = 0,firstsquareriseidx=-1,firstsquarefallidx=-1,secondsquareriseidx=-1;
	int firstsquarenumdetectpts_peakidx,secondsquarenumdetectpts_peakidx,leftidx_tostart,rightidx_tostart;
	int secondsquarefallidx=-1,doppler_crosssearchrange=2,count_Num_zerodopplerinmid=0,searchrange_forminmaxdoppler=3,count_posangle_atgesture=0,count_negangle_atgesture=0;
	int searchrange_foranglerisefall = 3,idx_initialanglecross=-1,idx_finalanglecross=-1,firstmax_numdetectptsidx,secondmax_numdetectptsidx,idx_forzerodopplerinmid;
	int leftidx_formaxmindopplersearch,rightidx_formaxmindopplersearch,idx_forfirstdopplermax,idx_forfirstdopplermin,idx_forseconddopplermax,idx_forseconddopplermin;
	int anglefall_leftidx,anglefall_rightidx,anglerise_leftidx,anglerise_rightidx,limit_lengesture=20,diff_inlengthforpartsofgesture=5,Thresh_changeinanglemag_anglecross=5;
	int Thresh_foranglemag_atdoppcross=-5,veldispminshift_numdetectpeak=2,wtrangeminshift_numdetectpeak=2;
	int maxelement_idx,temp_val,falsedetection=0;
	int idx_firstrangemin,idx_secondrangemin,idx_firstveldispmin,idx_secondveldispmin,angle_crosssearchrange=2,idx_initialzerodoppcross,idx_finalzerodoppcross;
	/* bool check_zerodoppleratnumdetectptspeaks=true,validate_initialcross=false,validate_finalcross=false,get_firstchangesign=false,findtwosquareframes_fornumdetectpts=true;
	bool gettwosquares=true,check_framesbetweensquares=false,checks_forweightedrange=true,check_rangeminwithnumdetectptsmax=true,checks_forveldisp=true;
	bool usenumdetect_asbasis = true,usefulframes_forgesture[lengesture],usefulwtrangeframes_forgesture[lengesture],find_gestureoccuringframes = true,count_Num_wtrangeptswithnumdetectpts=false;
	bool usefulveldispframes_forgesture[lengesture],check_veldispminwithnumdetectptsmax=true,checks_angleatnumdetectptspeaks=true,check_countposneganglesatgesture=false;
	bool check_angledirectionatdopplercrossing=false,check_anglemagatdopplercrossing=false,check_changeinangledirection=true,checkanglemag_duringcross=false;
	bool check_maxmindopplerposition=true,checklimit_fordifferentgestureparts=true,check_gesturelength=true,check_countofzerodopplerinmid=false;
	bool count_Num_veldispptswithnumdetectpts=true; */
	int check_zerodoppleratnumdetectptspeaks=1,validate_initialcross=0,validate_finalcross=0,get_firstchangesign=0,findtwosquareframes_fornumdetectpts=1;
	int gettwosquares=1,check_framesbetweensquares=0,checks_forweightedrange=1,check_rangeminwithnumdetectptsmax=1,checks_forveldisp=1;
	int usenumdetect_asbasis = 1,find_gestureoccuringframes = 1,count_Num_wtrangeptswithnumdetectpts=0;
	int usefulveldispframes_forgesture[lengesture],check_veldispminwithnumdetectptsmax=1,checks_angleatnumdetectptspeaks=1,check_countposneganglesatgesture=0;
	int check_angledirectionatdopplercrossing=0,check_anglemagatdopplercrossing=0,check_changeinangledirection=1,checkanglemag_duringcross=0;
	int check_maxmindopplerposition=1,checklimit_fordifferentgestureparts=1,check_gesturelength=1,check_countofzerodopplerinmid=0;
	int count_Num_veldispptswithnumdetectpts=1,checklimit_fornmaxmindopplerrise=1,usethresh_forwtrangenumdetectptscross=0,usethresh_forveldispnumdetectptscross=0;
	float lowerthresh_maxmindopplerjump=0.2;
	float max_element,numdetectpts_thresh,anglediff_posnegpts=0,upperthresh_forwtrange=0.3,upperthresh_forveldisp=1.15;
	//FILE *outputdata = fopen("outputhandclssdata.txt","a");
	if(find_gestureoccuringframes) {
		if(usenumdetect_asbasis) {
			maxelement_idx = getmaxidx_inarray(0,lengesture-1,numdetectpts);
			max_element = numdetectpts[maxelement_idx];
			numdetectpts_thresh = 0.45*(max_element);
			for(i=0;i<lengesture;i++) {
				//printf(" %f ",numdetectpts[i]);
				usefulframes_forgesture[i] = 0;
				if(numdetectpts[i]>numdetectpts_thresh) {
					usefulframes_forgesture[i] = 1;
				}
			}
		}
		//printf(" %f %d ",numdetectpts_thresh,!usefulframes_forgesture[1]);
		if(findtwosquareframes_fornumdetectpts) {
			if(gettwosquares) {
				for(i=0;i<(lengesture-1);i++) {
					if(!usefulframes_forgesture[i] && usefulframes_forgesture[i+1]) {
						steepriseidxs[len_forsteeprise] = i+1;
						len_forsteeprise++;
					}
					if(usefulframes_forgesture[i] && !usefulframes_forgesture[i+1]) {
						steepfallidxs[len_forsteepfall] = i;
						len_forsteepfall++;
					}
				}
				for(i=0;i<findmin(len_forsteepfall,len_forsteeprise);i++) {
					if(steepfallidxs[i]>=steepriseidxs[i]) {
						numdetectptspeak_idx[len_squaresdetected] = getmaxidx_inarray(steepriseidxs[i],steepfallidxs[i],numdetectpts);
						len_squaresdetected++;
					}
				}
				//printf("%d %d ",len_forsteepfall,len_forsteeprise);
				if(len_squaresdetected>=2) {
					firstmax_numdetectptsidx = 0;
					secondmax_numdetectptsidx = 0;
					for(i=0;i<len_squaresdetected;i++) {
						temp_val = numdetectptspeak_idx[i];
						if(numdetectpts[secondmax_numdetectptsidx]<numdetectpts[temp_val]) {
							if(numdetectpts[firstmax_numdetectptsidx]<numdetectpts[temp_val]) {
								secondmax_numdetectptsidx = firstmax_numdetectptsidx;
								firstmax_numdetectptsidx = temp_val;
							}
							else {
								secondmax_numdetectptsidx = temp_val;
							}
						}
					}
					//printf("%d %d ",firstmax_numdetectptsidx,secondmax_numdetectptsidx);
					firstsquarenumdetectpts_peakidx  = findmin(secondmax_numdetectptsidx,firstmax_numdetectptsidx);
					secondsquarenumdetectpts_peakidx  = findmax(secondmax_numdetectptsidx,firstmax_numdetectptsidx);
					for(i=0;i<findmin(len_forsteepfall,len_forsteeprise);i++) {
						if(steepfallidxs[i]>=firstsquarenumdetectpts_peakidx && steepriseidxs[i]<=firstsquarenumdetectpts_peakidx) {
							firstsquareriseidx = steepriseidxs[i];
							firstsquarefallidx = steepfallidxs[i];
						}
						if(steepfallidxs[i]>=secondsquarenumdetectpts_peakidx && steepriseidxs[i]<=secondsquarenumdetectpts_peakidx) {
							secondsquareriseidx = steepriseidxs[i];
							secondsquarefallidx = steepfallidxs[i];
						}
					}
					//fprintf(outputdata,"%d %d %d %d ",firstsquareriseidx,firstsquarefallidx,secondsquareriseidx,secondsquarefallidx);
					if(firstsquarefallidx==-1 || firstsquareriseidx==-1 || secondsquarefallidx==-1 || secondsquareriseidx==-1) {
						//printf("false detection due to invalid squares");
						falsedetection = 1;
						return (!falsedetection);
					}
					if(firstsquarefallidx>secondsquareriseidx || firstsquareriseidx>firstsquarefallidx || secondsquareriseidx>secondsquarefallidx) {
						//printf("false detection due to invalid squares");
						falsedetection = 1;
						return (!falsedetection);
					}
				}
				else {
					//printf("false detection due to less squares");
					falsedetection = 1;
					return (!falsedetection);
				}
				if(check_zerodoppleratnumdetectptspeaks) {
					for(i=1;i<=doppler_crosssearchrange;i++) {
						if(!validate_initialcross) {
							leftidx_tostart = findmax(0,firstsquarenumdetectpts_peakidx-i);
							rightidx_tostart = findmin(lengesture-1,firstsquarenumdetectpts_peakidx+i);
							if((wt_doppler[leftidx_tostart]*wt_doppler[firstsquarenumdetectpts_peakidx])>0 && (wt_doppler[rightidx_tostart]*wt_doppler[firstsquarenumdetectpts_peakidx])>0) {
								validate_initialcross = 0;
							}
							else {
								validate_initialcross = 1;
							}
						}
						if(!validate_finalcross) {
							leftidx_tostart = findmax(0,secondsquarenumdetectpts_peakidx-i);
							rightidx_tostart = findmin(lengesture-1,secondsquarenumdetectpts_peakidx+i);
							if((wt_doppler[leftidx_tostart]*wt_doppler[secondsquarenumdetectpts_peakidx])>0 && (wt_doppler[rightidx_tostart]*wt_doppler[secondsquarenumdetectpts_peakidx])>0) {
								validate_finalcross = 0;
							}
							else {
								validate_finalcross = 1;
							}
						}
					}
					if(!validate_initialcross || !validate_finalcross) {
						//printf("false detection due to no zero doppler crossing beside numdetectpts peak");
						falsedetection = 1;
						return (!falsedetection);
					}
				}
				if(check_framesbetweensquares) {
					if((secondsquareriseidx - firstsquarefallidx)<=2) {
						//printf("false detection due to less frames b/w two squares");
						falsedetection = 1;
						return (!falsedetection);
					}
				}
				for(i=firstsquarefallidx;i<=secondsquareriseidx;i++) {
					if(wt_doppler[i]>=0 && wt_doppler[i+1]<=0) {
						idx_forzerodopplerinmid = i;
						count_Num_zerodopplerinmid++;
					}
				}
				if(check_countofzerodopplerinmid) {
					if(count_Num_zerodopplerinmid!=1) {
						//printf("false detection due to inappropriate no. of zeroes in middle");
						falsedetection = 1;
						return (!falsedetection);
					}
				}
				//fprintf(outputdata,"%d %d ",idx_forzerodopplerinmid,count_Num_zerodopplerinmid);
				if(count_Num_zerodopplerinmid>=1) {
					leftidx_formaxmindopplersearch = findmax(0,firstsquareriseidx-searchrange_forminmaxdoppler);
					rightidx_formaxmindopplersearch = findmin(lengesture-1,secondsquarefallidx+searchrange_forminmaxdoppler);
					idx_forfirstdopplermax = getmaxidx_inarray(leftidx_formaxmindopplersearch,idx_forzerodopplerinmid,wt_doppler);
					idx_forfirstdopplermin = getminidx_inarray(leftidx_formaxmindopplersearch,idx_forzerodopplerinmid,wt_doppler);
					idx_forseconddopplermax = getmaxidx_inarray(idx_forzerodopplerinmid+1,rightidx_formaxmindopplersearch,wt_doppler);
					idx_forseconddopplermin = getminidx_inarray(idx_forzerodopplerinmid+1,rightidx_formaxmindopplersearch,wt_doppler);	
					
					//printf("%d %d %d %d ",idx_forfirstdopplermax,idx_forfirstdopplermin,idx_forseconddopplermax,idx_forseconddopplermin);
					if(check_maxmindopplerposition) {
						if((idx_forfirstdopplermax-idx_forfirstdopplermin)<=0 || (idx_forseconddopplermax-idx_forseconddopplermin)<=0) {
							//printf("false detection due to wrong position of max and min doppler");
							falsedetection = 1;
							return (!falsedetection);
						}
					}
					if(checks_forweightedrange) {
						idx_firstrangemin = getminidx_inarray(idx_forfirstdopplermin,idx_forfirstdopplermax,wt_range);
						idx_secondrangemin = getminidx_inarray(idx_forseconddopplermin,idx_forseconddopplermax,wt_range);
						//printf("%d %d ",idx_firstrangemin,idx_secondrangemin);
						if(usethresh_forwtrangenumdetectptscross) {
							for(i=0;i<lengesture;i++) {
								usefulwtrangeframes_forgesture[i] = 0;
								if(wt_range[i]<upperthresh_forwtrange) {
									usefulwtrangeframes_forgesture[i] = 1;
								}
							}
							for(i=firstsquareriseidx;i<=firstsquarefallidx;i++) {
								if(usefulwtrangeframes_forgesture[i]) {
									count_Num_wtrangeptswithnumdetectpts = 1;
								}
							}
							if(!count_Num_wtrangeptswithnumdetectpts) {
								//printf("false detection because of no wt range points in gesture region");
								falsedetection = 1;
								return (!falsedetection);
							}
							count_Num_wtrangeptswithnumdetectpts = 0;
							for(i=secondsquareriseidx;i<=secondsquarefallidx;i++) {
								if(usefulwtrangeframes_forgesture[i]) {
									count_Num_wtrangeptswithnumdetectpts = 1;
								}
							}
							if(!count_Num_wtrangeptswithnumdetectpts) {
								//printf("false detection because of no wt range points in gesture region");
								falsedetection = 1;
								return (!falsedetection);
							}
						}
						if(check_rangeminwithnumdetectptsmax) {
							if((abs(firstmax_numdetectptsidx-idx_firstrangemin)>wtrangeminshift_numdetectpeak && abs(firstmax_numdetectptsidx-idx_secondrangemin)>wtrangeminshift_numdetectpeak) 
							   || (abs(secondmax_numdetectptsidx-idx_firstrangemin)>wtrangeminshift_numdetectpeak && abs(secondmax_numdetectptsidx-idx_secondrangemin)>wtrangeminshift_numdetectpeak)) {
								   //printf("false detection due to more difference b/w wt range min and numdetectpts max");
								   falsedetection = 1;
								   return (!falsedetection);
							   } 
						}
					}
					if(checks_forveldisp) {
						idx_firstveldispmin = getminidx_inarray(idx_forfirstdopplermin,idx_forfirstdopplermax,vel_disp);
						idx_secondveldispmin = getminidx_inarray(idx_forseconddopplermin,idx_forseconddopplermax,vel_disp);
						//printf("%d %d ",idx_firstveldispmin,idx_secondveldispmin);
						if(usethresh_forveldispnumdetectptscross) {
							for(i=0;i<lengesture;i++) {
								usefulveldispframes_forgesture[i] = 0;
								if(vel_disp[i]<upperthresh_forveldisp) {
									usefulveldispframes_forgesture[i] = 1;
									//printf(" %d ",i);
								}
							}
							for(i=firstsquareriseidx;i<=firstsquarefallidx;i++) {
								if(usefulveldispframes_forgesture[i]) {
									count_Num_veldispptswithnumdetectpts = 1;
								}
							}
							if(!count_Num_veldispptswithnumdetectpts) {
								//printf("false detection because of no vel disp points in gesture region1");
								falsedetection = 1;
								return (!falsedetection);
							}
							count_Num_veldispptswithnumdetectpts = 0;
							for(i=secondsquareriseidx;i<=secondsquarefallidx;i++) {
								if(usefulveldispframes_forgesture[i]) {
									count_Num_veldispptswithnumdetectpts = 1;
								}
							}
							if(!count_Num_veldispptswithnumdetectpts) {
								//printf("false detection because of no vel disp points in gesture region2");
								falsedetection = 1;
								return (!falsedetection);
							}
						}
						if(check_veldispminwithnumdetectptsmax) {
							if((abs(firstmax_numdetectptsidx-idx_firstveldispmin)>veldispminshift_numdetectpeak && abs(firstmax_numdetectptsidx-idx_secondveldispmin)>veldispminshift_numdetectpeak) 
							   || (abs(secondmax_numdetectptsidx-idx_firstveldispmin)>veldispminshift_numdetectpeak && abs(secondmax_numdetectptsidx-idx_secondveldispmin)>veldispminshift_numdetectpeak)) {
								   //printf("false detection due to more difference b/w vel disp min and numdetectpts max");
								   falsedetection = 1;
								   return (!falsedetection);
							   } 
						}
					}
					if(checks_angleatnumdetectptspeaks) {
						validate_finalcross = 0;
						validate_initialcross = 0;
						for(i=1;i<=angle_crosssearchrange;i++) {
							if(!validate_initialcross) {
								leftidx_tostart = findmax(0,firstsquarenumdetectpts_peakidx-i);
								rightidx_tostart = findmin(lengesture-1,firstsquarenumdetectpts_peakidx+i);
								if(angle[leftidx_tostart]*angle[firstsquarenumdetectpts_peakidx]>0 && angle[rightidx_tostart]*angle[firstsquarenumdetectpts_peakidx]>0) {
									validate_initialcross = 0;
								}
								else {
									validate_initialcross = 1;
								}
							}
							if(!validate_finalcross) {
								leftidx_tostart = findmax(0,secondsquarenumdetectpts_peakidx-i);
								rightidx_tostart = findmin(lengesture-1,secondsquarenumdetectpts_peakidx+i);
								if(angle[leftidx_tostart]*angle[secondsquarenumdetectpts_peakidx]>0 && angle[rightidx_tostart]*angle[secondsquarenumdetectpts_peakidx]>0) {
									validate_finalcross = 0;
								}
								else {
									validate_finalcross = 1;
								}
							}
						}
						if(!validate_initialcross || !validate_finalcross) {
							//printf("false detection due to no zero angle crossing beside numdetectpts peak");
							falsedetection = 1;
							return (!falsedetection);
						}
						if(check_countposneganglesatgesture) {
							for(i=idx_forfirstdopplermax;i<=idx_forseconddopplermin;i++) {
								if(angle[i]>0) {
									count_posangle_atgesture++;
								}
								if(angle[i]<0) {
									count_negangle_atgesture++;
								}
							}
							if((count_negangle_atgesture-count_posangle_atgesture)<(anglediff_posnegpts*(count_posangle_atgesture+count_negangle_atgesture))) {
								//printf("false detection due to less negative pts in angle data");
								falsedetection = 1;
								return (!falsedetection);
							}
						}
						if(check_angledirectionatdopplercrossing) {
							if(angle[idx_forzerodopplerinmid]>0 && angle[idx_forzerodopplerinmid+1]>0) {
								//printf("false detection due to non negative angle at zero crossing of doppler");
								falsedetection = 1;
								return (!falsedetection);
							}
						}
						if(check_anglemagatdopplercrossing) {
							if(angle[idx_forzerodopplerinmid]>Thresh_foranglemag_atdoppcross && angle[idx_forzerodopplerinmid+1]>Thresh_foranglemag_atdoppcross) {
								//printf("false detection due to big value of angle at zero doppler crossing");
								falsedetection = 1;
								return (!falsedetection);
							}
						}
						if(check_changeinangledirection) {
							for(i=idx_forfirstdopplermin;i<=idx_forfirstdopplermax;i++) {
								if(!get_firstchangesign && wt_doppler[i]>=0) {
									idx_initialzerodoppcross = i;
									get_firstchangesign=1;
								}
							}
							get_firstchangesign=0;
							for(i=idx_forseconddopplermin;i<=idx_forseconddopplermax;i++) {
								if(!get_firstchangesign && wt_doppler[i]>=0) {
									idx_finalzerodoppcross = i;
									get_firstchangesign=1;
								}
							}
							anglefall_leftidx = findmax(0,idx_initialzerodoppcross-searchrange_foranglerisefall);
							anglefall_rightidx = findmin(lengesture-1,idx_initialzerodoppcross+searchrange_foranglerisefall-1);
							for(i=anglefall_leftidx;i<anglefall_rightidx;i++) {
								if(angle[i]>=0 && angle[i+1]<=0) {
									idx_initialanglecross = i;
								}
							}
							anglerise_leftidx = findmax(0,idx_finalzerodoppcross-searchrange_foranglerisefall);
							anglerise_rightidx = findmin(lengesture-1,idx_finalzerodoppcross+searchrange_foranglerisefall-1);
							for(i=anglerise_leftidx;i<anglerise_rightidx;i++) {
								if(angle[i]<=0 && angle[i+1]>=0) {
									idx_finalanglecross = i;
								}
							}
							//printf("%d %d %d %d\n",idx_initialzerodoppcross,idx_finalzerodoppcross,idx_initialanglecross,idx_finalanglecross);
							if(idx_initialanglecross==-1 || idx_finalanglecross==-1) {
								//printf("false detection due to wrong angle direction");
								falsedetection = 1;
								return (!falsedetection);
							}
							else if(checkanglemag_duringcross) {
								if((angle[idx_initialanglecross]-angle[idx_initialanglecross+1])<Thresh_changeinanglemag_anglecross 
								|| (angle[idx_finalanglecross]-angle[idx_finalanglecross+1])<Thresh_changeinanglemag_anglecross) {
									//printf("false detection due to less magnitude jump in angle value at crossing");
									falsedetection = 1;
									return (!falsedetection);
								}
							}
						}
					}
					if(checklimit_fornmaxmindopplerrise) {
						if((wt_doppler[idx_forfirstdopplermax]-wt_doppler[idx_forfirstdopplermin])<lowerthresh_maxmindopplerjump) {
							//printf("false detection due to less max and min doppler value difference");
							falsedetection = 1;
							return (!falsedetection);
						}
						if((wt_doppler[idx_forseconddopplermax]-wt_doppler[idx_forseconddopplermin])<lowerthresh_maxmindopplerjump) {
							//printf("false detection due to less max and min doppler value difference");
							falsedetection = 1;
							return (!falsedetection);
						}
						if((wt_doppler[idx_forfirstdopplermax]-wt_doppler[idx_forseconddopplermin])<lowerthresh_maxmindopplerjump) {
							//printf("false detection due to less max and min doppler value difference");
							falsedetection = 1;
							return (!falsedetection);
						}
					}
					if(checklimit_fordifferentgestureparts) {
						if(abs((idx_forfirstdopplermax-idx_forfirstdopplermin)-(idx_forseconddopplermax-idx_forseconddopplermin))>diff_inlengthforpartsofgesture) {
							//printf("false detection due to large difference in parts of gesture");
							falsedetection = 1;
							return (!falsedetection);
						}
						if(abs((idx_forseconddopplermin-idx_forfirstdopplermax)-(idx_forfirstdopplermax-idx_forfirstdopplermin))>(2*diff_inlengthforpartsofgesture)) {
							//printf("false detection due to large difference in parts of gesture");
							falsedetection = 1;
							return (!falsedetection);
						}
					}
					if(check_gesturelength) {
						if((idx_forseconddopplermax-idx_forfirstdopplermax)>limit_lengesture) {
							//printf("false detection due to large gesture");
							falsedetection = 1;
							return (!falsedetection);
						}
					}
				}
				else {
					//printf("false detection due to zero doppler crossings in middle");
					falsedetection = 1;
					return (!falsedetection);
				}
				
			}
		}
	}
	//fclose(outputdata);
	return (!falsedetection);
}
