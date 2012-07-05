int brights[14] = {0};  //定义14个引脚的初始亮度，可以随意设置
int StartPWMPin = 0, EndPWMPin = 13; //设置D0~D13为PWM引脚
int PWMResolution = 255; //设置PWM占空比分辨率

void setup()
{
    //定义所有IO端输出
    for(int i = StartPWMPin; i <= EndPWMPin; i++)
    {
        pinMode(i, OUTPUT);
        //随便定义个初始亮度,便于观察
        brights[i] = random(0, 255);
    }

}
void loop()
{
    //这for循环是为14盏灯做渐亮的。每次Arduino loop()循环，
    //brights自增一次。直到brights=255时候，将brights置零重新计数。
    for(int i = StartPWMPin; i <= EndPWMPin; i++)
    {
        if((brights[i]++) == PWMResolution) brights[i] = 0;
    }

    for(int i = 0; i <= PWMResolution; i++)  //i是计数一个PWM周期
    {
        for(int j = StartPWMPin; j <= EndPWMPin; j++) //每个PWM周期均遍历所有引脚
        {
            if(i < brights[j])
            {
                digitalWrite(j, HIGH);
                delayMicroseconds(2);
            }
            else
            {
                digitalWrite(j, LOW);
                delayMicroseconds(2);
            }
        }
    }
}
