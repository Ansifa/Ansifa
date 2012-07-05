#include <REG51.H>
#define nDots 30
#define uint unsigned int
#define uchar unsigned char
sbit SCLK = P3 ^ 2;
sbit SDO = P3 ^ 3;


void Delay()		//@12.000MHz
{
    uchar i, j, k;

    i = 1;
    j = 186;
    k = 19;
    do
    {
        do
        {
            while (--k);
        }
        while (--j);
    }
    while (--i);
}


void LED_SendData(uint color[nDots])
{
    uint mask;
    uchar i, j;
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
        //输出15位色
        mask = 0x4000;
        for(j = 0; j < 15; j++)
        {
            if(mask & color[i])  SDO = 1;
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
/*uint rgb2color(uchar dR,uchar dG,uchar dB)
{
 return(((uint)dB & 0x1F )<<10 | ((uint)dG & 0x1F)<<5 | (uint)dR & 0x1F);
}*/
void main()
{
    uint color[nDots] = 0;
    uchar i;
    while(1)
    {
        for(i = 0; i < nDots; i++)
        {
            if(color[i] > 0x3FFF) color[i] = 0;
            color[i] = color[i] + 32*i;
        }
        LED_SendData(color);
        for(i = 0; i < 6; i++) Delay();
    }
}
