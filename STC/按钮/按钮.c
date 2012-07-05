/*
STC15F104E按键实验
Ansifa
2012-2-19

硬件连接方法：
一个LED串联一个220Ω电阻，接到VCC和P3.3引脚。
一个按钮接到P3.2与GND之间。
*/

#include "reg51.h"
sbit key = P3 ^ 2;  //按钮
sbit LED = P3 ^ 3;  //灯

void DelayMS(n)		//@5MHz 软件延时n毫秒函数
{
	unsigned char i, j;
	i = 5*n;
	j = 218;
	do
	{
		while (--j);
	} while (--i);
}


void main()
{   LED=1;     //LED=0是亮，1是灭，初值灭灯
    while(1)  //制造一个死循环
    {
	 //先检测高电平，等待30ms后检测是否低电平，就能把下降沿检测出来，这个是下降沿触发电路，有防抖功能。
        if(key == 1)   //因为按钮接key端和GND，所以是下降沿触发。
        {
            DelayMS(30);
            if(key == 0)
            {
				LED = !LED;  //翻转LED亮灭状态
            }
        }
    }
}