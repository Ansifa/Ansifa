/*************	用户系统配置	**************/
#define MAIN_Fosc		12000000L	//定义主时钟, 模拟串口和红外接收会自动适应。5~36MHZ
#define D_TIMER0		125			//选择定时器时间, us, 红外接收要求在60us~250us之间
#define	User_code		0xFD02		//定义红外接收用户码

/*************	以下宏定义用户请勿修改	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int
#define freq_base			(MAIN_Fosc / 1200)
#define Timer0_Reload		(65536 - (D_TIMER0 * freq_base / 10000))

/*************	本地常量声明	**************/

/*************	本地变量声明	**************/
sbit	P_TXD1 = P3 ^ 1;		//定义模拟串口发送脚，打印信息用
sbit	P_IR_RX = P3 ^ 2;		//定义红外接收输入端口
sbit	LED1 = P3 ^ 3;
sbit	LED2 = P3 ^ 4;
sbit	LED3 = P3 ^ 5;
bit		P_IR_RX_temp;		//Last sample
bit		B_IR_Sync;			//已收到同步标志
uchar	IR_SampleCnt;		//采样计数
uchar	IR_BitCnt;			//编码位数
uchar	IR_UserH;			//用户码(地址)高字节
uchar	IR_UserL;			//用户码(地址)低字节
uchar	IR_data;			//数据原码
uchar	IR_DataShit;		//数据反码
bit		B_IrUserErr;		//User code error flag
bit		B_IR_Press;			//Key press flag,include repeat key.
uchar	IR_code;			//IR code	红外键码

/*************	本地函数声明	**************/
void Tx1Send(uchar dat);
uchar	HEX2ASCII(uchar dat);
void InitTimer(void);
void PrintString(unsigned char code *puts);

/*************  外部函数和变量声明 *****************/

/********************* 主函数 *************************/
void main(void)
{
    InitTimer();		//初始化Timer
    LED1 = LED2 = LED3 = 1;
    PrintString("****** STC系列MCU红外接收程序 2010-12-10 ******\r\n");	//上电后串口发送一条提示信息
    while(1)
    {
        if(B_IR_Press)		//有IR键按下
        {
            PrintString("红外码: 0x");			//提示红外键码
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
            Tx1Send(HEX2ASCII(IR_code >> 4));	//键码高半字节
            Tx1Send(HEX2ASCII(IR_code));		//键码低半字节
            if(B_IrUserErr)						//用户码错误，则发送用户码
            {
                Tx1Send(' ');					//发空格
                Tx1Send(' ');					//发空格
                PrintString("用户码: 0x");		//提示用户码
                Tx1Send(HEX2ASCII(IR_UserH >> 4));	//用户码高字节的高半字节
                Tx1Send(HEX2ASCII(IR_UserH));		//用户码高字节的低半字节
                Tx1Send(HEX2ASCII(IR_UserL >> 4));	//用户码低字节的高半字节
                Tx1Send(HEX2ASCII(IR_UserL));		//用户码低字节的低半字节
            }
            Tx1Send(0x0d);		//发回车
            Tx1Send(0x0a);		//发回车
            B_IR_Press = 0;		//清除IR键按下标志
        }
    }
}

/********************* 十六进制转ASCII函数 *************************/
uchar	HEX2ASCII(uchar dat)
{
    dat &= 0x0f;
    if(dat <= 9)	return (dat + '0');	//数字0~9
    return (dat - 10 + 'A');			//字母A~F
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
/******************** 红外采样时间宏定义, 用户不要随意修改	*******************/
#if ((D_TIMER0 <= 250) && (D_TIMER0 >= 60))
#define	D_IR_sample			D_TIMER0		//定义采样时间，在60us~250us之间
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
    if(F0 && !P_IR_RX_temp)					//Last sample is high，and current sample is low, so is fall edge
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
                    if(~IR_DataShit == IR_data)		//判断数据正反码
                    {
                        if((IR_UserH == (User_code / 256)) &&
                                IR_UserL == (User_code % 256))
                            B_IrUserErr = 0;	//User code is righe
                        else	B_IrUserErr = 1;	//user code is wrong

                        IR_code      = IR_data;
                        B_IR_Press   = 1;			//数据有效
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

/**************** Timer初始化函数 ******************************/
void InitTimer(void)
{
    TMOD = 0;		//for STC15Fxxx系列	Timer0 as 16bit reload timer.
    TH0 = Timer0_Reload / 256;
    TL0 = Timer0_Reload % 256;
    ET0 = 1;
    TR0 = 1;
    EA  = 1;
}

/********************** Timer0中断函数************************/
void timer0 (void) interrupt 1
{
    IR_RX_HT6121();
}

/********************** 模拟串口相关函数************************/
void BitTime(void)	//位时间函数
{
    uint i;
    i = ((MAIN_Fosc / 100) * 104) / 140000 - 1;		//根据主时钟来计算位时间
    while(--i);
}
//模拟串口发送
void Tx1Send(uchar dat)		//9600，N，8，1		发送一个字节
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
void PrintString(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//遇到停止符0结束
}
