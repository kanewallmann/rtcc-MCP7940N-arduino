/* Part of RTCC (MCP7940N) Arduino library
 * Copyright (C) 2017 Kane Wallmann
 * See LICENCE.txt for license (MIT)*/

#include "RTCC_MCP7940N.h"

RTCC_MCP7940N::RTCC_MCP7940N()
{
	this->i2c_addr = RTCC_MCP7940_I2C_ADDR;
}

void RTCC_MCP7940N::ReadTime(rtcc_time *time)
{
	// Set read address to 0
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(0);
	Wire.endTransmission();

	// Request 7 byte Time and Date registers
	Wire.requestFrom(this->i2c_addr, 7, true);

	byte time_data[7];
	Wire.readBytes(time_data, 7);

	time->secten = (time_data[0] & B1110000) >> 4;
	time->secone = time_data[0] & B1111;
	time->minten = (time_data[1] & B111000) >> 4;
	time->minone = time_data[1] & B1111;
	time->hrten = (time_data[2] & B110000) >> 4;
	time->hrone = time_data[2] & B1111;
	time->wkday = time_data[3] & B111;
	time->dateten = (time_data[4] & B110000) >> 4;
	time->dateone = time_data[4] & B1111;
	time->mthten = (time_data[5] & B10000) >> 4;
	time->mthone = time_data[5] & B1111;
	time->yrten = (time_data[6] & B11110000) >> 4;
	time->yrone = time_data[6] & B1111;
}

void RTCC_MCP7940N::SetBits(int address, int mask, int values)
{
	// Set read address
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(address);
	Wire.endTransmission();

	// Read the existing value
	Wire.requestFrom(this->i2c_addr, 1, true);
	byte data = Wire.read();

	// Clear out the existing bits according to mask
	data &= ~mask;

	// Set bits according to values
	data |= (values & mask);

	Wire.beginTransmission(this->i2c_addr);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}

byte RTCC_MCP7940N::GetByte(int address)
{
	// Set read address to 0
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(address);
	Wire.endTransmission();

	Wire.requestFrom(this->i2c_addr, 1);
	return Wire.read();
}

bool RTCC_MCP7940N::GetFlag(int address, int mask)
{
	byte data = this->GetByte(address);
	return data & mask;
}

void RTCC_MCP7940N::SetTime(const rtcc_time *time)
{
	// Set read address to 0
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(0);
	Wire.endTransmission();

	// Request 7 byte Time and Date registers
	Wire.requestFrom(this->i2c_addr, 7, true);

	byte time_data[7];
	Wire.readBytes(time_data, 7);

	// Clear ST and EXTOSC during write operation, these were read above so will be reset to original values after the write
	this->SetBits(0x00, B10000000, 0);
	this->SetBits(0x07, B100, 0);

	// Wait for OSCRUN to go low
	while( this->GetByte(0x03) & B100000 )
	{
	}

	// Save flags but clear all date/time data
	time_data[0] = time_data[0] & B10000000;
	time_data[1] = time_data[1] & B10000000;
	time_data[2] = time_data[2] & B11000000;
	time_data[3] = time_data[3] & B11111000;
	time_data[4] = time_data[4] & B11000000;
	time_data[5] = time_data[5] & B11100000;
	time_data[6] = time_data[6] & B00000000;

	// Seconds
	time_data[0] |= ((time->secten) << 4) & B1110000;
	time_data[0] |= time->secone & B1111;

	// Minutes
	time_data[1] |= ((time->minten) << 4) & B1110000;
	time_data[1] |= time->minone & B1111;

	// Hours
	if( time->mode_24 )
	{
		time_data[2] |= ((time->hrten) << 4) & B110000;
		time_data[2] |= time->hrone & B1111;
	}
	else
	{
		time_data[2] |= time->pm << 5;
		time_data[2] |= ((time->hrten) << 4) & B10000;
		time_data[2] |= time->hrone & B1111;
	}

	// Week day
	time_data[3] |= time->wkday & B111;

	// Date
	time_data[4] |= ((time->dateten) << 4) & B110000;
	time_data[4] |= time->dateone & B1111;

	// Month
	time_data[5] |= ((time->mthten) << 4) & B10000;
	time_data[5] |= time->mthone & B1111;

	// Year
	time_data[6] |= (time->yrten << 4) & B11110000;
	time_data[6] |= time->yrone & B1111;

	// Write new data into chip
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(0);
	for( int a = 0; a < 7; a++ )
	{
		Wire.write(time_data[a]);
	}
	Wire.endTransmission();
}

bool RTCC_MCP7940N::GetGPOState()
{
	return this->GetFlag(0x07, B10000000);
}

