/*************	�û�ϵͳ����	**************/
#define MAIN_Fosc		12000000L	//������ʱ��, ģ�⴮�ںͺ�����ջ��Զ���Ӧ��5~36MHZ
#define D_TIMER0		125			//ѡ��ʱ��ʱ��, us, �������Ҫ����60us~250us֮��
#define	User_code		0xFD02		//�����������û���

/*************	���º궨���û������޸�	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int
#define freq_base			(MAIN_Fosc / 1200)
#define Timer0_Reload		(65536 - (D_TIMER0 * freq_base / 10000))

/*************	���س�������	**************/

/*************	���ر�������	**************/
sbit	P_TXD1 = P3 ^ 1;		//����ģ�⴮�ڷ��ͽţ���ӡ��Ϣ��
sbit	P_IR_RX = P3 ^ 2;		//��������������˿�
sbit	LED1 = P3 ^ 3;
sbit	LED2 = P3 ^ 4;
sbit	LED3 = P3 ^ 5;
bit		P_IR_RX_temp;		//Last sample
bit		B_IR_Sync;			//���յ�ͬ����־
uchar	IR_SampleCnt;		//��������
uchar	IR_BitCnt;			//����λ��
uchar	IR_UserH;			//�û���(��ַ)���ֽ�
uchar	IR_UserL;			//�û���(��ַ)���ֽ�
uchar	IR_data;			//����ԭ��
uchar	IR_DataShit;		//���ݷ���
bit		B_IrUserErr;		//User code error flag
bit		B_IR_Press;			//Key press flag,include repeat key.
uchar	IR_code;			//IR code	�������

/*************	���غ�������	**************/
void Tx1Send(uchar dat);
uchar	HEX2ASCII(uchar dat);
void InitTimer(void);
void PrintString(unsigned char code *puts);

/*************  �ⲿ�����ͱ������� *****************/

/********************* ������ *************************/
void main(void)
{
    InitTimer();		//��ʼ��Timer
    LED1 = LED2 = LED3 = 1;
    PrintString("****** STCϵ��MCU������ճ��� 2010-12-10 ******\r\n");	//�ϵ�󴮿ڷ���һ����ʾ��Ϣ
    while(1)
    {
        if(B_IR_Press)		//��IR������
        {
            PrintString("������: 0x");			//��ʾ�������
            if(IR_code == 0x45)
            {
                LED1 = !LED1;
            }
            if(IR_code == 0x46)
            {
                LED2 = !LED2;
            }
            if(IR_code == 0x47)
            {
                LED3 = !LED3;
            }
            Tx1Send(HEX2ASCII(IR_code >> 4));	//����߰��ֽ�
            Tx1Send(HEX2ASCII(IR_code));		//����Ͱ��ֽ�
            if(B_IrUserErr)						//�û�����������û���
            {
                Tx1Send(' ');					//���ո�
                Tx1Send(' ');					//���ո�
                PrintString("�û���: 0x");		//��ʾ�û���
                Tx1Send(HEX2ASCII(IR_UserH >> 4));	//�û�����ֽڵĸ߰��ֽ�
                Tx1Send(HEX2ASCII(IR_UserH));		//�û�����ֽڵĵͰ��ֽ�
                Tx1Send(HEX2ASCII(IR_UserL >> 4));	//�û�����ֽڵĸ߰��ֽ�
                Tx1Send(HEX2ASCII(IR_UserL));		//�û�����ֽڵĵͰ��ֽ�
            }
            Tx1Send(0x0d);		//���س�
            Tx1Send(0x0a);		//���س�
            B_IR_Press = 0;		//���IR�����±�־
        }
    }
}

/********************* ʮ������תASCII���� *************************/
uchar	HEX2ASCII(uchar dat)
{
    dat &= 0x0f;
    if(dat <= 9)	return (dat + '0');	//����0~9
    return (dat - 10 + 'A');			//��ĸA~F
}

//*******************************************************************
//*********************** IR Remote Module **************************
//*********************** IR Remote Module **************************
//this programme is used for Receive IR Remote (HT6121).
//data format: Synchro,AddressH,AddressL,data,/data, (total 32 bit).
//send a frame(85ms), pause 23ms, send synchro of another frame, pause 94ms
//data rate: 108ms/Frame

