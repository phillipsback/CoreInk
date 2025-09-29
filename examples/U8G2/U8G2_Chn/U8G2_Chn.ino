#include <CoreInk.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Arduino_GFX.h>
#include "font/u8g2_font_unifont_t_chinese.h"

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
uint16_t bg = GxEPD_WHITE;
uint16_t fg = GxEPD_BLACK;


void setup()
{
  M5.begin();
  u8g2Fonts.begin(M5.Ink);

  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_unifont_t_chinese); 
  M5.Ink.firstPage();
  do
  {
    M5.Ink.fillScreen(bg);
    u8g2Fonts.setCursor(20, 20);
    u8g2Fonts.print("測試文字");
  }
  while (M5.Ink.nextPage());
}

void loop()
{
}
