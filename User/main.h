#ifndef __MAIN_H
#define __MAIN_H
#include "sys.h" 

/**************************************************************************
功能：主函数，用于初始化以及测试
时间：20190712 
作者：郭浩
**************************************************************************/
extern int pointX,pointY,SpeedX,SpeedY;
extern int chang_PointY,chang_PointX,chang_SpeedY,chang_SpeedX;
extern float globalValue_x,globalValue_y;
extern u8 point_Flag,Mode_move;
extern u8 Mode_move;
extern u8 FlagX,FlagY,FlagPoint,FlagSpeed,control_Flag;
extern int Last_pointX,Last_pointY;
extern int Flag_ms,Flag2_ms,count_Flag;

void init(void);
void oled_show(void);
void USART_test(void);
void openmv_Init(void);
void DataScope(void);
void control(void);
void chang_point(void);
void chang_Mode(void);
void controlPoint(void);
void controlSpeed(void);
void control(void);
#endif

