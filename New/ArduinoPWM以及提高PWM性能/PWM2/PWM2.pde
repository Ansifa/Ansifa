const int PWMPin = 13;
int bright = 0;
void setup()
{
    pinMode(PWMPin, OUTPUT);
}
void loop()
{
    digitalWrite(PWMPin, HIGH);
    delayMicroseconds(bright*30);
    digitalWrite(PWMPin, LOW);
    delayMicroseconds((255 - bright)*30);
    if((bright++) == 255) bright = 0;
}

