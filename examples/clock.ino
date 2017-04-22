#include <Wire.h>
#include <RTCC_MCP7940N.h>

RTCC_MCP7940N rtc();

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

	// rtcc_time struct represents how the data is stored on RTCC chip, see tm_format example for info on converting
	rtcc_time time;
	rtc.ReadTime(&time);

	sprintf(buff, "%u%u-%u%u-%u%u %u%u:%u%u:%u%u", time.dateten, time.dateone, time.mthten, time.mthone, time.yrten,
			time.yrone, time.hrten, time.hrone, time.minten, time.minone, time.secten, time.secone);
	Serial.println(buff);

	delay(1000);
}
