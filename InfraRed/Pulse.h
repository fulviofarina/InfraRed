#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Pulse
{
protected:

public:
	uint8_t tone = 0;
	bool isSame = false;
	unsigned long  start = 0;
	unsigned long end = 0;
	unsigned long fluxCum = 0; //cummulative intensity
	double fluxAVG = 0; //actual average value intensity
	unsigned long flux = 0; //last value instensity
	uint8_t n = 1; //counter for fluxes (is it the same as nFlux?)
	double tauCum = 0; //cummulative of tau
	double tau = 0; //tau of pulse (current?)
	bool mark = false;
	double tauAVG = 0; //average of tau

	void readIntensity(uint8_t pin, uint8_t n);
	double calculateFrequency();
	double calculatePeriod();
	bool checkPulse(uint8_t pin, int BkgLimit);
	//	 bool checkPulse(uint8_t pin);
};