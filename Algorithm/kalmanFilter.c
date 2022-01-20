#include "kalmanFilter.h"

/**************************************************************************
功能：卡尔曼滤波参数初始化
输入：
输出：无
时间：2019071
作者：网友Flaribbit&郭浩
**************************************************************************/
void KalmanInit(Kalman *k,float kGain,float p,float q,float r,float prevData){
	k->kGain=kGain;
	k->p=p;		//p初值可以随便取,但是不能为0(0的话最优滤波器了)
	k->q=q;		//q参数调滤波后的曲线平滑程度,q越小越平滑
	k->r=r;		//r参数调整滤波后的曲线与实测曲线的相近程度,越小越接近
	k->prevData=prevData;
}

/**************************************************************************
功能：卡尔曼滤波结构体指针，float数据
输入：
输出：float型
时间：2019071
作者：网友Flaribbit&郭浩
**************************************************************************/
float KalmanFilter(Kalman *k,float data){
	k->p=k->p+k->q;
	k->kGain=k->p/(k->p+k->r);
	data=k->prevData+k->kGain*(data-k->prevData);
	k->p=(1-k->kGain*k->p);
	k->prevData=data;
	return data;
}
