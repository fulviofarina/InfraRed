// InfraRedData.h

#ifndef _INFRAREDDATA_h
#define _INFRAREDDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "IRPreferences.h"
#include "Pulse.h"

//#define FLUXDBUG 1 //VAL
//#define TAUDBUG 1 // TAU AVG
//#define CRYPTDBUG 1 //Crypto

class InfraRedDataClass
{
protected:

	// bool _checkPulse();
	void printMsg(char c);
	// bool _compareALetter(uint8_t u);
	void _sendPkg(const char *code, uint8_t _toneDPin);

	void _calculateTau();
	// void _train();
	bool _train(double freqMultiplier);
	bool _compareALetter(uint8_t alphaBetIter, float freq, float margin);

	IRPreferences _pin;

	Message *_msg;
	Pulse *_pulse; //strcuture is in ///Util.h

	int training = -1;
	// int training2 = -1;

	 ///////////////////////////////////////////////////////////////////////////

//	 bool msgTest = false;

	void _testAlphabet();
	void _testCode();

public:

	Message Msg;
	Pulse Pulse;

	bool sendMsg(const char * msgToSend, uint8_t times = 1U);

	void configurePins(uint8_t interruptPin = 2U, uint8_t analogPin = 14U, uint8_t senderPin = 9U, unsigned long bkgLimit = 100);
	void begin(bool shouldBeTrained = false);
	void listen();
	void readInterrupt();
};

extern InfraRedDataClass InfraRedData;

#endif
