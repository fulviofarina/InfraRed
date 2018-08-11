
#define SEPARATOR_1 '\n'
#define SEPARATOR_2 '\rn'
#define SEPARATOR_3 ' '

String inputSerial = "";         // a string to hold input serial data
bool finished = false;  // to flag if the string is complete

void setupSerialEvent()
{
	// reserves 1024 bytes for the string inputSerial:
	inputSerial.reserve(140); // 1 TWEET or 140 characters
}
void(*resetFunc) (void) = 0; //declare reset function @ address 0

String loopSerialEvent()
{
	
	// prints the strings when a carriage return or newline arrives:
	if (finished)
	{
		finished = false;
		String msg =  inputSerial;
		// clear the string:
		inputSerial.replace(msg,"");
		if (strstr(msg.c_str(), "<Reset>") != NULL)
		{
			resetFunc();
			return;
		}
	
		return msg;
	}
	else return "";
	
}

void serialEvent()
{
	while (Serial.available())
	{
		// get the new byte:
		char aux = (char)Serial.read();
		// add it to the inputSerial:
		inputSerial += aux;

	//	Serial.println(inputSerial.length());
		// if the incoming character is a carriage return or newline, finish the string concatenation
	
		if (aux == SEPARATOR_1 || aux == SEPARATOR_2 || aux == SEPARATOR_3)
	//	if (inputSerial.length() > 0)
		{
			finished = true;
		}
	}
}
