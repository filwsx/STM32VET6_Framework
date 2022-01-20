#ifndef __EXTI_H
#define __EXTI_H
#include "sys.h"

void MY_EXTI_Init(void);	//外部中断初始化		 					    

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void button_flag_control(u8 buttonX);
void KEY_Init(void);
void button_control(u8 Add);
void button_display(void);
#endif
































