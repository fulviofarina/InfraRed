#include <stdio.h>
#include <math.h>
#include "Tools.h"
#include "InfraRedData.h"
#include "AlphaBeta.h"
#include "Pulse.h"
#include "SerialEvent.h"

InfraRedDataClass InfraRedData;

/////////////////////////////////////////////////////////////////
//B Subroutine for sending code from Msg
void InfraRedDataClass::_sendPkg(const char *code, uint8_t _toneDPin)
{
	String msg = code;
	uint8_t t = 0;

	while (t < MAX_ALPHABET) //eight tones only?
	{
		uint8_t u = 0;
		u =abc.auxMsgRcvd.indexOf(msg[t]);
		float au = 0;
		au = (float)(abc.Freq[u] * _pulse->FREQ_MULTI); // SET THE FREQUENCY OF EAC CHARACTER
		float dur = THOUS_CONV / au;
		tone(_toneDPin, au, dur); //10

		unsigned int delays = dur*THOUS_CONV*_pin.PCT_DELAY;
		delayMicroseconds(delays);

		t++;
	}

	msg = "";
}

void InfraRedDataClass::_showTrainingFinished()
{
	for (int i = 0; i < MAX_ALPHABET; i++)
	{
		Serial.print("Rcv x = ");
		Serial.print(abc.Mean[i]);
		Serial.print(SEPARATOR);
		Serial.print(abc.Freq[i]);
		Serial.print(SEPARATOR);
		Serial.println(i);
	}

	Serial.println("\nTraining Finished!\n");
	Serial.flush();
}

bool InfraRedDataClass::_train(double freqMultiplier)
{
	double margin = _pin.TONE_ERROR*freqMultiplier;

	bool isSame = freqMultiplier <= _pulse->tone + margin;
	isSame = isSame && freqMultiplier >= _pulse->tone - margin;

	if (_training < 0) return isSame;

	int index = (MAX_ALPHABET - 1) - _training;
	//set value in library
	abc.Freq[index] = round(freqMultiplier);

	

	return isSame;
}
void InfraRedDataClass::_testAlphabet()
{
	for (uint8_t i = 0; i < MAX_ALPHABET; i++) //twelve tones only?
	{
		Serial.print("Sent x = ");
		Serial.print(abc.Mean[i]);
		Serial.print(SEPARATOR);
		Serial.println(abc.Freq[i]);
		float fr = abc.Freq[i] * _pulse->FREQ_MULTI;
		float del = THOUS_CONV / fr;

		tone(_pin.SendPin, fr, del); //10
		delayMicroseconds(del*_pin.PCT_DELAY*THOUS_CONV);
	}
}
void InfraRedDataClass::_testCode()
{
	String msg = "hahbhchd";
	_sendPkg(msg.c_str(), _pin.SendPin);
	delayMicroseconds(_pin.DELAY_CHAR);
}
void InfraRedDataClass::_debug_FluxTone(float margin, float freqMultiplier)
{
	Serial.print(freqMultiplier);
	Serial.print(SEPARATOR);
	Serial.print(_pulse->tone);
	Serial.print("\tD = ");
	Serial.println(margin);
}

void InfraRedDataClass::_debug_TauChar()
{

	Serial.print("Tau");
	
	Serial.print(SEPARATOR);
	Serial.print(_pulse->tau);
	
	Serial.print(SEPARATOR);
	
	
	Serial.print((unsigned int)_msg->LastChar);
	Serial.print(SEPARATOR);
	Serial.println(_msg->LastChar);

	
}
void InfraRedDataClass::_debug_CryptChar(unsigned char c, unsigned int data, const char * pkg)
{
	for (uint8_t j = 0; j < DIGITS; j++)
	{
		Serial.print(_msg->Rcvd[j]);
	}
Serial.print(SEPARATOR);
Serial.print(pkg);
Serial.print(SEPARATOR);
Serial.print(data);
Serial.print(SEPARATOR);
Serial.println((char)c);
}
void InfraRedDataClass::_debug_Multiplier(float margin)
{
	Serial.print("Multi");
	Serial.print(SEPARATOR);
	Serial.println(_pulse->FREQ_MULTI);
	Serial.print("Margin");
	Serial.print(SEPARATOR);
	Serial.println(margin);
}
void InfraRedDataClass::_debug_CryptCode(const char * aux)
{
	Serial.print("RcvdCode");
	Serial.print(SEPARATOR);
	Serial.print(aux);
	Serial.print(SEPARATOR);
}
void InfraRedDataClass::_debug_Crypt(const char *raw, const char * toSend, unsigned char character)
{
	Serial.print("SendCode");
	Serial.print(SEPARATOR);
	Serial.print(raw);
	Serial.print(',');
	Serial.print(sizeof(raw));
	Serial.print(','); 
	Serial.print(sizeof(toSend)); 
	Serial.print(SEPARATOR);
	Serial.print(toSend);
	Serial.print(SEPARATOR);
	Serial.print(character);
	Serial.print(SEPARATOR);
	Serial.println((unsigned int)character);
}

