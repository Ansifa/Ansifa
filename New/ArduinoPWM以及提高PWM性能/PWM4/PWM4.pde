int brights[14] = {0};  //����14�����ŵĳ�ʼ���ȣ�������������
int StartPWMPin = 0, EndPWMPin = 13; //����D0~D13ΪPWM����
int PWMResolution = 255; //����PWMռ�ձȷֱ���

void setup()
{
    //��������IO�����
    for(int i = StartPWMPin; i <= EndPWMPin; i++)
    {
        pinMode(i, OUTPUT);
        //��㶨�����ʼ����,���ڹ۲�
        brights[i] = random(0, 255);
    }

}
void loop()
{
    //��forѭ����Ϊ14յ���������ġ�ÿ��Arduino loop()ѭ����
    //brights����һ�Ρ�ֱ��brights=255ʱ�򣬽�brights�������¼�����
    for(int i = StartPWMPin; i <= EndPWMPin; i++)
    {
        if((brights[i]++) == PWMResolution) brights[i] = 0;
    }

    for(int i = 0; i <= PWMResolution; i++)  //i�Ǽ���һ��PWM����
    {
        for(int j = StartPWMPin; j <= EndPWMPin; j++) //ÿ��PWM���ھ�������������
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
