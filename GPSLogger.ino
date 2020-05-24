/*
 * GPS Logging Device
 * Christopher F. Moran, May 2020
 */

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>
#include <LedControl.h>

#define DIN   7
#define CLK   6
#define CS    5

#define USER_NAME "Chris"

TinyGPS gps;
SoftwareSerial ss(8, 9);
File logFile;
LedControl lc = LedControl(DIN, CLK, CS, 1);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up");
  ss.begin(9600);
  Serial.print("Initializing SD card...");

  lc.shutdown(0,false);
  lc.setIntensity(0,7);
  lc.clearDisplay(0);

  // Start the memory card.  Warn user if there's a problem
  if (!SD.begin(4)) {
    lc.setDigit(0, 7, 0x0d, false);
    lc.setDigit(0, 6, 0x01, false);
    lc.setDigit(0, 5, 0x05, false);
    lc.setDigit(0, 4, 0x0c, false);
    lc.setDigit(0, 2, 0x0b, false);
    lc.setDigit(0, 1, 0x0a, false);
    lc.setDigit(0, 0, 0x0d, false);
    while (1);
  } 
  Serial.println("initialization done.");
  Serial.println("Ready");

  displaySpeed(0);
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float flat;
  float flon;
  float altit;
  unsigned long age;
  int YY;
  byte MM;
  byte DD;
  byte hrs;
  byte mins;
  byte secs;
  byte hunds;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    
    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&YY, &MM, &DD, &hrs, &mins, &secs, &hunds, &age);
    altit = gps.altitude();
#ifdef _DEBUG    
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.println();
#endif
    logFile = SD.open("test.csv", FILE_WRITE);
    if(logFile) {
      logFile.println(String(USER_NAME) + "," + String(YY) + "," + String(MM) + "," + String(DD) + "," + String(hrs) + "," + String(mins) + "," + String(secs) + "," + String(flat,6) + "," + String(flon,6) + "," + String(altit / 100, 1) + "," + String(gps.f_speed_kmph(), 1) + "," + String(gps.f_course(), 1));
    }
    logFile.close();
    displaySpeed(gps.f_speed_kmph());
    displaySats(gps.satellites());
  }
  
  gps.stats(&chars, &sentences, &failed);
  if (chars == 0) {
    Serial.println("** No characters received from GPS: check wiring **");
    lc.setDigit(0, 7, 0x0E, true);
    lc.setDigit(0, 6, 0x00, true);
    lc.setDigit(0, 5, 0x01, true);
  }
}

void displaySpeed(float spd) {
  int ispd = int(spd);
  
  lc.clearDisplay(0);

  // Split the incoming speed into digits
  byte digit2 = byte((ispd / 100) % 10);
  byte digit1 = byte((ispd / 10) % 10);
  byte digit0 = byte((ispd % 10));

  // Display the data and also implement Leading-Zero Blanking (LZB)
  lc.setDigit(0, 0, digit0, false);
  if(spd > 9.99)
    lc.setDigit(0, 1, digit1, false);
  if(spd > 99.99)
    lc.setDigit(0, 2, digit2, false);
}

void displaySats(int sats) {
  lc.setDigit(0, 7, sats, false);
}
