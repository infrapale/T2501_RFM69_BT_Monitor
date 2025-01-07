// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "main.h"
#include "RTClib.h"
#include "rtc_time.h"
#include "atask.h"


RTC_PCF8523 rtc;

typedef struct 
{
  bool rtc_is_ok;
  DateTime now;
} rtc_time_st;

DateTime now = DateTime(F(__DATE__), F(__TIME__));
rtc_time_st rtc_time { false, now };

  //.now = DateTime(F(__DATE__), F(__TIME__));


atask_st rtc_handle                 = {"RTC TIme       ", 1000,0, 0, 255, 0, 1, rtc_time_task};

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void rtc_time_initialize(void)
{
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC"); Serial.flush();
    //while (1) delay(10);
  }
  else {
    rtc_time.rtc_is_ok = true;
    rtc_time.now = rtc.now();
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  atask_add_new(&rtc_handle);
  rtc_time_print_time();

}

void rtc_time_task(void)
{
  if (rtc_time.rtc_is_ok)
  {
    rtc_time.now = rtc.now();
  }
}

uint16_t rtc_time_get_year(void) { return rtc_time.now.year();}
uint8_t rtc_time_get_month(void) { return rtc_time.now.month();}
uint8_t rtc_time_get_day(void) { return rtc_time.now.day();}
uint8_t rtc_time_get_hour(void) { return rtc_time.now.hour();}
uint8_t rtc_time_get_minute(void) { return rtc_time.now.minute();}
uint8_t rtc_time_get_second(void) { return rtc_time.now.second();}

void rtc_time_print_time(void)
{
  Serial.print(rtc_time.now.year(), DEC);
    Serial.print('/');
    Serial.print(rtc_time.now.month(), DEC);
    Serial.print('/');
    Serial.print(rtc_time.now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(rtc_time.now.hour(), DEC);
    Serial.print(':');
    Serial.print(rtc_time.now.minute(), DEC);
    Serial.print(':');
    Serial.print(rtc_time.now.second(), DEC);
    Serial.println();

}
