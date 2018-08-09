/*
 Name:		InfraRed.ino
 Created:	8/8/2018 20:02:18
 Author:	Fulvio
*/


#include "Msg.h"
#include "Tools.h"
#include "SerialEvent.h"

#include "InfraRedData.h"



// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(153600);

	setupSerialEvent();

	InfraRedData.configurePins(2, 14U, 9U, 100);
	InfraRedData.begin(true);
}

// the loop function runs over and over again until power down or reset
void loop() {

	InfraRedData.listen();

	String msg = loopSerialEvent();

	if (msg.compareTo("") != 0)
	{
		InfraRedData.sendMsg(msg.c_str(), 1U);
	}


}


void setup2()
{


	//Chip c = C16;
	//setupMemo(c);

	//Wire.begin();

	//for training

}




