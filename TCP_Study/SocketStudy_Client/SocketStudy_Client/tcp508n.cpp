#include "tcp508n.h"

//-------�궨��-------
#ifndef TCP_MOD
#define TCP_MOD
#endif

#ifndef TCP_CHECK
#define TCP_CHECK   //�������򷵻�ֵ���
#endif

extern TCP_Client TCP_Clt;//����

#ifndef TCP_MOD
const char DO_S[][12]{  //�̵�������ָ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0xFF ,0x00 },//�������� Relay1 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 },//�������� Relay1 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x01 ,0xFF ,0x00 },//�������� Relay2 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x01 ,0x00 ,0x00 },//�������� Relay2 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x02 ,0xFF ,0x00 },//�������� Relay3 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x02 ,0x00 ,0x00 },//�������� Relay3 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x03 ,0xFF ,0x00 },//�������� Relay4 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x03 ,0x00 ,0x00 },//�������� Relay4 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x04 ,0xFF ,0x00 },//�������� Relay5 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x04 ,0x00 ,0x00 },//�������� Relay5 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x05 ,0xFF ,0x00 },//�������� Relay6 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x05 ,0x00 ,0x00 },//�������� Relay6 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x06 ,0xFF ,0x00 },//�������� Relay7 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x06 ,0x00 ,0x00 },//�������� Relay7 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x07 ,0xFF ,0x00 },//�������� Relay8 ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x07 ,0x00 ,0x00 },//�������� Relay8 ��

};
/*
#else
char DO_Single[][12]{  //�̵�������ָ��                     idx
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0xFF ,0x00 },//�������� Relay ��
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 },//�������� Relay ��
};*/
#endif

int TCP_Client::GetPort()
{
	return htons(ServerAddr.sin_port);
}

TCP_Client::TCP_Client()
{
	port = 502;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(502);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.75");
}
int TCP_Client::Send(const char *buf, int len, int flag = 0)
{
	int ret;
	if ((ret = send(net, buf, len, flag)) == SOCKET_ERROR)
	{
		printf("send  error\n");
		closesocket(net);
		WSACleanup();
		return 0;
	}
}
int TCP_Client::Recv()
{
	int ret;
	if ((ret = recv(net, RecvBufer, sizeof(RecvBufer), 0)) == SOCKET_ERROR)
	{
		printf("receive   failed  with  error%d\n", WSAGetLastError());
		Close();
		return -1;
	}
	RecvBufer[ret] = '\0';
	return ret;
}

