
#define separator1 '\n'
#define separator2 '\rn'

String inputSerial = "";         // a string to hold input serial data
bool finished = false;  // to flag if the string is complete
void setupSerialEvent()
{
	// reserves 1024 bytes for the string inputSerial:
	inputSerial.reserve(500); // 1 TWEET or 140 characters
}

String loopSerialEvent()
{
	String msg = "";
	// prints the strings when a carriage return or newline arrives:
	if (finished)
	{

		finished = false;
		msg = inputSerial;

		// clear the string:
		inputSerial = "";
	}

	return msg;
}

void serialEvent()
{
	while (Serial.available())
	{
		// get the new byte:
		char aux = (char)Serial.read();
		// add it to the inputSerial:
		inputSerial += aux;
		// if the incoming character is a carriage return or newline, finish the string concatenation
		if (aux == separator1 || aux == separator2) 	finished = true;
	}
}
