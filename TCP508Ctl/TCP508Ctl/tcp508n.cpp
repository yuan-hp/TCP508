#include"stdafx.h"
#include "tcp508n.h"

//-------宏定义-------
#ifndef TCP_MOD
#define TCP_MOD
#endif

#ifndef TCP_CHECK
#define TCP_CHECK   //定义了则返回值检测
#endif

extern TCP_Client TCP_Clt;//网口

#ifndef TCP_MOD
const char DO_S[][12]{  //继电器开关指令
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0xFF ,0x00 },//主机发送 Relay1 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 },//主机发送 Relay1 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x01 ,0xFF ,0x00 },//主机发送 Relay2 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x01 ,0x00 ,0x00 },//主机发送 Relay2 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x02 ,0xFF ,0x00 },//主机发送 Relay3 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x02 ,0x00 ,0x00 },//主机发送 Relay3 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x03 ,0xFF ,0x00 },//主机发送 Relay4 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x03 ,0x00 ,0x00 },//主机发送 Relay4 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x04 ,0xFF ,0x00 },//主机发送 Relay5 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x04 ,0x00 ,0x00 },//主机发送 Relay5 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x05 ,0xFF ,0x00 },//主机发送 Relay6 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x05 ,0x00 ,0x00 },//主机发送 Relay6 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x06 ,0xFF ,0x00 },//主机发送 Relay7 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x06 ,0x00 ,0x00 },//主机发送 Relay7 关
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x07 ,0xFF ,0x00 },//主机发送 Relay8 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x07 ,0x00 ,0x00 },//主机发送 Relay8 关

};
/*
#else
char DO_Single[][12]{  //继电器开关指令                     idx
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0xFF ,0x00 },//主机发送 Relay 开
	{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 },//主机发送 Relay 关
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
	return 1;
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
	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0)//加载和初始化套接字
	{
		printf("WSAStartup  failed   with   error\n");
		return 0;
	}
	if ((net = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//创建套接字
	{
		printf("socket failed  with  error\n");
		Close();
		return 0;
	}

	if (connect(net, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)//连接
	{
		printf("Connect  failed  with  error\n");
		Close();
		return 0;
	}
	return 1;
}

//tcp508n控制
/****************************************************************
*函数名：DO_Ctl
*函数功能：打开编号为idx的继电器 idx：1 - 8
*输入参数：
*        idx：继电器编号
*        open: RELAY_OPEN:打开继电器   RELAY_CLOSE:关闭继电器
*返回值：1 成功     0 失败
*****************************************************************/
int DO_Ctl(char idx,int open)
{
	int i = 0;
	char DO_Single[][12]{  //继电器开关指令                     idx
		{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0xFF ,0x00 },//主机发送 Relay 开
		{ 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 },//主机发送 Relay 关
	};
	if (idx <= 0 || idx >= 9)return 0;
#ifdef TCP_MOD
	if (open == RELAY_OPEN)
	{
		DO_Single[0][9] = idx - 1;
		TCP_Clt.Send(&DO_Single[0][0], 20, 0);
#ifdef TCP_CHECK  //返回值校验
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
	TCP_CHECK.Send(&DO_S[i][0], 20, 0);
#endif
	return 1;
}

int RelayTest(int idx, int open)
{
	int i = 0;
	//TCP_CHECK.Send(&DO[0][0], 20, 0);
	return 0;
}


/****************************************************************
*函数名：AOSet_Ctl
*函数功能：设置AO通道输出电压
*输入参数：
*        idx：AO通道  1 - 4
*        mod: MOD_V:电压模式   MOD_I:电流模式
*        value:电压模式下：0-10.0V    电流模式下：0-20mA
*返回值：1 成功     0 失败
*****************************************************************/
int AOSet_Ctl(char idx, char mod ,double value)
{
	//                                                           7      8    9     10    11 
	//														   写指令  寄存器0-3    写入的值
	char AO_Cmd[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x06 ,0x00 ,0x00 ,0x00 ,0x04 };
	//char AO_Cmd_ARC[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x0D ,0x01 ,0x03 ,0x08 ,0x21 ,0xE6 ,0x3B ,0x95 ,0x11 ,0x22 ,0x00 ,0x00 ,0x7C ,0x6D };//返回值
	if (idx <= 0 || idx >= 5)return 0;
	switch (idx) //AO通道配置
	{
		case(1): AO_Cmd[9] = 0x00; break;//AO通道1
		case(2): AO_Cmd[9] = 0x01; break;//AO通道2
		case(3): AO_Cmd[9] = 0x02; break;//AO通道3
		case(4): AO_Cmd[9] = 0x03; break;//AO通道4
		default:AO_Cmd[9] = 0x04;
	}
	if (mod == MOD_V) //电压模式
	{
		int v = 0;
		v = 1.0 * value *(50000 / 10);
		if (v >= 50000)v = 50000;
		AO_Cmd[10] =(char)(v/ 256);
		AO_Cmd[11] =(char)(v % 256);
		TCP_Clt.Send(AO_Cmd,20,0);
#ifdef TCP_CHECK  //检验
		TCP_Clt.Recv();
		if (strcmp(TCP_Clt.RecvBufer, AO_Cmd) == 0) return 1;
		else return 0;
#endif
	}
	else if (mod == MOD_I)//电流模式
	{
		int I = 0;
		I = 1.0 * value *(50000 / 20);
		if (I >= 50000)I = 50000;
		AO_Cmd[10] = (char)(I / 256);
		AO_Cmd[11] = (char)(I % 256);
		TCP_Clt.Send(AO_Cmd, 20, 0);
#ifdef TCP_CHECK  //检验
		TCP_Clt.Recv();
		if (strcmp(TCP_Clt.RecvBufer, AO_Cmd) == 0) return 1;
		else return 0;
#endif
	}
	return 1;
}
/****************************************************************
*函数名：AORead_Ctl
*函数功能：读取AO通道寄存器的值
*输入参数：
*返回值：返回四个寄存器的值，存放于数组中，分别对应 1 - 4
*****************************************************************/
unsigned int * AORead_Ctl(void)
{
	   //                                                           7      8    9     10    11 
	  //														   读指令  寄存器0-3    写入的值
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
*函数名：AORead_Real_Ctl
*函数功能：读取单路AO的输出值
*输入参数：
*        idx：AO通道  1 - 4
*        mod: MOD_V:电压模式   MOD_I:电流模式
*返回值：返回实际的电压值（V）或者电流值（mA）
*****************************************************************/
double AORead_Real_Ctl(char idx, char mod = MOD_V)
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
/****************************************************************
*函数名：AIRead_Ctl
*函数功能：读取AI通道寄存器的值
*输入参数：
*返回值：返回八个寄存器的值，存放于数组中，分别对应 1 - 8
*****************************************************************/
unsigned int* AIRead_Ctl(void)
{
	char AI_Cmd[] = { 0x00 ,0x00  ,0x00  ,0x00  ,0x00  ,0x06  ,0x01  ,0x04  ,0x00  ,0x00  ,0x00  ,0x08 };//查询AI输入的值
	//char AI_Cmd_ARC[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x13 ,0x01 ,0x04 ,0x10 ,0xA0 ,0xB0 ,0xA1 ,0xB1 ,0xA2 ,0xB2 ,0xA3 ,0xB3 ,0xA4 ,0xB4 ,0xA5 ,0xB5 ,0xA6 ,0xB6 ,0xA7 ,0xB7 };
	TCP_Clt.Send(AI_Cmd, 20, 0);
	TCP_Clt.Recv();
	unsigned int ai[8] = {0};
	ai[0] = TCP_Clt.RecvBufer[9] * 256 + TCP_Clt.RecvBufer[10];
	ai[1] = TCP_Clt.RecvBufer[11] * 256 + TCP_Clt.RecvBufer[12];
	ai[2] = TCP_Clt.RecvBufer[13] * 256 + TCP_Clt.RecvBufer[14];
	ai[3] = TCP_Clt.RecvBufer[15] * 256 + TCP_Clt.RecvBufer[16];
	ai[4] = TCP_Clt.RecvBufer[17] * 256 + TCP_Clt.RecvBufer[18];
	ai[5] = TCP_Clt.RecvBufer[19] * 256 + TCP_Clt.RecvBufer[20];
	ai[6] = TCP_Clt.RecvBufer[21] * 256 + TCP_Clt.RecvBufer[22];
	ai[7] = TCP_Clt.RecvBufer[23] * 256 + TCP_Clt.RecvBufer[24];
	/*
	for (int i = 0; i < 8;i++)
		cout << "==================="<<ai[i] << endl;*/
	return ai;
}

/****************************************************************
*函数名：AIRead_Real_Ctl
*函数功能：读取单路AI的输出值
*输入参数：
*        idx：AI通道  1 - 8
*        mod: MOD_V:电压模式   MOD_I:电流模式
*        range:电压量程（RANGE_10V:10V量程 RANGE_5V:5V量程）
*              电流模式下忽略此参数
*返回值：返回实际的电压值（0-10V）或者电流值（0-20mA）
*****************************************************************/
double AIRead_Real_Ctl(char idx, char mod = MOD_V,char range = RANGE_10V)
{
	double rt = 0.0;
	unsigned int *ar = { 0 };
	if (idx <= 0 || idx >= 9)return 0;
	ar = AIRead_Ctl();
	if (idx > 0 && idx < 9)
	{
		if (mod == MOD_V)
		{
			if (range == RANGE_10V)
			{
				rt = *(ar + idx - 1) * (10.0 / 50000.0);
				return rt;
			}
			else
			{
				rt = *(ar + idx - 1) * (5.0 / 50000.0);
				return rt;
			}
		}
		else
		{
			rt = *(ar + idx - 1) * (20.0 / 50000.0);
			return rt;
		}
	}
	return rt;
}
/****************************************************************
*函数名：DIRead_Ctl
*函数功能：读取8路DI通道的值,高位到低位对应[DI_7:DI_0]
*输入参数：
*返回值：返回电平结果
*****************************************************************/
char DIRead_Ctl(void)
{
	char DI_Cmd[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x06 ,0x01 ,0x02 ,0x00 ,0x00 ,0x00 ,0x08 };
	TCP_Clt.Send(DI_Cmd, 20, 0);
	TCP_Clt.Recv();
	return TCP_Clt.RecvBufer[9];
}
/****************************************************************
*函数名：DIRead_Bit_Ctl
*函数功能：读取指定 io值
*输入参数：idx :I/O标号 1-8
*返回值：返回指定IO的电平结果
*****************************************************************/
char DIRead_Bit_Ctl(char idx)
{
	char rt,ans;
	if (idx < 1 || idx>8)return 0;
	rt = DIRead_Ctl();
	ans = (rt >> (idx - 1)) & 0x01;
	return ans;
}
