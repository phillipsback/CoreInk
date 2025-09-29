#include "CoreInk.h"
#include <Fonts/TomThumb.h>

void setup() {
  M5.begin();
  M5.Ink.setFont(&TomThumb);
  M5.Ink.InitClib(0,1);
  M5.Ink.drawLine(20,20,50,50,0);
  M5.Ink.setTextColor(GxEPD_BLACK);
  M5.Ink.setCursor(3, 10);
  M5.Ink.print("test lines");
  M5.Ink.cSimPrint("陳進昇", 40, 40);
  M5.Ink.fillArc(/*x*/100, /*y*/100, /*st-D*/0, /*ed-D*/45, /*R*/50, /*w*/4, /*color*/0);
  M5.Ink.update();
}

void loop() {

}
