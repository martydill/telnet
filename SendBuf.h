
// SendBuf.h
// Definition of the SendBuf class

#ifndef SENDBUF_H
#define SENDBUF_H

//#include <fstream.h>

class SendBuf
{
public:
	SendBuf();
	SendBuf(int bufferSize);
	~SendBuf();

	void add(unsigned char byteToAdd);
	void add(int byteToAdd);
	void add(char* stringToAdd);

	char* getData(void);
	//void outputData(ofstream outfile);

private:
	char* buffer;
	int bufferSize;
	int currentPosition;
};


#endif
