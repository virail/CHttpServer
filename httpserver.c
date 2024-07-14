#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main(void)
{

	WORD wVersionRequested;
	WSADATA lpWSAData;

	wVersionRequested = MAKEWORD(2, 2);

	int res = WSAStartup(wVersionRequested, &lpWSAData);

	if (res != 0)
	{

		printf("Error initialising sockets on windows");
		return -1;

	}

	SOCKET sock = INVALID_SOCKET;
	
	struct sockaddr_in service;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{

		printf("Error initialising sockets on windows");
		WSACleanup();
		return -1;

	}

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	res = bind(sock, (SOCKADDR *) &service, sizeof(service));
	if (res != 0)
	{

		printf("Error initialising sockets on windows");
		closesocket(sock);
		WSACleanup();
		return -1;

	}
	else
		printf("Success! socket bound");

	WSACleanup();

	return -1;
}