void TCP_Client::Close(void)
{
	closesocket(net);
	WSACleanup();
}
int TCP_Client::Init()
{

	int ret;
	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0)//���غͳ�ʼ���׽���
	{
		printf("WSAStartup  failed   with   error\n");
		return 0;
	}
	if ((net = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//�����׽���
	{
		printf("socket failed  with  error\n");
		Close();
		return 0;
	}

	if (connect(net, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)//����
	{
		printf("Connect  failed  with  error\n");
		Close();
		return 0;
	}
	return 1;
}

//tcp508n����
/****************************************************************
*��������DO_Ctl
*�������ܣ��򿪱��Ϊidx�ļ̵��� idx��1 - 8
*���������
*        idx���̵������
*        open: RELAY_OPEN:�򿪼̵���   RELAY_CLOSE:�رռ̵���
*����ֵ��1 �ɹ�     0 ʧ��
*****************************************************************/
int DO_Ctl(char idx,int open)
{
	int i = 0;
	char DO_Single[][12]{  //�̵�������ָ��                     idx
		{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0xFF ,0x00 },//�������� Relay ��
		{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 },//�������� Relay ��
	};
	if (idx <= 0 || idx >= 9)return 0;
#ifdef TCP_MOD
	if (open == RELAY_OPEN)
	{
		DO_Single[0][9] = idx - 1;
		TCP_Clt.Send(&DO_Single[0][0], 20, 0);
#ifdef TCP_CHECK  //����ֵУ��
		TCP_Clt.Recv();
		if(strcmp(TCP_Clt.RecvBufer,&DO_Single[0][0]) == 0) return 1;
		else return 0;
#endif
	}
	else
	{
		DO_Single[1][9] = idx - 1;
		TCP_Clt.Send(&DO_Single[1][0], 20, 0);
#ifdef TCP_CHECK
		TCP_Clt.Recv();
		if (strcmp(TCP_Clt.RecvBufer, &DO_Single[1][0]) == 0) return 1;
		else return 0;
#endif
	}

#else
	if (open == RELAY_OPEN)i = (idx - 1) * 2;
	else i = (idx - 1) * 2 + 1;
	c.Send(&DO_S[i][0], 20, 0);
#endif
	return 1;
}

int RelayTest(int idx, int open)
{
	int i = 0;
	//c.Send(&DO[0][0], 20, 0);
	return 0;
}


/****************************************************************
*��������AOSet_Ctl
*�������ܣ�����AOͨ�������ѹ
*���������
*        idx��AOͨ��  1 - 4
*        mod: MOD_V:��ѹģʽ   MOD_I:����ģʽ
*        value:��ѹģʽ�£�0-10.0V    ����ģʽ�£�0-20mA
*����ֵ��1 �ɹ�     0 ʧ��
*****************************************************************/
int AOSet_Ctl(char idx, char mod ,double value)
{
	//                                                           7      8    9     10    11 
	//														   дָ��  �Ĵ���0-3    д���ֵ
	char AO_Cmd[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x06 ,0x00 ,0x00 ,0x00 ,0x04 };
	//char AO_Cmd_ARC[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x0D ,0x01 ,0x03 ,0x08 ,0x21 ,0xE6 ,0x3B ,0x95 ,0x11 ,0x22 ,0x00 ,0x00 ,0x7C ,0x6D };//����ֵ
	if (idx <= 0 || idx >= 5)return 0;
	switch (idx) //AOͨ������
	{
		case(1): AO_Cmd[9] = 0x00; break;//AOͨ��1
		case(2): AO_Cmd[9] = 0x01; break;//AOͨ��2
		case(3): AO_Cmd[9] = 0x02; break;//AOͨ��3
		case(4): AO_Cmd[9] = 0x03; break;//AOͨ��4
		default:AO_Cmd[9] = 0x04;
	}
	if (mod == MOD_V) //��ѹģʽ
	{
		int v = 0;
		v = 1.0 * value *(50000 / 10);
		if (v >= 50000)v = 50000;
		AO_Cmd[10] =(char)(v/ 256);
		AO_Cmd[11] =(char)(v % 256);
		TCP_Clt.Send(AO_Cmd,20,0);
#ifdef TCP_CHECK  //����
		TCP_Clt.Recv();
		if (strcmp(TCP_Clt.RecvBufer, AO_Cmd) == 0) return 1;
		else return 0;
#endif
	}
	else if (mod == MOD_I)//����ģʽ
	{
		int I = 0;
		I = 1.0 * value *(50000 / 20);
		if (I >= 50000)I = 50000;
		AO_Cmd[10] = (char)(I / 256);
		AO_Cmd[11] = (char)(I % 256);
		TCP_Clt.Send(AO_Cmd, 20, 0);
#ifdef TCP_CHECK  //����
		TCP_Clt.Recv();
		if (strcmp(TCP_Clt.RecvBufer, AO_Cmd) == 0) return 1;
		else return 0;
#endif
	}
	return 1;
}
/****************************************************************
*��������AORead_Ctl
*�������ܣ���ȡAOͨ���Ĵ�����ֵ
*���������
*����ֵ�������ĸ��Ĵ�����ֵ������������У��ֱ��Ӧ 1 - 4
*****************************************************************/
unsigned int * AORead_Ctl()
{
	   //                                                           7      8    9     10    11 
	  //														   ��ָ��  �Ĵ���0-3    д���ֵ
	char AORead_Cmd[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x03 ,0x00 ,0x00 ,0x00 ,0x04 };
	unsigned int a[4] = {0};
	TCP_Clt.Send(AORead_Cmd, 20, 0);
	TCP_Clt.Recv();
	a[0] = TCP_Clt.RecvBufer[9] * 256 +	TCP_Clt.RecvBufer[10];
	a[1] = TCP_Clt.RecvBufer[11] * 256 + TCP_Clt.RecvBufer[12];
	a[2] = TCP_Clt.RecvBufer[13] * 256 + TCP_Clt.RecvBufer[14];
	a[3] = TCP_Clt.RecvBufer[15] * 256 + TCP_Clt.RecvBufer[16];
	return a;
}
/****************************************************************
*��������AORead_Real_Ctl
*�������ܣ���ȡ��·AO�����ֵ
*���������
*        idx��AOͨ��  1 - 4
*        mod: MOD_V:��ѹģʽ   MOD_I:����ģʽ
*����ֵ������ʵ�ʵĵ�ѹֵ��V�����ߵ���ֵ��mA��
*****************************************************************/
double AORead_Real_Ctl(char idx, char mod)
{
	double rt = 0.0;
	unsigned int *ar = {0};
	if (idx <= 0 || idx >= 5)return 0;
	ar = AORead_Ctl();
	if (idx > 0 && idx < 5)
	{
		if (mod == MOD_V)
		{
			rt = *(ar + idx - 1) / 5000.0;
			return rt;
		}
		else
		{
			rt = *(ar + idx - 1) / 2500.0;
			return rt;
		}
	}
	return rt;

}