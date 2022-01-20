#ifndef __kalmanFilter_H
#define __kalmanFilter_H
#include "sys.h"  
/**************************************************************************
功能：卡尔曼滤波
时间：20190713 1435 
作者：网友Flaribbit&郭浩
**************************************************************************/
typedef struct{
	double prevData;
	double p,q,r,kGain;
}Kalman;

void KalmanInit(Kalman *k,float kGain,float p,float q,float r,float prevData);
float KalmanFilter(Kalman *k,float data);
#endif





























