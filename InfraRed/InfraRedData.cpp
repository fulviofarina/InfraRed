#include <stdio.h>
#include <math.h>
#include "Tools.h"
#include "Msg.h"
#include "InfraRedData.h"
#include "AlphaBeta.h"
#include "Pulse.h"

InfraRedDataClass InfraRedData;

/////////////////////////////////////////////////////////////////
//B Subroutine for sending code from Msg
void InfraRedDataClass::_sendPkg(const char *code, uint8_t _toneDPin)
{
	String msg = code;
	uint8_t t = 0;
	int factor = 1e3;

	while (t < MAX_ALPHABET) //eight tones only?
	{
		uint8_t u = 0;
		u = abc.auxMsgRcvd.indexOf(msg[t]);
		float au = 0;
		au = (float)(abc.Freq[u] * FREQ_MULTI); // SET THE FREQUENCY OF EAC CHARACTER
		float dur = factor / au;
		tone(_toneDPin, au, dur); //10

		unsigned int delays = dur*factor*PCT_DELAY;
		delayMicroseconds(delays);

		t++;
	}

	msg = "";
}

void InfraRedDataClass::_calculateTau()
{
	double aux = 0;
	aux = _pulse->calculatePeriod(); //in microseconds
	aux /= 1e3; //in miliseconds

	_pulse->tau = aux;

	double freqMultiplier = _pulse->calculateFrequency();
	freqMultiplier /= FREQ_MULTI;

	_pulse->isSame = _train(freqMultiplier);

	_pulse->tone = round(freqMultiplier);
}

bool InfraRedDataClass::_train(double freqMultiplier)
{
	double margin = TONE_ERROR*freqMultiplier;

	bool isSame = freqMultiplier <= _pulse->tone + margin;
	isSame = isSame && freqMultiplier >= _pulse->tone - margin;

	if (training < 0) return isSame;

	int index = (MAX_ALPHABET - 1) - training;
	//set value in library
	abc.Freq[index] = round(freqMultiplier);

	if (!isSame)
	{
#ifdef FLUXDBUG
		Serial.print(freqMultiplier);
		Serial.print(sep);
		Serial.print(_pulse->tone);
		Serial.print("\tD = ");
		Serial.println(margin);
#endif // DEBUG

		training--;

		//finito?
		if (training == 0)
		{
			for (int i = 0; i < MAX_ALPHABET; i++)
			{
				Serial.print("Rcv x = ");
				Serial.print(abc.Mean[i]);
				Serial.print(sep);
				Serial.print(abc.Freq[i]);
				Serial.print(sep);
				Serial.println(i);
			}

			Serial.println("\nTraining Finished!\n");
			Serial.flush();
		}
	}

	return isSame;
}
bool InfraRedDataClass::_compareALetter(uint8_t alphaBetIter, float freq, float margin)
{
	bool ok = false;

	bool higher = freq >= (abc.Freq[alphaBetIter] - margin);

	if (higher)
	{
		bool lower = freq <= (abc.Freq[alphaBetIter] + margin);
		if (lower)
		{
			char equi = abc.Mean[alphaBetIter]; //equivalent INT
			int lenght = _msg->Pkg.length();
			if (_msg->LastChar != equi)
			{
				_msg->LastChar = equi;
				_msg->Pkg += equi;
				ok = true;
			}
		}
	}

	return ok;
}
/////////////////////////////////////////////

