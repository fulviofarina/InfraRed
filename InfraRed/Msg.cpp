//
//
//

#include "Msg.h"
#include "AlphaBeta.h"

bool Message::_checkTimeOut(unsigned int timeOut)
{
	unsigned int diff = (micros() - LastTime);
	bool timeout = (diff > timeOut);
	return timeout;
}
void Message::_cleanMsg()
{
	Final = "";
	LastTime = micros();
}

void Message::_cleanPkg()
{
	Pkg = "";
	LastTime = micros();
}
String Message::protocolize(const char *Send, char split)
{
	String toSend = Send;
	String finaltxt = "";
	//finaltxt = ""; //this is the final string
	finaltxt += split; //put the header (k) first

					   //split the code-message in gagaga or gagbgc, etc mini-messages
	for (int u = 0; u < toSend.length(); u++)
	{
		finaltxt += toSend[u];
		if (u != (toSend.length() - 1)) finaltxt += split;
	}
	//if (finaltxt.length() == 0) Serial.print("empty2");
	//Serial.print(finaltxt);
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