#include "pid.h"

#define KP (pid_example->KP)
#define KI (pid_example->KI)
#define KD (pid_example->KD)
#define KP2 (pid_example->KP2)
#define KI2 (pid_example->KI2)
#define KD2 (pid_example->KD2)
#define globalValue (pid_example->globalValue)
#define nowValue (pid_example->nowValue)
#define Last_Bias (pid_example->Last_Bias)
#define DLast_Bias (pid_example->DLast_Bias)
#define balance_Integral (pid_example->balance_Integral)
#define result (pid_example->result)
#define result2 (pid_example->result2)
#define deadbandDOWN (pid_example->deadband)
#define deadbandUP (pid_example->UP)

/**************************************************************************
功能：含死区和分段PID的位置PID算法
输入：pid参数结构体
输出：int型结果
时间：20190713  1400
作者：郭浩
**************************************************************************/
int getPID(pid_k* pid_example)
{
	float D_Bias,Bias;
	
	Bias=nowValue-globalValue;
	D_Bias=Bias-Last_Bias;
	Last_Bias=Bias;
	balance_Integral += Bias;

	result=KP*Bias+KI*balance_Integral+KD*D_Bias;
	
	//死区成立条件
	if(wucha(nowValue,globalValue,deadbandDOWN)==1)
	{
		result=0;
		//到达死区后，静态参数置零
		//Last_Bias=0;
		//balance_Integral=0;
	}
	
	//大于死区但是符合分段PID的deadbandUP
	else if(wucha(nowValue,globalValue,deadbandUP)==1)
	{
		result=KP2*Bias+KI2*balance_Integral+KD2*D_Bias;
	}

	return result;
}

/**************************************************************************
功能：增量式PID算法，只有KP和KI，分段PID与死区
输入：pid参数结构体
输出：int型结果
时间：20190722  1522
作者：郭浩
**************************************************************************/
int getPID2(pid_k* pid_example)
{
	float D_Bias,Bias,DD_Bias,RES;
	
	Bias=nowValue-globalValue;
	D_Bias=Bias-Last_Bias;
	DD_Bias = Bias-2*Last_Bias+DLast_Bias;
	
	//result2存储上上次差值
	DLast_Bias = Last_Bias;
	//保存本次差值，下一次使用时成为上次差值
	Last_Bias=Bias;
	
	result += KP*D_Bias+KI*Bias+KD*DD_Bias;
	
	RES=result;
	
	//死区成立条件
	if(wucha(nowValue,globalValue,deadbandDOWN)==1)
	{
		RES=0;
		//到达死区后，静态参数置零
		//Last_Bias=0;
		//balance_Integral=0;
		//DLast_Bias =0;
	}
	//大于死区但是符合分段PID的deadbandUP
	else if(wucha(nowValue,globalValue,deadbandUP)==1)
	{
		result2 += KP2*D_Bias+KI2*Bias+KD2*DD_Bias;
		RES=result2;
	}
	return RES;

}

/**************************************************************************
功能：获取pid算法结果
输入：控制目标的当前值，pid参数结构体，使用的pid算法，1为位置pid，2为速度增量型pid
输出：int型结果
时间：20190713  1400
作者：郭浩
**************************************************************************/
int updatePID(float nowValue0,pid_k* pid_example,u8 flag)
{
	nowValue = nowValue0;
	int result0;
	
	if(flag==1)
		result0 = getPID(pid_example);
	
	if(flag==2)
		result0 = getPID2(pid_example);
	
	return result0;
}

/**************************************************************************
功能：pid结构体初始化
输入：结构体变化指针，float：kp，ki，kd，目标值
输出：无
时间：20190713  1400
作者：郭浩
**************************************************************************/
void PID2init(pid_k* pid_example,float P,float I,float D,float globalValue0,float P2,float I2,float D2)
{
	KP = P;
	KI = I;
	KD = D;
	globalValue = globalValue0;
	balance_Integral=0;
	KP2 = P2;
	KI2 = I2;
	KD2 = D2;
	result=0;
	result2=0;
}

void PIDinit(pid_k* pid_example,float P,float I,float D,float globalValue0)
{
	KP = P;
	KI = I;
	KD = D;
	globalValue = globalValue0;
	balance_Integral=0;
	KP2 = P;
	KI2 = I;
	KD2 = D;
	result=0;
	result2=0;
}

void chang_Global_Value(float globalValue0,pid_k* pid_example)
{
	globalValue = globalValue0;
}

/**************************************************************************
功能：添加允许的误差范围，实现分段PID
输入：当前值，目标值
输出：0 使用正常KP，1 使用放大后的KP2，即第二段
时间：20190719  2151
作者：郭浩
**************************************************************************/
float wucha(float nowValue0,float globalValue0,float deadband)
{
	u8 result0=0;
	//判断当前值是否满足误差要求
	if(nowValue0-globalValue0<=deadband&&nowValue0-globalValue0>=-deadband)
	{
		result0=1;
	}
	return result0;
}
