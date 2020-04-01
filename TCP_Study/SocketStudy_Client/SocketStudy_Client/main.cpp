
#include "tcp508n.h"
//================== Client socket======================

//======================================================

void DO(void)//DO≤‚ ‘
{
	
	for (char i = 1; i <= 8; i++)
	{
		if (1 == DO_Ctl(i, RELAY_OPEN))printf("ok\n");
		Sleep(500);
	}
	for (char i = 1; i <= 8; i++)
	{
		if (1 == DO_Ctl(i, RELAY_CLOSE))printf("ok\n");
		Sleep(500);
	}
}

void AO(void) //AO≤‚ ‘
{
	for(char i = 1;i<=4;i++)
	if(1 == AOSet_Ctl(i, MOD_V, i))printf("ok\n");
	unsigned int *a;
	a = AORead_Ctl();
	for (char j = 1; j <= 4; j++)
		cout << AORead_Real_Ctl(j, MOD_V) << endl;
}

TCP_Client TCP_Clt; //Client∂‘œÛ
void main(int argc, char **argv)
{

	TCP_Clt.Init();
	DO();
	TCP_Clt.Close();
	system("pause");
}
