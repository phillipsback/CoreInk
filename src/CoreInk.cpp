#include "CoreInk.h"

LED::LED(uint8_t pinNum){
	pinMode(pinNum, OUTPUT);
	_pin=pinNum;
	_val=LOW;
}
	
void LED::on(){
	digitalWrite(_pin,LOW);
}
void LED::off(){
	digitalWrite(_pin,HIGH);
}

CoreInk::CoreInk()
{

}

CoreInk::~CoreInk()
{
}

int CoreInk::begin(bool InkEnable, bool wireEnable, bool SpeakerEnable)
{
    pinMode(POWER_HOLD_PIN, OUTPUT);
    digitalWrite(POWER_HOLD_PIN, HIGH); // Hold power

    pinMode(1, OUTPUT);

    Serial.begin(115200);
    Serial.printf("initializing.....OK\n");

    if (SpeakerEnable)
    {
        Speaker.begin();
    }
	
    if (wireEnable)
    {
        Wire.begin(32, 33);
    }
	
    rtc.begin();
    rtc.clearIRQ();

    if (InkEnable)
    {
		Ink.begin();
		Ink.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
		Ink.clear();
		Ink.display();
		Ink.setTextColor(GxEPD_BLACK);
		Ink.setFont(&FreeSans9pt7b);
		Ink.setCursor(0, 10);
    }

    return 0;
}

void CoreInk::update()
{
    BtnUP.read();
    BtnDOWN.read();
    BtnMID.read();
    BtnEXT.read();
    BtnPWR.read();

    Speaker.update();
}

void CoreInk::shutdown()
{
    Ink.powerOff();

    pinMode(1, OUTPUT);
    digitalWrite(1, LOW);

    digitalWrite(POWER_HOLD_PIN, LOW);
}
int CoreInk::shutdown(int seconds)
{
    Ink.powerOff();
    rtc.clearIRQ();
    rtc.SetAlarmIRQ(seconds);
    delay(10);

    pinMode(1, OUTPUT);
    digitalWrite(1, LOW);

    digitalWrite(POWER_HOLD_PIN, LOW);
    return 0;
}
int CoreInk::shutdown(int8_t Hour, int8_t Minute)
{
    Ink.powerOff();
    rtc.clearIRQ();
    rtc.SetAlarmIRQ(Hour, Minute);
    delay(10);

    pinMode(1, OUTPUT);
    digitalWrite(1, LOW);

    digitalWrite(POWER_HOLD_PIN, LOW);
    return 0;
}
int CoreInk::shutdown(int Day, int Hour, int Minute, int Wday)
{
    Ink.powerOff();
    rtc.clearIRQ();
    rtc.SetAlarmIRQ(Day, Hour, Minute, Wday);
    delay(10);

    pinMode(1, OUTPUT);
    digitalWrite(1, LOW);
    
    digitalWrite(POWER_HOLD_PIN, LOW);
    return 0;
}


CoreInk M5;