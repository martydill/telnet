
#include <iostream>
#include "Console.h"


// Default constructor
Console::Console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
}


// Default destructor
Console::~Console()
{
	FreeConsole();
	//CloseHandle(consoleHandle);
}


// Sets the cursor position
bool Console::setCursorPos(unsigned char x, unsigned char y)
{
	cout.flush();
	xPos = x;
	yPos = y;
	COORD coord = {xPos, yPos};
	SetConsoleCursorPosition(consoleHandle, coord);
	cout.flush();

	return true;
}


// Retrieves the cursor position
bool Console::findCursorPos(void)
{
	cout.flush();
	GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);

	xPos = (unsigned char)consoleInfo.dwCursorPosition.X;
	yPos = (unsigned char)consoleInfo.dwCursorPosition.Y;

	cout.flush();

	return true;
}


// Erases everything from the current cursor position to the end of the line
bool Console::eraseToEndOfLine(void)
{
	cout.flush();
	findCursorPos();

	for(int i = xPos; i < 80; i++)
		cout << " ";
	cout.flush();
	setCursorPos(xPos, yPos);
		
	return true;
}


// Erases everything from the start of the current line to the cursor position
bool Console::eraseFromStartOfLineToCursor(void)
{
	cout.flush();
	findCursorPos();
		
	unsigned char tempX = xPos;
	
	setCursorPos(0, yPos);

	for(int i = 0; i < xPos; i++)
		cout << " ";
	cout.flush();
	setCursorPos(tempX, yPos);

	return true;
}


// Erases the entire line that the cursor is on
bool Console::eraseEntireLine(void)
{
	cout.flush();
	findCursorPos();
	unsigned char tempX = xPos;
	
	setCursorPos(0, yPos);

	for(int i = 0; i < 80; i++)
		cout << " ";

	cout.flush();
	setCursorPos(tempX, yPos);
	
	return true;
}


// Clears the entire screen
bool Console::clearScreen(void)
{
	cout.flush();
	setCursorPos(0, 0);
	DWORD numCharactersWritten;
	COORD consoleCoords = {0, 0};
    GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
    FillConsoleOutputCharacter(consoleHandle, ' ', consoleInfo.dwSize.X * consoleInfo.dwSize.Y, consoleCoords, &numCharactersWritten);
	FillConsoleOutputAttribute(consoleHandle, consoleInfo.wAttributes, consoleInfo.dwSize.X * consoleInfo.dwSize.Y, consoleCoords, &numCharactersWritten);
	setCursorPos(0, 0);
	cout.flush();

	return true;
}


// Erases everything from the cursor position to the end
bool Console::eraseFromCursorToEnd(void)
{
	cout.flush();
	findCursorPos();
	DWORD numCharactersWritten;
	COORD consoleCoords = {xPos, yPos};
	FillConsoleOutputCharacter(consoleHandle, ' ', (25 - yPos) * 80 + (80 - xPos), consoleCoords, &numCharactersWritten);
	FillConsoleOutputAttribute(consoleHandle, consoleInfo.wAttributes, (25 - yPos) * 80 + (80 - xPos), consoleCoords, &numCharactersWritten);
	setCursorPos(xPos, yPos);
	cout.flush();
	
	return true;
}


// Erases everything from (0,0) to the cursor position
bool Console::eraseFromStartToCursor(void)
{
	cout.flush();
	findCursorPos();
	DWORD numCharactersWritten;
	COORD consoleCoords = {0, 0};
	FillConsoleOutputCharacter(consoleHandle, ' ', 80 * yPos + xPos, consoleCoords, &numCharactersWritten);
	FillConsoleOutputAttribute(consoleHandle, consoleInfo.wAttributes, 80 * yPos + xPos, consoleCoords, &numCharactersWritten);

	setCursorPos(xPos, yPos);
	cout.flush();

	return true;
}
			

// Sets inverse (black on white) text
bool Console::setInverseText(void)
{
	cout.flush();
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	return true;
}


// Sets normal text
bool Console::setNormalText(void)
{
	cout.flush();
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return true;
}
