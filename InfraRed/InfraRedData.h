#ifndef _INFRAREDDATA_h
#define _INFRAREDDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define TITLE "\n\nInfraRed Data Transmission Program\n"
#define RCVD_MSG  "\nIR RCVD:\n"

#include "IRPreferences.h"
#include "Pulse.h"
#include "Msg.h"

//#define FLUXDBUG 1 //VAL
//#define TAUDBUG 1 // TAU AVG
//#define CRYPTDBUG 1 //Crypto

class InfraRedDataClass
{
protected:
	void _showTrainingFinished();

	bool _sendChar(int index, const char *msg);
	void _printMsg(char c);
	void _sendPkg(const char *code, uint8_t _toneDPin);

	bool _train(double freqMultiplier);
	bool _compareALetter(uint8_t alphaBetIter, float freq);

	IRPreferences _pin;
	Message *_msg;
	Pulse *_pulse; //strcuture is in ///Util.h

	int _training = -1;
	bool ended = false;
	 ///////////////////////////////////////////////////////////////////////////

	void _testAlphabet();
	void _testCode();

	void _debug_CryptCode(const char * aux);
	void	_debug_FluxTone(float margin, float freqMultiplier);
	void _debug_Multiplier(float margin);
	void _debug_CryptChar(unsigned char c, unsigned int data, const char* pkg);
	void _debug_TauChar();
	void _debug_Crypt(const char *raw, const char * toSend, unsigned char character);

public:
	void end();
	bool standBy(uint8_t timesToTrnasmite = 1U);
	void readInterrupt();
	Message Msg;
	Pulse Pulse;
	bool sendMsg(const char * msgToSend, uint8_t times = 1U);
	void configureAsTransmitter(uint8_t senderPin = 9U);
	void configureAsReceiver(uint8_t interruptPin = 2U);
	void begin(bool shouldBeTrained = false);
	void listen();
};

extern InfraRedDataClass InfraRedData;

#endif
