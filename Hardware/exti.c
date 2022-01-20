#include "exti.h"

#define xiaodouMS 15
#define Mode1Count 0x70
#define Mode2Count 0x08
u8 mode_flag=0x00;
int mode_value[12];

extern pid_k pidx,pidy,pidvx,pidvy;

/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void MY_EXTI_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_E,0,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_E,1,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_E,2,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_E,3,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_E,4,FTIR);		//下降沿触发

	MY_NVIC_Init(2,2,EXTI0_IRQn,2);  	//抢占2，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI1_IRQn,2);  	//抢占2，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI2_IRQn,2);  	//抢占2，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);  	//抢占2，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI4_IRQn,2);  	//抢占2，子优先级2，组2
	
	for(int i=0;i<12;i++)
	{
		mode_value[i]=1;
	}
}


void EXTI0_IRQHandler(void)
{			
	delay_ms(xiaodouMS);   //消抖
	//pause or start
	button_flag_control(0);
  EXTI->PR=1<<0; //清除LINE0上的中断标志位  
}

void EXTI1_IRQHandler(void)
{			
	delay_ms(xiaodouMS);   //消抖 
	//sub
	button_flag_control(4);
 	EXTI->PR=1<<1;     //清除LINE1上的中断标志位  
}

void EXTI2_IRQHandler(void)
{
	delay_ms(xiaodouMS);   //消抖
	//模式2
	button_flag_control(2);
	EXTI->PR=1<<2;     //清除LINE2上的中断标志位
}

void EXTI3_IRQHandler(void)
{
	delay_ms(xiaodouMS);   //消抖
	//add
	button_flag_control(3);	
	EXTI->PR=1<<3;     //清除LINE3上的中断标志位
}

void EXTI4_IRQHandler(void)
{
	delay_ms(xiaodouMS);   //消抖
	//模式1
	button_flag_control(1);
	EXTI->PR=1<<4;     //清除LINE4上的中断标志位
}

void KEY_Init(void)
{
	RCC->APB2ENR|=1<<6;

	GPIOE->CRL&=0XFFF00000; 
	GPIOE->CRL|=0X00088888;
	
	GPIOE->ODR|=1<<0; //PE 0
	GPIOE->ODR|=1<<1; //PE 1
	GPIOE->ODR|=1<<2; //PE 2
	GPIOE->ODR|=1<<3; //PE 3
	GPIOE->ODR|=1<<4; //PE 4
} 

/*****************************
按键逻辑简述：
	按键有5个，一个控制开关，两个控制两个模式切换，两个用于该模式下的控制
	mode_flag为u8,分为高位低位。高/低为两组不同模式标志。
	按下一个模式另一个模式标志为清零
	按下加/减按键之后读取mode_flag来判断模式之后做出响应动作
*****************************/
void button_flag_control(u8 buttonX)
{
	//控制硬件开关
	if(buttonX==0)
	{
		//控制总开关
		if(control_Flag ==1)
		{
			control_Flag = 0;
		}else
		{
			control_Flag = 1;
		}
	}
	//模式1选择
	else if(buttonX==1)
	{
		//将另一模式位清零
		mode_flag &=0xf0;
		
		//这个按键我定义了6种模式，到达最高应改回模式1
		if(mode_flag ==Mode1Count)
		{
			mode_flag =0x00;
		}
		mode_flag += 0x10;

	}
	//模式2选择
	else if(buttonX==2)
	{
		//将另一模式位清零
		mode_flag &=0x0f;
		//这个按键我定义了8种模式，到达最高应改回模式1
		if(mode_flag ==Mode2Count)
		{
			mode_flag =0x00;
		}
		mode_flag += 0x01;
		
	}
	//模式选择的加按键
	else if(buttonX==3)
	{
		//传入0或1这个参数用于区分按下哪个键
		//这两个按键才是真正的执行按键
		button_control(0);
	}
	//模式选择的减按键
	else if(buttonX==4)
	{
		button_control(1);
	}

}

