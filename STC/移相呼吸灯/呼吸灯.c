/*
STC15F104E ºôÎüµÆ
Ê±ÖÓÆµÂÊ5.004MHz
µÆÔÚP3.2Òý½Å
By:Ansifa 2012-1-25
*/
#include <REG51.h>
sbit RED = P3 ^ 2;
sbit GREEN = P3 ^ 4;
sbit BLUE = P3 ^ 3;
void Delay(unsigned int a);

main()
{
    int i;
    bit mark1 = 0, mark2 = 0, mark3 = 0;
    int bright1 = 0, bright2 = 100, bright3 = 200;
    while(1)
    {

        if(bright1 == 255 || bright1 == 0) mark1 = !mark1;
        if(bright2 == 255 || bright2 == 0) mark2 = !mark2;
        if(bright3 == 255 || bright3 == 0) mark3 = !mark3;
        if(mark1 == 1)
        {
            bright1++;
        }
        else
        {
            bright1--;
        }

        if(mark2 == 1)
        {
            bright2++;
        }
        else
        {
            bright2--;
        }

        if(mark3 == 1)
        {
            bright3++;
        }
        else
        {
            bright3--;
        }

        for(i = 0; i < 255; i++)
        {

            if(i < bright1)
            {
                RED = 0;
                Delay(3);
            }
            else
            {
                RED = 1;
                Delay(3);
            }

			if(i < bright2)
            {
                GREEN = 0;
                Delay(3);
            }
            else
            {
                GREEN = 1;
                Delay(3);
            }

            if(i < bright3)
            {
                BLUE = 0;
                Delay(3);
            }
            else
            {
                BLUE = 1;
                Delay(3);
            }
        }
    }
}



void Delay(unsigned int a)
{
    while( a-- != 0)
    {
    }
}