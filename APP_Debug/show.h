#ifndef __show_H
#define __show_H
#include "sys.h" 
/**************************************************************************
功能：显示与上位机通信
时间：20190717 1438
作者：郭浩
**************************************************************************/
void oled_show(void);
void DataScope(void);

extern unsigned char DataScope_OutPut_Buffer[42];	   //待发送帧数据缓存区
void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // 写通道数据至 待发送帧数据缓存区
unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // 发送帧数据生成函数
extern u8 page_flag;
#endif


























