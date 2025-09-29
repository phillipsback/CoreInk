#include "BM8563.h"

RTC::RTC()
{
}

void    RTC::begin   (void){
    Wire1.begin(21, 22);
	status.Value = ReadReg(0x01);
    WriteReg(0x00, 0x00);			// 不停止時鐘
    WriteReg(0x01, 0x00);			// 關閉中斷
    WriteReg(0x0D, 0x00);			// 關閉頻率輸出
}
void    RTC::WriteReg(uint8_t reg, uint8_t data){
    Wire1.beginTransmission(0x51);
    Wire1.write(reg);
    Wire1.write(data);
    Wire1.endTransmission();
}
uint8_t RTC::ReadReg (uint8_t reg){
    Wire1.beginTransmission(0x51);
    Wire1.write(reg);
    Wire1.endTransmission();
    Wire1.requestFrom(0x51, 1);
    return Wire1.read();
}
void    RTC::update(void){
    Wire1.beginTransmission(0x51);
    Wire1.write(0x02);
    Wire1.endTransmission();
    Wire1.requestFrom(0x51, 7);
    while (Wire1.available()){
		for(int i=0;i<7;i++){
			rtcDATA.trdata[i] = Wire1.read();
		}
	}
	Second = rtcDATA.sec.sec_0   + rtcDATA.sec.sec_1  *10;
	Minute = rtcDATA.min.min_0   + rtcDATA.min.min_1  *10;
	Hour   = rtcDATA.hour.hour_0 + rtcDATA.hour.hour_1*10;
	Day    = rtcDATA.date.date_0 + rtcDATA.date.date_1*10;
	Weekday= rtcDATA.wday.weekday;
	Month  = rtcDATA.mon.month_0 + rtcDATA.mon.month_1*10+1;
	Year   = rtcDATA.year.year_0 + rtcDATA.year.year_1*10;
	Year  += rtcDATA.mon.centry?1900:2000;

    // Bcd2asc();
}

