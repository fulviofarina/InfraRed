#ifndef _UTIL_h
#define _UTIL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <stdio.h>

#define thousand  1000

bool copy = false; //to mark the high and low of a pulse
//bool inform = false;

struct Pulse
{
	unsigned long  start = 0;
	unsigned long end = 0;

	unsigned long fluxCum = 0; //cummulative intensity
	double fluxAVG = 0; //actual average value intensity
	unsigned long flux = 0; //last value instensity

	uint8_t n = 1; //counter for fluxes (is it the same as nFlux?)

	double tauCum = 0; //cummulative of tau
	double tau = 0; //tau of pulse (current?)
	uint16_t nPulse = 1; //counter for pulses
	uint16_t lastnPulse = 0; //last n?? why?
	double tauAVG = 0; //average of tau
};

//1-A intensity
unsigned long readIntensity(uint8_t pin, Pulse *p, uint8_t n)
{
	if (p->n == n)
		//for averaging the background, last 9 values
	{
		p->fluxCum = 0;
		p->fluxAVG = 0;
		p->n = 1;
	}

	//read value from analog
	unsigned long Val = analogRead(pin);
	//make average of values
	p->fluxCum += Val; //add to total
	p->fluxAVG = (p->fluxCum / p->n); //take average
	p->n++;

	return Val;
}

int power(int x, int y)
{
	if (y == 0)
		return 1;
	else if (y % 2 == 0)
		return power(x, y / 2)*power(x, y / 2);
	else
		return x*power(x, y / 2)*power(x, y / 2);
}

bool doBinary(int data, int iter)
{
	int iter1 = (1 << iter);
	int result = data & iter1;
	bool r = result ? 1 : 0;
	//u += r;
	//*p = r;
	//Serial.print(r);
	return r;
}

// Converts a number into a given base representation of it of size outbits, based on the provided alphabet
// returns the composed string
String  conversionToBase(int number_to_convert, uint8_t  base, const uint8_t outbits, const char alphaB[])
{
	char converted_number[outbits];
	int aux = number_to_convert;
	for (uint8_t j = 0; j < outbits; j++)
	{
		converted_number[j] = alphaB[0]; //initializes the array with the first letter of the alphabet
	}
	uint8_t i = 0;
	while (aux != 0)
	{
		int index = aux % base;
		converted_number[i] = alphaB[index];

		aux = (int)(aux / base);

		++i;
	}

	String msg = "";
	//store each char in array (string)
	for (uint8_t j = 0; j < outbits; j++)
	{
		char t = converted_number[j];

		msg += t;
	}

	return msg.c_str();
}

//AUTOMATIC FOR BINARY
int conversionInt(bool arrBits[], int arrSize)
{
	int data = 0;

	for (uint8_t j = 0; j < arrSize; j++)
	{
		int bit = arrBits[j];
		//data += bit * power(2, (arrSize - 1) - j); //conversion to unsigned int representing a char (or word)
		data += bit * power(2, j); //conversion to unsigned int representing a char (or word)
	}
	return data;
}
//conversion from another base
unsigned int conversionInt(unsigned int arrBits[], int arrSize, int base)
{
	unsigned int data = 0;
	for (uint8_t j = 0; j < arrSize; j++)
	{
		unsigned int bit = arrBits[j];
		//data += bit * power(2, (arrSize - 1) - j); //conversion to unsigned int representing a char (or word)
		data += bit * power(base, j); //conversion to unsigned int representing a char (or word)
	}
	return data;
}
int convertToScientific(double valor)

{
	float Val = valor;
	int toShow = 9999;

	//unsigned long cap2 = Capacitance;
	uint8_t cumm = 0;
	uint8_t i = 9;
	float tope = pow(10, i);

	if (Val > tope) return toShow;
	if (Val < 1) return toShow;

	while (cumm == 0)
	{
		if (tope > Val)
		{
			tope /= 10;
			i--;
		}
		else cumm = i; //this gives me the power of 10th
	}

	for (uint8_t i = 1; i <= cumm; i++)  Val /= 10;

	//Serial.println(Val);

	String newAux = "";

	char arr[10];
	//	char *p = arr;
	//	sprintf(arr, "%1.2f", Val);

	newAux = dtostrf(Val, 4, 2, arr); //volver a poner esta linea

//int returne =	snprintf(arr, 4, "%i", Val);
	//  Serial.println(returne);

	//Serial.println(newAux);

	newAux.remove(1, 1);

	//newAux = *p;
	/*
	for (uint8_t i = 0; i < 4; i++)
	{
		if (arr[i] != '.')
		{
			newAux += arr[i];
			//Serial.print(arr[i]);
		}
	}
	*/

	newAux += String(cumm); //ad the scientific number

	//Serial.println(newAux);

	toShow = atoi(newAux.c_str());

	Val = 0;
	newAux = "";

	return toShow;
}

//not used
double thermistorConversion(int RawADC) {
	double Temp;
	Temp = (1024.0 / RawADC) - 1;
	double thermistorVal = 100;
	Temp = log(thermistorVal*Temp);
	Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp))* Temp);
	Temp = Temp - 273.15;
	//	Temp = 42 - Temp;
	//Temp = (Temp * 9.0) / 5.0 + 32.0;
	return Temp;
}

#endif