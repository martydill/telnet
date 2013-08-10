
// Connection.cpp
// A wrapper class for sockets.


#include "Connection.h"
#include <iostream>



// Default constructor
Connection::Connection()
{
	port = 23;
	memset(&addr, 0, sizeof(addr));
	s = socket(AF_INET, SOCK_STREAM, 0);
}


// Constructor with port specification
Connection::Connection(int port)
{
	this->port = port;
	memset(&addr, 0, sizeof(addr));
	s = socket(AF_INET, SOCK_STREAM, 0);
}


// Open a connection to a remote host
bool Connection::open(char* address)
{
	// Make sure the user has specified a port
	if(port < 0 || port > 65535)
		return false;
	
	// See if we have a valid URL
	hostent* host = gethostbyname(address);

	// Not a valid URL
	if(host == NULL)
	{
		// See if we have a vaild IP address
		host = gethostbyaddr(address, strlen(address), AF_INET);
		
		// Still not valid
		if(host == NULL)
			return false;
	}

	// Set up connection
	memcpy((char *)&addr.sin_addr, host->h_addr, host->h_length);   
	addr.sin_family = host->h_addrtype;
	addr.sin_port = htons(port);

	// Establish connection
	int connectionStatus = connect(s, (struct sockaddr *)&addr, sizeof(addr));
	
	if(connectionStatus == 0)
		return true;
	else
		return false;
}


// Default destructor
Connection::~Connection()
{
	closesocket(s);
    port = 0;
}


// Send data
bool Connection::sendData(char* data)
{
	int result = send(s, data, strlen(data), NULL);
	
	if(result == SOCKET_ERROR)
		throw "unable to send data!";

	return true;
}


// Receive data - takes in 4k chunks.
char* Connection::receive(void)
{
	int bytesReceived = 0;
	char* recvBuf = new char[1024 * 4];
	
	bytesReceived = recv(s, recvBuf, 1024 * 4,0);
	
	if(bytesReceived == SOCKET_ERROR)
		throw "unable to receive data!";

	recvBuf[bytesReceived] = '\0';

	char* receivedData = new char[bytesReceived];
	strcpy(receivedData, recvBuf);
	delete[] recvBuf;

	return receivedData;
}