int brights[8] = {0};
int StartPWMPin=2,EndPWMPin=9;
int PWMResolution=255;

void setup()
{
  for(int i = StartPWMPin; i <= EndPWMPin; i++) pinMode(i, OUTPUT);
  bright[i]=random(0, 255);
}
void loop()
{
  for(int i = StartPWMPin; i <= EndPWMPin; i++)
  {
    analogWrite(i,bright++);
  }
}

