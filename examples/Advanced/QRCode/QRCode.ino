#include <M5CoreInk.h>

void setup() {
    M5.begin();
    M5.Ink.qrcode("http://www.m5stack.com", 0, 0, 120); 
    M5.Ink.update();
}

void loop() {
}
