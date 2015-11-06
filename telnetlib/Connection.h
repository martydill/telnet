
// Connection.h: interface for the Connection class.

#ifndef CONNECTION_H
#define CONNECTION_H

#include <winsock2.h>
#include <string.h>


class Connection  
{
public:
	Connection();
	Connection(int port);
	virtual ~Connection();

	bool open(const char* address);
	int sendData(char* data);
	int sendData(char* buf, int len);
	char* receive(void);
	//bool close(void);
	// FIXME: add function for closing connection

private:
	int port;
	struct sockaddr_in addr;
	SOCKET s;

};


#endif
