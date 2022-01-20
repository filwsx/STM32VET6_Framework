#include "main.h"

#define ms20 3
#define deadbandValue 0
///////////////////////////////全局变量部分/////////////////////////////////

//0-8为九个点的X值，9-17为Y的值
u8 point_Array[]={66,165,225,65,170,222,66,145,221,38,39,43,111,117,118,192,193,192};//用于定点的9个坐标

//用于按键控制标志
u8 FlagX=1,FlagY=1,FlagPoint=1,FlagSpeed=1,control_Flag =1;

//用于计算速度
int Last_pointX,Last_pointY;

//定义存储当前位置和速度的变量
int pointX,pointY,SpeedX,SpeedY;

//用于存储pid运算结果
int chang_PointY,chang_PointX,chang_SpeedY,chang_SpeedX;

//定义目标点X,Y
float globalValue_x,globalValue_y;

//用于位置与速度的控制节奏
int Flag_ms = 0,Flag2_ms = 0;

//初始化目标点,0 is null			//初始化运动模式
u8 point_Flag = 5,					Mode_move = 0;
u8 A,B,C,D;

//小球在目标点允许的误差范围内就加一，可用于计时，加1为50ms，取决于定时器1多久执行一次control();
int count_Flag= 0;

//将二维平面的控制转化为两个方向，除了位置还要控制速度，速度目标值为0
////定义个PID结构体
pid_k pidx,pidy,pidvx,pidvy;

int main(void)
{ 
	init();
	
	while(1)
	{
		DataScope();
		oled_show();
	}
} 

