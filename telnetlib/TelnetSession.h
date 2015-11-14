
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

	static unsigned long WINAPI OutputThread(void* param); // add by watertoest, 2015-11-6
	unsigned long InnerOutputThread(); // add by watertoest, 2015-11-6
	
	void openConnection(const char* address, int port) throw(char*);
	void mainLoop(void);
	
	int handleCommand(unsigned char* commandData);
	void respondToRequest(unsigned char command, unsigned char option);
	void respondToStatement(unsigned char command, unsigned char option);

	int handleVTCommand(unsigned char* commandData);

	bool SendBuffer(SendBuf buffer);
	void sendString(char* characterToSend);
	void setConnectionState(bool state);
	bool getConnectionState(void);

	// add by watertoeast, 2015-11-4
	bool Connet(const char* address, int port);
	int Write(char* wrBuf, int bufLen);
	int Read(string &rdStr);
	int ReadUtil(string &rdStr, string endStr, int iTimeOutSec);
	void Close();
	void KillThreads();
	// 

private:
	Connection* connection;
	bool activeConnection;

	Console* console;

	ofstream debugLog;

	
	string m_s2cBuf; // the string buf of server send to client, add by watertoeast, 2015-11-4
	HANDLE m_hMtxS2CBuf; // add by watertoeast, 2015-11-4
	HANDLE m_hOutputThread; // add by watertoeast, 2015-11-6
};


#endif