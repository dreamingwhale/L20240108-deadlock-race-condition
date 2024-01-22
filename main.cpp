#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib,"ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));	//메모리를 비워라.
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//INADDR_ANY 다 받아라
	ListenSockAddr.sin_port = htons(10880);
	bind(ListenSocket, (struct sockaddr*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	struct sockaddr_in ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));	//메모리를 비워라.
	int ClientSockAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClientSockAddr, &ClientSockAddrLength);

	char Buffer[1024] = { 0, };
	int RecvLength = recv(ClientSocket, Buffer, 1024, 0);
	if (RecvLength == 0)
	{
		//정상종료
	}
	else if (RecvLength < 0)
	{
		//error

	}
	else
	{
		int SendLength = send(ClientSocket, Buffer, RecvLength, 0);//음수면 끊긴것, 0도 끊긴것, 2면 남아있는것
	}


	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();
	return 0;
}