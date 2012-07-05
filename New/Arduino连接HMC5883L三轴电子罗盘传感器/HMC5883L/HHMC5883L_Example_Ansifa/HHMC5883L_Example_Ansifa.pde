#include <Wire.h>
#include <EEPROM.h>
#include <HMC5883L.h>
HMC5883L compass;
int xBianyi,yBianyi,zBianyi;
void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(57600);
  Wire.begin();
  compass = HMC5883L();
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);
  xBianyi = EEPROM.read(9);
  yBianyi = EEPROM.read(19);
  zBianyi = EEPROM.read(29);
}
void loop()
{
  MagnetometerRaw raw = compass.ReadRawAxis();
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  float xHeading = atan2(scaled.YAxis, scaled.XAxis);
  float yHeading = atan2(scaled.ZAxis, scaled.XAxis);
  float zHeading = atan2(scaled.ZAxis, scaled.YAxis);
  if(xHeading < 0)    xHeading += 2*PI;
  if(xHeading > 2*PI)    xHeading -= 2*PI;
  if(yHeading < 0)    yHeading += 2*PI;
  if(yHeading > 2*PI)    yHeading -= 2*PI;
  if(zHeading < 0)    zHeading += 2*PI;
  if(zHeading > 2*PI)    zHeading -= 2*PI;
  float xDegrees = xHeading * 180/M_PI; 
  float yDegrees = yHeading * 180/M_PI; 
  float zDegrees = zHeading * 180/M_PI; 
  if(digitalRead(13)==1)
  {
    xBianyi=xDegrees;
    yBianyi=yDegrees;
    zBianyi=zDegrees;
    EEPROM.write(9, xDegrees);
    EEPROM.write(19, yDegrees);
    EEPROM.write(29, zDegrees);
  }
  Serial.print(xDegrees-xBianyi+10);
  Serial.print(",");
  Serial.print(yDegrees-yBianyi+10);
  Serial.print(",");
  Serial.print(zDegrees-zBianyi+10);
  Serial.println(";");
  delay(100);
}

