String comdata = "";
int numdata[6] = {0}, PWMPin[6] = {3, 5, 6, 9, 10, 11}, mark = 0;
void setup()
{
  for(int i = 0; i < 6; i++) pinMode(PWMPin[i], OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int j = 0;
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
    mark = 1;
  }

  if(mark == 1)
  {
    Serial.print("Your Input : ");
    Serial.println(comdata);
    Serial.print("Data length : ");
    Serial.println(comdata.length());
    for(int i = 0; i < comdata.length() ; i++)
    {
      if(comdata[i] == ',')
      {
        j++;
      }
      else
      {
        numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
      }
    }
    comdata = String("");


    for(int i = 0; i < 6; i++)
    {
      Serial.print("Pin ");
      Serial.print(PWMPin[i]);
      Serial.print(" = ");
      Serial.println(numdata[i]);
      analogWrite(PWMPin[i], numdata[i]);
      numdata[i] = 0;
    }
    mark = 0;
  }
}


