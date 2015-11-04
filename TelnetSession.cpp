
// TelnetSession.cpp
// Controls the telnet session

#include <iostream>
#include <string.h>
#include <conio.h>

#include "TelnetSession.h"
#include "TelnetProtocol.h"
#include "SendBuf.h"


// Default constructor
TelnetSession::TelnetSession()
{
	debugLog.open("log.txt", ios::out);
}


// Default destructor
TelnetSession::~TelnetSession()
{
	WSACleanup();
	//if(console)
		//delete console;
}


// Opens a connection 
void TelnetSession::openConnection(char* address, int port)
{
	WSADATA ws;
	
	// Load the WinSock dll
	long status = WSAStartup(0x0101,&ws);
	if(status != 0)
		throw "Unable to load WinSock!";

	cout << "Opening connection to " << address << endl;
	
	// Open the connection
	connection = new Connection(port);
	if(!connection->open(address))
		throw "Unable to open connection!";

	activeConnection = true;

	// Set up the console
	console = new Console;
   
	return;
}


// The 'get input' thingy...
unsigned long WINAPI inputThread(void* data)
{
	char* msg = new char[2];
	msg[1] = '\0';
	
	// Get a pointer to the TelnetSession object
	TelnetSession* session = (TelnetSession*)data;
	
	// Main loop...
	while(session->getConnectionState() == true)
	{
		msg[0]= (char)getch();
	
		try
		{
			session->sendString(msg);
		}
		catch(char* errorMessage)	// Connection has been closed.
		{
			session->setConnectionState(false);
			return 0;
		}
	}

	delete[] msg;
	
	// Kill this thread.
	ExitThread(0);
	return 0;
}


// Sends a single character
void TelnetSession::sendString(char* stringToSend)
{
	connection->sendData(stringToSend);
}


// Checks to see if the connection is active
void TelnetSession::setConnectionState(bool state)
{
	activeConnection = state;
}


// Checks to see if the connection is active
bool TelnetSession::getConnectionState(void)
{
	return activeConnection;
}


// Enter the program's main loop
void TelnetSession::mainLoop(void)
{
	unsigned char* receivedData = NULL;
	HANDLE threadHandle;

	// Create a thread for getting keyboard input
	unsigned long threadId;
	threadHandle = CreateThread(NULL, NULL, inputThread, this, NULL, &threadId);

	// Main program loop
	while(activeConnection)
	{
		try
		{
			// Download data
			receivedData = (unsigned char*)connection->receive();
		}
		catch(char* errorMessage)
		{
			// Connection has been dropped
			activeConnection = false;
			CloseHandle(threadHandle);
			return;
		}

		// Scan for command codes
		unsigned char *pPos = receivedData;
		while(*pPos != '\0')
		{
			// Telnet control code
			if(*pPos == IAC)
			{
				int bytesRead = handleCommand(pPos);
				pPos += bytesRead;
			}
			
			// VT100 control code
			else if(*pPos == VT_ESC)
			{
				int bytesRead = handleVTCommand(pPos);
				pPos += bytesRead;
			}

			// Just text
			else
			{
				cout << *pPos;
				cout.flush();
				pPos++;
			}
		}

		delete[] receivedData;
	}

	return;
}


// Deal with a single command
int TelnetSession::handleCommand(unsigned char* commandData)
{
	unsigned char currentByte = *(++commandData);
	commandData++;
	
	if(currentByte == DO || currentByte == DONT)
		respondToRequest(currentByte, *commandData);	
	else if(currentByte == WILL || currentByte == WONT)
		respondToStatement(currentByte, *commandData);
	//else
	//	throw "Unknown code!";
		
	return 3;
}


// Respond to a DO/DONT request
void TelnetSession::respondToRequest(unsigned char command, unsigned char option)
{
	SendBuf buf(3);
	buf.add(IAC);
	
	if(command == DONT ||  option == TERMINALTYPE || option == WINDOWSIZE || option == TERMINALSPEED || option == ENVIRONMENTOPTION || option == XDISPLAYLOCATION || option == ENVIRONMENTOPTION2)
		buf.add(WONT);
	else
		buf.add(WILL);
	
	buf.add(option);
	SendBuffer(buf);

	return;
}


// Respond to a WILL/WONT statement
void TelnetSession::respondToStatement(unsigned char command, unsigned char option)
{
	SendBuf buf(3);
	buf.add(IAC);
	
	if(command == WONT)
		buf.add(DONT);
	else
		buf.add(DO);
	
	buf.add(option);
	SendBuffer(buf);

	return;
}
	