//Add为0则代表按下加
//Add为1则代表按下减
//这个是实际的代码执行部分
void button_control(u8 Add)
{
	//这个功能是读9个坐标点
	if(mode_flag ==0x10)
	{
		//openmv_Init();
		mode_flag=0x00;
	}
	
	//这个是让板球水平
	else if(mode_flag ==0x20)
	{
		control_Flag=0;
		SGear_move(1,0);
		SGear_move(0,0);
	}
	
	//这个合适演示模式
	else if(mode_flag ==0x30)
	{
		if(Add == 0)
		{
			//改变模式
			Mode_move++;
			//如果超过可演示的数目，归为1
			if(Mode_move==10)
			{
				Mode_move=1;
			}
		}

		//减按键用于控制Y方向是否有效
		if(Add == 1)
		{
			Mode_move--;
			//如果第于可演示的数目，归为9
			//这样就能1-9循环
			if(Mode_move<1)
			{
				Mode_move=9;
			}
		}
	}
	
	//这个是控制x和y方向是否有效
	else if(mode_flag ==0x40)
	{
		//加按键用于控制X方向是否有效
		if(Add == 0)
		{
			if(FlagX  ==1)
			{
				FlagX  = 0;
			}else
			{
				FlagX  = 1;
			}
		}

		//减按键用于控制Y方向是否有效
		if(Add == 1)
		{
			if(FlagY ==1)
			{
				FlagY = 0;
			}else
			{
				FlagY = 1;
			}
		}
	}
	
	//这个是控制速度和位置环有效
	else if(mode_flag ==0x50)
	{
		//加按键用于控制位置是否有效
		if(Add == 0)
		{
			if(FlagPoint ==1)
			{
				FlagPoint = 0;
			}else
			{
				FlagPoint = 1;
			}
		}

		//减按键用于控制速度是否有效
		if(Add == 1)
		{
			if(FlagSpeed ==1)
			{
				FlagSpeed = 0;
			}else
			{
				FlagSpeed = 1;
			}
		}
	}
	//change pages 屏幕太小，换页操作
	else if(mode_flag ==0x60)
	{
		
		if(page_flag==0)
		{
			page_flag=1;
		}
		else
		{
			page_flag=0;
		}
	}
	//change piont flag
	else if(mode_flag ==0x70)
	{
		if(Add == 0)
		{
			//改变模式
			point_Flag++;
			//如果超过可演示的数目，归为1
			if(point_Flag==10)
			{
				point_Flag=1;
			}
		}

		//减按键用于控制Y方向是否有效
		if(Add == 1)
		{
			point_Flag--;
			//如果第于可演示的数目，归为9
			//这样就能1-9循环
			if(point_Flag<1)
			{
				point_Flag=9;
			}
		}
		
		chang_point();
	}
	
	//位置环KP调节
	else if(mode_flag ==0x01)
	{
		if(Add == 0)
		{
			pidx.KP++;
			pidy.KP++;
		}

		if(Add == 1)
		{
			pidx.KP--;
			pidy.KP--;
		}
	}
	
	//位置环KI调节
	else if(mode_flag ==0x02)
	{
		if(Add == 0)
		{
			pidx.KI+=0.0001;
			pidy.KI+=0.0001;
		}

		if(Add == 1)
		{
			pidx.KI-=0.0001;
			pidy.KI-=0.0001;
		}
	}
	
	//位置环KD调节
	else if(mode_flag ==0x03)
	{
		if(Add == 0)
		{
			pidx.KD+=10;
			pidy.KD+=10;
		}

		if(Add == 1)
		{
			pidx.KD-=10;
			pidy.KD-=10;
		}
	}
	
	//速度环KP调节
	else if(mode_flag ==0x04)
	{
		if(Add == 0)
		{
			pidvx.KP++;
			pidvy.KP++;
		}

		if(Add == 1)
		{
			pidvx.KP--;
			pidvy.KP--;
		}
	}
	
	//速度环KI调节
	else if(mode_flag ==0x05)
	{
		if(Add == 0)
		{
			pidvx.KI+=0.0001;
			pidvy.KI+=0.0001;
		}

		if(Add == 1)
		{
			pidvx.KI-=0.0001;
			pidvy.KI-=0.0001;
		}
	}
	
	//速度环KD调节
	else if(mode_flag ==0x06)
	{
		if(Add == 0)
		{
			pidvx.KD+=10;
			pidvy.KD+=10;
		}

		if(Add == 1)
		{
			pidvx.KD-=10;
			pidvy.KD-=10;
		}
	}
	
	//舵机X方向输出PWM波修改
	else if(mode_flag ==0x07)
	{
		//暂停PID的输出控制
		control_Flag=0;
		
		if(Add == 0)
		{
			pwmoutX+=10;
		}

		if(Add == 1)
		{
			pwmoutX-=10;
		}
		
		SGear_Output(0,pwmoutX);
	}
	
	//舵机Y方向输出PWM波修改
	else if(mode_flag ==0x08)
	{
		//暂停PID的输出控制
		control_Flag=0;
		
		if(Add == 0)
		{
			pwmoutY+=10;
		}

		if(Add == 1)
		{
			pwmoutY-=10;
		}
		
		SGear_Output(1,pwmoutY);
	}
}

