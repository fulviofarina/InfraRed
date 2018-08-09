<body>
<h1>How it works </h1>
<p>#include "Msg.h"</p>
<p>#include "Tools.h"</p>
<p>#include "SerialEvent.h"</p>
<p>#include "InfraRedData.h"</p>

// the setup function runs once when you press reset or power the board

void setup() {

	Serial.begin(153600);
	setupSerialEvent();
	uint8_t receiverInterruptPin = 2;
	uint8_t analogReadPin = 14U;
	uint8_t senderPing = 9U;
	unsigned long bkgLimit = 100;

	InfraRedData.configurePins(receiverInterruptPin, analogReadPin, senderPing, bkgLimit);
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

</body>