void RTCC_MCP7940N::SetGPOState(bool state)
{
	this->SetBits(0x07, B10000000, state << 7);
}

void RTCC_MCP7940N::SetExternalOscillatorEnabled(bool enabled)
{
	this->SetBits(0x07, B1000, enabled << 3);
}

bool RTCC_MCP7940N::GetExternalOscillatorEnabled()
{
	return this->GetFlag(0x07, B1000);
}

bool RTCC_MCP7940N::GetAlarm1Enabled()
{
	return this->GetFlag(0x07, B10000);
}

bool RTCC_MCP7940N::GetAlarm2Enabled()
{
	return this->GetFlag(0x07, B100000);
}

void RTCC_MCP7940N::SetCourseTrimEnabled(bool enabled)
{
	this->SetBits(0x07, B100, enabled << 2);
}

bool RTCC_MCP7940N::GetCourseTrimEnabled()
{
	return this->GetFlag(0x08, B100);
}

void RTCC_MCP7940N::SetAlarm1Enabled(bool enabled)
{
	this->SetBits(0x07, B10000, enabled << 4);
}

void RTCC_MCP7940N::SetAlarm2Enabled(bool enabled)
{
	this->SetBits(0x07, B100000, enabled << 5);
}

bool RTCC_MCP7940N::IsOscillatorRunning()
{
	return this->GetFlag(0x03, B100000);
}

bool RTCC_MCP7940N::HasPowerFailed()
{
	return this->GetFlag(0x03, B10000);
}

bool RTCC_MCP7940N::GetBatteryEnabled()
{
	return this->GetFlag(0x03, B1000);
}

bool RTCC_MCP7940N::IsLeapYear()
{
	return this->GetFlag(0x05, B100000);
}

void RTCC_MCP7940N::ClearPowerFailedFlag()
{
	this->SetBits(3, B00010000, 0);
}

void RTCC_MCP7940N::SetOscillatorEnabled(bool enabled)
{
	this->SetBits(0, B10000000, enabled << 7);
}

void RTCC_MCP7940N::SetBatteryEnabled(bool enabled)
{
	this->SetBits(3, B00001000, enabled << 3);
}

void RTCC_MCP7940N::Set24HourTimeEnabled(bool enabled)
{
	this->SetBits(2, B10000000, (!enabled) << 7);
}

void RTCC_MCP7940N::SetSquareWaveOutputState(bool enabled, RtccSquareWaveFrequency frequency)
{
	this->SetBits(0x07, B1000000, enabled << 6);

	if( enabled )
	{
		this->SetBits(0x07, B11, (byte)frequency & B11);
	}
}

signed char RTCC_MCP7940N::GetTrimValue()
{
	signed char val = this->GetByte(0x08);
	val &= B01111111;

	if( !(val & B1000000))
	{
		val = -val;
	}
	return val;
}

void RTCC_MCP7940N::SetTrimValue(signed char value)
{
	this->SetBits(0x08, B1111111, value);

	if( value < 0 )
	{
		this->SetBits(0x08, B10000000, 0);
	}
	else
	{
		this->SetBits(0x08, B10000000, B10000000);
	}
}

void RTCC_MCP7940N::SetAlarm1(const rtcc_time *time)
{
	this->SetAlarm(0x0A, time);
}

void RTCC_MCP7940N::SetAlarm2(const rtcc_time *time)
{
	this->SetAlarm(0x11, time);
}

void RTCC_MCP7940N::SetAlarm(int address, const rtcc_time *time)
{
	// Set read address to 0
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(address);
	Wire.endTransmission();

	// Request 7 byte Time and Date registers
	Wire.requestFrom(this->i2c_addr, 6, true);

	byte time_data[6];
	Wire.readBytes(time_data, 6);

	// Save flags but clear all date/time data
	time_data[0] = time_data[0] & B10000000;
	time_data[1] = time_data[1] & B10000000;
	time_data[2] = time_data[2] & B10000000;
	time_data[3] = time_data[3] & B11111000;
	time_data[4] = time_data[4] & B11000000;
	time_data[5] = time_data[5] & B11100000;

	// Seconds
	time_data[0] |= ((time->secten) << 4) & B1110000;
	time_data[0] |= time->secone & B1111;

	// Minutes
	time_data[1] |= ((time->minten) << 4) & B1110000;
	time_data[1] |= time->minone & B1111;

	// Hours
	if( time->mode_24 )
	{
		time_data[2] |= ((time->hrten) << 4) & B110000;
		time_data[2] |= time->hrone & B1111;
	}
	else
	{
		time_data[2] |= time->pm << 5;
		time_data[2] |= ((time->hrten) << 4) & B10000;
		time_data[2] |= time->hrone & B1111;
	}

	// Week day
	time_data[3] |= time->wkday & B111;

	// Date
	time_data[4] |= ((time->dateten) << 4) & B110000;
	time_data[4] |= time->dateone & B1111;

	// Month
	time_data[5] |= ((time->mthten) << 4) & B10000;
	time_data[5] |= time->mthone & B1111;

	// Write new data into chip
	Wire.beginTransmission(this->i2c_addr);
	Wire.write(address);
	for( int a = 0; a < 6; a++ )
	{
		Wire.write(time_data[a]);
	}
	Wire.endTransmission();
}

