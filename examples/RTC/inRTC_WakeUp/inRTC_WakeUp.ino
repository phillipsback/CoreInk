#include <CoreInk.h>

#define WAKEUP_MS 60*1000000   //60秒

void ShowClock(){
    char TempStr[64];
    M5.Ink.clear();
    M5.rtc.update();

    sprintf(TempStr, "現在時間:%02d時%02d分", M5.rtc.Hour, M5.rtc.Minute);
    M5.Ink.cSimPrint(TempStr, 10, 30);
    M5.Ink.update();
}

void setup() {
    M5.begin();
    M5.Ink.InitClib(GxEPD_BLACK, GxEPD_WHITE);
    esp_sleep_enable_timer_wakeup(WAKEUP_MS);
    ShowClock();
}

void loop() {
    esp_deep_sleep_start();
}



//RTC_DATA_ATTR int wake = 0;
//RTC_Date dt;
//int rtcIrq = 0;



//void setup()
//{
//    TTV.begin();
//    delay(300);
    
//    attachInterrupt(RTC_INT_PIN, [] {
//        rtcIrq = 1;
//    }, FALLING);
//
//    rtc.disableAlarm();
//    rtc.setAlarmByMinutes(0);
//    rtc.enableAlarm();
//}

//void loop()
//{
//  ShowClock();
//  delay(28000);
////  esp_sleep_enable_ext1_wakeup(GPIO_SEL_26, ESP_EXT1_WAKEUP_ANY_HIGH);
//  esp_deep_sleep_start();
//}
