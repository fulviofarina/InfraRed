//#include "arduino.h"

#include <stdint.h>

struct gTempo
{
	int8_t hour = 0;
	int8_t minute = 0;
	int8_t day = 0;
	int8_t month = 0;

	int8_t  hmax = 20;
	int8_t hmin = 6;

	float lastMicro = 0;
	int8_t watchDog = 0;

	int RAM = 0;
};

#define nextSecs 3600
#define divisor 1000

static String toPost = "";
static bool post = false;

gTempo gt;

bool toBoot = false;
bool toPseudoReset = false;
float lastMicro = 0;
bool alarm = false;
int soundCnt = 0;
bool watchDog = false;

void Convert(String * time, int8_t gmt, int8_t gmtmin)
{
	String timeTxt = *time;

	//Serial.println("AGAINA: "+ timeTxt);

	if (timeTxt.compareTo("") == 0) return;

	const int8_t timeSep = timeTxt.lastIndexOf(':') - 3;

	String aux = timeTxt.substring(timeSep - 8, timeSep - 6);

	//gTempo gt = *gte;
	//	Serial.println(aux);

	gt.month = atoi(aux.c_str());
	aux = timeTxt.substring(timeSep - 5, timeSep - 3);

	//Serial.println(gt.month);

	gt.day = atoi(aux.c_str());

	//Serial.println(gt.day);

	aux = timeTxt.substring(timeSep - 2, timeSep);
	gt.hour = atoi(aux.c_str());

	gt.hour += gmt;
	//Serial.print(gt.hour);

	if (gt.hour <= 0) //only if UTC time is tomorrow
	{
		gt.hour += 24;
		gt.day--;

		//if (month == 1 && day == 0) month = 12;
		if (gt.day == 0)
		{
			gt.month--;
		}

		if (gt.month == 0 && gt.day == 0)
		{
			gt.day = 31;
			gt.month = 12;
		}
	}
	//Serial.println(gt.hour);

	aux = timeTxt.substring(timeSep + 1, timeSep + 3);
	gt.minute = atoi(aux.c_str());

	gt.minute += gmtmin;

	if (gt.minute < 0)
	{
		gt.hour--;
		//if (hour >= 23) day--;
		gt.minute = 60 + gt.minute;
	}

	if (gt.hour == 24) gt.hour = 0;

	aux = "";
	timeTxt = aux;

	//	Serial.println("tiempoooooo");

	//timeChat.dis
}

static int8_t findCmd(const char *cmd)
{
	String order = cmd;
	//Serial.println(aux[0]);
	const int8_t arrMax = 6;

	String Cmd[arrMax] = { "ac" ,"blink","flush", "pin","finance","time" };

	//finance 0, 1 ,2 (finance,alarms,update)
	//time 0, 1 (time,dog)
	//flush 0, 1, 2, 3 (flush,quit,reset,reboot)
	//pin0 to read
	//pin1 to write

	int8_t cmdInd = 0;
	while (cmdInd < arrMax) //size of array of cmds
	{
		//Serial.println(order[0].length());
		//Serial.println(Cmd[cmdInd].length());
		//Serial.println("testing " + Cmd[cmdInd] + " " + String(cmdInd));
		if (order.compareTo(Cmd[cmdInd]) == 0)
		{
			//cmd = true;
			break;
			//Serial.println("indice" + String(cmdInd));
		}
		else cmdInd++;

		//if (cmd) break;
	}

	if (cmdInd == arrMax) cmdInd = -1; //not found, outside of array

	return cmdInd;
}

