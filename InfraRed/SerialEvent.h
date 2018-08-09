String inputSerial = "";         // a string to hold input serial data
bool finished = false;  // to flag if the string is complete
void setupSerialEvent()
{
	// initialize serial communication
	// reserves 1024 bytes for the string inputSerial:
	inputSerial.reserve(500); // 1 TWEET or 140 characters
}

String loopSerialEvent() {
	String msg = "";
	// prints the strings when a carriage return or newline arrives:
	if (finished)
	{
		// You should call routines to work with the content
		//of 'inputSerial' before it gets erased
		//	sendToneExample();

		finished = false;

		msg = inputSerial;

		// clear the string:
		inputSerial = "";

		//	Serial.print("Serial In: ");
		//	Serial.println(msg);
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
		if (aux == '\n' || aux == '\rn') 	finished = true;
	}
}
