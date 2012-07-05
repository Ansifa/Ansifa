/*
STC15F104E中断实验
Ansifa
2012-2-19
时钟频率5~12M随意
*/

#include "reg51.h"
bit FLAG;
sbit Pin = P3 ^ 3;

void delay(unsigned int a) //延时
{
    unsigned int i;
    while( a-- != 0)
    {
        for(i = 0; i < 600; i++);
    }
}

void exint0() interrupt 0
{
    FLAG = INT0;
    //闪三次LED
    Pin = 1;
    delay(100);
    Pin = 0;
    delay(100);
    Pin = 1;
    delay(100);
    Pin = 0;
}

void main()
{
    IT0 = 1;   //设置 INT0 的中断触发方式 (1:Falling only 0:Rising & Falling)
    EX0 = 1;                   //允许INT0 中断
    EA = 1;                    //开总中断
    while (1);
}