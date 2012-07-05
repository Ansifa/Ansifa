String ch="";
long n=1244635,i=0;
void setup()
{
  Serial.begin(9600);
  
  while (n>0)
  {
    ch= char(int(n%10)+'0')+ch;
    Serial.println(char(int(n%10)+'0'));
    n/=10;
  }
  i=0;
  Serial.print("Answer=");
  Serial.println(ch);
}
void loop()
{

}



