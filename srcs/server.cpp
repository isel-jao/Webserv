#if 1
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <iostream>
#include <stdlib.h>

#define PORT 9909

#define RESET "\033[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#endif

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	struct sockaddr_in srv;
	struct fd_set fr, fw, fe;
	int nRet = 0;

	// create a socket
	// socket(int domain, int type, int protocol)
	int nsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nsocket < 0)
	{
		std::cout << YELLOW << "the socket not opened"
				  << RESET << std::endl;
	}
	else
	{
		std::cout << GREEN
				  << "the socket opened successfuly\n"
				  << RESET << "nsocket: " << nsocket
				  << std::endl;
	}

	// Initilize the enviroment for the seckaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1)
	memset(&(srv.sin_zero), 0, 8);

	// Bind to local host
	nRet = bind(nsocket, (sockaddr *)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		std::cout << RED << "failed to bind to local prot"
				  << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << GREEN << "the socket bound successfuly "
				  << RESET << std::endl;
	}

	// listen the request from client (queues the requests)
	nRet = listen(nsocket, 5);
	if (nRet < 0)
	{
		std::cout << RED << "failed to start  to listen to local prot"
				  << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << GREEN << "the socket started to listen to local port "
				  << RESET << std::endl;
	}

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (1)
	{
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nsocket, &fr);
		FD_SET(nsocket, &fe);

		nRet = select(nsocket + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0)
		{
		}
		if (nRet == 0)
		{
			std::cout << "no requset yet" << std::endl;
		}
		else
		{
			std::cout << "failed" << std:: endl;
			exit (1);
		}
	}

	return 0;
}