void cReport(String *message)
{
	String msg = *message;

	Serial.println("REPORTING");

	//const char brn[] = "\r\n";
	const char brn[] = "\n";
	toPost = "";

	for (int a = 4; a < 8; a++)
	{
		toPost += "PIN";
		toPost += String(a);
		toPost += " ";
		toPost += String(analogRead(a));
		toPost += brn;
	}

	toPost += F("MIC ");
	toPost += String(millis() / divisor);
	toPost += brn;
	toPost += F("CHK ");
	toPost += String(gt.lastMicro);
	toPost += brn;
	toPost += F("DTI ");
	toPost += String(gt.day) + "-" + String(gt.month);
	toPost += F(" @ ");
	toPost += String(gt.hour) + ":" + String(gt.minute);
	toPost += brn;
	toPost += F("DOG ");
	toPost += String(gt.watchDog);
	toPost += brn;
#if defined TFT

#else
	toPost += F("ALR ");
	toPost += String(alarm);
	toPost += brn;
#endif

	toPost += F("RAM ");
	//toPost += String(getFreeRam());
	toPost += brn;
	toPost += msg;

	toPost += brn;

	//aux += brn;
	//toPost += "</html>\n";
}

static void executeCmd(const char *Char)
{
	//	String thisChar = *tC;

	String thisChar = Char;

	if (thisChar.length() <= 2) return;

	String order[3];
	order[1] = order[2] = ""; //order (,) // usually an INT
							  //order[2] = ""; // order (*) //usually a BOOL
							  //order[0] = ""; //CMD
	order[0] = thisChar;

	int8_t aux[3];
	aux[0] = aux[1] = aux[2] = 0;
	//o is relay // 1 is order nr and 2 is subOrder nr

	int8_t comma = thisChar.indexOf(',');

	if (comma > 0)
	{
		order[1] = thisChar.substring(comma + 1);
		order[0] = thisChar.substring(0, comma);

		comma = order[1].indexOf('*');

		if (comma > 0)
		{
			order[2] = order[1].substring(comma + 1);
			order[1] = order[1].substring(0, comma);

			aux[2] = atoi(order[2].c_str());
		}
		aux[1] = atoi(order[1].c_str());
	}

	const int8_t  lastdigit = order[0].length() - 1;
	aux[0] = atoi(String(order[0][lastdigit]).c_str());
	order[0] = order[0].substring(0, lastdigit);

	const int8_t cmdInd = findCmd(order[0].c_str());

	Serial.println("CMD NMR =" + String(cmdInd));

	if (cmdInd < 0) return; // was no command

	post = true;

	//------------------------------------------------------------------------

	const String tt = "=\t";
	const String rn = "\n";

	String report = rn + "CR" + tt + order[0] + "+" + String(aux[0]);
	report += "," + order[1] + "*" + order[2] + rn;

	if (cmdInd == 0)
	{
		//TFT
#if defined (TFT)

		//AC LAMP
#else
		//ON
		bool on = false;
		//aux1 is after the comma,
		//while aux0 gives the relay

		if (aux[1] == 1) // ON#,1 or ON#,0 where # is the relay number
		{
			on = true;
		}
		//setAClamp(on, aux[0]);
#endif
	}
	else if (cmdInd == 1)
	{
#if defined (TFT)

		//BLINK
#else
		if (order[1].compareTo("") == 0) aux[1] = 10;

		//	blinkAClamp(aux[0], aux[1]);
		report += "BLK " + order[1] + " s" + rn;
#endif
	}

	else if (cmdInd == 2)
	{
		//FLUSH 0, 1
		toBoot = false;
		toPseudoReset = false;

		if (aux[0] == 0)
		{
			//RESET
			toBoot = true;
			report += "Boot";
			//main();
		}
		else if (aux[0] == 1)
		{
			//REBOOT
			toPseudoReset = true;

			report += "Pseudo Reset";
			//pseudoReset();
		}
		report += rn;
	}

	else if (cmdInd == 3)
	{
		//PIN

		if (order[1].compareTo("") == 0) aux[1] = 3; // a7??
		int16_t  statusSolar = 0;
		//aux[2] has the digital HIGH OR LOW
		//	int8_t statusdigital = aux[2];
		//analogRead(aux[1]); //check solar lamp
		report += "PIN " + String(aux[1]);
		if (aux[0] == 1) //to write
		{
			//PIN1
			pinMode(aux[1], OUTPUT);
			digitalWrite(aux[1], aux[2]); //check solar lamp
										  //statusSolar = analogRead(aux[1]); //check solar lamp
			report += rn + "WRT " + aux[2];
		}
		else //to read
			 //PIN0
		{
			pinMode(aux[1], INPUT);

			//	report += rn + "PIN";// +tt + String(aux[1]) + "\tSTA" + tt + String(statusSolar);
		}

		statusSolar = analogRead(aux[1]); //check solar lamp
		delay(10);
		aux[2] = digitalRead(aux[1]);
		report += rn + "VAL " + String(statusSolar) + "\tDIG " + String(aux[2]);
	}
#if defined (FINANCE2)

	else if (4 == cmdInd)
	{
		//FINANCE0 shows lists

		if (aux[0] == 1 || aux[0] == 0)
		{
			report += "SID" + tt + String(mIndex) + rn;
		}

		bool rep = false;

		if (aux[0] == 1)  rep = true; //finance1 show lists of stocks in alarm

		else //finance2 updates limits
			if (aux[0] == 2)
			{
				if (order[1].compareTo("") == 0) aux[1] = 1;

				float percent = (100 + aux[1])*0.01;

				report += "SUP" + tt + order[1];

				limitStocks(percent);
			}

		report += makeReport(rep);

		//chat.println();
	}
#endif

	else if (cmdInd == 5)
	{
		//		String  time = checkTime();
		String time = "";
		//Serial.println(time);

		Convert(&time, -4, -30);

		//	time = "";

		Serial.println("CONVERTED");

		if (aux[0] == 0)
		{
			//TIME
		}
		else
		{
			// DOG
			if (order[1].compareTo("") == 0) aux[1] = gt.hmax;
			if (order[2].compareTo("") == 0) aux[2] = gt.hmin;

			//checkTime(-4, -30);

			gt.hmax = aux[1];
			gt.hmin = aux[2];

			//wDogCounter = hmin - hmax;

			//  if(wDogCounter<0)    wDogCounter +=24;

			//uint8_t  equi = 0;
			if (gt.hour >= gt.hmax || gt.hour <= gt.hmin)
			{
				//hora de prender las luces cada hora//

				gt.watchDog = 1;
			}
			else
			{
				gt.watchDog = 0;
				gt.lastMicro = 0;
			}

			//"\tENA" +  tt + String((int)(watchDog));
		}
	}

	report += rn;

	cReport(&report);
}

