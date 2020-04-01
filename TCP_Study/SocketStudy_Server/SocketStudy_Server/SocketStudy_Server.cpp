#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void  main(void)
{
	int          ret;
	WSADATA         wsaData;
	SOCKET          ListeningSocket;
	SOCKET          NewConnection;
	SOCKADDR_IN     ServerAddr;
	SOCKADDR_IN     ClientAddr;
	int             ClientAddrLen;
	int             Port = 5555;
	char      DataBufer[1024];
	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)//初始化windows套接字
	{
		printf("WSASrartup  failed  with  error\n");
		return;
	}
	if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//创建套接字
	{
		printf("socket  failed  with  error%d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//指定任一客户端可以连接
	if ((bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr))) == SOCKET_ERROR)//绑定套接字
	{
		printf("bind  failed  with  error  %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	if ((listen(ListeningSocket, 5)) == SOCKET_ERROR)//监听
	{
		printf("Listen  failed  with  error%d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	printf("We  are  waiting  a  connection  on  port  %d\n", Port);
	printf("Listen ....");
	ClientAddrLen = sizeof(SOCKADDR);
	if ((NewConnection = accept(ListeningSocket, (SOCKADDR*)&ClientAddr, &ClientAddrLen)) == INVALID_SOCKET)//接受客户端连接
	{
		printf("accept  failed  with  error\n");
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	printf("We  successfully  got  a  connection  from  %s:%d\n",inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));
	closesocket(ListeningSocket);
	printf("We  are  waiting to  receive data\n");
	if((ret=recv(NewConnection,DataBufer,sizeof(DataBufer),0))==SOCKET_ERROR)
	{
		printf("receive   failed  with  error%d\n",WSAGetLastError());
		closesocket(NewConnection);
		WSACleanup();
		return;
	}
	printf("we  successfuly  receive  %d  bytes  .\n ",ret);
	DataBufer[ret] = '\0';
	printf("we  received  the  data  is\n %s\n",DataBufer);
	closesocket(NewConnection);
	WSACleanup();
	system("pause");
}