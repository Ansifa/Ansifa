/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
�¶ȴ�����DS18B20���Գ���
��оƬ  : STC12C5A60S2 (1T)
����Ƶ��: 12.000MHz
**************************************/
#define MAIN_Fosc		11059200L	//������ʱ��, ģ�⴮�ںͺ�����ջ��Զ���Ӧ��5~36MHZ
#include "REG51.H"
#include "INTRINS.H"
typedef unsigned char BYTE;
sbit	P_TXD1 = P3 ^ 1;
sbit DQ = P3 ^ 5;                   //DS18B20�����ݿ�λP3.3
sbit LED = P3 ^ 2;
BYTE a;                           //����¶�ֵ�ĸ��ֽ�
BYTE b;                           //����¶�ֵ�ĵ��ֽ�
unsigned int t = 0;
unsigned char *p;
float tt = 0;
void DelayXus(BYTE n);
void DS18B20_Reset();

void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();
void BitTime(void);
void Tx1Send(unsigned char dat);
void PrintString(unsigned char code *puts);
void main()
{
    LED = 1;

    while(1)
    {
        DS18B20_Reset();                //�豸��λ
        DS18B20_WriteByte(0xCC);        //����ROM����
        DS18B20_WriteByte(0x44);        //��ʼת������
        while (!DQ);                    //�ȴ�ת�����
        DS18B20_Reset();                //�豸��λ
        DS18B20_WriteByte(0xCC);        //����ROM����
        DS18B20_WriteByte(0xBE);        //���ݴ�洢������
        a = DS18B20_ReadByte();       //���¶ȵ��ֽ�
        b = DS18B20_ReadByte();       //���¶ȸ��ֽ�
        t = b;
        t <<= 8;
        t = t | a;
        tt = t * 0.0625;
        t = tt * 10 + 0.5;
        if((t % 100) > 32) LED = 0;
        else LED = 1;
        PrintString("�¶�: ");		//��ʾ�û���
        //Tx1Send('0' + t / 1000);	//�û�����ֽڵĸ߰��ֽ�
        Tx1Send('0' + (t / 100) % 10);		//�û�����ֽڵĵͰ��ֽ�
        Tx1Send('0' + (t / 10) % 10);	//�û�����ֽڵĸ߰��ֽ�
        Tx1Send('.');
        Tx1Send('0' + t % 10);		//�û�����ֽڵĵͰ��ֽ�
        PrintString("��");		//��ʾ�û���
        Tx1Send(10);

    }
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

void BitTime(void)	//λʱ�亯��
{
    unsigned int i;
    i = ((MAIN_Fosc / 100) * 104) / 140000 - 1;		//������ʱ��������λʱ��
    while(--i);
}
//ģ�⴮�ڷ���
void Tx1Send(unsigned char dat)		//9600��N��8��1		����һ���ֽ�
{
    unsigned char i;
    EA = 0;
    P_TXD1 = 0;
    BitTime();
    for(i = 0; i < 8; i++)
    {
        if(dat & 1)		P_TXD1 = 1;
        else			P_TXD1 = 0;
        dat >>= 1;
        BitTime();
    }
    P_TXD1 = 1;
    EA = 1;
    BitTime();
    BitTime();
}

void PrintString(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//����ֹͣ��0����
}