bool InfraRedDataClass::_compareALetter(uint8_t alphaBetIter, float freq)
{
	bool ok = false;
	float margin = freq*_pin.TONE_ERROR;
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

/////////////////////////////////////////////////////////
// D do the storing procedure and printing
void InfraRedDataClass::_printMsg(char c)
{
	_msg->Final += c;
	bool go = (c == SPACE || c == DOT || c == ENTER);
	bool length = (_msg->Final.length() > _msg->TOP_MSG_LENGTH);

	if (go || length)
	{
		Serial.print(_msg->Final);
		_msg->cleanMsg();
	}
	if (c == DOT)
	{
		//Serial.println();
	//	Serial.println(RCVD_MSG);
	}
}

//INTERRUPT
void InfraRedDataClass::readInterrupt(void) //works so great
{
	//if (ended) return;

	bool beloLim = _pulse->checkPulse(_pin.InterruptPin); //check if pulse finished and return a bool

	if (!beloLim) return;

	double freqMultiplier = _pulse->calculateFrequency();
	
	_pulse->isSame = _train(freqMultiplier);

	if (_pulse->isSame) return; //not worth looking in the abc
								//else it is worth looking...


#ifdef FLUXDBUG
		_debug_FluxTone(margin, freqMultiplier);
#endif // DEBUG

		_training--;

		//finito?
		if (_training == 0)
		{
			_showTrainingFinished();
		}
								//just finished training, next time will enter
	if (_training >= 0) return;


	_pulse->tone = round(freqMultiplier);
#if defined (FLUXDBUG)
	_debug_Multiplier(_pulse->tone*TONE_ERROR);
#endif

	//find tau in alphabet cuz pulse finnished
	for (uint8_t u = 0; u < MAX_ALPHABET; u++)
	{
		bool ok = _compareALetter(u, _pulse->tone);
		if (ok)
		{
#if defined (TAUDBUG)
			_debug_TauChar();
#endif
			break;
		}
	}
}
////////////////////////////////////////////////////////////

void interrupt()
{
	InfraRedData.readInterrupt();
}
//1reception message print loop
void InfraRedDataClass::configureAsTransmitter(uint8_t senderPin = 9U)
{
	_msg = &Msg;
	_pulse = &Pulse;
	_pin.SendPin = senderPin;
}
void InfraRedDataClass::configureAsReceiver(uint8_t interruptPin = 2U)
{
	_msg = &Msg;
	_pulse = &Pulse;
	_pin.InterruptPin = interruptPin;

}
//0 - SETUP FOR SETTING UP THE INTERRUPT FOR RECEPTION
void InfraRedDataClass::begin(bool shouldbeTrained = false)
{

	ended = false;

	setupSerialEvent();



	_msg->reset();

	Serial.println(TITLE);

	if (shouldbeTrained)
	{
		_training = MAX_ALPHABET - 1;

		Serial.println("\nTraining...\n");

		delay(THOUS_CONV);

		_testAlphabet();

		Serial.println();

		delay(THOUS_CONV);

		//	msgTest = true;
	}


	attachInterrupt(digitalPinToInterrupt(_pin.InterruptPin), interrupt, CHANGE);


	//Serial.println(RCVD_MSG);
}

void InfraRedDataClass::listen()
{
	if (ended) return;

	uint8_t maxSize = 2 * DIGITS;
	unsigned char d = ENTER;
	unsigned char c = ENTER;
	//if pacage is larger than 8
	if (_msg->Pkg.length() >= maxSize)
	{
		String aux = _msg->Pkg.substring(0, maxSize);

#if defined (CRYPTDBUG)
		_debug_CryptCode(aux.c_str());
#endif
		aux = _msg->getPkgValues(aux.c_str(), abc.split);
		const unsigned int data = ToolsClass::conversionInt(_msg->Rcvd, DIGITS, BASE);
	
		c = (unsigned char)data;

#if defined (CRYPTDBUG)
		_debug_CryptChar(c, data, aux.c_str());
#endif
		_msg->Pkg.remove(0, maxSize); //CLEAR THE msgTemp
	}

	bool timeout = _msg->checkTimeOut(_msg->TIMEOUT_WORD);
	timeout = timeout && _msg->Final.length() > 1;
	if (timeout)
	{
		//Serial.println("timed out #1!");
	}
	if (d != c || timeout) _printMsg(c);

	timeout = _msg->checkTimeOut(_msg->TIMEOUT_PKG);

	if (timeout)
	{
		//Serial.println("timed out! #2");
		_msg->cleanPkg();
		_msg->cleanMsg();
	}

	
}


void InfraRedDataClass::end()
{
	ended = true;
	detachInterrupt(digitalPinToInterrupt(_pin.InterruptPin));

}

bool InfraRedDataClass::standBy(uint8_t timesToTrnasmite = 1U )
{
	if (ended) return;

	String msg = loopSerialEvent();
	sendMsg(msg.c_str(), timesToTrnasmite);
}

bool InfraRedDataClass::_sendChar(int index, const char *msg)
{
	String toSend = "";
	String raw = "";
	unsigned char character = '\0';

	character = (unsigned char)msg[index];

	raw = ToolsClass::conversionToBase(character, BASE, DIGITS, abc.Mean); // 3 digits

	toSend = _msg->protocolize(raw.c_str(), abc.split);
	_sendPkg(toSend.c_str(), _pin.SendPin);

	delayMicroseconds(_pin.DELAY_CHAR);

#if defined (CRYPTDBUG)
	_debug_Crypt(raw.c_str(), toSend.c_str(), character);
#endif
}

//A FOR SENDING MESSAGES
bool InfraRedDataClass::sendMsg(const char * msg, uint8_t n = 1U)
{
	if (ended) return;

	if (((String)msg).compareTo("") == 0) return false;

	bool ok = false;

	unsigned int max = ((String)msg).length() - 1;

	while (n > 0)
	{
		//Serial.println("IR SND");
		//for each letter in the message,
		for (uint8_t j = 0; j <= max; j++)
		{
			_sendChar(j, msg);
		}
		n--;
	}

	msg = "";

	ok = true;

	return ok;
}

//////////////////////////////////////////////////////////////////////////