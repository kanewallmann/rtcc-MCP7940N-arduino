/* Part of RTCC (MCP7940N) Arduino library
 * Copyright (C) 2017 Kane Wallmann
 * See LICENCE.txt for license (MIT)*/

#ifndef __RTCC_MCP7940N_H
#define __RTCC_MCP7940N_H

#include <Arduino.h>
#include <Wire.h>
#include <time.h>

#define RTCC_SQWFS_1HZ                (B00)
#define RTCC_SQWFS_4KHZ                (B01)
#define RTCC_SQWFS_8KHZ                (B10)
#define RTCC_SQWFS_32KHZ            (B11)

#define RTCC_ALARM_MASK_SECONDS        (B000)
#define RTCC_ALARM_MASK_MINUTES        (B001)
#define RTCC_ALARM_MASK_HOURS        (B010)
#define RTCC_ALARM_MASK_WDAY        (B011)
#define RTCC_ALARM_MASK_DATE        (B100)
#define RTCC_ALARM_MASK_ALL            (B111)

typedef struct
{
	byte secten;
	byte secone;
	byte minten;
	byte minone;
	byte hrten;
	byte hrone;
	byte wkday;
	byte dateten;
	byte dateone;
	byte mthten;
	byte mthone;
	byte yrten;
	byte yrone;
	bool mode_24;
	bool pm;
}
		rtcc_time;

class RTCC_MCP7940N
{
public:
	RTCC_MCP7940N();

	void ReadTime(rtcc_time *time);

	void SetTime(const rtcc_time *time);

	void SetOscillatorEnabled(bool enabled);

	void SetAlarm1(const rtcc_time *time);

	void SetAlarm2(const rtcc_time *time);

	void Set24HourTime(bool enabled);

	bool Get24HourTime();

	void SetSquareWaveOutputState(bool enabled, int frequency = 0);

	bool GetSquareWaveOutputEnabled();

	bool IsOscillatorRunning();

	bool HasPowerFailed();

	void ClearPowerFailedFlag();

	void SetBatteryEnabled(bool enabled);

	bool GetBatteryEnabled();

	bool IsLeapYear();

	void SetAlarm1Enabled(bool enabled);

	bool GetAlarm1Enabled();

	void SetAlarmPolarity(bool high);

	bool GetAlarmPolarity();

	bool GetAlarm1Flag();

	void ClearAlarm1Flag();

	void SetAlarm1Mask(byte mask);

	byte GetAlarm1Mask();

	void SetAlarm2Enabled(bool enabled);

	bool GetAlarm2Enabled();

	bool GetAlarm2Flag();

	void ClearAlarm2Flag();

	void SetAlarm2Mask(byte mask);

	byte GetAlarm2Mask();

	void SetExternalOscillatorEnabled(bool enabled);

	bool GetExternalOscillatorEnabled();

	void SetCourseTrimEnabled(bool enabled);

	bool GetCourseTrimEnabled();

	bool GetGPOState();

	void SetGPOState(bool state);

	signed char GetTrimValue();

	void SetTrimValue(signed char value);

	static void ConvertTime(const tm *from, rtcc_time *to);

	static void ConvertTime(const rtcc_time *from, tm *to);

private:

	void SetBits(int address, int mask, int values);

	byte GetByte(int address);

	bool GetFlag(int address, int mask);

	void SetAlarm(int address, const rtcc_time *timestamp);

	int i2c_addr;
};

#endif //__RTCC_MCP7940N_H
