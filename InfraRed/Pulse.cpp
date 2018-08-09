#include "Pulse.h"

void Pulse::readIntensity(uint8_t pin, uint8_t cnt)
{
	if (n == cnt)
		//for averaging the background, last 9 values
	{
		fluxCum = 0;
		fluxAVG = 0;
		n = 1;
	}

	//read value from analog
	unsigned long Val = analogRead(pin);
	//make average of values
	fluxCum += Val; //add to total
	fluxAVG = (fluxCum / n); //take average
	n++;

	flux = Val;
}

double Pulse::calculateFrequency()
{
	return 1e3 / tau;
}
///In microseconds
double Pulse::calculatePeriod()
{
	//	double aux = 0;
	return (end - start) * 2;
}
bool Pulse::checkPulse(uint8_t pin, int BkgLimit)
{
	readIntensity(pin, 4); //4 times average flux
	bool beloLim = (flux < BkgLimit);

	//if higher than background it is a HIGH!
	if (beloLim == true && !mark) // 21 is the background error limit by experience
	{
		start = micros(); //start high
		mark = true;
	}
	else if (beloLim == false && mark)
	{
		end = micros(); //_end high of pulse
		mark = false;
	}

	return !mark;
}