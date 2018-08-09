#pragma once

struct IRPreferences
{
	uint8_t FluxPin = 14U; // an analog input Pin A0 to A5
	unsigned long BkgLimit = 100; //bkg limit for flux
	uint8_t SendPin = 9U; //PWM pin
};

#define TITLE "\n\nInfraRed Data Transmission Program\n"
#define RCVD_MSG  "\nIR RCVD:\n"

//#define toneDur 5 //minimum 10
#define DELAY_CHAR 1 //minim 20
#define FREQ_MULTI 40 //in Hz (or 50, original)
#define PCT_DELAY 1.1 //DELTA DELAY BETWEEN TONES (10%)
#define TONE_ERROR 0.1 //TONE ERROR IN FRACTION (10%)
