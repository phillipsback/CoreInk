#include <CoreInk.h>

int num=10;
bool isRun=true;

void setup() {
  M5.begin();
  M5.Ink.print("Hello World!");
  M5.Ink.update();
//  M5.Ink.powerOff();
}

void loop() {
  if(num){
    M5.Speaker.tone(10000, 100);
    delay(100);
    M5.Speaker.mute();
    delay(1000);
    
    M5.Speaker.tone(4000, 20);
    delay(100);
    M5.Speaker.mute();
    delay(1000);

    num--;
  }else{
    if(isRun){
      M5.Ink.clear();
      M5.Ink.setCursor(0,10);
      M5.Ink.print("Finish!");
      M5.Ink.display(true);
      isRun=false;
    }
  }
  delay(500);
}
