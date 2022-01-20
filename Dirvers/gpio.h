#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"
/**************************************************************************
���ܣ�GPIO��װ
ʱ�䣺20190712 
���ߣ�����
**************************************************************************/
//GPIO_pin��
//#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
//#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
//#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
//#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
//#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
//#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
//#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
//#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
//#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
//#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
//#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
//#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
//#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
//#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
//#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
//#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
//#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */
//GPIO_mode��
/*
typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;*/

//GPIO_SPEED��
/*
typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef; */

void GPIO_Init_ALL_Out_PP(void);//all_gpio_init_out_pp,GPIOͨ�����������оƬ���� 
void GPIO_Set_high_ALL_Out_PP(void);//оƬ���� 
void GPIO_Set_low_ALL_Out_PP(void);//оƬ���� 
//1.gpio_out_pp��GPIOͨ�����������������ٶ�Ϊ50MHz ,��Ϊ����Ϳ�©ģʽ 
void GPIO_Init_One_Out(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed);//one_gpio_init_out		
//2.gpio_af_pp, GPIO�������������������ٶ�Ϊ50MHz ,��Ϊ����Ϳ�©ģʽ
void GPIO_Init_One_AF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed);
//3.gpio_input,GPIO���룬��Ϊģ�����룬�������룬 ����������������롣 
void GPIO_Init_One_IN(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode);
#endif