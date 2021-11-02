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

void DieWithError(char const *errorMessage); // Error handling function
int AcceptTCPConnection(int servSock, struct sockaddr_in &servAddress);
int CreateTCPServerSocket(unsigned short port, struct sockaddr_in &echoServAddr);
void HandleTCPClient(int sock_fd);

int main(int argc, char const *argv[])
{
	std::vector<int> servSock; // Socket descriptors for server
	struct sockaddr_in servAddress;
	int maxDescriptor;		   // Maximum socket descriptor value
	fd_set sockSet;			   // Set of socket descriptors for select()
	long timeout;			   // Timeout value given on command-line
	struct timeval selTimeout; // Timeout for select()
	int running = 1;		   // 1 if server should be running; 0 otherwise
	int noPorts;			   // Number of port specified on command-line
	int port;				   // Looping variable for ports
	unsigned short portNo;	   // actual port number

	if (argc < 3)
	{
		std::cerr
			<< "Usage: " << argv[0] << " <Timeout (sec.)> <Port1> ..."
			<< std::endl;
		exit(EXIT_FAILURE);
	}

	timeout = atol(argv[1]);

	noPorts = argc - 2;

	servSock.reserve(noPorts); // Allocate list of sockets for incoming connections
	maxDescriptor = -1;		   // Initialize maxDescriptor for use by select()

	for (port = 0; port < noPorts; port++)
	{
		portNo = atoi(argv[port + 2]);
		servSock[port] = CreateTCPServerSocket(portNo, servAddress);

		if (servSock[port] > maxDescriptor)
			maxDescriptor = servSock[port];
	}

	std::cout << "Starting Server:\tHit return to shutdown\n"
			  << std::endl;
	while (running)
	{
		// Zero socket descriptor vector and set for server sockets
		// This must be reset every time select() is called
		FD_ZERO(&sockSet);
		FD_SET(STDIN_FILENO, &sockSet); // Add keyboard to descriptor vector
		for (port = 0; port < noPorts; port++)
			FD_SET(servSock[port], &sockSet);

		selTimeout.tv_usec = timeout;
		selTimeout.tv_usec = 0;
		// Timeout specification
		// This must be reset every time select() is called

		// Suspend program until descriptor is ready or timeout
		if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
		{
			std::cout << "No echo requests for " << timeout
					  << " secs...Server still alive" << std::endl;
		}
		else
		{
			if (FD_ISSET(0, &sockSet))
			{
				std::cout << "Shutting down server" << std::endl;
				getchar();
				running = 0;
			}
			for (port = 0; port < noPorts; port++)
			{
				if (FD_ISSET(servSock[port], &sockSet))
				{
					std::cout << "Request on port: " << servSock[port] << std::endl;
					HandleTCPClient(servSock[port]);
				}
			}
		}
		sleep(2);
	}
	for (port = 0; port < noPorts; port++)
		close(servSock[port]); // Close sockets
	return 0;
}

int AcceptTCPConnection(int servSock, struct sockaddr_in &servAddress)
{
	int addrlen = sizeof(servAddress);
	int new_socket = accept(servSock, (struct sockaddr *)&servAddress, (socklen_t *)&addrlen);
	if (new_socket < 0)
		DieWithError("In accept");
	return new_socket;
}

#define MAXPENDING 5 // Maximum outstanding connection requests

int CreateTCPServerSocket(unsigned short port, struct sockaddr_in &echoServAddr)
{
	int sock; // socket to create
	// struct sockaddr_in echoServAddr; // Local address

	// Create socket for incoming connections
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");

	// Construct local address structure
	memset(&echoServAddr, 0, sizeof(echoServAddr));	  // Zero out structure
	echoServAddr.sin_family = AF_INET;				  // Internet address family
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
	echoServAddr.sin_port = htons(port);			  // Local port

	// Bind to the local address
	if (bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed");

	// Mark the socket so it will listen for incoming connections
	if (listen(sock, MAXPENDING) < 0)
		DieWithError("listen() failed");

	return sock;
}
// void HandleTCPClient(int sock_fd)
// {

// 	// int bytesRecv = 0;
// 	// int bytesRecv = 0;
// 	std::string response = std::string("HTTP/1.1 200 OK\n") + std::string("Content-Type: text/html\n") + std::string("Connection: Closed\n") +
// 	std::string("<html>\n") + std::string("<body>\n") +
// 	std::string("<h1>Hello, World!</h1>\n") + std::string("<body>\n") +
// 	std::string("<html>\n");
// 	// char buffer[LENGTH] = {0};
// 	// bytesRecv = recv(sock_fd, buffer, bytesRecv, 0);
// 	// printf("%s\n", buffer);
// 	// bytesRecv = send(sock_fd, response.c_str(), response.length(), 0);
// 	// close(sock_fd);
// }

void HandleTCPClient(int clntSocket)
{
	std::string response = std::string("HTTP/1.1 200 OK\n") + std::string("Content-Type: text/html\n") + std::string("Connection: Closed\n") +
						   std::string("<html>\n") + std::string("<body>\n") +
						   std::string("<h1>Hello, World!</h1>\n") + std::string("<body>\n") +
						   std::string("<html>\n");
	int res_len = response.length();
	char echoBuffer[LENGTH]; /* Buffer for echo string */
	int recvMsgSize;		 /* Size of received message */
	/* Receive message from client */
	// if ((recvMsgSize = recv(clntSocket, echoBuffer, LENGTH, 0)) < 0)
	// 	DieWithError("recv() failed");
	int valread = read(clntSocket, echoBuffer, 30000);
	printf("%s\n", echoBuffer);

	/* Send received string and receive again until end of transmission */
	// while (res_len > 0) /* zero indicates end of transmission */
	// {
	/* Echo message back to client */
	// if (send(clntSocket, response.c_str(), res_len, 0) != res_len)
	// 	DieWithError("send() failed");
	// close(clntSocket); /* Close client socket */
					   // }
}
void DieWithError(char const *errorMessage)
{
	perror(errorMessage);
	exit(EXIT_FAILURE);
}