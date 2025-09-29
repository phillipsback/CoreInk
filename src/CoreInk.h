#ifndef _CoreInk_H_
#define _CoreInk_H_

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeSans9pt7b.h>
#include "utility/config.h"
#include "utility/BM8563.h"
#include "utility/Button.h"
#include "utility/Speaker.h"
#include "utility/Display.h"


class LED
{
public:
	LED(uint8_t pinNum=LED_EXT_PIN);
	
	void on();
	void off();
private:
	uint8_t _pin;
	uint8_t _val;
};

class CoreInk
{
public:
    CoreInk();
    ~CoreInk();

    int begin(bool InkEnable = true, bool wireEnable = true, bool SpeakerEnable = false);
    void update();

    void shutdown();
    int shutdown( int seconds );
    int shutdown( int8_t Hour, int8_t Minute);
    int shutdown( int Day, int Hour, int Minute, int Wday);

    #define DEBOUNCE_MS 10
    Button BtnUP    = Button( BUTTON_UP_PIN, true, DEBOUNCE_MS);
    Button BtnDOWN  = Button( BUTTON_DOWN_PIN, true, DEBOUNCE_MS);
    Button BtnMID   = Button( BUTTON_MID_PIN, true, DEBOUNCE_MS);
    Button BtnEXT   = Button( BUTTON_EXT_PIN, true, DEBOUNCE_MS);
    Button BtnPWR   = Button( BUTTON_PWR_PIN, true, DEBOUNCE_MS);
    
    SPEAKER Speaker;
	LED Led=LED(10);
    RTC rtc;
	Display Ink{GxEPD2_154_M09(INK_SPI_CS, INK_SPI_DC, INK_SPI_RST, INK_SPI_BUSY)};
	//GxIO_Class io = GxIO_Class(SPI, /*CS=5*/ 9, /*DC*/ 15, /*RST*/ 0);
	//Display Ink = Display(io, /*RST*/ 0, /*BUSY*/ 4);
private:
	RTC_STATUS_t status;

};

extern CoreInk M5;


#endif