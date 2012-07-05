//Arduino frequency counter/duty cycle meter
//www.electronicsblog.net/
#include <LiquidCrystal.h>

#define Button 52

int divider[6] ={
  0,1,8,64,256,1024};

int prescaler=5;

int b=0;

int screen =0;

double count =0;
double middle =0;
double usage =0;
char x=0;

ISR(TIMER1_OVF_vect) {

  if (prescaler<4) {
    prescaler++;
  }

}
void interrupt()
{
  if (!x) {
    count=TCNT1;
    TCNT1=0x000;  
    TCCR1B=prescaler;
    attachInterrupt(2, interrupt, FALLING);

  }

  else {
    middle=TCNT1;
    attachInterrupt(2, interrupt, RISING);

  }

  x=~x; 
}

void setup()   { 

  pinMode(Button, INPUT);
  digitalWrite(Button, HIGH); //pull up resistor

  TIMSK1=0x01; // enabled global and timer overflow interrupt;
  TCCR1A = 0x00; // normal operation page 148 (mode0);
  attachInterrupt(2, interrupt, RISING);

}

void loop()
{ 
/// screen modes
  switch (screen) {

  case 0: 


    Serial.println("                ");

    Serial.println("Freq ");
    Serial.println(16000000.0/divider[prescaler]/count);
    Serial.println(" Hz");

    Serial.println("Duty ");
    Serial.println(middle/count*100);
    Serial.println(" % ");
    Serial.println("    ");
    break;

  case 1:

    Serial.println("Period: ");

    Serial.println(0.0000625*divider[prescaler]*count);
    Serial.println(" ms     ");
    break;   

  case 2:

    Serial.println("H ");
    Serial.println(0.0000625*divider[prescaler]*middle);
    Serial.println(" ms    ");

    Serial.println("L ");
    Serial.println(0.0000625*divider[prescaler]*(count-middle));
    Serial.println(" ms    ");
    break;   

  case 3:

    Serial.println("Prescaler /");
    Serial.println(divider[prescaler]);

    Serial.println("Count.use ");
    usage=count/65536*100;
    Serial.println(usage);
    Serial.println("%  ");
    break; 

  }

  delay(250);

  if (prescaler>1) {

    if (usage<0.15) {
      prescaler--; 
      delay(200);
    } 

  }    

  ///button /////////

  if (!digitalRead(Button)&&!b) {

    screen++;
    if (screen==4) {
      screen=0 ;   
    }
    lcd.clear();

    b=3;

  };

  if (!b==0) b--;

}