#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

char** split(char* string, char delimitter)
{
	// first find number of parts
	int parts = 1;
	int index = 0;
	while (string[index] != '\0' && string[index] != '\r') 
	{
		if (string[index] == delimitter)
			parts++;

		index++;
	}

	printf("number of splits: %d\n", parts);

	char** res = (char**) malloc(sizeof(char*) * parts);

	int end = 0;
	int part = 0;
	int lastPart = 0;
	while (string[end] != '\0' && string[end - 1] != '\r')
	{

		if (string[end] == delimitter || string[end] == '\r')
		{

			int size = end - lastPart;
			res[part] = (char*) malloc(sizeof(char) * (size + 1));

			int partIndex = 0;
			for (int i = lastPart; i < end; i++)
			{

				res[part][partIndex] = string[i];
				partIndex++;

			}
			
			res[part][size] = '\0';

			part++;
			lastPart = end + 1;

		}

		end++;

	}

	for (int i = 0; i < parts; i++)
	{

		printf("result of split: %s\n", res[i]);

	}

}
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

	printf("Accepting connections!\n");

	char recvbuf[512];
	res = recv(acceptSocket, recvbuf, 512, 0);

	if (res > 0)
		printf("Bytes received: %d\n", res);
	else if (res == 0)
		printf("Connection closed!\n");
	else
		printf("recv failed: %d\n", WSAGetLastError());

	printf("%s\n", recvbuf);

	int endLine = 0;
	while (recvbuf[endLine] != '\n') 
		endLine++;

	char* line = (char*) malloc(sizeof(char) * endLine);

	for (int i = 0; i < endLine; i++)
		line[i] = recvbuf[i];

	printf("First Line: %s\n", line);

	char** splitLine = split(line, ' ');
;
	
	WSACleanup();

	return -1;
}
