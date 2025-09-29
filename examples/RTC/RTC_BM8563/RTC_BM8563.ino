#include <CoreInk.h>

char timeStrbuff[64];
char* WDStr[7] = {"(週日)","(週一)","(週二)","(週三)","(週四)","(週五)","(週六)"};

void updateTime(){
  M5.Ink.setPartialWindow(0, 0, M5.Ink.width(), 22);
  M5.rtc.update();
  sprintf(timeStrbuff, 
            "%02d:%02d", 
            M5.rtc.Hour, 
            M5.rtc.Minute);
  M5.Ink.setPartialWindow(165, 0, 50, 20);
  M5.Ink.firstPage();
  do{
    M5.Ink.cSimPrint(timeStrbuff, 165, 5, 0);
  }while (M5.Ink.nextPage());
  //M5.Ink.update();
  M5.Ink.powerOff();
}

void flushTime() {
    M5.Ink.clear();
    M5.rtc.update();
    sprintf(timeStrbuff, 
            "%4d年%02d月%02d日%s", 
            M5.rtc.Year,
            M5.rtc.Month, 
            M5.rtc.Day, 
            WDStr[M5.rtc.Weekday]);
    M5.Ink.cSimPrint(timeStrbuff, 5, 5, 0);
    M5.Ink.drawLine(3,  20,  197,  20,  0);

    M5.Ink.update();
    M5.Ink.powerOff();
}

void setup() {
    M5.begin(); 
    
    M5.Ink.InitClib(GxEPD_BLACK, GxEPD_WHITE);
    M5.Ink.clear();
    delay(1000);
    flushTime();
}

void loop() {
    updateTime();
    delay(60000);
    //M5.shutdown(60);
}
