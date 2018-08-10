#pragma once



#define DELAY_CHAR 1 //minim 20 or 1?

#define PCT_DELAY 1.3 //DELTA DELAY BETWEEN TONES (10%)
#define TONE_ERROR 0.1 //TONE ERROR IN FRACTION (10%)


struct IRPreferences
{
	uint8_t SendPin = 9U; //PWM pin
	uint8_t InterruptPin = 2U; //PWM pin
};

