/*
ʹ��Arduino����˫����Ƶ
Ansifa	2011/11/27

* �������ҪTone�⣬���ص�ַ��http://rogue-code.googlecode.com/files/Arduino-Library-Tone.zip
* ���ϣ�����1����100������1����1uF��������
* Ӳ�����ӣ���D11��D12���Ÿ���һ��1uF���ݣ�֮��ϲ���100������һ�ˣ�
  100��������һ�˽����ȣ�Ȼ��������һ�˽ӵء�

�÷��������������绰����Ͳ��Ȼ����Ҫ���ĵ绰���뵽���ڣ�����10000�����ɲ�ͨ�绰10000
*/

#include <Tone.h>

String Phone_Number = "";
int i = 0, mark = 0;

//����freq1��freq2ΪToneʵ�������Ҷ���˫����Ƶ��Ƶ��
//DTMFƵ�ʶ���μ���http://zh.wikipedia.org/zh/%E5%8F%8C%E9%9F%B3%E5%A4%9A%E9%A2%91
Tone freq1;
Tone freq2;
const int DTMF_freq1[] = {1336, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477};
const int DTMF_freq2[] = {941,  697,  697,  697,  770,  770,  770,  852,  852,  852};

void setup()
{
    Serial.begin(9600);
    //������������������Arduino��D11��D12
    freq1.begin(11);
    freq2.begin(12);
}

void loop()
{
    //�����������ݣ����ӳ�Phone_Number�ַ���
    while (Serial.available() > 0)
    {
        Phone_Number += char(Serial.read());
        delay(2);
        mark = 1;
    }
    //����DTMF��Ƶ���绰��������Phone_Number����������200ms���������300ms
    PlayDTMF(Phone_Number, 200, 300);
	
	//����ղŽ��յ����ڵĵ绰���룬��Ϊ�����Ѿ�����������ˣ���յ绰���벢������mark
    if(mark == 1)
    {
        Phone_Number = "";
        Serial.println();
        mark = 0;
    }
}

/*
DTMF���ź���
���ø�ʽ��playDTMF(����(0~9), ����ʱ��)��
*/
void PlayDTMF(String Number, long duration, long pause)
{
	//�����������Ϊ�գ����߳���ʱ�䲻Ϊ������������ͣʱ�䲻Ϊ����
	//��������������ִֹͣ�з���������
    if(Number.length() == 0 || duration <= 0 || pause <= 0) return;
    //��Number������ַ��룬
    for(i = 0; i < Number.length(); i++)
    {
        //���Number����0~9�������ַ���
        if(Number[i] >= '0' && Number[i] <= '9')
        {
            //����ASCII���ȥ��0�����õ������֣�
            Number[i] -= '0';
            //Ȼ���ڴ������һ�ݣ����ڲ鿴
            Serial.print(Number[i], DEC);
            //���˫����Ƶ֮һ
            freq1.play(DTMF_freq1[Number[i]], duration);
            //���˫����Ƶ֮��
            freq2.play(DTMF_freq2[Number[i]], duration);
            delay(pause);
        }
    }


}