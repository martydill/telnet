
// Main.cpp
// The telnet client's entry point

/*
Fun stuff to do:
Add NTLM authentication (for connecting to Win2k telnet server)
Finish off terminal emulation
Replace stream i/o with stdio stuff to improve performance
Fix 'need to press a key to quit' bug after leaving a server
*/

#include <stdio.h>


#include <fstream>

#include "TelnetSession.h"
#include "Console.h"
#include <windows.h>


const int TELNET_PORT = 23;



// Entry point
int main(int argc, char* argv[])
{
	TelnetSession session;

    // Make sure we have valid cmd line arguments
	if(argc == 1)
	{
		cout << "Usage: telnet hostname [port]" << endl;
		return 1;		
	}

	// Establish the connection
	try
	{
		if(argc == 2)
			session.openConnection(argv[1], TELNET_PORT);
		else
			session.openConnection(argv[1], atoi(argv[2]));
		
		session.mainLoop();
	}
	catch(char* errorMessage)
	{
		// Something bad happened ...
		cout << "\n\nA critical error has occured!\n" << errorMessage << endl << endl;
		return 1;
	}

	return 0;
}