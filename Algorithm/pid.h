#ifndef __PID_H
#define __PID_H
#include "sys.h"  
/**************************************************************************
功能：PID算法
时间：20190713 1359
作者：郭浩
**************************************************************************/
typedef struct
{
	//两组PID
	float KP,KI,KD,KP2,KI2,KD2;
	//目标值与当前值
	float globalValue,nowValue;
	//静态变量，保存用于计算
	//这两个变量没有分段
	float Last_Bias,balance_Integral,DLast_Bias;
	//结果，可用于分段PID结果
	float result,result2;
	//定义死区
	float deadband;
	//定义分段PID界限
	float UP;
}pid_k;

int getPID(pid_k* pid_example);
int updatePID(float nowValue0,pid_k* pid_example,u8 flag);
void PID2init(pid_k* pid_example,float P,float I,float D,float globalValue0,float P2,float I2,float D2);
void PIDinit(pid_k* pid_example,float P,float I,float D,float globalValue0);
float wucha(float nowValue0,float globalValue0,float deadband);
int getPID2(pid_k* pid_example);
void chang_Global_Value(float globalValue0,pid_k* pid_example);
#endif

