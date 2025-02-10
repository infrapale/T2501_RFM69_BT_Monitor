#ifndef __RTC_TIME_H__
#define __RTC_TIME_H__


typedef enum
{
    RTC_YEAR = 0,
    RTC_MONTH,
    RTC_DAY,
    RTC_HOUR,
    RTC_MINUTE,
    RTC_SECOND,
    RTC_NBR_OF
} rtc_value_et;


void rtc_time_initialize(void);
void rtc_time_task(void);
uint16_t rtc_time_get_year(void);
uint8_t rtc_time_get_month(void);
uint8_t rtc_time_get_day(void);
uint8_t rtc_time_get_hour(void);
uint8_t rtc_time_get_minute(void);
uint8_t rtc_time_get_second(void);
void rtc_time_print_time(void);

void rtc_time_get_stamp(char *stamp_str);

void rtc_time_get_date_str(char *date_str);




#endif