uint8_t RTC::ByteToBcd(uint8_t Value){
	RTC_BCD_t RtnValue;
	
	RtnValue.L = Value%10;
	RtnValue.H = (Value-RtnValue.L)/10;

    return RtnValue.Value;
}
void    RTC::GetTime(RTC_Time_t *RTC_TimeStruct){
	update();

    RTC_TimeStruct->Seconds = Second; //秒
    RTC_TimeStruct->Minutes = Minute; //分
    RTC_TimeStruct->Hours   = Hour;   //时
}
void    RTC::SetTime(RTC_Time_t *RTC_TimeStruct){

    if (RTC_TimeStruct == NULL)
        return;

    Wire1.beginTransmission(0x51);
    Wire1.write(0x02);
    Wire1.write(ByteToBcd(RTC_TimeStruct->Seconds));
    Wire1.write(ByteToBcd(RTC_TimeStruct->Minutes));
    Wire1.write(ByteToBcd(RTC_TimeStruct->Hours));
    Wire1.endTransmission();
}
void    RTC::GetDate(RTC_Date_t *RTC_DateStruct){
	update();

    RTC_DateStruct->Day     = Day;
    RTC_DateStruct->WeekDay = Weekday;
    RTC_DateStruct->Month   = Month;
    RTC_DateStruct->Year    = Year;
}
void    RTC::SetDate(RTC_Date_t *RTC_DateStruct){

    if (RTC_DateStruct == NULL)
        return;
    Wire1.beginTransmission(0x51);
    Wire1.write(0x05);
    Wire1.write(ByteToBcd(RTC_DateStruct->Day));
    Wire1.write(ByteToBcd(RTC_DateStruct->WeekDay));

    if (RTC_DateStruct->Year < 2000)
    {

        Wire1.write(ByteToBcd((RTC_DateStruct->Month-1)) | 0x80);
        Wire1.write(ByteToBcd((uint8_t)(RTC_DateStruct->Year % 100)));
    }
    else
    {
        /* code */
        Wire1.write(ByteToBcd((RTC_DateStruct->Month-1)) | 0x00);
        Wire1.write(ByteToBcd((uint8_t)(RTC_DateStruct->Year % 100)));
    }

    Wire1.endTransmission();
}
int     RTC::SetAlarmIRQ(int afterSeconds){
	uint8_t div = 1;
	RTC_STATUS_t status;
	RTC_TIMER_t  tmr;
	
	tmr.Value = 0;
	
	status.Value = ReadReg(0x01);

    if (afterSeconds < 0){
		status.timer_enable = 0;
        WriteReg(0x01, status.Value);
        tmr.Value = 0x03;
        WriteReg(0x0E, tmr.Value);
        return -1;
    }

    tmr.type   = 2;
    tmr.enable = 1;
    if (afterSeconds > 255){
        div      = 60;
		tmr.type = 3;
    }

    afterSeconds = (afterSeconds / div) & 0xFF;
    WriteReg(0x0F, afterSeconds);
    WriteReg(0x0E, tmr.Value);

	status.timer_enable = 1;
    //status.Value &= ~(1 << 7);
    WriteReg(0x01, status.Value);
    return afterSeconds * div;
}
int     RTC::SetAlarmIRQ(int8_t Hr, int8_t Mt){
    uint8_t irq_enable = false;
	RTC_MINUTE_t m = {};
	RTC_HOUR_t   h = {};
	RTC_STATUS_t status;
	
    if (Mt >= 0){
        irq_enable = true;
		m.min_0    = Mt%10;
		m.min_1    = (Mt/10)%10;
		m.disable  = 0;
		WriteReg(0x09,  m.Value);
    }

    if (Hr >= 0){
        irq_enable = true;
		h.hour_0   = Hr%10;
		h.hour_1   = (Hr/10)%10;
		h.disable  = 0;
		WriteReg(0x0A,  h.Value);
    }

    status.Value = ReadReg(0x01);
	status.alarm_enable = irq_enable?1:0;
    WriteReg(0x01, status.Value);

    return irq_enable ? 1 : 0;
}
int     RTC::SetAlarmIRQ(int Dy, int Hr, int Mt, int Wd){
    uint8_t irq_enable = false;
	RTC_MINUTE_t  m = {};
	RTC_HOUR_t    h = {};
	RTC_DATE_t    d = {};
	RTC_WEEKDAY_t w = {};
	RTC_STATUS_t status;

    if (Mt >= 0){
        irq_enable = true;
		m.min_0    = Mt%10;
		m.min_1    = (Mt/10)%10;
		m.disable  = 0;
		WriteReg(0x09,  m.Value);
    }

    if (Hr >= 0){
        irq_enable = true;
		h.hour_0   = Hr%10;
		h.hour_1   = (Hr/10)%10;
		h.disable  = 0;
		WriteReg(0x0A,  h.Value);
    }

    if (Dy >= 0){
        irq_enable = true;
		d.date_0   = Dy%10;
		d.date_1   = (Dy/10)%10;
		d.disable  = 0;
		WriteReg(0x0B,  d.Value);
    }

    if (Wd >= 0){
        irq_enable = true;
		w.weekday  = Hr%7;
		w.disable  = 0;
		WriteReg(0x0C,  h.Value);
    }

    status.Value = ReadReg(0x01);
	status.alarm_enable = irq_enable?1:0;
    WriteReg(0x01, status.Value);

    return irq_enable ? 1 : 0;
}

void    RTC::clearIRQ(){
	RTC_STATUS_t status;
    status.Value = ReadReg(0x01);
	status.timer_flag = 0;
    status.alarm_flag = 0;
    WriteReg(0x01, status.Value);
}
void    RTC::disableIRQ(){
	RTC_STATUS_t status;
    status.Value = ReadReg(0x01);
	status.timer_flag   = 0;
    status.alarm_flag   = 0;
    status.timer_enable = 0;
    status.alarm_enable = 0;
    WriteReg(0x01, status.Value);
}

uint32_t RTC::getDayOfWeek(uint32_t day, uint32_t month, uint32_t year)
{
    uint32_t val;
	
    if (month < 3) {
        month = 12u + month;
        year--;
    }
    val = (day + (((month + 1u) * 26u) / 10u) + year + (year / 4u) + (6u * (year / 100u)) + (year / 400u)) % 7u;
    if (0u == val) {
        val = 7;
    }
    return (val - 1);
}

bool     RTC::isWakeup(){
	return status.timer_flag;
}