
#include "Msg.h"
#include "AlphaBeta.h"

bool Message::checkTimeOut(unsigned int timeOut)
{
	unsigned int diff = (micros() - LastTime);
	bool timeout = (diff >= timeOut);
	return timeout;
}
void Message::cleanMsg()
{
	Final = "";
	LastTime = micros();

	LastChar = '\0';

}
void Message::reset()
{
	Final = "";
	Pkg = "";
	LastChar = '\0';
	LastTime = 0;
}
void Message::cleanPkg()
{
	Pkg = "";
	LastTime = micros();
	LastChar = '\0';
}
String Message::protocolize(const char *Send, char split)
{
	String toSend = Send;

	String finaltxt = "";

	finaltxt += split; //put the header (k) first

	 //split the code-message in gagaga or gagbgc, etc mini-messages
	for (int u = 0; u < toSend.length(); u++)
	{
		finaltxt += toSend[u];
		int length = toSend.length() - 1;
		if (u != length) finaltxt += split;
	}
	toSend = "";

	return finaltxt.c_str();
}
String Message::getPkgValues(const char * toConvert, char split)
{
	String aux = toConvert;
	aux.replace((String)split, "");

	for (uint8_t j = 0; j < DIGITS; j++)
	{
		uint8_t indexOfPower = abc.auxMsgRcvd.indexOf(aux[j]);
		Rcvd[j] = indexOfPower;
	}
	return aux;
}