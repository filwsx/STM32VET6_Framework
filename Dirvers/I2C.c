#include "I2C.h"

#define GPIO_Pin_SDA (I2CX->GPIO_Pin_SDA)
#define GPIO_Pin_SCL (I2CX->GPIO_Pin_SCL)
#define GPIOX (I2CX->GPIOX)

/************************************************************************************************************************
时间：20190730  2013
作者：网友&郭浩
初始化模拟IIC：结构体赋值，引脚使能
犯错地方：
	结构体成员定义有问题，以GPIOE为例，在库函数内为((GPIO_TypeDef *) GPIOE_BASE)，是GPIO_TypeDef类型的指针
	但是我直接定义为GPIO_TypeDef结构体，然后使用&取指，这样取出来的是有问题的，不是GPIO_TypeDef类型的指针
************************************************************************************************************************/
void I2C_Soft_Init(I2C_Soft* I2CX,GPIO_TypeDef* GPIOX0,uint16_t GPIO_Pin_SCL0,uint16_t GPIO_Pin_SDA0)
{
	GPIOX = GPIOX0;
	GPIO_Pin_SCL = GPIO_Pin_SCL0;
	GPIO_Pin_SDA = GPIO_Pin_SDA0;
	
	GPIO_Init_One_AF(GPIOX,GPIO_Pin_SDA|GPIO_Pin_SCL,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	
	GPIO_SetBits(GPIOX,GPIO_Pin_SDA|GPIO_Pin_SCL);
}


void IIC_Delay()
{
	delay_us(2);
}

void IIC_Start(I2C_Soft* I2CX)
{
	SDA_OUT(I2CX);     //sda线输出
	IIC_SDA(I2CX,1);	  	  
	IIC_SCL(I2CX,1);
	IIC_Delay();
 	IIC_SDA(I2CX,0);//START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	IIC_SCL(I2CX,0);//钳住I2C总线，准备发送或接收数据 
}

void IIC_Stop(I2C_Soft* I2CX)
{
	SDA_OUT(I2CX);//sda线输出
	IIC_SCL(I2CX,0);
	IIC_SDA(I2CX,0);//STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	IIC_SCL(I2CX,1); 
	IIC_SDA(I2CX,1);//发送I2C总线结束信号
	IIC_Delay();
}

void IIC_Send_Byte(I2C_Soft* I2CX,u8 txd)
{
  u8 t;   
	SDA_OUT(I2CX); 	    
	IIC_SCL(I2CX,0);//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
			IIC_SDA(I2CX,(txd&0x80)>>7);
			txd<<=1; 	  
			IIC_SCL(I2CX,1);
			IIC_Delay(); 
			IIC_SCL(I2CX,0);	
			IIC_Delay();
	}
}

u8 IIC_Read_Byte(I2C_Soft* I2CX,unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(I2CX);//SDA设置为输入
  for(i=0;i<8;i++ )
	{
    IIC_SCL(I2CX,0); 
		IIC_Delay();
		IIC_SCL(I2CX,1);
    receive<<=1;
		if(READ_SDA(I2CX))receive++;   
		IIC_Delay(); 
   }					 
	if(!ack)
		IIC_NAck(I2CX);//发送nACK
	else
		IIC_Ack(I2CX); //发送ACK 
	
	return receive;
}

u8 IIC_Wait_Ack(I2C_Soft* I2CX)
{
	u8 ucErrTime=0;
	SDA_IN(I2CX);      //SDA设置为输入  
	IIC_SDA(I2CX,1);IIC_Delay();	   
	IIC_SCL(I2CX,1);IIC_Delay();	 
	while(READ_SDA(I2CX))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(I2CX);
			return 1;
		}
	}
	IIC_SCL(I2CX,0);//时钟输出0 	   
	return 0;
}

void IIC_Ack(I2C_Soft* I2CX)
{
	IIC_SCL(I2CX,1);
	SDA_OUT(I2CX);
	IIC_SDA(I2CX,0);
	IIC_Delay();
	IIC_SCL(I2CX,1);
	IIC_Delay();
	IIC_SCL(I2CX,0);
}

void IIC_NAck(I2C_Soft* I2CX)
{
	IIC_SCL(I2CX,0);
	SDA_OUT(I2CX);
	IIC_SDA(I2CX,0);
	IIC_Delay();
	IIC_SCL(I2CX,1);
	IIC_Delay();
	IIC_SCL(I2CX,0);
}


u8 READ_SDA(I2C_Soft* I2CX)
{
	u8 result=GPIO_ReadInputDataBit(GPIOX,GPIO_Pin_SDA);

	return result;
}

void IIC_SDA(I2C_Soft* I2CX,u8 Level)
{
	if(Level==0)
	{
		GPIO_ResetBits(GPIOX,GPIO_Pin_SDA);
	}
	else if(Level==1)
	{
		GPIO_SetBits(GPIOX,GPIO_Pin_SDA);
	}
}

void IIC_SCL(I2C_Soft* I2CX,u8 Level)
{
	if(Level==0)
	{
		GPIO_ResetBits(GPIOX,GPIO_Pin_SCL);
	}
	else if(Level==1)
	{
		GPIO_SetBits(GPIOX,GPIO_Pin_SCL);
	}
}

void SDA_IN(I2C_Soft* I2CX)
{
	GPIO_Init_One_IN(GPIOX,GPIO_Pin_SDA,GPIO_Mode_IPD);
}

void SDA_OUT(I2C_Soft* I2CX)
{
	GPIO_Init_One_AF(GPIOX,GPIO_Pin_SDA,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
}

