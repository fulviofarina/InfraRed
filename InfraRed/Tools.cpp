#include "Tools.h"

void ToolsClass::init()
{
}

int ToolsClass::power(int x, int y)
{
	if (y == 0)
		return 1;
	else if (y % 2 == 0)
		return power(x, y / 2)*power(x, y / 2);
	else
		return x*power(x, y / 2)*power(x, y / 2);
}

String  ToolsClass::conversionToBase(int number_to_convert, uint8_t  base, const uint8_t outbits, const char alphaB[])
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

//conversion from another base
unsigned int ToolsClass::conversionInt(unsigned int arrBits[], int arrSize, int base = 2)
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