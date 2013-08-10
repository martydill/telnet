
// Console.h

#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>

using namespace std;

class Console
{
public:
	Console();
	~Console();

	bool setCursorPos(unsigned char x, unsigned char y);
	bool findCursorPos(void);
		
	bool eraseToEndOfLine(void);
	bool eraseFromStartOfLineToCursor(void);
	bool eraseEntireLine(void);
	bool clearScreen(void);
	bool eraseFromCursorToEnd(void);
	bool eraseFromStartToCursor(void);

	bool setInverseText(void);
	bool setNormalText(void);

private:
	HANDLE consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	unsigned char xPos, yPos;
};


#endif