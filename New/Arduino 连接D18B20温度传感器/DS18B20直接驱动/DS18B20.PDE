/*
Arduino 控制 D18B20 温度传感器
By: Ansifa 2012-1-1
电路接法：
1、面朝DS18B20管体的平面，左到右记为引脚1~3，功能分别为GND、DQ、Vcc。按照下面顺序接Arduino。
	D18B20 Pin 1 (GND) -> Arduino GND
	D18B20 Pin 2 (DQ) -> Arduino D2
	D18B20 Pin 3 (Vcc) -> Arduino +5v
2、在DQ和Vcc直接接一个4.7kΩ的电阻。
3、如果需要连接多个DS18B20的话，只需要将他们的相同功能脚（所有Vcc一束，DQ一束，GND一束）直接连接起来，最后引出三条线接单片机即可。
*/

const int DQ=A5;
byte CY;
byte a;                           //存放温度值的高字节
byte b;                           //存放温度值的低字节
unsigned int t = 0;
float tt = 0;

/**************************************
复位DS18B20,并检测设备是否存在
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        digitalWrite(DQ,LOW);                     //送出低电平复位信号
        delayMicroseconds(480);              //延时至少480us
        //digitalWrite(DQ,HIGH);                     //释放数据线
        pinMode(DQ,INPUT);
        delayMicroseconds(60);               //等待60us
        CY = digitalRead(DQ);                    //检测存在脉冲
        pinMode(DQ,OUTPUT);
        delayMicroseconds(420);              //等待设备释放数据线
    }
}
/**************************************
从DS18B20读1字节数据
**************************************/
byte DS18B20_Readbyte()
{
    byte dat = 0;
    for (byte i = 0; i < 8; i++)         //8位计数器
    {
        dat >>= 1;
        digitalWrite(DQ,LOW);                     //开始时间片
        delayMicroseconds(1);                //延时等待
        //digitalWrite(DQ,HIGH);                     //准备接收
        pinMode(DQ,INPUT);
        delayMicroseconds(1);                //接收延时
        if (digitalRead(DQ)) dat =dat |0x80;        //读取数据
        pinMode(DQ,OUTPUT);
        delayMicroseconds(60);               //等待时间片结束
    }
    return dat;
}
/**************************************
向DS18B20写1字节数据
**************************************/
void DS18B20_Writebyte(byte dat)
{
    for (byte i = 0; i < 8; i++)         //8位计数器
    {
        digitalWrite(DQ,LOW);                     //开始时间片
        delayMicroseconds(1);                //延时等待
        dat >>= 1;                  //送出数据
       // CY=dat&0x01;
        digitalWrite(DQ,dat&0x01);
        delayMicroseconds(60);               //等待时间片结束
        pinMode(DQ,INPUT);
        digitalWrite(DQ,HIGH);                     //恢复数据线
        delayMicroseconds(1);                //恢复延时
        pinMode(DQ,OUTPUT);
    }
}

void setup()
{
    Serial.begin(9600);
    pinMode(DQ,OUTPUT);   
}
void loop()
{
        DS18B20_Reset();                //设备复位
        DS18B20_Writebyte(0xCC);        //跳过ROM命令
        DS18B20_Writebyte(0x44);        //开始转换命令
        pinMode(DQ,INPUT);
        while (!digitalRead(DQ));                    //等待转换完成
        pinMode(DQ,OUTPUT);
        DS18B20_Reset();                //设备复位
        DS18B20_Writebyte(0xCC);        //跳过ROM命令
        DS18B20_Writebyte(0xBE);        //读暂存存储器命令
        a = DS18B20_Readbyte();       //读温度低字节
        b = DS18B20_Readbyte();       //读温度高字节
        t = b;
        t <<= 8;
        t = t | a;
        tt = t * 0.0625;
        t = tt * 10 + 0.5;
        Serial.print(a,HEX);
        Serial.print("--");
        Serial.print(b,HEX);
        Serial.print("--");
        Serial.println(t,DEC);
}

