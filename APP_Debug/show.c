#include "show.h"

unsigned char DataScope_OutPut_Buffer[42] = {0};	   //串口发送缓冲区
unsigned char i,temp;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
u8 page_flag=0;
extern pid_k pidx,pidy,pidvx,pidvy;

void DataScope(void)
{   
	DataScope_Get_Channel_Data( pointX, 1 );      
	DataScope_Get_Channel_Data( globalValue_x, 2 );         
	DataScope_Get_Channel_Data( SpeedX, 3 ); 
	DataScope_Get_Channel_Data( pointY, 4 );
	DataScope_Get_Channel_Data( globalValue_y, 5 ); 
	DataScope_Get_Channel_Data( SpeedY, 6 );
	DataScope_Get_Channel_Data( 0, 7 ); 
	Send_Count = DataScope_Data_Generate(7);
	for( i = 0 ; i < Send_Count; i++) 
	{
		while((USART2->SR&0X40)==0);  
		USART2->DR = DataScope_OutPut_Buffer[i]; 	
	}
}

void oled_show(void)
{	
	static u8 last_page_flag;
	
	//用于判断是否换页，换页的话需要清屏
	//不能每次都清屏，会造成屏幕闪烁
	if (last_page_flag!=page_flag)
	{
			OLED_Clear();
			last_page_flag=page_flag;
	}
	
	if(page_flag==0){
		//1 line
		OLED_ShowString(0,0,"X:");	  OLED_ShowNumber(15,0,pointX,3,12) ; 
		OLED_ShowString(40,0,"Y:");   OLED_ShowNumber(55,0,pointY,3,12);
		
		if(SpeedX>=0)
		{
			OLED_ShowString(80,0,"Vx:+");	OLED_ShowNumber(110,0,SpeedX,2,12) ; 
		}
		else
		{
			OLED_ShowString(80,0,"Vx:-");	OLED_ShowNumber(110,0,-SpeedX,2,12) ; 
		}
		
		//2 line
		OLED_ShowString(0,13,"X:");	  OLED_ShowNumber(15,13,globalValue_x,3,12) ; 
		OLED_ShowString(40,13,"Y:");  OLED_ShowNumber(55,13,globalValue_y,3,12); 
		OLED_ShowString(80,13,"Vy:");	OLED_ShowNumber(105,13,SpeedY,3,12) ; 
		
		if(SpeedY>=0)
		{
			OLED_ShowString(80,13,"Vy:+");	OLED_ShowNumber(110,13,SpeedY,2,12) ; 
		}
		else
		{
			OLED_ShowString(80,13,"Vy:-");	OLED_ShowNumber(110,13,-SpeedY,2,12) ; 
		}
		//3 line
		OLED_ShowString(0,26,"P:");	  OLED_ShowNumber(15,26,pidx.KP,3,12) ; 
		OLED_ShowString(40,26,"I:");  OLED_ShowNumber(55,26,pidx.KI*10000,3,12); 
		OLED_ShowString(80,26,"D:");	OLED_ShowNumber(105,26,pidx.KD,3,12) ; 
		
		//4 line
		OLED_ShowString(0,39,"P:");	  OLED_ShowNumber(15,39,pidvx.KP,3,12) ; 
		OLED_ShowString(40,39,"I:");  OLED_ShowNumber(55,39,pidvx.KI*10000,3,12); 
		OLED_ShowString(80,39,"D:");	OLED_ShowNumber(105,39,pidvx.KD,3,12) ; 
	}
	else
	{
		OLED_ShowString(0,0,"DJX:");	  OLED_ShowNumber(32,0,pwmoutX,4,12) ; 
		OLED_ShowString(65,0,"DJY:");   OLED_ShowNumber(97,0,pwmoutY,4,12); 
		
		//2 line
		OLED_ShowString(0,13,"CPX:");	  OLED_ShowNumber(32,13,chang_PointX,4,12) ; 
		OLED_ShowString(65,13,"CPY:");  OLED_ShowNumber(97,13,chang_PointY,4,12); 
		
		//3 line
		OLED_ShowString(0,26,"CSX:");	  OLED_ShowNumber(32,26,chang_SpeedX,4,12) ; 
		OLED_ShowString(65,26,"CSY:");  OLED_ShowNumber(97,26,chang_SpeedY,4,12); 
		
		//4 line
		OLED_ShowString(0,39,"MM:");	  OLED_ShowNumber(22,39,Mode_move,2,12) ; 
		OLED_ShowString(42,39,"PF:");	OLED_ShowNumber(64,39,point_Flag,2,12) ; 
		OLED_ShowString(84,39,"CF:");  OLED_ShowNumber(106,39,count_Flag,2,12); 
	}

	
	//5 line
	if(control_Flag==1)
	{
		OLED_ShowString(10,52,"ON  ");
	}
	else
	{
		OLED_ShowString(10,52,"OFF");
	}
	
	OLED_ShowString(40,52,"MD:");
	//OLED_ShowString(70,52,"TEST00") ; 
	
	button_display();
	OLED_Refresh_Gram();
}

