#include "UsbKeyboard.h"
int InData1 = 0, InData2 = 0, InData3 = 0, InData4 = 0, InData5 = 0, InData0 = 0;  //��������ֵ�ݴ�
int TouchSensitivity = 30; //���������ȡ�0~1023��Խ��Խ������
void setup()
{
  for(int i = A0; i <= A5; i++)
  {
    pinMode(i, INPUT);			//A0~A5������Ϊ����
    digitalWrite(i, HIGH);		//��������
  }
  TIMSK0 &= !(1 << TOIE0);
}

void loop()
{
  UsbKeyboard.update();
  //��ȡ�������ŵ�ѹֵ������������������ԭ��
  //Ĭ����������Ϊ��ߵ�ƽ1023��ͨ�������������ŵ�ƽ��
  //������ֵ��1024-analogRead(A0);
  InData0 = 1024 - analogRead(A0);		
  InData1 = 1024 - analogRead(A1);
  InData2 = 1024 - analogRead(A2);
  InData3 = 1024 - analogRead(A3);
  InData4 = 1024 - analogRead(A4);
  InData5 = 1024 - analogRead(A5);
  //���ո��ֿ��ܴ��������¼�
  if(InData0 >= TouchSensitivity)
  {
    UsbKeyboard.sendKeyStroke(79); //right
  }
  if(InData1 >= TouchSensitivity)
  {
    UsbKeyboard.sendKeyStroke(80);  //left
  }
  if(InData2 >= TouchSensitivity)
  {
    UsbKeyboard.sendKeyStroke(81);  //down
  }
  if(InData3 >= TouchSensitivity)
  {
    UsbKeyboard.sendKeyStroke(82);  //up
  }
  if(InData4 >= TouchSensitivity)
  {
    UsbKeyboard.sendKeyStroke(KEY_SPACE);
  }
  if(InData5 >= TouchSensitivity)
  {
    UsbKeyboard.sendKeyStroke(KEY_ENTER);
  }
  delay(100);
}