//Synchro:low=9ms,high=4.5/2.25ms,low=0.5626ms
//Bit0:high=0.5626ms,low=0.5626ms
//Bit1:high=1.6879ms,low=0.5626ms
//frame space = 23 ms or 96 ms
/******************** �������ʱ��궨��, �û���Ҫ�����޸�	*******************/
#if ((D_TIMER0 <= 250) && (D_TIMER0 >= 60))
#define	D_IR_sample			D_TIMER0		//�������ʱ�䣬��60us~250us֮��
#endif
#define D_IR_SYNC_MAX		(15000/D_IR_sample)	//SYNC max time
#define D_IR_SYNC_MIN		(9700 /D_IR_sample)	//SYNC min time
#define D_IR_SYNC_DIVIDE	(12375/D_IR_sample)	//decide data 0 or 1
#define D_IR_DATA_MAX		(3000 /D_IR_sample)	//data max time
#define D_IR_DATA_MIN		(600  /D_IR_sample)	//data min time
#define D_IR_DATA_DIVIDE	(1687 /D_IR_sample)	//decide data 0 or 1
#define D_IR_BIT_NUMBER		32					//bit number
//*******************************************************************************************
//**************************** IR RECEIVE MODULE ********************************************
void IR_RX_HT6121(void)
{
    uchar	SampleTime;
    IR_SampleCnt++;							//Sample + 1
    F0 = P_IR_RX_temp;						//Save Last sample status
    P_IR_RX_temp = P_IR_RX;					//Read current status
    if(F0 && !P_IR_RX_temp)					//Last sample is high��and current sample is low, so is fall edge
    {
        SampleTime = IR_SampleCnt;			//get the sample time
        IR_SampleCnt = 0;					//Clear the sample counter
        if(SampleTime > D_IR_SYNC_MAX)		B_IR_Sync = 0;	//large the Maxim SYNC time, then error
        else if(SampleTime >= D_IR_SYNC_MIN)					//SYNC
        {
            if(SampleTime >= D_IR_SYNC_DIVIDE)
            {
                B_IR_Sync = 1;					//has received SYNC
                IR_BitCnt = D_IR_BIT_NUMBER;	//Load bit number
            }
        }
        else if(B_IR_Sync)						//has received SYNC
        {
            if(SampleTime > D_IR_DATA_MAX)		B_IR_Sync = 0;	//data samlpe time to large
            else
            {
                IR_DataShit >>= 1;					//data shift right 1 bit
                if(SampleTime >= D_IR_DATA_DIVIDE)	IR_DataShit |= 0x80;	//devide data 0 or 1
                if(--IR_BitCnt == 0)				//bit number is over?
                {
                    B_IR_Sync = 0;					//Clear SYNC
                    if(~IR_DataShit == IR_data)		//�ж�����������
                    {
                        if((IR_UserH == (User_code / 256)) &&
                                IR_UserL == (User_code % 256))
                            B_IrUserErr = 0;	//User code is righe
                        else	B_IrUserErr = 1;	//user code is wrong

                        IR_code      = IR_data;
                        B_IR_Press   = 1;			//������Ч
                    }
                }
                else if((IR_BitCnt & 7) == 0)		//one byte receive
                {
                    IR_UserL = IR_UserH;			//Save the User code high byte
                    IR_UserH = IR_data;				//Save the User code low byte
                    IR_data  = IR_DataShit;			//Save the IR data byte
                }
            }
        }
    }
}

/**************** Timer��ʼ������ ******************************/
void InitTimer(void)
{
    TMOD = 0;		//for STC15Fxxxϵ��	Timer0 as 16bit reload timer.
    TH0 = Timer0_Reload / 256;
    TL0 = Timer0_Reload % 256;
    ET0 = 1;
    TR0 = 1;
    EA  = 1;
}

/********************** Timer0�жϺ���************************/
void timer0 (void) interrupt 1
{
    IR_RX_HT6121();
}

/********************** ģ�⴮����غ���************************/
void BitTime(void)	//λʱ�亯��
{
    uint i;
    i = ((MAIN_Fosc / 100) * 104) / 140000 - 1;		//������ʱ��������λʱ��
    while(--i);
}
//ģ�⴮�ڷ���
void Tx1Send(uchar dat)		//9600��N��8��1		����һ���ֽ�
{
    uchar	i;
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
