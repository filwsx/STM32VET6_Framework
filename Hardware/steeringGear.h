#ifndef __steeringGear_H
#define __steeringGear_H
#include "sys.h"  
/**************************************************************************
功能：舵机控制
时间：20190712 
作者：郭浩
**************************************************************************/
#define PWM_DUTY        20000       //PWM波周期，duty=72M/PWM_DUTY/TIMER_PSC=50Hz
#define TIMER_PSC       72           //时钟预分频数
#define PWM_HIGH_MIN_X  SGear0_Level-800   		//0度占空比,脉宽长度0.5ms，占空比2.5%
#define PWM_HIGH_MAX_X  SGear0_Level+800    	//180度占空比脉宽长度2.5ms，占空比12.5%
#define PWM_HIGH_MIN_Y  SGear1_Level-800   		//0度占空比,脉宽长度0.5ms，占空比2.5%
#define PWM_HIGH_MAX_Y	SGear1_Level+800   	//180度占空比脉宽长度2.5ms，占空比12.5%
#define PWM_HIGH_MIN		1150   		//0度占空比,脉宽长度0.5ms，占空比2.5%
#define PWM_HIGH_MAX		1650    	//180度占空比脉宽长度2.5ms，占空比12.5%
#define SGear0  TIM3->CCR3   //(PB0脚)
#define SGear1  TIM3->CCR4   //(PB1脚)

extern int pwmoutX,pwmoutY;
extern u16 SGear0_Level,SGear1_Level;
void SGear_Init(void);
void SGear_move(u8 SGear_NO,int change);//舵机驱动函数
void SGear_Level_Init(void);
void SGear_Test(void);
void SGear_Level_Test(void);
void SGear_Output(u8 SGear_NO,int pwm);
#endif



