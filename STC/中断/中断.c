/*
STC15F104E�ж�ʵ��
Ansifa
2012-2-19
ʱ��Ƶ��5~12M����
*/

#include "reg51.h"
bit FLAG;
sbit Pin = P3 ^ 3;

void delay(unsigned int a) //��ʱ
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
    //������LED
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
    IT0 = 1;   //���� INT0 ���жϴ�����ʽ (1:Falling only 0:Rising & Falling)
    EX0 = 1;                   //����INT0 �ж�
    EA = 1;                    //�����ж�
    while (1);
}