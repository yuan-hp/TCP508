#pragma once

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#pragma  comment (lib,"ws2_32.lib")
#include <WinSock2.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "iostream"
using namespace std;
//-------DO继电器控制参数---------
#define RELAY_OPEN 1
#define RELAY_CLOSE 0
//-------AO模式选择---------
#define MOD_I 0  //电流
#define MOD_V 1  //电压


//================== Client socket======================

//typedef string TCP_String;
class TCP_Client
{
public:
	WSADATA  wsadata;
	SOCKET  net;
	SOCKADDR_IN    ServerAddr;//服务器地址
	char RecvBufer[100];//接收的数据
	int port;
	TCP_Client();
	int Init(void);//初始化
	int GetPort(void);
	int Send(const char *buf, int len, int flag);
	int Recv();
	void Close(void);
};

//tcp508n控制
/****************************************************************
*函数名：DO_Ctl
*函数功能：打开编号为idx的继电器 idx：1 - 8
*输入参数：
*        idx：继电器编号
*        open: RELAY_OPEN:打开继电器   RELAY_CLOSE:关闭继电器
*返回值：1 成功     0 失败
*****************************************************************/
int DO_Ctl(char idx,int open); //继电器控制
int AOSet_Ctl(char idx, char mod, double value);//设置AO输出电压或者电流
unsigned int* AORead_Ctl(void);  //读取AO寄存器的值
double AORead_Real_Ctl(char idx, char mod);//读取AO通道为idx的值
//test
int RelayTest(int idx, int open);