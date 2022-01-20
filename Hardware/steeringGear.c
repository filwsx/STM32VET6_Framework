#include "steeringGear.h"

int pwmoutX;
int pwmoutY;

u16 SGear0_Level = 1565;
u16 SGear1_Level = 1410;

/******
SGear0  小于平衡值约1500时，会向左侧倾倒，即小球会向X减小方向运动
SGear1  小于平衡值约1500时，会向前侧倾倒，即小球会向Y减小方向运动
*************/
void SGear_Init()
{
	//使用定时器3输出pwm波
	TIM3_PWM_Init(PWM_DUTY-1,TIMER_PSC-1);//f=50hz.
    SGear_Level_Init();
}

//SGear move function
void SGear_move(u8 SGear_NO,int change)
{
	int pwm_val;

	if(SGear_NO==0)
	{
		pwm_val = SGear0_Level + change;
	}
  else if(SGear_NO==1)
	{
		pwm_val = SGear1_Level + change;
	}
	
	SGear_Output(SGear_NO,pwm_val);
}

//水平位置初始化
void SGear_Level_Init()
{
	SGear_move(0,0);
	SGear_move(1,0);
}

void SGear_Output(u8 SGear_NO,int pwm)
{
	if(pwm > PWM_HIGH_MAX)  pwm = PWM_HIGH_MAX; //判断pwm_val是否超出范围
  if(pwm < PWM_HIGH_MIN)  pwm = PWM_HIGH_MIN;
	
	if(SGear_NO==0)
	{
		SGear0=pwm;
		pwmoutX = pwm;
	}
	  
  else if(SGear_NO==1)
  {
		SGear1=pwm;
		pwmoutY = pwm;
	}
}

//测试舵机旋转
void SGear_Test ()
{
		delay_ms(1000);
		SGear0=1500;
		//SGear1=1500;

		delay_ms(1000);
		SGear0=700;
		//SGear1=700;

		delay_ms(1000);
		SGear0=1500;
		//SGear1=1500;

		delay_ms(1000);
		SGear0=2200;
		//SGear1=2200;

}

//水平位置测试调试使用
void SGear_Level_Test()
{
	SGear0=1420;
	SGear1=1500;
}

