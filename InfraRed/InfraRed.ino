/*
 Name:		InfraRed.ino
 Created:	8/8/2018 20:02:18
 Author:	Fulvio
*/

#include "InfraRedData.h"

// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(9600);
//	153600
	uint8_t receiverInterruptPin = 2;
	InfraRedData.configureAsReceiver(receiverInterruptPin);

	uint8_t senderPin = 9U;
	InfraRedData.configureAsTransmitter(senderPin);

	bool shouldbeTrained = false;
	InfraRedData.begin(shouldbeTrained);
}

// the loop function runs over and over again until power down or reset
void loop() {


	InfraRedData.standBy();
	InfraRedData.listen();

	
}

void setup2()
{
	//Chip c = C16;
	//setupMemo(c);
	//Wire.begin();
	//for _training
}




