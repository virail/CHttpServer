#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

int StringLength(char* string)
{

	int count = 0;
	while (*string != '\0')
	{
		count++;
		string++;
	}

	return count;

}

int CompareStrings(char* string1, char* string2)
{

	while (*string1 && *string2)
	{

		if (*string1 != *string2)
		{

			return -1;

		}

		string1++;
		string2++;

	}

	return (*string1 == '\0' && *string2 == '\0');

}

/*****************************************
******** own split function **************
*****************************************/
/*
* @Params: char* string, char delimitter
* @Returns: char**
*
* @Info: takes a string and splits by delimitter
*/
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

	return res;

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

	printf("Accepted connections!\n");

	// ------------------------ SETUP -------------------

	char recvbuf[512];
	res = recv(acceptSocket, recvbuf, 512, 0);
	if (res > 0)
	{
		printf("Bytes received: %d\n", res);

		printf("%s\n", recvbuf);

		// --------------------- Get first line ---------------------
		int endLine = 0;
		while (recvbuf[endLine] != '\n') 
			endLine++;

		char* line = (char*) malloc(sizeof(char) * endLine);

		for (int i = 0; i < endLine; i++)
			line[i] = recvbuf[i];

		printf("First Line: %s\n", line);

		// ------------- Split Line -------------

		char** splitLine = split(line, ' ');
		
		if (CompareStrings(splitLine[1], "/") == 1)
		{

			FILE* f = fopen("index.html", "r");
			fseek(f, 0, SEEK_END);
			long fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			char* file = (char*) malloc(sizeof(char) * (fsize + 1));
			fread(file, fsize, 1, f);
			fclose(f);

			int i = 0;
			while (file[i] != '\0')
			{

				printf("%c",file[i]);
				i++;

			}

			char* headers = "HTTP/1.1 200 OK\r\n"
							"\r\n";
			int headersLen = StringLength(headers);

			char *response = (char*) malloc(sizeof(char) * (headersLen + fsize + 1));

			for (int i = 0; i < headersLen; i++)
			{

				response[i] = headers[i];

			}
			response[headersLen] = '\n';
			int fileIndex = 0;
			for (int i = headersLen + 1; i < headersLen + fsize + 1; i++)
			{

				response[i] = file[fileIndex];
				fileIndex++;

			}

			int responseLen = StringLength(response);

			send(acceptSocket, response, responseLen, 0);

		}
		else if (CompareStrings(splitLine[1], "/about") == 1)
		{

			FILE* f = fopen("about.html", "r");
			fseek(f, 0, SEEK_END);
			long fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			char* file = (char*) malloc(sizeof(char) * (fsize + 1));
			fread(file, fsize, 1, f);
			fclose(f);

			int i = 0;
			while (file[i] != '\0')
			{

				printf("%c",file[i]);
				i++;

			}

			char* headers = "HTTP/1.1 200 OK\r\n"
							"\r\n";
			int headersLen = StringLength(headers);

			char *response = (char*) malloc(sizeof(char) * (headersLen + fsize + 1));

			for (int i = 0; i < headersLen; i++)
			{

				response[i] = headers[i];

			}
			response[headersLen] = '\n';
			int fileIndex = 0;
			for (int i = headersLen + 1; i < headersLen + fsize + 1; i++)
			{

				response[i] = file[fileIndex];
				fileIndex++;

			}

			int responseLen = StringLength(response);

			send(acceptSocket, response, responseLen, 0);

		}
		else 
		{
		
			char* response = "HTTP/1.1 404 ERROR\r\n"
							"\r\n";
			int responseLen = StringLength(response);

			send(acceptSocket, response, responseLen, 0);

		}

	}
	else if (res == 0)
		printf("Connection closed!\n");
	else
		printf("recv failed: %d\n", WSAGetLastError());

	closesocket(acceptSocket);

	closesocket(sock);

	WSACleanup();

	return -1;
}