//按键显示控制
//仅控制切换操作模式的拿一块区域
//用于给用户显示当前模式
void button_display(void)
{
	if(mode_flag==0x00)
	{
		OLED_ShowString(70,52,"       ") ; 
		OLED_Refresh_Gram();
	}
	//这个功能是读9个坐标点
	//读点
	if(mode_flag ==0x10)
	{
		OLED_ShowString(70,52,"duPoint") ; 
		OLED_Refresh_Gram();
	}
	
	//这个是让板球水平
	else if(mode_flag ==0x20)
	{
		OLED_ShowString(70,52,"level  ") ; 
		OLED_Refresh_Gram();
	}
	
	//这个合适演示模式
	else if(mode_flag ==0x30)
	{
		OLED_ShowString(70,52,"Demo   ") ; 
		OLED_Refresh_Gram();
	}
	
	//这个是控制x和y方向是否有效
	else if(mode_flag ==0x40)
	{
		OLED_ShowString(70,52,"X&Y    ") ; 
		OLED_Refresh_Gram();
	}
	
	//这个是控制速度和位置环有效
	//p为位置，v为速度
	else if(mode_flag ==0x50)
	{
		OLED_ShowString(70,52,"P&V    ") ; 
		OLED_Refresh_Gram();
	}
	else if(mode_flag ==0x60)
	{
		OLED_ShowString(70,52,"pages  ") ; 
		OLED_Refresh_Gram();
	}
	else if(mode_flag ==0x70)
	{
		OLED_ShowString(70,52,"point  ") ; 
		OLED_Refresh_Gram();
	}
	
	//位置环KP调节
	//PP：point KP
	else if(mode_flag ==0x01)
	{
		OLED_ShowString(70,52,"PP     ") ; 
		OLED_Refresh_Gram();
	}
	
	//位置环KI调节
	//PI：point KI
	else if(mode_flag ==0x02)
	{
		OLED_ShowString(70,52,"PI     ") ; 
		OLED_Refresh_Gram();
	}
	
	//位置环KD调节
	//PD：point KD
	else if(mode_flag ==0x03)
	{
		OLED_ShowString(70,52,"PD     ") ; 
		OLED_Refresh_Gram();
	}
	
	//速度环KP调节
	//VP：Speed KP
	else if(mode_flag ==0x04)
	{
		OLED_ShowString(70,52,"VP     ") ; 
		OLED_Refresh_Gram();
	}
	
	//速度环KI调节
	//VI：Speed KI
	else if(mode_flag ==0x05)
	{
		OLED_ShowString(70,52,"VI     ") ; 
		OLED_Refresh_Gram();
	}
	
	//速度环KD调节
	//VD：Speed KD
	else if(mode_flag ==0x06)
	{
		OLED_ShowString(70,52,"VD     ") ; 
		OLED_Refresh_Gram();
	}
	
	//舵机X方向输出PWM波修改
	//DJX：X方向的舵机 DuoJi
	else if(mode_flag ==0x07)
	{
		OLED_ShowString(70,52,"DJX    ") ; 
		OLED_Refresh_Gram();
	}
	
	//舵机Y方向输出PWM波修改
	else if(mode_flag ==0x08)
	{
		OLED_ShowString(70,52,"DJY    ") ; 
		OLED_Refresh_Gram();
	}
}

/************************************************************
怎么增加功能，即操作模式
	给该模式分配唯一的mode_flag
			目前只有0x01-0x0f
			0x10-0xf0 累计30个
	修改这里：		
		#define Mode1Count 0x60
		#define Mode2Count 0x08
		这个代表每个模式的上限值
	在void button_control(u8 Add)内为对应ID添加逻辑
	在void button_display(void)内为对应ID添加显示内容
推荐逻辑内仅修改用于控制的标志位，或者调用参数简单的函数方法
*************************************************************/