void RTCC_MCP7940N::SetAlarmPolarity(bool high)
{
	this->SetBits(0x0D, B10000000, high << 7);
}

bool RTCC_MCP7940N::GetAlarmPolarity()
{
	return this->GetFlag(0x0D, B10000000);
}

bool RTCC_MCP7940N::GetAlarm1Flag()
{
	return this->GetFlag(0x0D, B1000);
}

bool RTCC_MCP7940N::GetAlarm2Flag()
{
	return this->GetFlag(0x14, B1000);
}

void RTCC_MCP7940N::ClearAlarm1Flag()
{
	this->SetBits(0x0D, B1000, 0);
}

void RTCC_MCP7940N::ClearAlarm2Flag()
{
	this->SetBits(0x14, B1000, 0);
}

void RTCC_MCP7940N::SetAlarm1Mask(RtccAlarmMask mask)
{
	this->SetBits(0x0D, B1110000, (byte)mask << 4);
}

RtccAlarmMask RTCC_MCP7940N::GetAlarm1Mask()
{
	return (RtccAlarmMask)((this->GetByte(0x0D) & B1110000) >> 4);
}

void RTCC_MCP7940N::SetAlarm2Mask(RtccAlarmMask mask)
{
	this->SetBits(0x14, B1110000, (byte)mask << 4);
}

RtccAlarmMask RTCC_MCP7940N::GetAlarm2Mask()
{
	return (RtccAlarmMask)((this->GetByte(0x14) & B1110000) >> 4);
}

void RTCC_MCP7940N::ConvertTime(const tm *from, rtcc_time *to)
{
	// Seconds
	to->secten = (from->tm_sec / 10);
	to->secone = (from->tm_sec % 10);

	// Minutes
	to->minten = (from->tm_min / 10);
	to->minone = (from->tm_min % 10);

	// Hours (handle 24 hour conversion)
	if( to->mode_24 )
	{
		to->hrten = (from->tm_hour / 10);
		to->hrone = (from->tm_hour % 10);
		to->pm = false;
	}
	else
	{
		int hr = from->tm_hour;

		if( from->tm_hour >= 12 )
		{
			to->pm = true;
			hr -= 12;
		}
		else
		{
			to->pm = false;
		}

		to->hrten = (hr / 10);
		to->hrone = (hr % 10);
	}

	// Weekday
	to->wkday = (from->tm_wday);

	// Date
	to->dateten = (from->tm_mday / 10);
	to->dateone = (from->tm_mday % 10);

	// Month - Stored 0-index in tm but 1-index in rtcc
	to->mthten = ((from->tm_mon + 1) / 10);
	to->mthone = ((from->tm_mon + 1) % 10);

	// Year - Stored years since 1900 in tm but years since 2000 in rtcc
	to->yrten = ((from->tm_year - 100) / 10);
	to->yrone = ((from->tm_year - 100) % 10);
}

void RTCC_MCP7940N::ConvertTime(const rtcc_time *from, tm *to)
{
	// Seconds
	to->tm_sec = (from->secten * 10) + from->secone;

	// Minutes
	to->tm_min = (from->minten * 10) + from->minone;

	// Hours - Handle 24 hour conversion
	if( from->mode_24 )
	{
		to->tm_hour = (from->hrten * 10) + from->hrone;
	}
	else
	{
		to->tm_hour = (from->hrten * 10) + from->hrone;
		to->tm_hour += from->pm ? 12 : 0;
	}

	// Weekday
	to->tm_wday = (from->wkday);

	// Date
	to->tm_mday = (from->dateten * 10) + from->dateone;

	// Month - Stored 0-index in tm but 1-index in rtcc
	to->tm_mon = ((from->mthten * 10) + from->mthone) - 1;

	// Year - Stored years since 1900 in tm but years since 2000 in rtcc
	to->tm_year = ((from->yrten * 10) + from->yrten) + 100;
}

bool RTCC_MCP7940N::Get24HourTimeEnabled()
{
	return this->GetFlag(0x02, B1000000);
}

bool RTCC_MCP7940N::GetSquareWaveOutputEnabled()
{
	return this->GetFlag(0x07, B1000000);
}


