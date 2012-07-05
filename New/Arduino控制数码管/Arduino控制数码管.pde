/*
Arduino驱动7段数码管
Ansifa 2011/12/27

电路连接方法：
根据“LED段码数据生成”软件对应的方法将D0~D7接上LED的a~g脚

*/
int a[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void setup()
{
DDRD=0xFF;
pinMode(12,INPUT);
}
void loop()
{
for(int i=0;i<10;i++)
{
PORTD=a[i];
delay(50);
while(digitalRead(12)){}
}
}