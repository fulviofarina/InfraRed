#include "Pulse.h"



void Pulse::readIntensity(uint8_t pin, uint8_t cnt)
{
	//for averaging the background, last n values
	if (n == cnt)
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
	return THOUS_CONV / tau;
}
///In microseconds
double Pulse::calculatePeriod()
{

	return (end - start) * PERIOD_CONV;
}
bool Pulse::checkPulse( uint8_t interruptPin, int BkgLimit)
{

	bool beloLim = digitalRead(interruptPin);
	//if higher than background it is a HIGH!
	if (beloLim == false && !mark) // 21 is the background error limit by experience
	{
		start = micros(); //start high
		mark = true;
	}
	else if (beloLim == true && mark)
	{
		end = micros(); //_end high of pulse
		tau = calculatePeriod(); //in microseconds
		mark = false;
	}
	return !mark;
}