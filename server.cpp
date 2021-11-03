#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <vector>
#include <string>
#include <map>
#include <arpa/inet.h> // for sockaddr_in and inet_ntoa()
#define LENGTH 30000

int AcceptTCPConnection(int sock);
int CreateTCPServerSocket(int portNo);
void HandleTCPClient(int clntSocket);
class responseHeader
{
private:
	/* data */
public:
	responseHeader(/* args */);
	~responseHeader();
};

responseHeader::responseHeader(/* args */)
{
}

responseHeader::~responseHeader()
{
}

int main(int argc, char *argv[])
{
	int *servSock;			   /* Socket descriptors for server */
	int maxDescriptor;		   /* Maximum socket descriptor value */
	fd_set sockSet;			   /* Set of socket descriptors for select() */
	long timeout;			   /* Timeout value given on command-line */
	struct timeval selTimeout; /* Timeout for select() */
	int running = 1;		   /* 1 if server should be running; 0 otherwise */
	int noPorts;			   /* Number of port specified on command-line */
	int port;				   /* Looping variable for ports */
	unsigned short portNo;	   /* Actual port number */
	if (argc < 3)
	{ /* Test for correct number of arguments */
		fprintf(stderr, "Usage: %s <Timeout (secs.)> <Port 1> ...\n", argv[0]);
		exit(1);
	}
	timeout = atol(argv[1]);						 /* First arg: Timeout */
	noPorts = argc - 2;								 /* Number of ports is argument count minus 2 */
	servSock = (int *)malloc(noPorts * sizeof(int)); /* Allocate list of sockets for incoming connections */
	maxDescriptor = -1;								 /* Initialize maxDescriptorfor use by select() */
	for (port = 0; port < noPorts; port++)
	{													/* Create list of ports and sockets to handle ports */
		portNo = atoi(argv[port + 2]);					/* Add port to port list. Skip first two arguments */
		servSock[port] = CreateTCPServerSocket(portNo); /* Create port socket */
		if (servSock[port] > maxDescriptor)				/* Determine if new descriptor is the largest */
			maxDescriptor = servSock[port];
	}

	printf("Startingserver: Hit return to shutdown\n");
	while (running)
	{
		/* Zero socket descriptor vector and set for server sockets */
		/* This must be reset every time select() is called */
		FD_ZERO(&sockSet);
		FD_SET(STDIN_FILENO, &sockSet); /* Add keyboard to descriptor vector */
		for (port = 0; port < noPorts; port++)
			FD_SET(servSock[port], &sockSet);
		/* Timeout specification */
		/* This must be reset every time select() is called */
		selTimeout.tv_sec = timeout; /* timeout (secs.) */
		selTimeout.tv_usec = 0;		 /* 0 microseconds */
		/* Suspend program until descriptor is ready or timeout */
		if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
			printf("No echo requests for %ld secs...Server still alive\n", timeout);
		else
		{
			if (FD_ISSET(0, &sockSet))
			{ /* Check keyboard */
				printf("Shuttingdown server\n");
				getchar();
				running = 0;
			}
			for (port = 0; port < noPorts; port++)
				if (FD_ISSET(servSock[port], &sockSet))
				{
					printf("Request on port %d: ", port);
					HandleTCPClient(AcceptTCPConnection(servSock[port]));
				}
		}
	}
	for (port = 0; port < noPorts; port++)
		close(servSock[port]); /* Close sockets */
	free(servSock);			   /* Free list of sockets */
	exit(0);
}
int CreateTCPServerSocket(int portNo)
{
	int server_fd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("cannot create socket");
		return 0;
	}
	/* htonl converts a long integer (e.g. address) to a network representation */
	/* htons converts a short integer (e.g. port) to a network representation */
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(portNo);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	return server_fd;
}

int AcceptTCPConnection(int sock)
{

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int new_socket;
	if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("In accept");
		exit(EXIT_FAILURE);
	}
	return new_socket;
}

void HandleTCPClient(int clntSocket)
{
	std::string response =  "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nhellooooo world!";
	int res_len = response.length();
	char echoBuffer[LENGTH]; /* Buffer for echo string */
	int recvMsgSize;		 /* Size of received message */
	/* Receive message from client */
	// if ((recvMsgSize = recv(clntSocket, echoBuffer, LENGTH, 0)) < 0)
	// 	DieWithError("recv() failed");
	std::cout << response << std::endl;
	int valread = read(clntSocket, echoBuffer, 30000);
	
	printf("%s\n", echoBuffer);
	write(clntSocket,response.c_str(), response.length());
	// sleep(2);
	
	/* Send received string and receive again until end of transmission */
	// while (res_len > 0) /* zero indicates end of transmission */
	// {
	/* Echo message back to client */
	// if (send(clntSocket, response.c_str(), res_len, 0) != res_len)
	// 	DieWithError("send() failed");
	close(clntSocket); /* Close client socket */
					   // }
}