//初始化分为硬件部分和软件部分
void init()
{
	delay_init();																	//延时初始化
	delay_ms(1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//中断初始化
	USART1_init(115200,0);//uart_init,remap enable			//串口1初始化,openmv通信
	USART2_init(115200,0);//uart_init,remap enable			//串口2初始化,上位机通信
	OLED_Init();																	//oled初始化，显示坐标参数
	SGear_Init();																	//舵机初始化
	Timer1_Init(49,7199);															//定时器1中断初始化，该中断进行控制
	MY_EXTI_Init();															//按键外部中断初始化，用于外部人机交互
	//adc初始化,pwm初始化,编码器初始化,电机初始化
	delay_ms(1000);

	//结构体，KP,KI,KD,目标值
	PIDinit(&pidx,5,0,260,globalValue_x);
	pidx.deadband=deadbandValue,pidx.UP=0;
	PIDinit(&pidy,5,0,260,globalValue_y);
	pidy.deadband=deadbandValue,pidy.UP=0;
	
	PIDinit(&pidvx,40,0,0,1);
	pidvx.deadband=0,pidvx.UP=0;
	PIDinit(&pidvy,40,0,0,1);
	pidvy.deadband=0,pidvy.UP=0;	//pid初始化
	//openmv_Init();
}

//更新速度PID的输出
void controlSpeed(void)
{
	//计算速度
	SpeedX = pointX - Last_pointX;
	SpeedY = pointY - Last_pointY;
	
	chang_SpeedX = updatePID(SpeedX,&pidvx,2);
	chang_SpeedY = updatePID(SpeedY,&pidvy,2);
	
	//保存此次位置，用于下次计算速度
	Last_pointX = pointX;
	Last_pointY = pointY;
}

//更新位置PID的输出
void controlPoint(void)
{
	//获取pid结果
	chang_PointX = updatePID(pointX,&pidx,1);
	chang_PointY = updatePID(pointY,&pidy,1);

	//必须两个方向同时满足才可进入死区，一个调整会影响另一个
	//pid 的死区优先级更高
	//用于计数到达目标点的时长
	if(wucha(pointX,globalValue_x,deadbandValue)&&wucha(pointY,globalValue_y,deadbandValue))
	{
		count_Flag++;
		chang_PointX=0;
		chang_PointY=0;
		
		if(wucha(SpeedX,0,2)&&wucha(SpeedY,0,2))
		{
			//SGear0_Level=pwmoutX;
			//SGear1_Level=pwmoutY;
		}
	}
		
}

//核心控制代码
//可调节位置于速度控制节奏
//按键功能实现也是在这里
void control()
{
		Flag_ms++;
		Flag2_ms++;
	
		//改变运动模式
		chang_Mode();
		//改变目标点，这两句有先后顺序，不能随意修改
		chang_point();
	
		pointX = USART1_RX_BUF[1]*256+USART1_RX_BUF[2];
		pointY = USART1_RX_BUF[3]*256+USART1_RX_BUF[4];
	
		if(Flag_ms==2)			//5ms*2
		{		
			Flag_ms=0;
			controlPoint();
		}
		
		if(Flag2_ms==20)			//5ms*20
		{		
			Flag2_ms=0;
			controlSpeed();
		}
		
		//如果X方向被屏蔽，那么X方向相关PID输出为0即可
		chang_PointX = chang_PointX*FlagX*FlagPoint;
		chang_SpeedX = chang_SpeedX*FlagX*FlagSpeed;
		
		//如果Y方向被屏蔽，那么Y方向相关PID输出为0即可
		chang_PointY = chang_PointY*FlagY*FlagPoint;	
		chang_SpeedY = chang_SpeedY*FlagY*FlagSpeed;
		
		if(control_Flag==1)
		{
			SGear_move(0,-chang_PointX-chang_SpeedX);
			SGear_move(1,-chang_PointY-chang_SpeedY);
		}
}

//openmv初始化，获取9个值，代表斜对角三个点
//理想情况下三个点位置可计算出其余6个点位置
//实际上不准
void openmv_Init(void)
{
	int i;
	
	//接收点的bug因为一直在发送点的坐标但是flag没有清除
	//在这里先手动清除一下
	USART1_RX_flag=0;
	USART1_RX_count=0;
	
	//随意发送一些东西就能让openmv发送九个点坐标
	USART1_send((u8*)0xaa);
	//等待接受完成
	while(USART1_RX_flag==0);
		//USART2_send_len(USART1_RX_BUF,38);
	
	//先屏蔽接收中断放置后续小球坐标覆盖
	USART_Cmd(USART1, DISABLE); 
	for(i=1;i<36;i+=2){
		point_Array[(i/2)]=USART1_RX_BUF[i]*256+USART1_RX_BUF[i+1];
	}
	USART1_RX_flag=0;
	USART1_RX_count=0;
	USART_Cmd(USART1, ENABLE); 
	chang_point();
}

//1-9模式分别代表9个点的坐标
void chang_point()
{
	if(point_Flag>0&&point_Flag<10)
	{
		globalValue_x=point_Array[point_Flag-1];
		globalValue_y=point_Array[point_Flag+8];
	}
	
	//P10 是过渡点
	if(point_Flag==10)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else if(point_Flag==11)
	{
		globalValue_x=134;
		globalValue_y=114;
	}
	else if(point_Flag==12)
	{
		globalValue_x=139;
		globalValue_y=142;
	}
	else if(point_Flag==13)
	{
		globalValue_x=168;
		globalValue_y=159;
	}
	else if(point_Flag==14)
	{
		globalValue_x=205;
		globalValue_y=149;
	}
	else if(point_Flag==15)
	{
		globalValue_x=212;
		globalValue_y=119;
	}
	else if(point_Flag==16)
	{
		globalValue_x=199;
		globalValue_y=83;
	}
	else if(point_Flag==17)
	{
		globalValue_x=170;
		globalValue_y=77;
	}
	else if(point_Flag==18)
	{
		globalValue_x=137;
		globalValue_y=86;
	}
	else if(point_Flag==19)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else if(point_Flag==20)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else if(point_Flag==21)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else
	{
		globalValue_x=point_Array[4];
		globalValue_y=point_Array[13];
	}
	
	chang_Global_Value(globalValue_x,&pidx);
	chang_Global_Value(globalValue_y,&pidy);
}

//该函数用于演示，完成比赛要求
void chang_Mode()
{
	//初始化默认为该模式，该模式可以看作启动引导
	if(Mode_move==0)
	{
		
	}
	//进入模式1
	if(Mode_move==1)
	{
		//不在2点则把目标点改为2
		//只要改参数就行，后续执行的函数会根据参数修改目标值
		if(point_Flag!=2)
		{
			point_Flag=2;
			count_Flag=0;
		}
		
	}
	
	//进入模式2
	if(Mode_move==2)
	{
		//用于检测在该点停留时间是否满足
		if(count_Flag==ms20)
		{
			//如果停留点为1且满足时间，则到下一点5
			if(point_Flag==1)
			{
				point_Flag=5;
				count_Flag=0;
			}
			else if(point_Flag==5)
			{
				point_Flag=5;
				count_Flag=0;
			}
			//否则不在模式2指定的点上，那么我们回到模式二的初始点1
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}
	
	//进入模式3
	if(Mode_move==3)
	{
		//到达当前目标点够2s了
		if(count_Flag==ms20)
		{
			//在点1待够时间则到指定的下一点
			if(point_Flag==1)
			{
				point_Flag=4;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==4)
			{
				point_Flag=5;
				count_Flag=0;
			}
			//停留在区域5
			else if(point_Flag==5)
			{
				point_Flag=5;
				count_Flag=0;
			}
			//否则不在指定点则运动到模式初始点
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}

	//进入模式4
	if(Mode_move==4)
	{
		if(count_Flag==ms20)
		{
			//点1待够时间则去指定的下一目标点9
			if(point_Flag==1)
			{
				point_Flag=10;
				count_Flag=0;
			}
			if(point_Flag==10)
			{
				point_Flag=9;
				count_Flag=0;
			}
			//停留在区域9
			if(point_Flag==9)
			{
				point_Flag=9;
				count_Flag=0;
			}
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}
	
	
	//进入模式5
	if(Mode_move==5)
	{
		if(count_Flag==ms20)
		{
			//在点1待够时间则到指定的下一点
			if(point_Flag==1)
			{
				point_Flag=2;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==2)
			{
				point_Flag=6;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==6)
			{
				point_Flag=9;
				count_Flag=0;
			}
			else if(point_Flag==9)
			{
				point_Flag=9;
				count_Flag=0;
			}
			//否则不在指定点则运动到模式初始点
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}
	
	//进入模式6
	//从A区域到B区域-C-D，在D停下来
	if(Mode_move==6)
	{
		if(count_Flag==ms20)
		{
			//在点1待够时间则到指定的下一点
			if(point_Flag==A)
			{
				point_Flag=B;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==B)
			{
				point_Flag=C;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==C)
			{
				point_Flag=D;
				count_Flag=0;
			}
			else if(point_Flag==D)
			{
				point_Flag=D;
				count_Flag=0;
			}
			//否则不在指定点则运动到模式初始点
			else
			{
				point_Flag=A;
				count_Flag=0;
			}
		}
	}
	
	
	//进入模式7，环绕点5
	if(Mode_move==7)
	{
		if(count_Flag==ms20)
		{
			//在点1待够时间则到指定的下一点
			if(point_Flag==4)
			{
				point_Flag=11;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==11)
			{
				point_Flag=12;
				count_Flag=0;
			}
			//不在点1判断在点4的话，去下一点
			else if(point_Flag==12)
			{
				point_Flag=13;
				count_Flag=0;
			}
			else if(point_Flag==13)
			{
				point_Flag=14;
				count_Flag=0;
			}
			else if(point_Flag==14)
			{
				point_Flag=15;
				count_Flag=0;
			}
			else if(point_Flag==15)
			{
				point_Flag=16;
				count_Flag=0;
			}
			else if(point_Flag==16)
			{
				point_Flag=17;
				count_Flag=0;
			}
			else if(point_Flag==17)
			{
				point_Flag=18;
				count_Flag=0;
			}
			else if(point_Flag==18)
			{
				point_Flag=11;
				count_Flag=0;
			}
			//否则不在指定点则运动到模式初始点
			else
			{
				point_Flag=4;
				count_Flag=0;
			}
		}
	}
	
	if(Mode_move==8)
	{
		if(count_Flag==ms20)
		{
		}
	}
}