// Handle a VT100 command
// Ugh .. what a pain ...
int TelnetSession::handleVTCommand(unsigned char* commandData)
{
	int bytesRead = 2;
	unsigned char firstByte, secondByte, thirdByte;

	commandData++;
	
	// We have a '['
	if(*commandData == VT_BRACE)
	{
		unsigned char* pTmp = commandData;	
		unsigned char currentByte;
		int count = 0;
		char strTmp[2];

		// extract a number
		do
		{
			currentByte = *(++pTmp);
			strTmp[count] = currentByte;
			count++;
		}
		while(isdigit(currentByte));

		bytesRead += count;

		// We have a ';'
		if(currentByte == VT_SUB)
		{
			int firstVal = atoi(strTmp);
			char strTmp2[2];
			count = 0;

			do
			{
				currentByte = *(++pTmp);
				strTmp2[count] = currentByte;
				count++;
			}
			while(isdigit(currentByte));
		
			bytesRead += count;

			int secondVal = atoi(strTmp2);
			console->setCursorPos((unsigned char)secondVal - 1, (unsigned char)firstVal - 1);
			debugLog << "Moving cursor to: " << ((unsigned char)secondVal - 1) << "," << ((unsigned char)firstVal -1 ) << endl;
		}

		// We have a '?'
		else if(currentByte == VT_QUESTION)
		{
			bytesRead += 2;
		}
	
		// We are erasing stuff
		else if(currentByte == VT_J)
		{
			int firstVal = atoi(strTmp);
			
			if(firstVal == 0)
				console->eraseFromCursorToEnd();
			else if(firstVal == 1)
				console->eraseFromStartToCursor();
			else if(firstVal == 2)
				console->clearScreen();
			else
				throw "Unimplemented VT_J";
		}

		// Also erasing stuff
		else if(currentByte == VT_K)
		{
			int firstVal = atoi(strTmp);
			
			if(firstVal == 0)
				console->eraseToEndOfLine();
			else if(firstVal == 1)
				console->eraseFromStartOfLineToCursor();
			else if(firstVal == 2)
				console->eraseEntireLine();
			else
				throw "Unimplemented VT_K";
		}

		// Graphics attributes
		else if(currentByte == VT_GRAPHICS_ATTRIBUTE)
		{
			int firstVal = atoi(strTmp);

			if(firstVal == 7)
				console->setInverseText();
			else if(firstVal == 0)
				console->setNormalText();
		}
		
		else if(currentByte == 'H')
		{
			console->setCursorPos(0, 0);
			bytesRead = 1;
		}

		else if(currentByte == 'A')
		{
			//console->moveUp();
			bytesRead = 1;
		}

		else if(currentByte == 'B')
		{
			//console->moveDown()
			bytesRead = 1;
		}

		else
		{
			debugLog << "@@@" << currentByte << endl;
		}
	}
	

	// We have a ( or a )
	else if(*commandData == '(' || *commandData == ')')
	{
		bytesRead = 2;
		debugLog << "** " << *commandData << " " << *(commandData + 1) << endl;
	}

	// Log the unimplemented stuff ...
	else 
	{
		if(*commandData == 'H')
			// move cursor to home
			debugLog <<  "home\n";
		
		else if(*commandData == 'D')
			// move the cursor to the left 1 character
			debugLog <<  "move horizontal\n";

		else if(*commandData == 'E')
			// new line	
			debugLog << "new line\n";

		else if(*commandData == 'M')
			// move up one
			debugLog <<  "move up\n";

		else if(*commandData == 'c')
			// reset
			debugLog <<  "reset\n";

		else if(*commandData == '7')
			// save cursor and attributes
			debugLog <<  "save cursor\n";

		else if(*commandData == '8')
			// restore cursor and attributes
			debugLog <<  "restore cursor\n";

		else if(*commandData == '>')
			// set numeric keypad mode
			debugLog <<  "numeric keypad\n";

		else if(*commandData == '=')
			// set application keyboard mode
			debugLog << "application keyboard mode\n"; 

		else if(*commandData == 'Z')
			// identify
			debugLog <<  "identify\n";
		
		else
		{	
			debugLog << "***" << *commandData << endl;
			//throw "Unimplemented single byte code";
		}

		bytesRead = 2;
	}

	return bytesRead;
}


// Sends the data from a SendBuf
bool TelnetSession::SendBuffer(SendBuf buffer)
{
	connection->sendData(buffer.getData());
	return true;
}

