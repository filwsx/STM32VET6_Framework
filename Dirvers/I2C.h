#ifndef __I2C_H
#define __I2C_H
#include "sys.h"  
/**************************************************************************
功能：I2C通信
时间：20190712 
作者：郭浩

I2C1:
		SDA:PB7				SCL:PB6
		
		Remap：
			SDA:PB9			SCL:PB8
I2C2:
		SDA:PB11			SCL:PB10
		
硬件I2C部分
**************************************************************************/
void I2CX_Init(I2C_TypeDef* I2CX);

/**************************************************************************
功能：软件模拟I2C部分
时间：20190715  
作者：郭浩
**************************************************************************/
typedef struct
{
	GPIO_TypeDef* GPIOX;
	uint16_t GPIO_Pin_SDA;
	uint16_t GPIO_Pin_SCL;
}I2C_Soft;

//IIC所有操作函数
void IIC_Delay(void);																		//IIC延时函数
void IIC_Start(I2C_Soft* I2CX);													//发送IIC开始信号
void IIC_Stop(I2C_Soft* I2CX);													//发送IIC停止信号
void IIC_Send_Byte(I2C_Soft* I2CX,u8 txd);							//IIC发送一个字节
u8 IIC_Read_Byte(I2C_Soft* I2CX,unsigned char ack);			//IIC读取一个字节
u8 IIC_Wait_Ack(I2C_Soft* I2CX);												//IIC等待ACK信号
void IIC_Ack(I2C_Soft* I2CX);														//IIC发送ACK信号
void IIC_NAck(I2C_Soft* I2CX);													//IIC不发送ACK信号

//初始化IIC：IO与结构体
void I2C_Soft_Init(I2C_Soft* I2CX,GPIO_TypeDef* GPIOX,uint16_t GPIO_Pin_SCL,uint16_t GPIO_Pin_SDA);

//IO操作函数
u8 READ_SDA(I2C_Soft* I2CX);														//输入SDA 
void IIC_SDA(I2C_Soft* I2CX,u8 Level);									//SDA
void IIC_SCL(I2C_Soft* I2CX,u8 Level);									//SCL

//IO方向设置
void SDA_IN(I2C_Soft* I2CX);
void SDA_OUT(I2C_Soft* I2CX);
#endif

