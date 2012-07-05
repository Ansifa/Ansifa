const int rst = 2; //���ӵ�RST�Ž�Arduino��D4��(�Զ���)
const int clk = 3; //���ӵ�CLK�Ž�Arduino��D3��(�Զ���)
const int data1 = 4; //���ӵ�DATA�Ž�Arduino��D2��(�Զ���)
//const int data2 = 5; //����еڶ�����ӵĻ���������ӹ���RST��CLK���š�DATA��Arduino��D5�ڣ���������ӿ������ƽ�D6��(�Զ���)
void setup()
{
  Serial.begin(9600);
  pinMode(rst,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(data1,INPUT);
  //pinMode(data2,INPUT); //����еڶ�����ӵĻ�Ҫ����IO
}
void loop()
{
  int KeyStatus[10] = {0};  //�����ܿ��������塣����Ҫ��Ϊ20,30��
  digitalWrite(rst, HIGH);
  delayMicroseconds(10);  //����delayMicroseconds(10);���Ǹ�4017һ����Ӧʱ�䡣
  digitalWrite(rst, LOW);
  delayMicroseconds(10);
  for(int i = 0; i < 10; i++)
  {
    KeyStatus[i] = digitalRead(data1);
    //KeyStatus[i+10] = digitalRead(data2);  //��ȡ�ڶ������ӵ�״̬����ַ����i+10
    digitalWrite(clk, HIGH);
    delayMicroseconds(10);
    digitalWrite(clk, LOW);
    delayMicroseconds(10);
  }
  
  for(int i=0;i<10;i++)  //ѭ����ӡKeyStatus���飬i<10����Ҫ��Ϊi<20,30��
  {
    Serial.print(KeyStatus[i]);
    Serial.print("\t");
  }
  Serial.println();
  delay(100);
}