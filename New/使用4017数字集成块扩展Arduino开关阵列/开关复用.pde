const int rst = 2; //板子的RST脚接Arduino的D4口(自定义)
const int clk = 3; //板子的CLK脚接Arduino的D3口(自定义)
const int data1 = 4; //板子的DATA脚接Arduino的D2口(自定义)
//const int data2 = 5; //如果有第二块板子的话，两块板子共用RST和CLK引脚。DATA接Arduino的D5口，第三块板子可以类推接D6口(自定义)
void setup()
{
  Serial.begin(9600);
  pinMode(rst,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(data1,INPUT);
  //pinMode(data2,INPUT); //如果有第二块板子的话要定义IO
}
void loop()
{
  int KeyStatus[10] = {0};  //按照总开关数定义。可能要改为20,30等
  digitalWrite(rst, HIGH);
  delayMicroseconds(10);  //所有delayMicroseconds(10);均是给4017一个反应时间。
  digitalWrite(rst, LOW);
  delayMicroseconds(10);
  for(int i = 0; i < 10; i++)
  {
    KeyStatus[i] = digitalRead(data1);
    //KeyStatus[i+10] = digitalRead(data2);  //读取第二个板子的状态，地址放在i+10
    digitalWrite(clk, HIGH);
    delayMicroseconds(10);
    digitalWrite(clk, LOW);
    delayMicroseconds(10);
  }
  
  for(int i=0;i<10;i++)  //循环打印KeyStatus数组，i<10可能要改为i<20,30等
  {
    Serial.print(KeyStatus[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(100);
}