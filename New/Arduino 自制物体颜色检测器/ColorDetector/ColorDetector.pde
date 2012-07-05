/*
Color of the object detector
������ɫ�����
Ansifa
2011-11-19

��·���ӣ�
1����3��4��5�������������RGB LED
2��


EEPROM���壺
��ַ		����
������������������������������������������������
0			ȫ������MinBright
2			У׼��ɫ�������RedCali
4			У׼��ɫ�������GreenCali
6			У׼��ɫ�������BlueCali
8			�������
*/
#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
//�������������ţ���ɫLED���ţ���ɫLED���ţ���ɫLED���š�
int Detector = A1, RedLED = 3, GreenLED = 4, BlueLED = 5;
//����У׼��ť���ţ������ع⣨У׼��ɣ������š�
int CaliButton = 2, Expo = 13;
//----------------------------------------
//����������ɫ���ȣ���ɫ���ȣ���ɫ���ȣ��������ȡ�
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
    //���У׼��ť������ʼУ׼��
    if(digitalRead(CaliButton) == HIGH) Calibration();
    //�ص�����LED
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, LOW);
    //��ⱳ�����ȣ������������̫ǿ��������صƣ����ѽ��ͻ������ȡ�
    do
    {
        AmbientBright = analogRead(Detector) - MinBright; //��ǰ���ȼ�ȥУ׼��ȫ������
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
    //��ť���������밴ס300ms������Ӧ
    delay(300);
    if(digitalRead(CaliButton) == LOW) return;
    //��ʼУ׼
    Serial.println("Calibration begin, please let the probe in dark :");
    //���EEPROM
    for (int i = 0; i < 512; i++) EEPROM.write(i, 0);
    //��ȡ������ɫ��С���ȣ�����д��EEPROM
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, LOW);
    delay(50);
    MinBright = analogRead(Detector);
    Serial.print("Min Brightness AD value = ");
    Serial.println(MinBright);
    EEPROM_16bit_Write(0, MinBright);

    //��ȡ������ɫ������ȣ�����д��EEPROM
    digitalWrite(RedLED, HIGH);
    digitalWrite(GreenLED, HIGH);
    digitalWrite(BlueLED, HIGH);
    delay(50);
    MaxBright = analogRead(Detector);
    Serial.print("Max Brightness AD value = ");
    Serial.println(MaxBright);
    EEPROM_16bit_Write(8, 1024 - MaxBright);
    //�ֱ��ȡ������ɫ������ȣ�����У������������
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
    //���ڷ���У׼�����Ϣ��������������У׼��ɵ�
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
    //�ȴ��ͷŰ�ť
    while(digitalRead(CaliButton) == HIGH) {}
}

//��EEPROMдһ��16bit int����
void EEPROM_16bit_Write(int addr, int num)
{
    //ȡ��λд��addr����λдaddr+1
    EEPROM.write(addr, num >> 8);
    EEPROM.write(addr + 1, num && 255);
}

//��EEPROM��һ��16bit int����
int EEPROM_16bit_Read(int addr)
{
    //����addr����8λ��addr+1�ĵ�λƴ�ϼ��ɵõ�16bit����
    int i = EEPROM.read(addr) << 8;
    return i + EEPROM.read(addr + 1);
}
