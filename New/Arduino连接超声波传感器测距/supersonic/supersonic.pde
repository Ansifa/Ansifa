const int TrigPin = 2;
const int EchoPin = 3;
float cm;
void setup()
{
    Serial.begin(9600);
    pinMode(TrigPin, OUTPUT);
    pinMode(EchoPin, INPUT);
}
void loop()
{
    digitalWrite(TrigPin, LOW);  //�͸ߵ͵�ƽ��һ����ʱ������ȥTrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    
    cm = pulseIn(EchoPin, HIGH) / 58;  //���ز�ʱ�任���cm
    cm = (int(cm * 100)) / 100; //������λС��
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    delay(1000);
}