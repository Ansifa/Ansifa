sbit	P_TXD1 = PinTxD;
/********************** 模拟串口相关函数************************/
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
