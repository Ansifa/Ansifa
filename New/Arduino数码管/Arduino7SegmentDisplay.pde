﻿/*
 Arduino单数码管骰子
 */
。
int a[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
void setup()
{
    DDRD = 0xFF;
    pinMode(12, INPUT);
}
void loop()
{
    for(int i = 0; i < 10; i++)
    {
    PORTD = a[i];
    delay(50);
	while(digitalRead(12)) {}
    }
}

