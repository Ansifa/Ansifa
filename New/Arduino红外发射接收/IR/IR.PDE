#define IRpin_PIN PIND
#define IRpin 2
// 最长等待时间
#define MAXPULSE 65000
//分辨率，越大越好，但是太大会识别不到正确时间
#define RESOLUTION 20
//为红外信号高低电平存储准备100x2个存储单元
uint16_t pulses[100][2];
uint8_t currentpulse = 0;
void setup(void)
{
    Serial.begin(9600);
}
void loop(void)
{
    uint16_t highpulse = 0, lowpulse = 0; //时间暂存

    //高电平部分检测，不用while (digitalRead(IRpin)) 是因为下面速度太慢
    while (IRpin_PIN & (1 << IRpin))
    {
        highpulse++;
        delayMicroseconds(RESOLUTION);
        //超时或者接收达到最大次数返回处理，并且将成功抓取波形次数5以下的记录抛弃。
        if ((highpulse >= MAXPULSE) && (currentpulse > 5))
        {
            printpulses();  //打印结果，结束本次接收
            currentpulse = 0;
            return;
        }
    }
    pulses[currentpulse][0] = highpulse;
    //低电平部分检测

    while (! (IRpin_PIN & _BV(IRpin)))
    {
        lowpulse++;
        delayMicroseconds(RESOLUTION);
        //超时或者接收达到最大次数返回处理，并且将成功抓取波形次数5以下的记录抛弃。
        if ((lowpulse >= MAXPULSE) && (currentpulse > 5))
        {
            printpulses();  //打印结果，结束本次接收
            currentpulse = 0;
            return;
        }
    }
    pulses[currentpulse][1] = lowpulse;
    currentpulse++;
}

//打印结果函数
void printpulses(void)
{
    Serial.println("\n\r\n\rReceived: \n\rOFF \tON\n\rCount:");
    Serial.println(currentpulse, DEC);
    for (uint8_t i = 0; i < currentpulse && pulses[i][0] != 0 && pulses[i][1] != 0; i++)
    {
        Serial.print(pulses[i][0] * RESOLUTION, DEC);
        Serial.print(" usec \t");
        Serial.print(pulses[i][1] * RESOLUTION, DEC);
        Serial.println(" usec");
    }
}
