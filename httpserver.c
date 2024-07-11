#include <stdio.h>
#include <WinSock2.h>

int main(void)
{

	WORD wVersionRequested;
	WSADATA lpWSAData;

	wVersionRequested = MAKEWORD(2, 2);

	int res = WSAStartup(wVersionRequested, &lpWSAData);

	if (res != 0)
	{

		printf("Error initialising sockets on windows");
		return EXIT_FAILURE;

	}

	WSACleanup();

	return EXIT_SUCCESS;
}
