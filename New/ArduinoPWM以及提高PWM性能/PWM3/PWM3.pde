int brights[14] = {0};
void setup()
{
  for(int i = 0; i < 14; i++) pinMode(i, OUTPUT);
}
void loop()
{

  for(int i = 0; i < 14; i++)
  {
    if((brights[i]++) == 255) brights[i] = 0;
    for(int j = 0; j < 255; j++)
    {
      if(j < brights[i])
      {
        digitalWrite(i, HIGH);
        delayMicroseconds(10);
      }
      else
      {
        digitalWrite(i, LOW);
        delayMicroseconds(10);
      }
    }
  }
}


