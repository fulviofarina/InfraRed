// Msg.h

#ifndef _MSG_h
#define _MSG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define TIMEOUT_WORD 1e3 //Final timeOut
#define TIMEOUT_PKG 8e4 //Pkg TimeOut

#define DIGITS 4
#define BASE 4

class Message
{
protected:

public:
	bool _checkTimeOut(unsigned int timeOut);
	void _cleanMsg();
	void _cleanPkg();
	String protocolize(const char * Send, char split);
	String getPkgValues(const char * toConvert, char split);
	//String protocolize(const char * Send);
	//String getPkgValues(const char * toConvert);
	//void _cleanMsg();

	unsigned int Rcvd[DIGITS]; //to fill with data,
	String Final = "";  //the final message
	unsigned int LastTime = 0; //last time Final was cleared
							   //auxiliar MsgRcvd for base 6
	String Pkg = ""; //last package containing a Message char
	char LastChar = '\0';//last character
};

#endif