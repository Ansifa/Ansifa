#define MAIN_Fosc		11059200L	//定义主时钟, 模拟串口和红外接收会自动适应。5~36MHZ
#define PinTxD			P3 ^ 1	   	//定义模拟串口发送脚，打印信息用
#include	"reg51.H"
#include	"Serial.h"

void main ()
{
    char *a = "Hello world!";
    PrintString(a);

	 PrintString("这是STC系列MCU串口测试程序\r\n");
}
