String comdata = "";
int numdata[]={
  0}
,j;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while (Serial.available() > 0)  
  {
    comdata += char(Serial.read());
    delay(2);
  }


  for(int i=0;i<comdata.length();i++)
  {
    if(comdata[i]!=',')
    {
      j++;
    }
    numdata[j]=numdata[j]*10+(comdata[i]-'0');
  }
  
  for(int i=0;i<10;i++)
  {
    Serial.println(numdata[i]);
  }
  delay(300);
}


