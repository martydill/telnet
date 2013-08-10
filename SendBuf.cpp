
// SendBuf.cpp
// Buffer used for storing IAC requests before sending

#include "SendBuf.h"

//#include <iostream.h>
#include <string.h>

//extern char* GetTxtFromCode(unsigned char);


// Default constructor
SendBuf::SendBuf()
{
	buffer = NULL;
	bufferSize = 32;
	currentPosition = 0;
}


// Constructor specifying buffer size
SendBuf::SendBuf(int bufSize)
{
	buffer = new char[bufSize];
	this->bufferSize = bufSize;
	currentPosition = 0;
}


// Default destructor
SendBuf::~SendBuf()
{
}


// Adding an unsigned char to the string
void SendBuf::add(unsigned char byteToAdd)
{
	buffer[currentPosition++] = byteToAdd;
	return;
}


// Adding an int to the string
void SendBuf::add(int byteToAdd)
{
	buffer[currentPosition++] = (unsigned char)byteToAdd;
	return;
}


// Adding a string to the string
void SendBuf::add(char* stringToAdd)
{
	char* stringIterator = stringToAdd;

	while(*stringIterator != '\0')
		buffer[currentPosition++] = *stringIterator++;

	return;
}


// Null terminates and returns the buffer
char* SendBuf::getData(void)
{
	buffer[currentPosition] = '\0';
	return buffer;
}


/*
// Outputs contents of buffer to screen
void SendBuf::outputData(ofstream outfile)
{
	for(int i = 0; i < currentPosition; i++)
	{
		char* textRepresentation = GetTxtFromCode(buffer[i]);
		
		if(textRepresentation != NULL)
			outfile << textRepresentation << " ";
		else
			outfile << buffer[i];
	}

	return;
}*/