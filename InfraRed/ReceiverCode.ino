/*
Created:	8/8/2018 20:02:18
Author:	Fulvio
*/

#include "InfraRedData.h"

// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(9600);
	
	uint8_t receiverInterruptPin = 2;
	InfraRedData.configureAsReceiver(receiverInterruptPin);
	
	InfraRedData.begin();
}

// the loop function runs over and over again until power down or reset
void loop() {

	
	InfraRedData.listen();

}




