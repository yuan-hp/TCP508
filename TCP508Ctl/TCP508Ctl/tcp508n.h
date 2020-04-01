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
//-------DO�̵������Ʋ���---------
#define RELAY_OPEN 1
#define RELAY_CLOSE 0
//-------AOģʽѡ��---------
#define MOD_I 0  //����
#define MOD_V 1  //��ѹ
//-------AIģʽ��ѹ����ѡ��---------
#define RANGE_5V 5  //5V
#define RANGE_10V 10  //10V

//================== Client socket======================

//typedef string TCP_String;
class TCP_Client
{
public:
	WSADATA  wsadata;
	SOCKET  net;
	SOCKADDR_IN    ServerAddr;//��������ַ
	char RecvBufer[100];//���յ�����
	int port;
	TCP_Client();
	int Init(void);//��ʼ��
	int GetPort(void);
	int Send(const char *buf, int len, int flag);
	int Recv();
	void Close(void);
};

//tcp508n����
/****************************************************************
*��������DO_Ctl
*�������ܣ��򿪱��Ϊidx�ļ̵��� idx��1 - 8
*���������
*        idx���̵������
*        open: RELAY_OPEN:�򿪼̵���   RELAY_CLOSE:�رռ̵���
*����ֵ��1 �ɹ�     0 ʧ��
*****************************************************************/
int DO_Ctl(char idx,int open); //�̵�������
int AOSet_Ctl(char idx, char mod, double value);//����AO�����ѹ���ߵ���
unsigned int* AORead_Ctl(void);  //��ȡAO�Ĵ�����ֵ
double AORead_Real_Ctl(char idx, char mod);//��ȡAOͨ��Ϊidx��ֵ
unsigned int* AIRead_Ctl(void);//��ȡAI�Ĵ�����ֵ
double AIRead_Real_Ctl(char idx, char mod, char range );//��ȡAIͨ��Ϊidx��ʵ��ֵ��ת��Ϊ��ѹ���ߵ���ֵ
char DIRead_Ctl(void);//��ȡDI�ĵ�ƽ
char DIRead_Bit_Ctl(char idx);//ָ��DI�ĵ�ƽ
//test
int RelayTest(int idx, int open);