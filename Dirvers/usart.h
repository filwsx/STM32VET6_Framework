#ifndef __USART_H
#define __USART_H
#include "sys.h"
/**************************************************************************
功能：串口初始化，发送，接收功能
	USART1,USART2,USART3,UART4,UART5
时间：20190712 
作者：郭浩
**************************************************************************/

#define USART_REC_LEN  	200  	//定义最大接收字节数 200
#define zhentou 0xaa		//帧头
#define zhenwei 0x0d		//帧尾

/**
//the usart pin
//@author:GuoHao
//@time:20190604 1524
USART1_TX:PA9  ,USART1_RX: PA10 
USART1_Remap_TX:PB6  ,USART1_Remap_RX:PB7

USART2_TX: PA2 ,USART2_RX:PA3
USART2_Remap_TX:PD5  ,USART2_Remap_RX:PD6

USART3_TX:PB10  ,USART3_RX:PB11
USART3_Remap_TX:PD8  ,USART3_Remap_RX:PD9
USART3_Remap_TX:PC10  ,USART3_Remap_RX:PC11

UART4_TX:PC10  ,UART4_RX:PC11 
UART5_TX:PC12  ,UART5_RX:PD2
**/

//@author:GuoHao
//@time:20190604 1257
//function:Here can enable the USART code
//1:enable , 0:disable
#define EN_USART1 1
#define EN_USART2 1
#define EN_USART3 0
#define EN_UART4 0
#define EN_UART5 0

int chart_count(u8 *content);
	
//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART1_RX_BUF[USART_REC_LEN];
//接收状态标记
extern u8 USART1_RX_count, USART1_RX_flag;		
//define the USART1 initialization
void USART1_init(u32 bound,u8 Remap_sign);
//define the USART1 send function
void USART1_send(u8 *content);
void USART1_send16(u16 *content,int j);
void USART1_send_len(u8 *content,int j);

extern u8  USART2_RX_BUF[USART_REC_LEN];
extern u8 USART2_RX_count, USART2_RX_flag;
void USART2_init(u32 bound,u8 Remap_sign);
void USART2_send(u8 *content);
void USART2_send16(u16 *content,int j);
void USART2_send_len(u8 *content,int j);


//The USART3 have 2 Remap
//Remap_sign == 1:PC10,11
//Remap_sign == 2:PD8,9  disable
extern u8  USART3_RX_BUF[USART_REC_LEN];
extern u8 USART3_RX_count, USART3_RX_flag;
void USART3_init(u32 bound,u8 Remap_sign);
void USART3_send(u8 *content);
void USART3_send16(u16 *content,int j);
void USART3_send_len(u8 *content,int j);

extern u8  UART4_RX_BUF[USART_REC_LEN];
extern u8 UART4_RX_count, UART4_RX_flag;
void UART4_init(u32 bound);
void UART4_send(u8 *content);
void UART4_send16(u16 *content,int j);
void UART4_send_len(u8 *content,int j);

extern u8  UART5_RX_BUF[USART_REC_LEN];
extern u8 UART5_RX_count, UART5_RX_flag;
void UART5_init(u32 bound);
void UART5_send(u8 *content);
void UART5_send16(u16 *content,int j);
void UART5_send_len(u8 *content,int j);

u8 getString(USART_TypeDef* USARTX);
#endif
