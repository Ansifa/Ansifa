/*
STC15F104E����ʵ��
Ansifa
2012-2-19

Ӳ�����ӷ�����
һ��LED����һ��220�����裬�ӵ�VCC��P3.3���š�
һ����ť�ӵ�P3.2��GND֮�䡣
*/

#include "reg51.h"
sbit key = P3 ^ 2;  //��ť
sbit LED = P3 ^ 3;  //��

void DelayMS(n)		//@5MHz �����ʱn���뺯��
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
{   LED=1;     //LED=0������1���𣬳�ֵ���
    while(1)  //����һ����ѭ��
    {
	 //�ȼ��ߵ�ƽ���ȴ�30ms�����Ƿ�͵�ƽ�����ܰ��½��ؼ�������������½��ش�����·���з������ܡ�
        if(key == 1)   //��Ϊ��ť��key�˺�GND���������½��ش�����
        {
            DelayMS(30);
            if(key == 0)
            {
				LED = !LED;  //��תLED����״̬
            }
        }
    }
}