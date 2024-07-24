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

		printf("Error initialising sockets\n");
		return -1;

	}

	SOCKET sock = INVALID_SOCKET;
	
	struct sockaddr_in service;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{

		printf("Error initialising sockets\n");
		WSACleanup();
		return -1;

	}

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	res = bind(sock, (SOCKADDR *) &service, sizeof(service));
	if (res != 0)
	{

		printf("Error initialising sockets");
		closesocket(sock);
		WSACleanup();
		return -1;

	}

	printf("Success! socket bound\n");

	int connectionBackLog = 5;
	res = listen(sock, connectionBackLog);
	if (res != 0)
	{

		printf("Error listening to socket\n");
		WSACleanup();
		return -1;

	}

	SOCKET acceptSocket;
	printf("Socket Listening to: 127.0.0.1:27015\n");

	acceptSocket = accept(sock, NULL, NULL);
	if (res != 0)
	{

		printf("Error accepting connections with socket\n");
		wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return -1;

	}

	printf("Accepting connections\n!");

	char recvbuf[512];

	res = recv(acceptSocket, recvbuf, 512, 0);

	if (res > 0)
		printf("Bytes received: %d\n", res);
	else if (res == 0)
		printf("Connection closed!\n");
	else
		printf("recv failed: %d\n", WSAGetLastError());

	WSACleanup();

	return -1;
}
