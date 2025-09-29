#include <CoreInk.h>

void setup() {
    M5.begin();                      // Initialize CoreInk. 初始化 CoreInk

    M5.Ink.clear();
    delay(1000);
    M5.Ink.setCursor(10, 50);
    M5.Ink.print("Press PWR Btn for sleep");
    M5.Ink.setCursor(15, 80);
    M5.Ink.print("after 5 sec wakeup.");
    M5.Ink.update();
}

void loop() {
    if (M5.BtnPWR.wasPressed()) {
        Serial.printf("Btn %d was pressed \r\n", BUTTON_PWR_PIN);
        M5.Ink.clear();
        M5.Ink.update();
        delay(1000);
        //M5.shutdown(10);                  // 關閉電源，設定10秒計時器喚醒開機
        M5.shutdown(22,45);                 // 關閉電源，設定22時45分時定時喚醒開機
    }
    M5.update();  // Refresh device button. 刷新设备按键
    if (M5.BtnUP.wasPressed()) {
       Serial.printf("Btn %d was pressed \r\n", BUTTON_UP_PIN);
    }
}
