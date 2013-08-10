
// TelnetSession.h
// Definitions for the TelnetSession class

#ifndef TELNETSESSION_H
#define TELNETSESSION_H

#include "Connection.h"
#include "SendBuf.h"
#include "Console.h"

#include <iostream>
#include <fstream>

class TelnetSession
{
public:
	TelnetSession();
	~TelnetSession();
	
	void openConnection(char* address, int port);
	void mainLoop(void);
	
	int handleCommand(unsigned char* commandData);
	void respondToRequest(unsigned char command, unsigned char option);
	void respondToStatement(unsigned char command, unsigned char option);

	int handleVTCommand(unsigned char* commandData);

	bool SendBuffer(SendBuf buffer);
	void sendString(char* characterToSend);
	void setConnectionState(bool state);
	bool getConnectionState(void);

private:
	Connection* connection;
	bool activeConnection;

	Console* console;

	ofstream debugLog;
};


#endif