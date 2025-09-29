#include <CoreInk.h>
#include <WiFi.h>
#include "time.h"

const char* ssid             = "chenHouse2.0";
const char* password         = "j5t2jj2p4zbr";
const char*    ntpServer      = "pool.ntp.org";
const uint16_t utcOffset      = 28800;
const uint8_t  daylightOffset = 0;

#define FULL_REFRESH_MINUTE (45)

void printLocalTimeAndSetRTC()
{
    struct tm timeinfo;

    if(getLocalTime(&timeinfo) == false)
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    RTC_Time_t time;
    time.Hours   = timeinfo.tm_hour;
    time.Minutes = timeinfo.tm_min;
    time.Seconds = timeinfo.tm_sec;
    M5.rtc.SetTime(&time);

    RTC_Date_t date;
    date.Day  = timeinfo.tm_mday;
    date.Month = timeinfo.tm_mon + 1;
    date.Year  = timeinfo.tm_year + 1900;
    M5.rtc.SetDate(&date);
}

void getNTPTime()
{
    uint32_t connect_timeout = millis() + 10000;

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while((WiFi.status() != WL_CONNECTED) && (millis() < connect_timeout))
    {
        delay(500);
        Serial.print(".");
    }
    if(WiFi.status() != WL_CONNECTED)
    {
        RTC_Time_t time;
        time.Hours = 6;
        time.Minutes = 43;
        time.Seconds = 50;
        M5.rtc.SetTime(&time);

        RTC_Date_t date;
        date.Day = 23;
        date.Month = 6;
        date.Year = 2024;
        M5.rtc.SetDate(&date);
        return;
    }

    Serial.println("Connected");

    configTime(utcOffset, daylightOffset, ntpServer);
    printLocalTimeAndSetRTC();

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void drawTimeAndDate()
{
    char buf[11];

    snprintf(buf, 6, "%02d:%02d", M5.rtc.Hour, M5.rtc.Minute);
    M5.Ink.setCursor(4, 60);
    M5.Ink.print(buf);
    snprintf(buf, 11, "%02d.%02d.%04d", M5.rtc.Day, M5.rtc.Month, M5.rtc.Year);
    M5.Ink.setCursor(4, 80);
    M5.Ink.print(buf);
}

void setup(){
    M5.begin();

    M5.Ink.setCursor(0,20); 
    if(M5.rtc.isWakeup()){                                 // 由RTC喚醒時
        M5.Ink.clear();
        M5.rtc.update();
        M5.Ink.firstPage();
        do{
          M5.Ink.print("Wake Up!");
        } while(M5.Ink.nextPage());
    }else{                                                    // 由開機鍵喚醒時    
        M5.Ink.clear();
        M5.Ink.firstPage();
        do{
          M5.Ink.print("Powered!");
        } while(M5.Ink.nextPage());
        getNTPTime();
        M5.rtc.update();
    }
    drawTimeAndDate();
    M5.Ink.update();
    M5.shutdown(58);
}

void loop()
{

}
