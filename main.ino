#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DS3231.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 16
#define NUM_ZONES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
DS3231 myRTC;

void setup() {
  Serial.begin(57600);
  P.begin(NUM_ZONES);

  // set the zone boundaries
  P.setZone(0, 0, 3);
  P.setZone(1, 4, 7);
  P.setZone(2, 8, 11);
  P.setZone(3, 12, 15);

  P.setIntensity(0);
}

char* getDow() {
    const char* weekdays[] = {"", "Monday", "Tueday", "Wedday", "Thuday", "Friday", "Satday", "Sunday"};
    byte theWeekday = myRTC.getDoW();

    return weekdays[theWeekday];
}

char* getDate() {
    bool CenturyBit;
    byte date = myRTC.getDate();
    byte month = myRTC.getMonth(CenturyBit);

    static char dateString[6];
    sprintf(dateString, "%02d/%02d", date, month);
    return dateString;
}


bool isDST(int day, int month, byte weekday) {
    // DST in Israel starts on the last Friday of March
    // and ends on the last Sunday of October

    if (month < 3 || month > 10) return false;
    if (month > 3 && month < 10) return true;

    if (month == 3) return (day + (5 - weekday) >= 31);
    if (month == 10) return (day + (7 - weekday) < 31);

    return false;
}

char* getTime() {
    bool h12;
    bool hPM;
    bool CenturyBit;

    byte hour = myRTC.getHour(h12, hPM);
    byte min = myRTC.getMinute();
    byte day = myRTC.getDate();
    byte month = myRTC.getMonth(CenturyBit);
    byte weekday = myRTC.getDoW();


    // Check DST and adjust hour if needed
    if (!isDST(day, month, weekday)) {
        hour = (hour - 1) % 24;
    } 
    
    static char timeString[7];
    sprintf(timeString, "%02d:%02d", hour, min);

    return timeString;
}

char* getTemp() {
    float rtcTemp = myRTC.getTemperature();

    static char tempString[6]; // Enough space to hold the temperature as a string
    // Convert float to string
    dtostrf(rtcTemp, 2, 1, tempString); // 2 is minimum width, 1 is precision
    sprintf(tempString, "%sc", tempString);

    return tempString;
}

void loop() {
  char* time = getTime();
  char* temp = getTemp();
  char* date = getDate();
  char* dow = getDow();

  if (P.displayAnimate())
    P.displayZoneText(3, time, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    P.displayZoneText(2, dow, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    P.displayZoneText(1, date, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    P.displayZoneText(0, temp, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);

  delay(1000);
}
