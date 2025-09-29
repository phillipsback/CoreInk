#include "CoreInk.h"

void setup() {
    M5.begin();
    M5.Ink.clear();
    delay(1000);
    
    M5.Ink.InitClib(0,1);
    M5.Ink.clear(); 
    //M5.Ink.SetFx(FX_HOLLOW);
    M5.Ink.cSimPrint("陳進昇", 2, 10);
    M5.Ink.update();
}

void loop() {

}
