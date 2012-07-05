/*
STC15F104E 呼吸灯
时钟频率5.004MHz
灯在P3.2引脚
By:Ansifa 2012-1-25
*/
#include <REG51.h>
sbit Pin = P3 ^ 3;
void Delay(unsigned int a)
{
    while( a-- != 0)
    {
    }
}
void Delay2(unsigned long a)
{
    while( a-- != 0)
    {
    }
}
main()
{
    int i, delaytime;
    bit mark = 0;
    int bright = 0;
    while(1)
    {
        if(bright == 0) Delay2(50000);
        if(bright == 255 || bright == 0) mark = !mark;
        if(mark == 1)
        {
            bright++;
            delaytime = 9;
        }
        else
        {
            bright--;
            delaytime = 4;
        }


        for(i = 0; i < 255; i++)
        {
            if(i < bright)
            {
                Pin = 0;
                Delay(delaytime);
            }
            else
            {
                Pin = 1;
                Delay(delaytime);
            }
        }
    }
}
