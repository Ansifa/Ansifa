String comdata = ""; 

void setup() 
{ 
Serial.begin(9600); 
} 

void loop() 
{ 

//��������ǴӴ��ڶ������� 
while (Serial.available() > 0) //�������ز�Ϊ�վ�ִ������Ķ� 
{ 
comdata += char(Serial.read()); //�Ӵ��ڶ������ݣ�����ת��Ϊchar����֮�����comdata���� 
delay(2); //��ʱһ���û�������㹻ʱ��������ݣ������ϴ��á�������Ӱ������ٶȡ� 
} 

//������������comdata�����ݣ������������֮�����comdata 
if (comdata.length() > 0) //����comdata��û���ݣ��о�ִ������������ 
{ 
Serial.println(comdata); //���comdata 
comdata = ""; //���comdata��������ÿ�ζ������ݱ���� 
} 

} 
