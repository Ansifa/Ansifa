#define MAIN_Fosc		11059200L	//������ʱ��, ģ�⴮�ںͺ�����ջ��Զ���Ӧ��5~36MHZ
#define PinTxD			P3 ^ 1	   	//����ģ�⴮�ڷ��ͽţ���ӡ��Ϣ��
#include	"reg51.H"
#include	"Serial.h"

void main ()
{
    char *a = "Hello world!";
    PrintString(a);

	 PrintString("����STCϵ��MCU���ڲ��Գ���\r\n");
}
