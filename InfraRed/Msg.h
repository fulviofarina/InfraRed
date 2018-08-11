#ifndef _MSG_h
#define _MSG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define DIGITS 4
#define BASE 4

class Message
{
protected:

public:

	bool checkTimeOut(unsigned int timeOut);
	void cleanMsg();
	void cleanPkg();
	String protocolize(const char * Send, char split);
	String getPkgValues(const char * toConvert, char split);
	void reset();
	unsigned int Rcvd[DIGITS]; //to fill with data,
	String Final = "";  //the final message
	unsigned int LastTime = 0; //last time Final was cleared
							   //auxiliar MsgRcvd for base 6
	String Pkg = ""; //last package containing a Message char
	char LastChar = '\0';//last character

	unsigned int TOP_MSG_LENGTH = 8;
	unsigned	int TIMEOUT_WORD = 1e6; //Final timeOut
	unsigned	int TIMEOUT_PKG = 8e8;//Pkg TimeOut

};

#endif
