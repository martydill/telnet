// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

#include <TelnetSession.h>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "telnetlib")

using namespace std;

int main(int argc, char* argv[])
{
	TelnetSession sess;
	sess.Connet("192.168.1.254", 23);
	sess.mainLoop();
// 	while (true)
// 	{
		string str;
		sess.ReadUtil(str, "Username: ", 10);
		sess.Write("cisco\n", 6);

		sess.ReadUtil(str, "Password:", 10);
		sess.Write("cisco\n", 6);

		sess.ReadUtil(str, "Switch>", 10);
		sess.Write("en\n", 3);

		sess.ReadUtil(str, "Password: ", 10);
		sess.Write("cisco\n", 6);
		sess.ReadUtil(str, "Switch#", 10);

		sess.Write("sh run\n", 7);
		if (-1 == sess.ReadUtil(str, "Switch#", 3))
		{
			if (-1 != sess.ReadUtil(str, "More-- ", 3))
			{
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
				sess.Write(" ", 1);
				sess.ReadUtil(str, "More-- ", 3);
			}
		}
		
// 	}
	return 0;
}

