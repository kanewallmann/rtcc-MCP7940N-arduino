/* Part of RTCC (MCP7940N) Arduino library
 * Copyright (C) 2017 Kane Wallmann
 * See LICENCE.txt for license (MIT)*/

#include <Wire.h>
#include <RTCC_MCP7940N.h>

RTCC_MCP7940N rtc;

void setup()
{
	Serial.begin(9600);

	// Must call before using rtcc
	Wire.begin();

	rtcc_time time;
	char buff[30];

	if( rtc.hasPowerFailed() )
	{
		rtc.getPowerDownTime(&time);

		Serial.println( "Power failure detected" );

		sprintf(buff, "OFF %u%u-%u%u-%u%u %u%u:%u%u:%u%u", time.dateten, time.dateone, time.mthten, time.mthone,
		        time.yrten,
		        time.yrone, time.hrten, time.hrone, time.minten, time.minone, time.secten, time.secone);

		Serial.println(buff);

		rtc.getPowerUpTime(&time);

		sprintf(buff, "ON %u%u-%u%u-%u%u %u%u:%u%u:%u%u", time.dateten, time.dateone, time.mthten, time.mthone,
		        time.yrten,
		        time.yrone, time.hrten, time.hrone, time.minten, time.minone, time.secten, time.secone);

		Serial.println(buff);

		rtc.clearPowerFailedFlag();
	}
	else
	{
		// If flag is clear than this is a clean startup

		// Make up a date/time
		tm current_time;
		current_time.tm_hour = 10;
		current_time.tm_min = 36;
		current_time.tm_mday = 21;
		current_time.tm_mon = 3;
		current_time.tm_year = 117;
		current_time.tm_wday = 5;
		current_time.tm_sec = 0;

		// Convert to rtcc format and set
		time.mode_24 = true;
		rtc.convertTime(&current_time, &time);
		rtc.setTime(&time);

		// Use crystal for timekeeping
		rtc.setOscillatorEnabled(true);
		rtc.setExternalOscillatorEnabled(false);
		rtc.setBatteryEnabled(true);
	}
}

void loop()
{
}
