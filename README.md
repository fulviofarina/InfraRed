<body>
<h1>How it works </h1>

<p>#include "InfraRedData.h"</p>

// the setup function runs once when you press reset or power the board

void setup() {

	Serial.begin(128000);

	uint8_t receiverInterruptPin = 2;
	uint8_t senderPin = 9U;

	InfraRedData.configurePins(receiverInterruptPin, senderPin);

	bool shouldbeTrained = false;
	InfraRedData.begin(shouldbeTrained);
}

// the loop function runs over and over again until power down or reset

void loop() {

	InfraRedData.listen();
}

</body>
