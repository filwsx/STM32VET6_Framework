//#ifndef  __BLUEBOOTH_H
//#define __BLUEBOOTH_H
#include "sys.h"
/**************************************************************************
功能：蓝牙模块，蓝牙连接与通信协议
时间：20190712 
作者：郭浩
**************************************************************************/
void bluebooth_Init(int bound);
int test(void);
int search(void);
int contact(int i);
void protocol(void);
float getNumber(u8 i);
void returnProtocol(void);
//#endif
