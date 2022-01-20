#include "sequence.h"

/**************************************************************************
功能：插入排序
输入：u16数组，无序
输出：u16数组，从大到小
时间：20190713 1111
作者：网友
**************************************************************************/
void InsertSort(u16* a,int len)
{
    int begin = 1;
    int i = 0;
    while(begin < len)
    {
        u16 key = a[begin];
        for(i = begin-1;i>=0;i--)
        {
            if(a[i]<=key)
            {
                a[i+1] = key;
                break;
            }
            a[i+1] = a[i];
        }
        if(i<0)
            a[0] = key;
        begin++;
    }
}

/**************************************************************************
功能：希尔排序
输入：u16数组，无序
输出：u16数组，从大到小
时间：20190713 1111
作者：网友
**************************************************************************/
void ShellSort(u16* a,int len)
{
    int gap = len;                                                                                                                          
    while(gap > 1)
    {   
        gap = gap/3 + 1;
        for(int i = gap;i < len;i++)
        {   
            u16 key = a[i];
            int start = i - gap;
            while(start >= 0 && key <= a[start])
            {   
                a[start+gap] = a[start];
                start -= gap;
            }   
            a[start + gap] = key;
        }   
    }   
}

/**************************************************************************
功能：选择排序
输入：u16数组，无序
输出：u16数组，从大到小
时间：20190713 1111
作者：网友
**************************************************************************/
void SelectSort(u16* a,int len)
{
    int begin = 0;
    int end = len - 1;
    int max = 0;
    int min = 0;
    while(begin < end)
    {
        max = begin,min = begin;
        for(int i = begin;i <= end;i++)
        {
            if(a[i]>=a[max])
            {
                max = i;
            }
            if(a[i]<a[min])
            {
                min = i;
            }
        }
        if(max == begin && min == end)
        {
            swap(&a[max],&a[end]);
            continue;
        }
 
        if(max == begin)
        {
            swap(&a[max],&a[end]);
            swap(&a[min],&a[begin]);
            continue;
        }
 
        if(min == end)
        {
            swap(&a[min],&a[begin]);
            swap(&a[max],&a[end]);
            continue;
        }
        swap(&a[min],&a[begin]);
        swap(&a[max],&a[end]);
        begin++;
        end--;
    } 
}

/**************************************************************************
功能：冒泡排序
输入：u16数组，无序
输出：u16数组，从大到小
时间：20190713 1111
作者：网友
**************************************************************************/
void BubbleSort(u16* a,int len)
{
    int end = len-1;
    while(end > 0)
    {
        u8 exchange = 0;
        for(int i = 0;i < end;i++)
        {
            if(a[i]>a[i+1])
            {
            swap(&a[i],&a[i+1]);
            exchange = 1;
            }
        }
        if(exchange == 0)
            return;
        else
            exchange = 0;
        end--;
    }
}

/**************************************************************************
功能：交换数组内的两个量的位置
输入：数组位置地址
输出：无
时间：20190713 1111
作者：网友
**************************************************************************/
void swap(u16 *a, u16 *b){
	u16 temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
