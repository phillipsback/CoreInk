#include <CoreInk.h>
#include <driver/rtc_io.h>

#define WAKEUP_MS 60*1000000   //60秒

int rtcIrq = 0;



void ShowClock(){
    char TempStr[64];
    M5.Ink.clear();
    M5.rtc.update();

    sprintf(TempStr, "現在時間:%02d時%02d分", M5.rtc.hour, M5.rtc.min);
    M5.Ink.cSimPrint(TempStr, 10, 30);
    M5.Ink.update();
}

void setup() {
    M5.begin();
    M5.Ink.InitClib(GxEPD_BLACK, GxEPD_WHITE);
    RTC_STATUS_T Temp;
    
    Temp.Value = 0x03;
    Serial.println(Temp.timer_enable);
//    attachInterrupt(19,
//                    [] { rtcIrq = 1; }, 
//                    FALLING);
//    ShowClock();
}

void loop() {
//    M5.Ink.powerOff();
//    M5.rtc.clearIRQ();
//    M5.rtc.SetAlarmIRQ(RTC_TimeTypeDef(18,20,0));
//    pinMode(1, OUTPUT);
//    digitalWrite(1, LOW);
//    rtc_gpio_init(GPIO_NUM_12);
//    rtc_gpio_set_direction(GPIO_NUM_12, RTC_GPIO_MODE_OUTPUT_ONLY);
//    rtc_gpio_set_level(GPIO_NUM_12, 1);
//    //digitalWrite(POWER_HOLD_PIN, LOW);
//    
//    esp_sleep_enable_ext0_wakeup(GPIO_NUM_19, 1);
//    esp_deep_sleep_start();
}



//RTC_DATA_ATTR int wake = 0;
//RTC_Date dt;



//void loop()
//{
//  ShowClock();
//  delay(28000);
////  
//  esp_deep_sleep_start();
//}
