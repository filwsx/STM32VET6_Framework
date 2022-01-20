#include "timer.h"

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟

	GPIO_Init_One_AF(GPIOB,GPIO_Pin_0, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//TIM3->CH3	 
	GPIO_Init_One_AF(GPIOB,GPIO_Pin_1, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//TIM3->CH4
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性low
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3

}

///1.通用定时器计数模式初始化函数 ,DEFAULT:TIM_CounterMode_Up
//TIMx:通用定时器（2,3,4,5） 
//arr：自动重装值。
//psc：时钟预分频数
void TIMx_Counter_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//timer base struct
       if(TIMx==TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
  else if(TIMx==TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
  else if(TIMx==TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
  else if(TIMx==TIM5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
  	//定时器TIMx初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE ); //使能指定的TIMx中断,允许更新中断

	//中断优先级NVIC设置
	      if(TIMx==TIM2) NVIC_Set_Init(TIM2_IRQn,0,3); 
  else if(TIMx==TIM3) NVIC_Set_Init(TIM3_IRQn,0,3);
  else if(TIMx==TIM4) NVIC_Set_Init(TIM4_IRQn,0,3); 
  else if(TIMx==TIM5) NVIC_Set_Init(TIM5_IRQn,0,3); 
	
	TIM_Cmd(TIMx, ENABLE);  //使能TIMx
}

int TIM1_UP_IRQHandler(void)  
{
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位
		control();
	}
	return 0;
}

void Timer1_Init(u16 arr,u16 psc)  
{  
	RCC->APB2ENR|=1<<11;//TIM1时钟使能    
 	TIM1->ARR=arr;      //设定计数器自动重装值   
	TIM1->PSC=psc;      //预分频器7200,得到10Khz的计数时钟
	TIM1->DIER|=1<<0;   //允许更新中断				
	TIM1->DIER|=1<<6;   //允许触发中断	   
	TIM1->CR1|=0x01;    //使能定时器
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);
}