static void checkRoutine()
{
	//currentMicro = millis(); //get milis
#if defined TFT

#else
	const float currentMicro = millis() / divisor;

	if (currentMicro > lastMicro || alarm)
	{
		//	executeCmd("time0"); //get time

		executeCmd("time1,20*6"); //execute the DOG (if any)

		if (watchDog || alarm)
		{
			//const String	blk = "blink";
			const char coma1[] = ",20";
			const char	blk[] = "blink";

			for (uint8_t i = 1; i < 5; i++)
			{
				String tex = String(blk) + String(i) + String(coma1);
				executeCmd(tex.c_str()); //execute the command (if any)

				Serial.print("B");
			}

			if (alarm)
			{
				alarm = false;
				soundCnt = 0;
			}
		}

		lastMicro = currentMicro;
		lastMicro += nextSecs;
	}

#endif
}
/*
void cReportOld(String msg)
{
String brn = "<br>\n";

toPost = "";

for (int a = 4; a < 8; a++)
{
toPost += "PIN";
toPost += String(a);
toPost += " ";
toPost += String(analogRead(a));
toPost += brn;
}

toPost += ("MIC " + String(millis() / divisor) + " s");
toPost += brn;
toPost += ("CHK " + String(lastMicro) + " s");
toPost += brn;
toPost += ("DTI " + String(day) + "-" + String(month));
toPost += ("@" + String(hour) + ":" + String(minute));
toPost += brn;
toPost += "DOG " + String(watchDog);
toPost += brn;
toPost += msg;
//aux += brn;
toPost += "</html>\n";

//Serial.println(toPost);

//	else Serial.println("NO CHAT");
}

*/
