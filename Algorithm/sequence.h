#ifndef __sequence_H
#define __sequence_H
#include "sys.h"  
/**************************************************************************
功能：排序
时间：20190713 1111 
作者：郭浩
**************************************************************************/

void InsertSort(u16* a,int len);
void ShellSort(u16* a,int len);
void SelectSort(u16* a,int len);
void swap(u16 *a, u16 *b);
void BubbleSort(u16* a,int len);

#endif
