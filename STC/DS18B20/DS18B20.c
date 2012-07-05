/******************************************************************
�������ƣ�DS18B20 �¶Ȳ���������ϵͳ
��Ҫ˵����DS18B20 �¶ȼƣ��¶Ȳ�����Χ 0~99.9 ���϶�
���������ޱ����¶ȡ����ޱ����¶�
����������ֵ���ߵ�������ֵʱ����������
Ĭ�����ޱ����¶�Ϊ 38�桢Ĭ�����ޱ����¶�Ϊ 5��
����ֵ�����÷�Χ��������ޱ���ֵ���ڵ�ǰ���ޱ���ֵ
������ޱ���ֵ���ڵ�ǰ���ޱ���ֵ
�����ޱ���ֵ��Ϊ 0 ʱΪ�ر����ޱ�������
******************************************************************/
#include <AT89X52.h>
#include "DS18B20.h"
#define uint unsigned int
#define uchar unsigned char
//�궨��
#define SET P3_1
#define DEC P3_2
#define ADD P3_3
#define BEEP P3_7
bit shanshuo_st;
//���������
//������ټ�
//�������Ӽ�
//���������
//��˸�����־
bit beep_st;
//�����������־
sbit DIAN = P2 ^ 7;
//С����
uchar x = 0;
//������
signed char m;
//�¶�ֵȫ�ֱ���
uchar n;
//�¶�ֵȫ�ֱ���
uchar set_st = 0;
//״̬��־
signed char shangxian = 38;
//���ޱ����¶ȣ�Ĭ��ֵΪ 38
signed char xiaxian = 5;
uchar
//���ޱ����¶ȣ�Ĭ��ֵΪ 38
code
LEDData[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xff
            };
/*****��ʱ�ӳ���*****/
void Delay(uint num)
{
    while( --num );
}
/*****��ʼ����ʱ�� 0*****/
void InitTimer(void)
{
    TMOD = 0x1;
    TH0 = 0x3c;
    TL0 = 0xb0;
    //50ms������ 12M��
}
/*****��ʱ�� 0 �жϷ������*****/
void timer0(void) interrupt 1
{
    TH0 = 0x3c;
    TL0 = 0xb0;
    x++;
}
/*****�ⲿ�ж� 0 �������*****/
void int0(void) interrupt 0
{
    EX0 = 0;
    //���ⲿ�ж� 0
    if(DEC == 0 && set_st == 1)
    {
        shangxian--;
        if(shangxian < xiaxian)shangxian = xiaxian;
    }
    else if(DEC == 0 && set_st == 2)
    {
        xiaxian--;
        if(xiaxian < 0)xiaxian = 0;
    }
}
/*****�ⲿ�ж� 1 �������*****/
void int1(void) interrupt 2
{
    EX1 = 0;
    //���ⲿ�ж� 1
    if(ADD == 0 && set_st == 1)
    {
        shangxian++;
        if(shangxian > 99)shangxian = 99;
    }
    else if(ADD == 0 && set_st == 2)
    {
        xiaxian++;
        if(xiaxian > shangxian)xiaxian = shangxian;
    }
}
/*****��ȡ�¶�*****/
void check_wendu(void)
{
    uint a, b, c;
    c = ReadTemperature() - 5; //��ȡ�¶�ֵ����ȥ DS18B20 ����Ư���
    a = c / 100;
    b = c / 10 - a * 10;
    m = c / 10;
    //����õ�ʮλ����
    //����õ���λ����
    //����õ�����λ
    n = c - a * 100 - b * 10;
    if(m < 0)
    {
        m = 0;
        n = 0;
    }
    //����õ�С��λ
    //�����¶���ʾ����
    if(m > 99)
    {
        m = 99;
        n = 9;
    }
    //�����¶���ʾ����
}
/*****��ʾ������ʼ���ȴ�����*****/
Disp_init()
{
    P2 = 0xbf;
    P1 = 0xf7;
    Delay(200);
    P1 = 0xfb;
    Delay(200);
    P1 = 0xfd;
    Delay(200);
    P1 = 0xfe;
    Delay(200);
    //��ʾ-
    P1 = 0xff;
    //�ر���ʾ
}
/*****��ʾ�¶��ӳ���*****/
Disp_Temperature()
{
    //��ʾ�¶�
    P2 = 0xc6;
    P1 = 0xf7;
    Delay(300);
    //��ʾ C
    P2 = LEDData[n];
    P1 = 0xfb;
    Delay(300);
    P2 = LEDData[m%10];
    //��ʾ��λ
    //��ʾʮλ
    DIAN = 0;
    P1 = 0xfd;
    Delay(300);
    //��ʾС����
    P2 = LEDData[m/10];
    P1 = 0xfe;
    Delay(300);
    P1 = 0xff;
    //��ʾ��λ
    //�ر���ʾ
}
/*****��ʾ�����¶��ӳ���*****/
Disp_alarm(uchar baojing)
{
    P2 = 0xc6;
    //��ʾ C
    P1 = 0xf7;
    Delay(200);
    P2 = LEDData[baojing%10]; //��ʾʮλ
    P1 = 0xfb;
    Delay(200);
    P2 = LEDData[baojing/10]; //��ʾ��λ
    P1 = 0xfd;
    Delay(200);
    if(set_st == 1)P2 = 0x89;
    else if(set_st == 2)P2 = 0xc7; //���� H������ L ��ʾ
    P1 = 0xfe;
    Delay(200);
    P1 = 0xff;
    //�ر���ʾ
}
/*****�����ӳ���*****/
void Alarm()
{
    if(x >= 10)
    {
        beep_st = ~beep_st;
        x = 0;
    }
    if((m >= shangxian && beep_st == 1) || (m < xiaxian && beep_st == 1))BEEP = 0;
    else BEEP = 1;
}
/*****������*****/
void main(void)
{
    uint z;
    InitTimer();
    //��ʼ����ʱ��
    EA = 1;
    TR0 = 1;
    ET0 = 1;
    //ȫ���жϿ���
    //������ʱ�� 0
    IT0 = 1;
    IT1 = 1;
    check_wendu();
    check_wendu();
    for(z = 0; z < 300; z++)
    {
        Disp_init();
    }
    while(1)
    {
        if(SET == 0)
        {
            Delay(2000);
            do {}
            while(SET == 0);
            set_st++;
            x = 0;
            shanshuo_st = 1;
            if(set_st > 2)set_st = 0;
        }
        if(set_st == 0)
        {
            EX0 = 0;
            EX1 = 0;
            //�ر��ⲿ�ж� 0
            //�ر��ⲿ�ж� 1
            check_wendu();
            Disp_Temperature();
        }
        Alarm();
        //�������
        if(set_st == 1)
        {
            BEEP = 1;
            EX0 = 1;
            EX1 = 1;
            //�رշ�����
            //�����ⲿ�ж� 0
            //�����ⲿ�ж� 1
            if(x >= 10)
            {
                shanshuo_st = ~shanshuo_st;
                x = 0;
            }
            if(shanshuo_st)
            {
                Disp_alarm(shangxian);
            }
        }
        else if(set_st == 2)
        {
            BEEP = 1;
            EX0 = 1;
            EX1 = 1;
            //�رշ�����
            //�����ⲿ�ж� 0
            //�����ⲿ�ж� 1
            if(x >= 10)
            {
                shanshuo_st = ~shanshuo_st;
                x = 0;
            }
            if(shanshuo_st)
            {
                Disp_alarm(xiaxian);
            }
        }
    }
}
/*****END*****/