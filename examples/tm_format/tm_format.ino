/* Part of RTCC (MCP7940N) Arduino library
 * Copyright (C) 2017 Kane Wallmann
 * See LICENCE.txt for license (MIT)*/

#include <Wire.h>
#include <RTCC_MCP7940N.h>
#include <time.h>

RTCC_MCP7940N rtc;

void setup()
{
	Serial.begin(9600);

	// Must call before using rtcc
	Wire.begin();

	// Use crystal for timekeeping
	rtc.SetOscillatorEnabled(true);
	rtc.SetExternalOscillatorEnabled(false);
}

void loop()
{
	char buff[20];

	// Read time in internal format
	rtcc_time time;
	rtc.ReadTime(&time);

	// Covner to tm format
	tm tm_time;
	rtc.ConvertTime( &time, &tm_time );

	// Format time and output to string
	strftime( buff, 20, "%d-%m-%y %H:%M:%s", &tm_time );

	Serial.println(buff);

	delay(1000);
}

