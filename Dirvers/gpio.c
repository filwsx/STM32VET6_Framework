#include "gpio.h"
/////////////////////////////

////////////////////////////
/**********************************GPIO_OUTPUT*************************************/
//1
//function1:you can use it to test connection between Chip and Circuit
void GPIO_Init_ALL_Out_PP(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 //all_gpio_clock_enable	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);

//setup gpio	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//GPIOA
 GPIO_Init(GPIOA, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOA,GPIO_Pin_All);
//GPIOB
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOB,GPIO_Pin_All);						
//GPIOC	
 GPIO_Init(GPIOC, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOC,GPIO_Pin_All);
//GPIOD 
 GPIO_Init(GPIOD, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOD,GPIO_Pin_All);
//GPIOE 
 GPIO_Init(GPIOE, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOE,GPIO_Pin_All);
}
//
//GPIO_Set_high_ALL_Out_PP
void GPIO_Set_high_ALL_Out_PP()
{
			GPIO_SetBits(GPIOA,GPIO_Pin_All);
			GPIO_SetBits(GPIOB,GPIO_Pin_All);
			GPIO_SetBits(GPIOC,GPIO_Pin_All);
			GPIO_SetBits(GPIOD,GPIO_Pin_All);
			GPIO_SetBits(GPIOE,GPIO_Pin_All);
}	
//GPIO_Set_low_ALL_Out_PP
void GPIO_Set_low_ALL_Out_PP()
{
			GPIO_ResetBits(GPIOA,GPIO_Pin_All);
			GPIO_ResetBits(GPIOB,GPIO_Pin_All);
			GPIO_ResetBits(GPIOC,GPIO_Pin_All);
			GPIO_ResetBits(GPIOD,GPIO_Pin_All);
			GPIO_ResetBits(GPIOE,GPIO_Pin_All);
}
//end

//2
//gpio_out_pp，GPIO通用输出，输出输出速度为50MHz ,分为推挽和开漏模式 
void GPIO_Init_One_Out(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 //
	 //gpiox_clock_enable	
	if(GPIOx==GPIOA) 
	{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if(GPIOx==GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if(GPIOx==GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	else if(GPIOx==GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	else if(GPIOx==GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	}
	//
	//setup gpio	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode; 	
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
 //init
  GPIO_Init(GPIOx, &GPIO_InitStructure);					
}
//end

//3
//gpio_af_pp, GPIO复用输出，输出最大输出速度为50MHz ,分为推挽和开漏模式
void GPIO_Init_One_AF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed)
{
  	 GPIO_InitTypeDef  GPIO_InitStructure;
	 //
	 //gpiox_clock_enable	
	if(GPIOx==GPIOA) 
	{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if(GPIOx==GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if(GPIOx==GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	else if(GPIOx==GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	else if(GPIOx==GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	}
	//
	//setup gpio	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin;				
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode; 	
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
   //init
   GPIO_Init(GPIOx, &GPIO_InitStructure);	
}
//end

/**********************************GPIO_INPUT*************************************/

//4
//gpio_input,GPIO输入，分为模拟输入，浮空输入， 上拉输入和下拉输入。
void GPIO_Init_One_IN(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
	 //
	 //gpiox_clock_enable	
	if(GPIOx==GPIOA) 
	{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if(GPIOx==GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if(GPIOx==GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	else if(GPIOx==GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	else if(GPIOx==GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	}
	//
	//setup gpio	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin;				
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode; 	
   //init
   GPIO_Init(GPIOx, &GPIO_InitStructure);	
}
