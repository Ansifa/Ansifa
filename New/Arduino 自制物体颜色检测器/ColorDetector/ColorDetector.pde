/*
Color of the object detector
物体颜色检测器
Ansifa
2011-11-19

电路连接：
1、在3、4、5脚与地依次连接RGB LED
2、


EEPROM定义：
地址		功能
————————————————————————
0			全黑亮度MinBright
2			校准红色最大亮度RedCali
4			校准绿色最大亮度GreenCali
6			校准蓝色最大亮度BlueCali
8			最大亮度
*/
#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
//定义检测输入引脚，红色LED引脚，绿色LED引脚，蓝色LED引脚。
int Detector = A1, RedLED = 3, GreenLED = 4, BlueLED = 5;
//定义校准按钮引脚，过量曝光（校准完成）灯引脚。
int CaliButton = 2, Expo = 13;
//----------------------------------------
//定义检测结果红色亮度，绿色亮度，蓝色亮度，背景亮度。
int Red = 0, Green = 0, Blue = 0, AmbientBright = 0;
int Red2 = 0, Green2 = 0, Blue2 = 0, buffer = 0;
int RedCali = 0, GreenCali = 0, BlueCali = 0, MaxBright = 0, MinBright = 0;
char Result[50];
void setup()
{
    pinMode(RedLED, OUTPUT);
    pinMode(GreenLED, OUTPUT);
    pinMode(BlueLED, OUTPUT);
    pinMode(Detector, INPUT);
    pinMode(Expo, OUTPUT);
    pinMode(CaliButton, INPUT);
    Serial.begin(9600);

    MinBright = EEPROM_16bit_Read(0);
}
void loop()
{
    //如果校准按钮按下则开始校准。
    if(digitalRead(CaliButton) == HIGH) Calibration();
    //关掉所有LED
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, LOW);
    //检测背景亮度，如果背景亮度太强则点亮过曝灯，提醒降低环境亮度。
    do
    {
        AmbientBright = analogRead(Detector) - MinBright; //当前亮度减去校准的全黑亮度
        if(AmbientBright > 50)
        {
            digitalWrite(Expo, HIGH);
            delay(50);
        }
        else digitalWrite(Expo, LOW);
    }
    while(AmbientBright > 50);

    digitalWrite(RedLED, HIGH);
    delay(1);
    buffer = analogRead(Detector);
    Red = map(buffer, MinBright, MaxBright, 0, 255);
    Red2 = map(buffer, AmbientBright, MaxBright, 0, 255);
    digitalWrite(RedLED, LOW);

    digitalWrite(GreenLED, HIGH);
    delay(1);
    buffer = analogRead(Detector);
    Green = map(buffer, MinBright, MaxBright, 0, 255);
    Green2 = map(buffer, AmbientBright, MaxBright, 0, 255);
    digitalWrite(GreenLED, LOW);

    digitalWrite(BlueLED, HIGH);
    delay(1);
    buffer = analogRead(Detector);
    Blue = map(buffer, MinBright, MaxBright, 0, 255);
    Blue2 = map(buffer, AmbientBright, MaxBright, 0, 255);
    digitalWrite(BlueLED, LOW);
    snprintf(Result, sizeof(Result), "RGB=(%3d,%3d,%3d) RGB2=(%3d,%3d,%3d)", Red, Green, Blue, Red2, Green2, Blue2);
    Serial.println(Result);
}

void Calibration()
{
    //按钮防抖，必须按住300ms才能响应
    delay(300);
    if(digitalRead(CaliButton) == LOW) return;
    //开始校准
    Serial.println("Calibration begin, please let the probe in dark :");
    //清空EEPROM
    for (int i = 0; i < 512; i++) EEPROM.write(i, 0);
    //读取红绿蓝色最小亮度，并且写到EEPROM
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, LOW);
    delay(50);
    MinBright = analogRead(Detector);
    Serial.print("Min Brightness AD value = ");
    Serial.println(MinBright);
    EEPROM_16bit_Write(0, MinBright);

    //读取红绿蓝色最大亮度，并且写到EEPROM
    digitalWrite(RedLED, HIGH);
    digitalWrite(GreenLED, HIGH);
    digitalWrite(BlueLED, HIGH);
    delay(50);
    MaxBright = analogRead(Detector);
    Serial.print("Max Brightness AD value = ");
    Serial.println(MaxBright);
    EEPROM_16bit_Write(8, 1024 - MaxBright);
    //分别读取红绿蓝色最大亮度，用于校正红绿蓝差异
    digitalWrite(RedLED, HIGH);
    delay(5);
    EEPROM_16bit_Write(2, analogRead(Detector));
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, HIGH);
    delay(5);
    EEPROM_16bit_Write(4, analogRead(Detector));
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, HIGH);
    delay(5);
    digitalWrite(BlueLED, LOW);
    EEPROM_16bit_Write(6, analogRead(Detector));
    //串口发送校准完成信息，并且闪动三次校准完成灯
    Serial.println("Calibration Finish.");
    digitalWrite(Expo, HIGH);
    delay(200);
    digitalWrite(Expo, LOW);
    delay(200);
    digitalWrite(Expo, HIGH);
    delay(200);
    digitalWrite(Expo, LOW);
    delay(200);
    digitalWrite(Expo, HIGH);
    delay(200);
    digitalWrite(Expo, LOW);
    delay(200);
    //等待释放按钮
    while(digitalRead(CaliButton) == HIGH) {}
}

//在EEPROM写一个16bit int数据
void EEPROM_16bit_Write(int addr, int num)
{
    //取高位写入addr，低位写addr+1
    EEPROM.write(addr, num >> 8);
    EEPROM.write(addr + 1, num && 255);
}

//在EEPROM读一个16bit int数据
int EEPROM_16bit_Read(int addr)
{
    //读出addr左移8位和addr+1的低位拼合即可得到16bit数据
    int i = EEPROM.read(addr) << 8;
    return i + EEPROM.read(addr + 1);
}