//函数说明：将单精度浮点数据转成4字节数据并存入指定地址 
//附加说明：用户无需直接操作此函数 
//target:目标单精度数据
//buf:待写入数组
//beg:指定从数组第几个元素开始写入
//函数无返回 
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到float的地址
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
 
 
//函数说明：将待发送通道的单精度浮点数据写入发送缓冲区
//Data：通道数据
//Channel：选择通道（1-10）
//函数无返回 
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
	if ( (Channel > 10) || (Channel == 0) ) return;  //通道个数大于10或等于0，直接跳出，不执行函数
  else
  {
     switch (Channel)
		{
      case 1:  Float2Byte(&Data,DataScope_OutPut_Buffer,1); break;
      case 2:  Float2Byte(&Data,DataScope_OutPut_Buffer,5); break;
		  case 3:  Float2Byte(&Data,DataScope_OutPut_Buffer,9); break;
		  case 4:  Float2Byte(&Data,DataScope_OutPut_Buffer,13); break;
		  case 5:  Float2Byte(&Data,DataScope_OutPut_Buffer,17); break;
		  case 6:  Float2Byte(&Data,DataScope_OutPut_Buffer,21); break;
		  case 7:  Float2Byte(&Data,DataScope_OutPut_Buffer,25); break;
		  case 8:  Float2Byte(&Data,DataScope_OutPut_Buffer,29); break;
		  case 9:  Float2Byte(&Data,DataScope_OutPut_Buffer,33); break;
		  case 10: Float2Byte(&Data,DataScope_OutPut_Buffer,37); break;
		}
  }	 
}


//函数说明：生成 DataScopeV1.0 能正确识别的帧格式
//Channel_Number，需要发送的通道个数
//返回发送缓冲区数据个数
//返回0表示帧格式生成失败 
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
	if ( (Channel_Number > 10) || (Channel_Number == 0) ) { return 0; }  //通道个数大于10或等于0，直接跳出，不执行函数
  else
  {	
	 DataScope_OutPut_Buffer[0] = '$';  //帧头
		
	 switch(Channel_Number)   
   { 
		 case 1:   DataScope_OutPut_Buffer[5]  =  5; return  6;  
		 case 2:   DataScope_OutPut_Buffer[9]  =  9; return 10;
		 case 3:   DataScope_OutPut_Buffer[13] = 13; return 14; 
		 case 4:   DataScope_OutPut_Buffer[17] = 17; return 18;
		 case 5:   DataScope_OutPut_Buffer[21] = 21; return 22;  
		 case 6:   DataScope_OutPut_Buffer[25] = 25; return 26;
		 case 7:   DataScope_OutPut_Buffer[29] = 29; return 30; 
		 case 8:   DataScope_OutPut_Buffer[33] = 33; return 34; 
		 case 9:   DataScope_OutPut_Buffer[37] = 37; return 38;
     case 10:  DataScope_OutPut_Buffer[41] = 41; return 42; 
   }	 
  }
	return 0;
}