/////////////////////////////////////////////////////////
// D do the storing procedure and printing
void InfraRedDataClass::printMsg(char c)
{
	_msg->Final += c;
	bool go = (c == space || c == dot || c == enter);
	bool length = (_msg->Final.length() > 100);

	if (go || length)
	{
		Serial.print(_msg->Final);

		_msg->_cleanMsg();
	}
	if (c == dot)
	{
		Serial.println();
		Serial.println(RCVD_MSG);
	}
}
void interrupt()
{
	InfraRedData.readInterrupt();
}
//INTERRUPT
void InfraRedDataClass::readInterrupt() //works so great
{
	bool beloLim = _pulse->checkPulse(_pin.FluxPin, _pin.BkgLimit); //check if pulse finished and return a bool

#if defined (FLUXDBUG)
	Serial.print("Flux");
	Serial.print(sep);
	Serial.println(_pulse->flux);
#endif

	if (beloLim)
	{
		_calculateTau();

#if defined (FLUXDBUG)
		Serial.print("Tau");
		Serial.print(sep);
		Serial.println(_pulse->tau);
#endif
		//no training
		if (training < 0)
		{
			//margin = pow(alphabet.eMargin[alphaBetIter], 2);

			double freq = _pulse->tone;
			//freq /= freqInterval;
			float margin = freq*TONE_ERROR; //take 10% error

#if defined (FLUXDBUG)
			Serial.print("Multi");
			Serial.print(sep);
			Serial.println(freqMultiplier);

			Serial.print("Margin");
			Serial.print(sep);
			Serial.println(margin);
#endif

			if (_pulse->isSame) return; //not worth looking in the abc
			//else it is worth looking...

			//find tau in alphabet cuz pulse finnished
			for (uint8_t u = 0; u < MAX_ALPHABET; u++)
			{
				bool ok = _compareALetter(u, freq, margin);

				if (ok)
				{
					//debug tau
#if defined (TAUDBUG)
					Serial.print("AVG");
					Serial.print(sep);
					Serial.print(_pulse->tau);
					Serial.print(sep);
					Serial.print((unsigned int)_msg->LastChar);
					Serial.print(sep);
					Serial.println(_msg->LastChar);
#endif
					break;
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////
void InfraRedDataClass::_testAlphabet()
{
	for (uint8_t i = 0; i < MAX_ALPHABET; i++) //twelve tones only?
	{
		Serial.print("Sent x = ");
		Serial.print(abc.Mean[i]);
		Serial.print(sep);
		Serial.println(abc.Freq[i]);
		//Serial.print(sep);
		//Serial.println(i);

		float fr = abc.Freq[i] * FREQ_MULTI;
		float del = 1e3 / fr;

		tone(_pin.SendPin, fr, del); //10
		delay(del*PCT_DELAY);
	}
}
void InfraRedDataClass::_testCode()
{
	String msg = "hahbhchd";
	_sendPkg(msg.c_str(), _pin.SendPin);
	delayMicroseconds(DELAY_CHAR);
}

//1reception message print loop
void InfraRedDataClass::configurePins(uint8_t interruptPin = 2U, uint8_t analogPin = 14U, uint8_t senderPin = 9U, unsigned long bkgLimit = 100)
{
	_pin.FluxPin = analogPin;
	_pin.BkgLimit = bkgLimit;
	_pin.SendPin = senderPin;

	pinMode(_pin.FluxPin, INPUT);

#if defined (GALILEO)
	Serial.print("GALILEO\n\n");
	bool isInterrupt = digitalPinToInterrupt(IRInterPin);
	if (isInterrupt) 	attachInterrupt(interruptPin, interrupt, CHANGE);
#else
	attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt, CHANGE);
#endif
}
//0 - SETUP FOR SETTING UP THE INTERRUPT FOR RECEPTION
void InfraRedDataClass::begin(bool shouldbeTrained = false)
{
	_msg = &Msg;
	_pulse = &Pulse;

	_msg->Final = "";
	_msg->Pkg = "";
	_msg->LastChar = '\0';
	_msg->LastTime = 0;

	Serial.println(TITLE);

	if (shouldbeTrained)
	{
		training = MAX_ALPHABET - 1;

		Serial.println("\nTraining...\n");

		delay(1000);

		_testAlphabet();

		Serial.println();

		delay(1000);

		//	msgTest = true;
	}

	Serial.println(RCVD_MSG);
}

void InfraRedDataClass::listen() {
	//if (msgTest)
	//{
		//sendMsg("Mensaje de prueba");
		//msgTest = false;
	//}
	uint8_t maxSize = 2 * DIGITS;
	unsigned char d = enter;
	unsigned char c = enter;
	//if pacage is larger than 8
	if (_msg->Pkg.length() >= maxSize)
	{
		String aux = _msg->Pkg.substring(0, maxSize);

		//Serial.println(aux);


#if defined (CRYPTDBUG)
		Serial.print("RcvdCode");
		Serial.print(sep);
		Serial.print(aux);
		Serial.print(sep);
#endif

		//Serial.println(toConvert);

		aux = _msg->getPkgValues(aux.c_str(), abc.split);

		//	Serial.println(toConvert);

		const unsigned int data = ToolsClass::conversionInt(_msg->Rcvd, DIGITS, BASE);

		//	toConvert = "";
		c = (unsigned char)data;

#if defined (CRYPTDBUG)
		for (uint8_t j = 0; j < DIGITS; j++)
		{
			Serial.print(_msg->Rcvd[j]);
		}
		Serial.print(sep);
		Serial.print(aux);
		Serial.print(sep);
		Serial.print(data);
		Serial.print(sep);
		Serial.println((char)c);
#endif

		_msg->Pkg.remove(0, maxSize); //CLEAR THE msgTemp
	}

	bool timeout = _msg->_checkTimeOut(TIMEOUT_WORD);

	timeout = timeout && _msg->Final.length() > 1;
	if (timeout)
	{
		//Serial.println("timed out #1!");
	}
	if (d != c || timeout) printMsg(c);

	timeout = _msg->_checkTimeOut(TIMEOUT_PKG);

	if (timeout)
	{
		//Serial.println("timed out! #2");
		_msg->_cleanPkg();
		_msg->_cleanMsg();
	}
}

//A FOR SENDING MESSAGES
bool InfraRedDataClass::sendMsg(const char * msg, uint8_t n = 1U)
{
	bool ok = false;
	String toSend = "";
	String raw = "";
	unsigned char character = '\0';

	unsigned int max = ((String)msg).length() - 1;

	while (n > 0)
	{
		//Serial.println("IR SND");
		//for each letter in the message,
		for (uint8_t j = 0; j < max; j++)
		{
			character = (unsigned char)msg[j];
			raw = ToolsClass::conversionToBase(character, BASE, DIGITS, abc.Mean); // 3 digits
			toSend = _msg->protocolize(raw.c_str(), abc.split);
			_sendPkg(toSend.c_str(), _pin.SendPin);
			delayMicroseconds(DELAY_CHAR);

#if defined (CRYPTDBUG)
			Serial.print("SendCode");
			Serial.print(sep);
			Serial.print(raw);
			Serial.print(','); //do not remove!!!!! it makes the system print properly
			Serial.print(raw.length()); //do not remove!!!!! it makes the system print properly
			Serial.print(','); //do not remove!!!!! it makes the system print properly
			Serial.print(toSend.length()); //do not remove!!!!! it makes the system print properly
			Serial.print(sep);
			Serial.print(toSend);
			Serial.print(sep);
			Serial.print(character);
			Serial.print(sep);
			Serial.println((unsigned int)character);
#endif
		}
		n--;
	}

	msg = "";

	ok = true;

	return ok;
}

//////////////////////////////////////////////////////////////////////////