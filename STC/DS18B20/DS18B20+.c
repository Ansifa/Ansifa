/**************************************
�¶ȴ�����DS18B20���Գ���
��оƬ  : STC12C5A60S2 (1T)
����Ƶ��: 12.000MHz
**************************************/

#include "REG51.H"
#include "INTRINS.H"

typedef unsigned char BYTE;

sbit DQ = P3 ^ 5;                   //DS18B20�����ݿ�λP3.3
BYTE TPH;                           //����¶�ֵ�ĸ��ֽ�
BYTE TPL;                           //����¶�ֵ�ĵ��ֽ�

void DelayXus(BYTE n);
void DS18B20_Reset();
void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();
int Nowtemp = 0;
void main()
{
    unsigned int t = 0;

}
/*****��ȡ�¶�*****/
void check_wendu(void)
{
    uint a, b, c;
    c = ReadTemperature() - 5; //��ȡ�¶�ֵ����ȥ DS18B20 ����Ư���
    a = c / 100;    //����õ�ʮλ����
    b = c / 10 - a * 10;    //����õ���λ����
    m = c / 10;    //����õ�����λ
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
}

/*��ȡ�¶�*/
unsigned int ReadTemperature(void)
{
    DS18B20_Reset();                //�豸��λ
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0x44);        //��ʼת������
    while (!DQ);                    //�ȴ�ת�����

    DS18B20_Reset();                //�豸��λ
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0xBE);        //���ݴ�洢������
    TPL = DS18B20_ReadByte();       //���¶ȵ��ֽ�
    TPH = DS18B20_ReadByte();       //���¶ȸ��ֽ�
    t = TPH;
    t <<= 8;
    t = t | TPL;
    tt = t * 0.0625;
    t = tt * 10 + 0.5;
    return(t);
}

/**************************************
��ʱX΢��(STC12C5A60S2@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
����ʱ������ʹ��1T��ָ�����ڽ��м���,�봫ͳ��12T��MCU��ͬ
**************************************/
void DelayXus(BYTE n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}

/**************************************
��λDS18B20,������豸�Ƿ����
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //�ͳ��͵�ƽ��λ�ź�
        DelayXus(240);              //��ʱ����480us
        DelayXus(240);
        DQ = 1;                     //�ͷ�������
        DelayXus(60);               //�ȴ�60us
        CY = DQ;                    //����������
        DelayXus(240);              //�ȴ��豸�ͷ�������
        DelayXus(180);
    }
}

/**************************************
��DS18B20��1�ֽ�����
**************************************/
BYTE DS18B20_ReadByte()
{
    BYTE i;
    BYTE dat = 0;

    for (i = 0; i < 8; i++)         //8λ������
    {
        dat >>= 1;
        DQ = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        DQ = 1;                     //׼������
        DelayXus(1);                //������ʱ
        if (DQ) dat |= 0x80;        //��ȡ����
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

/**************************************
��DS18B20д1�ֽ�����
**************************************/
void DS18B20_WriteByte(BYTE dat)
{
    char i;

    for (i = 0; i < 8; i++)         //8λ������
    {
        DQ = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        dat >>= 1;                  //�ͳ�����
        DQ = CY;
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
        DQ = 1;                     //�ָ�������
        DelayXus(1);                //�ָ���ʱ
    }
}
