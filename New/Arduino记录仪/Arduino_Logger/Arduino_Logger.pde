#include <SD.h>
const int chipSelect = 4;
void setup()
{
    Serial.begin(9600);
    Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT);
    if (!SD.begin(chipSelect))
    {
        Serial.println("Card failed, or not present");
        return;
    }
    Serial.println("card initialized.");
}
void loop()
{
    String dataString = "";
    for (int analogPin = 0; analogPin < 6; analogPin++)
    {
        int sensor = analogRead(analogPin);
        dataString += String(sensor);
        if (analogPin < 5)
        {
            dataString += ",";
        }
    }
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else
    {
        Serial.println("error opening datalog.txt");
    }
}
