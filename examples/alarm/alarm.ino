/* Part of RTCC (MCP7940N) Arduino library
 * Copyright (C) 2017 Kane Wallmann
 * See LICENCE.txt for license (MIT)*/

#include <Wire.h>
#include <RTCC_MCP7940N.h>

#define PIN_INT 	(3)

RTCC_MCP7940N rtc;

volatile bool itr = false;

void alarmint()
{
	// Just set flag so main "thread" can act on it
	itr = true;
}

void setup()
{
	Serial.begin(9600);

	// Must call before using rtcc
	Wire.begin();

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
	rtcc_time time;
	time.mode_24 = true;
	rtc.ConvertTime( &current_time, &time );
	rtc.SetTime( &time );

	char buff[40];
	strftime( buff, 40, "Time set to: %d-%m-%Y %H:%M:%S", &current_time );
	Serial.println( buff );

	// Must disable square wave for alarm to work
	rtc.SetSquareWaveOutputState( false );
	rtc.SetAlarm2Enabled( false );
	rtc.ClearAlarm2Flag();
	rtc.SetAlarm1Enabled( false );
	rtc.ClearAlarm1Flag();

	// Alarm sends PIN_INT HIGH
	rtc.SetAlarmPolarity( true );

	// Set alarm to go off in 5 seconds
	time.secone += 5;
	rtc.SetAlarm1Mask( RTCC_ALARM_MASK_ALL );
	rtc.SetAlarm1( &time );
	rtc.SetAlarm1Enabled( true );

	// Enable interrupt
	pinMode( PIN_INT, INPUT_PULLUP );
	attachInterrupt( digitalPinToInterrupt(PIN_INT), alarmint, RISING );
}

void loop()
{
	char buff[20];

	rtcc_time time;
	rtc.ReadTime(&time);

	// Print time every second
	sprintf(buff, "%u%u-%u%u-%u%u %u%u:%u%u:%u%u", time.dateten, time.dateone, time.mthten, time.mthone, time.yrten,
			time.yrone, time.hrten, time.hrone, time.minten, time.minone, time.secten, time.secone);
	Serial.println(buff);

	// Print ALARM when interrupt occurs
	if( itr )
	{
		Serial.println( "ALARM" );
		rtc.ClearAlarm1Flag();
		itr = false;
	}

	delay(1000);
}

