
#ifndef _TOOLS_h
#define _TOOLS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define ENTER  '\n'
#define DOT  '.'
#define SPACE  ' '
#define NULO '\0'
#define SEPARATOR  " = "

class ToolsClass
{
protected:

public:
	static String  conversionToBase(int number_to_convert, uint8_t  base, const uint8_t outbits, const char alphaB[]);
	static unsigned int conversionInt(unsigned int arrBits[], int arrSize, int base);
	static int power(int x, int y);
	void init();
};

#endif
