String comdata = ""; 

void setup() 
{ 
Serial.begin(9600); 
} 

void loop() 
{ 

//下面代码是从串口读入数据 
while (Serial.available() > 0) //如果缓冲池不为空就执行下面的读 
{ 
comdata += char(Serial.read()); //从串口读入数据，并且转换为char类型之后加入comdata变量 
delay(2); //延时一下让缓冲池有足够时间接收数据，理论上大点好。不过会影响程序速度。 
} 

//下面代码是输出comdata的内容，并且在输出完之后，清空comdata 
if (comdata.length() > 0) //看看comdata有没内容，有就执行下面语句输出 
{ 
Serial.println(comdata); //输出comdata 
comdata = ""; //清空comdata，不让它每次都有内容被输出 
} 

} 
