# GPS Speedometer and Data Logger #

Christopher F. Moran, May 2020

This is a very simple sketch to read NMEA strings from a GPS module and display the current ground speed.  In addition, it logs position information to an SD Card.

## Libraries Required ##

* tinygps
* LedControl

## Hardware Required

* Arduino UNO or Nano.  It should work on the later high-powered versions of these boards, but I haven't tested it
* Serial GPS Receiver Module.  All the generic modules I have tried worked just fine.
* 8-digit LED display module.  Needs to be compatible with the LEDControl Library.  Serial interface.
