RTCC (MCP7940N)
===============

![travis](https://travis-ci.org/kanewallmann/rtcc-MCP7940N-arduino.svg?branch=master)

An Arduino library to control a Microchip MCP7940N or compatible Real Time Clock and Calendar.

Datasheet available here: http://ww1.microchip.com/downloads/en/DeviceDoc/20005010F.pdf

Wiring
==========

|MCP7940N|Arduino|
|:------:|:-----:|
|SDA     |SDA    |
|SCL     |SCL    |

Timing can be controlled in one of two ways. First is to connect a 32.768 kHz crystal between pins X1 and X2.
Or you can provide a 32.768 kHz clock signal to X1. Use SetExternalOscillatorEnabled() to switch between crystal and external clock.

MFP can be used as an interrupt for alarms (See examples/alarm.ino for details), as a square wave generator, or as
a general purpose output.

V<sub>BAT</sub> can be connected to a battery to retain time if the main power goes down. Enable this with SetBatteryEnabled( true ). If not being used,
it should be connected to GND.

Important Notes
============

This library uses the Wire library from Arduino. You must call Wire.begin() before attempting to use this library. You
may also select the i2c bus speed by calling Wire.setClock()

Usage
=======

View the examples folder for examples on usage.

License
========

See LICENCE.txt for license (MIT)
