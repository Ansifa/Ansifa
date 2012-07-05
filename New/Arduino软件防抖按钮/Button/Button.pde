int Button=3; //连接开关到IO3
int LED=13; //连接LED到13（实际上UNO等版本已经有了）
boolean onoff=LOW; //设定一个变量用于记录开关被按下了偶数还是奇数次，按一次，为奇数次，=1；两下就是偶数次，=0
void setup()
{
  pinMode(Button,INPUT);
  pinMode(LED,OUTPUT);
}
void loop(){
  if(digitalRead(Button)==LOW)
  {
    delay(10);
    if(digitalRead(Button)==HIGH)
    {
      digitalWrite(LED,onoff);
      onoff=(!onoff);
      delay(10);
      while(digitalRead(Button)==HIGH)
      {
        delay(1);
      }
    }
  }
}



