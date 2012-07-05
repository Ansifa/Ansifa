/*
Arduinoģ����� by Ansifa
2012.6.8

�������������ϴ�ģ����̣��򿪼��±���Ȼ���°�ť�������ڼ��±���ӡ��HELLO WORLD����

���߷�����
Arduino D2��68������󣬽�USB��D+
Arduino D4��68������󣬽�USB��D-
Arduino D5��2.2k������󣬽�USB��D-
Arduino D2��3.6v��ѹ�ܵ�GND
Arduino D4��3.6v��ѹ�ܵ�GND
+5v��USB��VCC
GND��USB��GND
Arduino D1��һ�����ص�GND

����USB������ɫ�����ڸ���������ͬ����һ��׼ȷ�������ο���
*USB����	|	*USB�ӿ�
��<->VCC	|	��<->VCC
��<->D-		|	��<->D-
��<->D+		|	��<->D+
��<->GND	|	��<->GND

*/

#include "UsbKeyboard.h"
int KEYPIN = 1;		//��������D1���ţ�Ҳ���Ըĳ��κ�����
void setup()
{
  TIMSK0 &= !(1 << TOIE0);	//
  pinMode(KEYPIN, INPUT);
  digitalWrite(KEYPIN, HIGH);
}
void loop()
{
  UsbKeyboard.update();
  if(digitalRead(KEYPIN) == HIGH)
  {
    delay(100);
    if(digitalRead(KEYPIN) == LOW)
    {
      UsbKeyboard.sendKeyStroke(KEY_H);
      UsbKeyboard.sendKeyStroke(KEY_E);
      UsbKeyboard.sendKeyStroke(KEY_L);
      UsbKeyboard.sendKeyStroke(KEY_L);
      UsbKeyboard.sendKeyStroke(KEY_O);
      UsbKeyboard.sendKeyStroke(KEY_SPACE);
      UsbKeyboard.sendKeyStroke(KEY_W);
      UsbKeyboard.sendKeyStroke(KEY_O);
      UsbKeyboard.sendKeyStroke(KEY_R);
      UsbKeyboard.sendKeyStroke(KEY_L);
      UsbKeyboard.sendKeyStroke(KEY_D);
      UsbKeyboard.sendKeyStroke(KEY_ENTER);
    }
  }
}

