#include <CoreInk.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void setup() {
    M5.begin();                      // Initialize CoreInk. 初始化 CoreInk

    u8g2Fonts.begin(M5.Ink);
    M5.Ink.setTextColor(GxEPD_BLACK);
    u8g2Fonts.setFontMode(1);                           // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(1);                      // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);          // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);          // apply Adafruit GFX color

}

uint32_t last = 0;
void loop() {
    if (millis() - last > 1000) {
        showtime();
        last = millis();
    }
}

void print02d(uint32_t d)
{
    if (d < 10) M5.Ink.print("0");
    M5.Ink.print(d);
}

void showtime()
{
    M5.rtc.update();

    M5.Ink.setRotation(1);
    M5.Ink.setFont(&FreeMonoBold9pt7b);
    M5.Ink.fillScreen(GxEPD_WHITE);
    uint16_t box_x = 0;
    uint16_t box_y = 0;
    uint16_t box_w = 170;
    uint16_t box_h = 40;
    uint16_t cursor_y = box_y + 16;
    M5.Ink.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    M5.Ink.setCursor(box_x, cursor_y);
    M5.Ink.print(M5.rtc.Year); 
    M5.Ink.print("/"); 
    M5.Ink.print(M5.rtc.Month); 
    M5.Ink.print("/"); 
    M5.Ink.print(M5.rtc.Day);
    M5.Ink.setCursor(box_x, cursor_y + 16);
    print02d(M5.rtc.Hour); 
    M5.Ink.print(":"); 
    print02d(M5.rtc.Minute); 
    M5.Ink.print(":"); 
    print02d(M5.rtc.Second);
    M5.Ink.displayWindow(box_x, box_y, box_w, box_h);
}
