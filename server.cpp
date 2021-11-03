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
#include <iostream>
#include <fstream>
#include <sstream>

#define LENGTH 30000
#define MAX_PEND 3
int AcceptTCPConnection(int sock);
int CreateTCPServerSocket(int portNo);
void HandleTCPClient(int clntSocket);
void dieWithError(const char *err);
std::string htmlToString(std::string file_name);

int main(int argc, char *argv[])
{
	std::vector<int> servSock; /* Socket descriptors for server */
	int maxDescriptor;		   /* Maximum socket descriptor value */
	fd_set sockSet;			   /* Set of socket descriptors for select() */
	long timeout;			   /* Timeout value given on command-line */
	struct timeval selTimeout; /* Timeout for select() */
	int running = 1;		   /* 1 if server should be running; 0 otherwise */
	int noPorts;			   /* Number of port specified on command-line */
	int port;				   /* Looping variable for ports */
	unsigned short portNo;	   /* Actual port number */
	int ret;

	if (argc < 3)
		dieWithError("Usage: Webserver <Timeout (secs.)> <Port 1> ...\n");

	timeout = atol(argv[1]); /* First arg: Timeout */
	noPorts = argc - 2;

	servSock.reserve(noPorts); /* Allocate list of sockets for incoming connections */
	maxDescriptor = -1;		   /* Initialize maxDescriptorfor use by select() */
	std::string body = htmlToString(std::string("index.html"));
	std::cout << body << std::endl;
	for (port = 0; port < noPorts; port++)
	{ /* Create list of ports and sockets to handle ports */
		portNo = atoi(argv[port + 2]);
		servSock[port] = CreateTCPServerSocket(portNo); /* Create port socket */
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
		ret = select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout);
		if (ret == -1)
			dieWithError("couldn't select socket");
		else if (ret == 0)
			std::cout << "No echo requests for " << timeout << "secs...Server still alive" << std::endl;
		else
		{
			if (FD_ISSET(0, &sockSet))
			{ /* Check keyboard */
				std::cout << "Shuttingdown server" << std::endl;
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
		close(servSock[port]);
	/* Close sockets */
	exit(0);
}
int CreateTCPServerSocket(int portNo)
{
	int server_fd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		dieWithError("couldn't create socket");

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(portNo);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		dieWithError("bind failed");

	if (listen(server_fd, MAX_PEND) < 0)
		dieWithError("In listen");

	return server_fd;
}

int AcceptTCPConnection(int sock)
{

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int new_socket;
	if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		dieWithError("In listen");
	return new_socket;
}

void HandleTCPClient(int clntSocket)
{

	std::string body =  htmlToString("index.html");
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	header += std::to_string( body.length()) + "\n\n";
	std::string response = header + body;
	char echoBuffer[LENGTH]; /* Buffer for echo string */
	int recvMsgSize;		 /* Size of received message */
	std::cout << response << std::endl;
	int valread = read(clntSocket, echoBuffer, 30000);

	std::cout << echoBuffer << std::endl;
	write(clntSocket, response.c_str(), response.length());
	close(clntSocket); /* Close client socket */
					   // }
}

void dieWithError(const char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

std::string htmlToString(std::string file_name)
{
	
	std::ifstream file(file_name.c_str()); //c_string because in c++98 iftream dont take a string file name but take cont char *

	std::stringstream string;
	string << file.rdbuf();
	std::string result = string.str();
	file.close(); // close the file(filename)
	return result;
}