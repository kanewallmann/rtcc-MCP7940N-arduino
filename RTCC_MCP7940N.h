/* Part of RTCC (MCP7940N) Arduino library
 * Copyright (C) 2017 Kane Wallmann
 * See LICENCE.txt for license (MIT)*/

#ifndef __RTCC_MCP7940N_H
#define __RTCC_MCP7940N_H

#include <Arduino.h>
#include <Wire.h>
#include <time.h>

typedef enum
{
	RTCC_SQWFS_1HZ,
	RTCC_SQWFS_4KHZ,
	RTCC_SQWFS_8KHZ,
	RTCC_SQWFS_32KHZ
}
RtccSquareWaveFrequency;

typedef enum
{
	RTCC_ALARM_MASK_SECONDS,
	RTCC_ALARM_MASK_MINUTES,
	RTCC_ALARM_MASK_HOURS,
	RTCC_ALARM_MASK_WDAY,
	RTCC_ALARM_MASK_DATE,
	RTCC_ALARM_MASK_UNUSED1,
	RTCC_ALARM_MASK_UNUSED2,
	RTCC_ALARM_MASK_ALL
}
RtccAlarmMask;

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

	/**
	 * Reads the time currently stored on the RTCC chip
	 *
	 * @param time 		Memory location to store the time
	 */
	void ReadTime(rtcc_time *time);

	/**
	 * Writes the supplied time to the RTCC chip
	 *
	 * The clock is briefly suspended during a write to ensure an atomic write operation
	 *
	 * @param time 		Pointer to time to write to chip
	 */
	void SetTime(const rtcc_time *time);

	/**
	 * Global enable of the clock oscillator circuit
	 *
	 * @param enabled 	True to enable, false to disable
	 */
	void SetOscillatorEnabled(bool enabled);

	/**
	 * Set alarm 1
	 *
	 * @param time 		The time to set alarm 1 to
	 */
	void SetAlarm1(const rtcc_time *time);

	/**
	 * Set alarm 2
	 *
	 * @param time 		The time to set alarm 2 to
	 */
	void SetAlarm2(const rtcc_time *time);

	/**
	 * Set the RTCC chip to use 24 time or 12 hour time, in 12 hour time rtcc_time.pm is set to true for pm
	 *
	 * @param enabled	True for 24 hour time, false for 12 hour time
	 */
	void Set24HourTimeEnabled(bool enabled);

	/**
	 * Check what time mode the RTCC chip is in
	 *
	 * @return 			True for 24 hour time, false for 12 hour time
	 */
	bool Get24HourTimeEnabled();

	/**
	 * Enable the square wave generator, square wave is output on MFP pin
	 *
	 * This overrides alarm settings, must be turned off to use alarms
	 *
	 * @param enabled 		True to enable generator
	 * @param frequency 	Square wave frequency
	 */
	void SetSquareWaveOutputState(bool enabled, RtccSquareWaveFrequency frequency = RTCC_SQWFS_1HZ );

	/**
	 * Check state of square wave generator
	 *
	 * @return 				True of generator is enabled
	 */
	bool GetSquareWaveOutputEnabled();

	/**
	 * Get the state of the oscillator
	 *
	 * If this returns false, the clock is not running and there could be an issue with crystal or clock input
	 *
	 * @return 				Returns true of the oscillator is running correctly
	 */
	bool IsOscillatorRunning();

	/**
	 * Check the power failure flag
	 *
	 * Only has meaning if you are using external battery, you must clear this flag with software if you want to detect
	 * a second power failure event
	 *
	 * @return 				True if a power fail event has been detected
	 */
	bool HasPowerFailed();

	/**
	 * Clear the power failure flag so it can be raised again if another failure is detected
	 */
	void ClearPowerFailedFlag();

	/**
	 * Enable or disable the low power battery-backed mode
	 *
	 * @param enabled 		True to enable
	 */
	void SetBatteryEnabled(bool enabled);

	/**
	 * Check if the battery-backed mode is enabled
	 *
	 * @return 				True if enabled
	 */
	bool GetBatteryEnabled();

	/**
	 * Returns whether the current time stored on the RTCC chip is a leap year
	 *
	 * @return 				True for leap year
	 */
	bool IsLeapYear();

	/**
	 * Enable or disable alarm 1
	 *
	 * @param enabled 		True to enable
	 */
	void SetAlarm1Enabled(bool enabled);

	/**
	 * Returns enabled state of alarm 1
	 *
	 * @return 				True for enabled
	 */
	bool GetAlarm1Enabled();

	/**
	 * Set the polarity on the alarm output on MFP
	 *
	 * @param high 			True results in a HIGH when the alarm is active, false for LOW
	 */
	void SetAlarmPolarity(bool high);

	/**
	 * Get the alarm polarity
	 *
	 * @return 				True for HIGH, false for LOW
	 */
	bool GetAlarmPolarity();

	/**
	 * Get state of alarm 1 interrupt flag
	 *
	 * Must be cleared to enable future interrupts
	 *
	 * @return 				True for asserted
	 */
	bool GetAlarm1Flag();

	/**
	 * Clear the alarm 1 interrupt flag
	 */
	void ClearAlarm1Flag();

	/**
	 * Set the alarm 1 condition flag
	 *
	 * @param mask 		The mask to use for alarm 1
	 */
	void SetAlarm1Mask(RtccAlarmMask mask);

	/**
	 * Get the alarm 1 condition flag
	 *
	 * @return 			The currently set alarm 1 mask
	 */
	RtccAlarmMask GetAlarm1Mask();

	/**
	 * Enable or disable alarm 2
	 *
	 * @param enabled 		True to enable
	 */
	void SetAlarm2Enabled(bool enabled);

	/**
	 * Get the enabled state of alarm 2
	 *
	 * @return 				True for enabled
	 */
	bool GetAlarm2Enabled();

	/**
	 * Get state of alarm 2 interrupt flag
	 *
	 * Must be cleared to enable future interrupts
	 *
	 * @return 				True for asserted
	 */
	bool GetAlarm2Flag();

	/**
	 * Clear the alarm 2 interrupt flag
	 */
	void ClearAlarm2Flag();

	/**
	 * Get the alarm 1 condition flag
	 *
	 * @return 			The mask to use for alarm 1
	 */
	void SetAlarm2Mask(RtccAlarmMask mask);

	/**
	 * Get the alarm 2 condition flag
	 *
	 * @return 			The currently set alarm 2 mask
	 */
	RtccAlarmMask GetAlarm2Mask();

	/**
	 * Set the state of the external oscillator
	 *
	 * @param enabled 		True to use external clock on X1, false for crystal across X1-X2
	 */
	void SetExternalOscillatorEnabled(bool enabled);

	/**
	 * Get the state of the external oscillator
	 *
	 * @return 				True means using external clock, false for crystal
	 */
	bool GetExternalOscillatorEnabled();

	/**
	 * Enable or disable course trim mode
	 *
	 * @param enabled 		True for course trim enabled
	 */
	void SetCourseTrimEnabled(bool enabled);

	/**
	 * Get course trim enabled state
	 *
	 * @return 				True for course trim enabled
	 */
	bool GetCourseTrimEnabled();

	/**
	 * Get current general purpose output state
	 *
	 * @return 				True for HIGH, false for LOW
	 */
	bool GetGPOState();

	/**
	 * Set general purpose output state
	 *
	 * Alarms and square wave generator must be disabled to use MFP as a GPO
	 *
	 * @param state 		True for HIGH, false for LOW
	 */
	void SetGPOState(bool state);

	/**
	 * Get the current oscillator trim value
	 *
	 * @return 				The value of the oscillator trim
	 */
	signed char GetTrimValue();

	/**
	 * Set the oscillator trim value
	 *
	 * See the datasheet for information on this value, allows you to fine tune oscillator for more precise timing
	 *
	 * @param value 		The oscillator trim value
	 */
	void SetTrimValue(signed char value);

	/**
	 * Convert time between different formats
	 *
	 * @param from 		A tm struct from time.h to convert from
	 * @param to 		An rtcc_time struct to convert to
	 */
	static void ConvertTime(const tm *from, rtcc_time *to);

	/**
	 * Convert time between different formats
	 *
	 * @param from 		An rtcc_time struct to convert from
	 * @param to 		A tm struct from time.h to convert to
	 */
	static void ConvertTime(const rtcc_time *from, tm *to);

private:

	void SetBits(int address, int mask, int values);

	byte GetByte(int address);

	bool GetFlag(int address, int mask);

	void SetAlarm(int address, const rtcc_time *timestamp);

	int i2c_addr;
};

#endif //__RTCC_MCP7940N_H
