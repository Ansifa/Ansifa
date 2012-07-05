const int PWMPin = 13;
int bright = 0;
void setup()
{
    pinMode(PWMPin, OUTPUT);
}
void loop()
{
    if((bright++) == 255) bright = 0;

    for(int i = 0; i < 255; i++)
    {
        if(i < bright)
        {
            digitalWrite(PWMPin, HIGH);
            delayMicroseconds(30);
        }
        else
        {
            digitalWrite(PWMPin, LOW);
            delayMicroseconds(30);
        }
    }
}
