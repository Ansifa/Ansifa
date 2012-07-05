#include <SPI.h>
#include <Peggy2.h>

#define SS_PIN  16

Peggy2 frame1;
byte toDisp = 0;
byte checker = 0;

void setup()
{
    frame1.HardwareInit();
    pinMode(SS_PIN,OUTPUT); //set pin16 to output, SS pin
    SPI.setClockDivider(SPI_CLOCK_DIV2); //set the SPI clock to f/2, fastest possible
    SPI.begin();    //SPI lib function which sets ddr for SCK and MOSI pin
                    //MISO is auto input
                    //see SPI.cpp for more info

}

void loop()
{

    if(!checker){
                enableProgramming();
        programData();
        toDisp = receiveByte(0);
        checker = 1;
        frame1.WriteRow(0,toDisp);
    }
    frame1.RefreshAll(2);

}

byte receiveByte(unsigned long startAddress)
{
    //Begin High Speed Read Instruction
    //See p. 10 of SST data sheet
    digitalWrite(SS_PIN,LOW);
    SPI.transfer(0x0B); //high speed read instruction
    SPI.transfer(0x00); //next 3 transfers are address bits A32 - A0
    SPI.transfer(0x00); //So this will read the first byte on the chip
    SPI.transfer(0x00); //last address bits
    SPI.transfer(0xFF); //dummy byte is required to start sending data back to uP
    SPI.transfer(0xFF); //I'm hoping that if I transfer a bullshit byte, the flash
                        //chip will transfer it's data to me in the same time
    digitalWrite(SS_PIN,HIGH);
    //End High Speed Read Instruction   
    return SPDR;    
}

//will perform the read instruction starting from
//startAddress and will receive numOfBytes bytes in
//succession
void receiveBytes(int numOfBytes, unsigned long startAddress)
{
    digitalWrite(SS_PIN,LOW);
    SPI.transfer(0x0B);//high speed read instruction

}

//will perform:
// 1) Chip Erase
// and loop through:
// 1) Page Program
// 2) increment Page
//until the data has finished **note this can loop and over write beginning of memory
void programData(){
    //Begin ChipErase Instruction
    //See p. 17 of SST data sheet
    digitalWrite(SS_PIN,LOW);
    SPI.transfer(0x60);//chip erase instruction
    digitalWrite(SS_PIN,HIGH);
    delay(50);//spec'd time for CE to finish
                //don't bother polling because time to program is irrelevant
    //End ChipErase Instruction

        //Begin WREN Instruction
    //See p. 18 of SST data sheet
    digitalWrite(SS_PIN,LOW);
    SPI.transfer(0x06);//write enable instruction
    digitalWrite(SS_PIN,HIGH);
    //End WREN Instruction

    digitalWrite(SS_PIN,LOW);
    SPI.transfer(0x02); //page program instruction
    SPI.transfer(0x00); //first 8 address bits
    SPI.transfer(0x00); //2nd 8 address bits
    SPI.transfer(0x00); //3rd 8 address bits
    SPI.transfer(0xAA); //10101010 is the byte I should be writing
    digitalWrite(SS_PIN,HIGH);
    delayMicroseconds(3000); //wait 3 ms for page program


    /*
    //Begin Page-Program Instruction
    //see p. 13 of SST data sheet
    byte firstAddress = 0;
    byte secondAddress = 0;
    //this loop will write to every byte in the chips memory
    //32,768 pages of 256 bytes = 8,388,608 bytes
    for(unsigned int i = 0; i < 32,768; ++i) //long variable is number of pages
    {
        digitalWrite(SS_PIN,LOW);
        ++secondAddress; //cycles from 0 to 255, counts pages
        firstAddress = i>>8; // floor(i/256)

        SPI.transfer(0x02);//Page-Program instruction byte
        SPI.transfer(firstAddress); //increments every 256 pages i.e. at page 256 this should be 1
        SPI.transfer(secondAddress); //increments every 256 bytes, i.e every page
        SPI.transfer(0x00); //beginning of a page boundary
        for(int j = 0; j < 256; ++j) //number of bytes per page
        {
            SPI.transfer(2program[(256*i) + j]);//data byte transfer            
        }
        digitalWrite(SS_PIN,HIGH);
        delayMicroseconds(2500); //2500us (2.5ms) delay for each page-program instruction to execute
    }
    //End Page-Program Instruction
    */
}

//Will prepare the chip for writing by performing:
// 1) arm the status register
// 2) Write Enable instruction
//Only needs to be performed once!
void enableProgramming(){
    //Begin EWSR & WRSR Instructions
    //See p. 20 of SST data sheet for more info
    digitalWrite(SS_PIN,LOW); //lower the SS pin
    SPI.transfer(0x50); //enable write status register instruction
    digitalWrite(SS_PIN,HIGH); //raise the SS pin
    delay(10);
    digitalWrite(SS_PIN,LOW); //lower the SS pin
    SPI.transfer(0x01); //write the status register instruction
    SPI.transfer(0x00);//value to write to register
                //xx0000xx will remove all block protection
    digitalWrite(SS_PIN,HIGH);
    //End EWSR & WRSR Instructions

    //Begin WREN Instruction
    //See p. 18 of SST data sheet
    digitalWrite(SS_PIN,LOW);
    SPI.transfer(0x06);//write enable instruction
    digitalWrite(SS_PIN,HIGH);
    //End WREN Instruction

}