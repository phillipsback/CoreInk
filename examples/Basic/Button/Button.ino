#include <CoreInk.h>

void ButtonTest(char* str) {
    M5.Ink.clear();
    M5.Ink.setCursor(15, 59);
    M5.Ink.print(str);
    M5.Ink.updateWindow(0, 35, GxEPD_WIDTH, 40, false);
    M5.Ink.powerDown();
    delay(2000);
}

void setup() {
    M5.begin();
    M5.Ink.clear();
    delay(1000);
    M5.Ink.setCursor(30, 20);
    M5.Ink.print("Hello Core-INK");
    M5.Ink.drawLine(5,32,195,32,0);
    M5.Ink.update();
    M5.Ink.powerDown();
}
void loop() {
    if (M5.BtnUP.wasPressed()){
        ButtonTest("Btn UP Pressed");
        M5.Led.on();
    }
    if (M5.BtnDOWN.wasPressed()){
        ButtonTest("Btn DOWN Pressed");
        M5.Led.off();
    }
    if (M5.BtnMID.wasPressed())
        ButtonTest("Btn MID Pressed");
    if (M5.BtnEXT.wasPressed())
        ButtonTest("Btn EXT Pressed"); 
    if (M5.BtnPWR.wasPressed()) { 
        ButtonTest("Btn PWR Pressed");
        M5.shutdown();
    }
    M5.update(); 
}
