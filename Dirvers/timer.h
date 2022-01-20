#ifndef __timer_H
#define __timer_H
#include "sys.h"  
/**************************************************************************
功能：定时器初始化等
时间：20190712 
作者：郭浩
**************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc);
void Timer1_Init(u16 arr,u16 psc);
int TIM1_UP_IRQHandler(void);
void TIMx_Counter_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc);
#endif






























