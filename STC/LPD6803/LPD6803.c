#include <REG51.H>
#define nDots 30
#define uchar unsigned char
sbit SCLK = P3 ^ 2;
sbit SDO = P3 ^ 3;


void Delay()		//@12.000MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 176;
	k = 19;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void LED_SendData(uchar Rdt[nDots], uchar Gdt[nDots], uchar Bdt[nDots])
{
    uchar i, j, mask;
    SCLK = 0;
    SDO = 0;
    for(i = 0; i < 32; i++)
    {
        SCLK = 1;    //送起始32bits 0
        SCLK = 0;
    }

    for(i = 0; i < nDots; i++)
    {
        SDO = 1;
        SCLK = 1;
        SCLK = 0;     //送起始位'1'

        //输出5位红色
        mask = 0x10;
        for(j = 0; j < 5; j++)
        {
            if(mask & Rdt[i])  SDO = 1;
            else     SDO = 0;
            SCLK = 1;
            SCLK = 0;
            mask >>= 1;
        }
        //输出5位绿色
        mask = 0x10;
        for(j = 0; j < 5; j++)
        {
            if(mask & Gdt[i])  SDO = 1;
            else     SDO = 0;
            SCLK = 1;
            SCLK = 0;
            mask >>= 1;
        }
        //输出5位蓝色
        mask = 0x10;
        for(j = 0; j < 5; j++)
        {
            if(mask & Bdt[i])  SDO = 1;
            else     SDO = 0;
            SCLK = 1;
            SCLK = 0;
            mask >>= 1;
        }
    }
    SDO = 0;
    for(i = 0; i < nDots; i++)
    {
        SCLK = 1;
        SCLK = 0;
    }
}

void main()
{
    int i;
    uchar Rdt[nDots] = 0, Gdt[nDots] = 0, Bdt[nDots] = 0, r = 0, g = 10, b = 20;
    while(1)
    {
        r++;
        g+=2;
        b+=3;
        if(r >= 32) r = 0;
        if(g >= 32) g = 0;
        if(b >= 32) b = 0;
        for(i = 0; i < nDots; i++)
        {
            Rdt[i] = (r+i)%32;
            Gdt[i] = (g+i)%32;
            Bdt[i] = (b+i)%32;
        }
        LED_SendData(Rdt, Gdt, Bdt);
        for(i = 0; i < 6; i++) Delay();
    }
    /*    for(i = 0; i < nDots; i++)
        {
            Rdt[i] = 0;
            Gdt[i] = 0;
            Bdt[i] = i;
        }
        LED_SendData(Rdt, Gdt, Bdt);
        Delay100ms();*/
}
