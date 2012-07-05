/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
温度传感器DS18B20测试程序
主芯片  : STC12C5A60S2 (1T)
工作频率: 12.000MHz
**************************************/
#define MAIN_Fosc		11059200L	//定义主时钟, 模拟串口和红外接收会自动适应。5~36MHZ
#include "REG51.H"
#include "INTRINS.H"
typedef unsigned char BYTE;
sbit	P_TXD1 = P3 ^ 1;
sbit DQ = P3 ^ 5;                   //DS18B20的数据口位P3.3
sbit LED = P3 ^ 2;
BYTE a;                           //存放温度值的高字节
BYTE b;                           //存放温度值的低字节
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
        DS18B20_Reset();                //设备复位
        DS18B20_WriteByte(0xCC);        //跳过ROM命令
        DS18B20_WriteByte(0x44);        //开始转换命令
        while (!DQ);                    //等待转换完成
        DS18B20_Reset();                //设备复位
        DS18B20_WriteByte(0xCC);        //跳过ROM命令
        DS18B20_WriteByte(0xBE);        //读暂存存储器命令
        a = DS18B20_ReadByte();       //读温度低字节
        b = DS18B20_ReadByte();       //读温度高字节
        t = b;
        t <<= 8;
        t = t | a;
        tt = t * 0.0625;
        t = tt * 10 + 0.5;
        if((t % 100) > 32) LED = 0;
        else LED = 1;
        PrintString("温度: ");		//提示用户码
        //Tx1Send('0' + t / 1000);	//用户码高字节的高半字节
        Tx1Send('0' + (t / 100) % 10);		//用户码高字节的低半字节
        Tx1Send('0' + (t / 10) % 10);	//用户码低字节的高半字节
        Tx1Send('.');
        Tx1Send('0' + t % 10);		//用户码低字节的低半字节
        PrintString("℃");		//提示用户码
        Tx1Send(10);

    }
}

/**************************************
延时X微秒(STC12C5A60S2@12M)
不同的工作环境,需要调整此函数
此延时函数是使用1T的指令周期进行计算,与传统的12T的MCU不同
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
复位DS18B20,并检测设备是否存在
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //送出低电平复位信号
        DelayXus(240);              //延时至少480us
        DelayXus(240);
        DQ = 1;                     //释放数据线
        DelayXus(60);               //等待60us
        CY = DQ;                    //检测存在脉冲
        DelayXus(240);              //等待设备释放数据线
        DelayXus(180);
    }
}

/**************************************
从DS18B20读1字节数据
**************************************/
BYTE DS18B20_ReadByte()
{
    BYTE i;
    BYTE dat = 0;

    for (i = 0; i < 8; i++)         //8位计数器
    {
        dat >>= 1;
        DQ = 0;                     //开始时间片
        DelayXus(1);                //延时等待
        DQ = 1;                     //准备接收
        DelayXus(1);                //接收延时
        if (DQ) dat |= 0x80;        //读取数据
        DelayXus(60);               //等待时间片结束
    }

    return dat;
}

/**************************************
向DS18B20写1字节数据
**************************************/
void DS18B20_WriteByte(BYTE dat)
{
    char i;

    for (i = 0; i < 8; i++)         //8位计数器
    {
        DQ = 0;                     //开始时间片
        DelayXus(1);                //延时等待
        dat >>= 1;                  //送出数据
        DQ = CY;
        DelayXus(60);               //等待时间片结束
        DQ = 1;                     //恢复数据线
        DelayXus(1);                //恢复延时
    }
}

void BitTime(void)	//位时间函数
{
    unsigned int i;
    i = ((MAIN_Fosc / 100) * 104) / 140000 - 1;		//根据主时钟来计算位时间
    while(--i);
}
//模拟串口发送
void Tx1Send(unsigned char dat)		//9600，N，8，1		发送一个字节
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

void PrintString(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//遇到停止符0结束
}

