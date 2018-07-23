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

	// Use crystal for timekeeping
	rtc.SetOscillatorEnabled(true);
	rtc.SetExternalOscillatorEnabled(false);
}

void loop()
{
	char buff[20];
	bool pwrOutage;

	// rtcc_time struct represents how the data is stored on RTCC chip, see tm_format example for info on converting
	rtcc_time time, dwTime, upTime;
	rtc.ReadTime(&time);

	sprintf(buff, "%u%u-%u%u-%u%u %u%u:%u%u:%u%u", time.dateten, time.dateone, time.mthten, time.mthone, time.yrten,
			time.yrone, time.hrten, time.hrone, time.minten, time.minone, time.secten, time.secone);
	Serial.println(buff);

	pwrOutage = rtc.HasPowerFailed(&dwTime, &upTime);
	if (pwrOutage) {
	    Serial.println("*****NEW OUTAGE*****");
	}
	else {
	    Serial.println("LastOutage: ");
	}
	Serial.print("    From: ");
	sprintf(buff, "%u%u-%u%u-%u%u %u%u:%u%u:%u%u", dwTime.dateten, dwTime.dateone, dwTime.mthten, dwTime.mthone, dwTime.yrten,
			dwTime.yrone, dwTime.hrten, dwTime.hrone, dwTime.minten, dwTime.minone, dwTime.secten, dwTime.secone);
	Serial.println(buff);
	Serial.print("    To: ");
	sprintf(buff, "%u%u-%u%u-%u%u %u%u:%u%u:%u%u", upTime.dateten, upTime.dateone, upTime.mthten, upTime.mthone, upTime.yrten,
			upTime.yrone, upTime.hrten, upTime.hrone, upTime.minten, upTime.minone, upTime.secten, upTime.secone);
	Serial.println(buff);

	delay(10000);
}
