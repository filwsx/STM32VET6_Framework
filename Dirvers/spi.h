#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
/**************************************************************************
功能：spi通信
时间：20190713  16XX 
作者：郭浩
**************************************************************************/
 
// SPI总线速度设置 
#define SPI_SPEED_2   	0
#define SPI_SPEED_4   	1
#define SPI_SPEED_8   	2
#define SPI_SPEED_16		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 	6
#define SPI_SPEED_256 	7

#define	EN_SPI1	1
#define	EN_SPI2	1
#define	EN_SPI3	0

#define	SPI1_CS 		PAout(4)
#define	SPI2_CS 		PBout(12)  		//片选信号
#define	SPI3_CS 		PAout(15)

/**************************************************************************
SPI_Mode=SPI_Mode_Slave||SPI_Mode_Master
SPIX=SPI1||SPI2||SPI3

SPI1:
			MOSI:PA7					MISO:PA6
			SCK:PA5						NSS:PA4
SPI2:
			MOSI:PB15					MISO:PB14
			SCK:PB13					NSS:PB12
SPI3:
			MOSI:PB5					MISO:PB4
			SCK:PB3						NSS:PA15

硬件SPI部分
**************************************************************************/
extern u8 temp_SPI1[],temp_SPI2[],temp_SPI3[];

void SPIX_Init(SPI_TypeDef* SPIX,uint16_t SPI_Mode,u8 SPI_BaudRatePrescaler);

void SPI1_ReadWriteString(u8 *temp);//SPI1总线读写一个字节
void SPI2_ReadWriteString(u8 *temp);//SPI3总线读写一个字节
void SPI3_ReadWriteString(u8 *temp);//SPI3总线读写一个字节



/**************************************************************************
功能：软件模拟SPI部分
时间：20190715  16XX 
作者：郭浩
**************************************************************************/
typedef struct
{
	uint16_t GPIOX;
	uint16_t GPIO_Pin_MOSI;
	uint16_t GPIO_Pin_MISO;
	uint16_t GPIO_Pin_SCK;
	uint16_t GPIO_Pin_NSS;
	uint16_t SPI_ModeX;
	u8 SPI_BaudRatePrescaler;
}SPI_Soft;

void SPI_Soft_Init(SPI_Soft* SPIX);

#endif

