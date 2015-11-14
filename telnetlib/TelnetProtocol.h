
// TelnetProtocol.h
// Definitions used in describing the telnet protocol

#ifndef TELNETPROTOCOL_H
#define TELNETPROTOCOL_H

// So messy ....
// Telnet protocol codes
const unsigned char IAC = 255;
const unsigned char DONT = 254;
const unsigned char DO = 253;
const unsigned char WONT = 252;
const unsigned char WILL = 251;
const unsigned char SB = 250;
const unsigned char SE = 240;
const unsigned char ECHO = 1;
const unsigned char STATUS = 5;
const unsigned char AUTH = 37;
const unsigned char WINDOWSIZE = 31;
const unsigned char SUPPRESS = 3;
const unsigned char TERMINALTYPE = 24;
const unsigned char TERMINALSPEED = 32;
const unsigned char XDISPLAYLOCATION = 35;
const unsigned char ENVIRONMENTOPTION = 39;
const unsigned char ENVIRONMENTOPTION2 = 36;
const unsigned char TRANSMITBINARY = 0;
const unsigned char IS = 0;
const unsigned char LOGOUT = 18;


// VT100/ANSI codes
const unsigned char VT_ESC = 27;
const unsigned char VT_BRACE = '['; 
const unsigned char VT_SUB = ';'; 
const unsigned char VT_QUESTION = '?';

const unsigned char VT_GRAPHICS_ATTRIBUTE = 'm';
const unsigned char VT_K = 'K';
const unsigned char VT_J = 'J';

const unsigned char VT_CURSOR_UP = 'A';
const unsigned char VT_CURSOR_DOWN = 'B';
const unsigned char VT_CURSOR_POS = 'H';



// One:
// H D E M c 7 8 > = Z

// Two:
// # ( )


// VT52:
// A B C D F G H I J K Z = > < 
#endif