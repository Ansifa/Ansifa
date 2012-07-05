/*
Arduino 控制 D18B20 温度传感器
 By: Ansifa 2012-1-1
 电路接法：
 1、面朝DS18B20管体的平面，左到右记为引脚1~3，功能分别为GND、DQ、Vcc。按照下面顺序接Arduino。
 	D18B20 Pin 1 (GND) -> Arduino GND
 	D18B20 Pin 2 (DQ) -> Arduino D2
 	D18B20 Pin 3 (Vcc) -> Arduino +5v
 2、在DQ和Vcc直接接一个4.7kΩ的电阻()。
 3、如果需要连接多个DS18B20的话，只需要将他们的相同功能脚（所有Vcc一束，DQ一束，GND一束）直接连接起来，最后引出三条线接单片机即可。
*/


const int DQ=2;  // DQ所接引脚，这里随便选了D2。对于UNO，NANO板子可以写0~13，或者A0~A5。

#include <OneWire.h>  // 来源：http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip
#include <DallasTemperature.h> // 来源：http://download.milesburton.com/Arduino/MaximTemperature/DallasTemperature_372Beta.zip

OneWire oneWire(DQ);							// 定义oneWire实例，DQ是传感器数据引脚
DallasTemperature sensors(&oneWire);			// 定义Dallas温度传感器实例
DeviceAddress tempDeviceAddress;				// 定义设备地址存储变量
void setup()
{
  Serial.begin(9600);
  sensors.begin();								//启动传感器
  sensors.setResolution(tempDeviceAddress, 12);	//设置传感器分辨率，对于DS18B20分辨率可以填9~12。小于或者大于这个范围的数值无效
}
void loop()
{
  sensors.requestTemperatures();				//一次获取所有传感器数据
  Serial.println(sensors.getTempCByIndex(0));	//按照传感器序号获取摄氏温度（get Temp ℃ By Index）
  delay(500);
}

