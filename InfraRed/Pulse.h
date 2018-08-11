#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define THOUS_CONV 1e3
#define PERIOD_CONV  2/THOUS_CONV


class Pulse
{
protected:

	bool mark = false;
	uint8_t n = 1; //counter for fluxes (is it the same as nFlux?)

	double tauCum = 0; //cummulative of tau
	unsigned long fluxCum = 0; //cummulative intensity

public:
	uint8_t tone = 0;
	bool isSame = false;
	
	int FREQ_MULTI = 50; //in Hz (or 50, original)

	double fluxAVG = 0; //actual average value intensity
	unsigned long flux = 0; //last value instensity
	
	double tau = 0; //tau of pulse (current?)

	double tauAVG = 0; //average of tau

	unsigned long  start = 0;
	unsigned long end = 0;

	void readIntensity(uint8_t analogPin, uint8_t n);
	double calculateFrequency();
	double calculatePeriod();
	bool checkPulse(uint8_t interruptpin);
	
};