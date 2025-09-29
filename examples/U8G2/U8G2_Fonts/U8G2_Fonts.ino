#include <CoreInk.h>
#include <U8g2_for_Adafruit_GFX.h>

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void setup()
{
  M5.begin();
  u8g2Fonts.begin(M5.Ink); // connect u8g2 procedures to Adafruit GFX
  helloWorld();
  delay(1000);
  helloArduino();
  delay(1000);
  helloEpaper();
  delay(1000);
  showFont("u8g2_font_helvR14_tf", u8g2_font_helvR14_tf); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  delay(2000);
  showFont("u8g2_font_profont22_mr", u8g2_font_profont22_mr); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  delay(1000);
  Serial.println("setup done");
}

void loop()
{
}

const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";
const char HelloEpaper[] = "Hello E-Paper!";

void helloWorld()
{
  //Serial.println("helloWorld");
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  int16_t tw = u8g2Fonts.getUTF8Width(HelloWorld); // text box width
  int16_t ta = u8g2Fonts.getFontAscent(); // positive
  int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
  int16_t th = ta - td; // text box height
  //Serial.print("ascent, descent ("); Serial.print(u8g2Fonts.getFontAscent()); Serial.print(", "); Serial.print(u8g2Fonts.getFontDescent()); Serial.println(")");
  // center bounding box by transposition of origin:
  // y is base line for u8g2Fonts, like for Adafruit_GFX True Type fonts
  uint16_t x = (M5.Ink.width() - tw) / 2;
  uint16_t y = (M5.Ink.height() - th) / 2 + ta;
  //Serial.print("bounding box    ("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(tw); Serial.print(", "); Serial.print(th); Serial.println(")");
  M5.Ink.firstPage();
  do
  {
    M5.Ink.fillScreen(bg);
    u8g2Fonts.setCursor(x, y); // start writing at this position
    u8g2Fonts.print(HelloWorld);
  }
  while (M5.Ink.nextPage());
  //Serial.println("helloWorld done");
}

void helloArduino()
{
  //Serial.println("helloArduino");
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  int16_t tw = u8g2Fonts.getUTF8Width(HelloArduino); // text box width
  int16_t ta = u8g2Fonts.getFontAscent(); // positive
  int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
  int16_t th = ta - td; // text box height
  uint16_t x = (M5.Ink.width() - tw) / 2;
  uint16_t y = M5.Ink.height() / 4 - th / 2 + ta;
  M5.Ink.setPartialWindow(0, y - 14, M5.Ink.width(), 20);
  M5.Ink.firstPage();
  do
  {
    M5.Ink.fillScreen(bg);
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(HelloArduino);
  }
  while (M5.Ink.nextPage());
  //Serial.println("helloArduino done");
}

void helloEpaper()
{
  //Serial.println("helloEpaper");
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  int16_t tw = u8g2Fonts.getUTF8Width(HelloArduino); // text box width
  int16_t ta = u8g2Fonts.getFontAscent(); // positive
  int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
  int16_t th = ta - td; // text box height
  uint16_t x = (M5.Ink.width() - tw) / 2;
  uint16_t y = M5.Ink.height() * 3 / 4 - th / 2 + ta;
  M5.Ink.setPartialWindow(0, y - 14, M5.Ink.width(), 20);
  M5.Ink.firstPage();
  do
  {
    M5.Ink.fillScreen(bg);
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(HelloEpaper);
  }
  while (M5.Ink.nextPage());
  //Serial.println("helloEpaper done");
}

void showFont(const char name[], const uint8_t *font)
{
  M5.Ink.setFullWindow();
  M5.Ink.setRotation(0);
  u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);              // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
  u8g2Fonts.setFont(font); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  M5.Ink.firstPage();
  do
  {
    drawFont(name);
  }
  while (M5.Ink.nextPage());
}

void drawFont(const char name[])
{
  //M5.Ink.setRotation(0);
  M5.Ink.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setCursor(0, 0);
  u8g2Fonts.println();
  u8g2Fonts.println(name);
  u8g2Fonts.println(" !\"#$%&'()*+,-./");
  u8g2Fonts.println("0123456789:;<=>?");
  u8g2Fonts.println("@ABCDEFGHIJKLMNO");
  u8g2Fonts.println("PQRSTUVWXYZ[\\]^_");
  u8g2Fonts.println("`abcdefghijklmno");
  u8g2Fonts.println("pqrstuvwxyz{|}~ ");
  u8g2Fonts.println("Umlaut ÄÖÜäéöü");
}
