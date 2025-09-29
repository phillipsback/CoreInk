#ifndef __BM8563_H__
#define __BM8563_H__

#include <Wire.h>

typedef struct RTC_Time
{
    int8_t Hours;
    int8_t Minutes;
    int8_t Seconds;
    RTC_Time() : Hours(),Minutes(),Seconds(){}
    RTC_Time(int8_t h,int8_t m,int8_t s) : Hours(h),Minutes(m),Seconds(s){}
} RTC_Time_t;

typedef struct RTC_Date
{
    int8_t WeekDay;
    int8_t Month;
    int8_t Day;
    int16_t Year;
    RTC_Date() : WeekDay(),Month(),Day(),Year(){}
    RTC_Date(int8_t w,int8_t m,int8_t d, int16_t y) : WeekDay(w),Month(m),Day(d),Year(y){}
} RTC_Date_t;

typedef union RTC_STATUS{
  uint8_t Value;
  struct{
    uint8_t timer_enable:1;
    uint8_t alarm_enable:1;
    uint8_t timer_flag:1;
    uint8_t alarm_flag:1;
    uint8_t interupt:1;
    uint8_t unuse:3;             
  };
} RTC_STATUS_t;
typedef union RTC_SECOND{
  uint8_t Value;
  struct{
    uint8_t sec_0:4;
    uint8_t sec_1:3;
    uint8_t vol_low:1;             
  };
} RTC_SECOND_t;
typedef union RTC_MINUTE{
  uint8_t Value;
  struct{
    uint8_t min_0:4;
    uint8_t min_1:3;
    uint8_t disable:1;  
  };
} RTC_MINUTE_t;
typedef union RTC_HOUR{
  uint8_t Value;
  struct{
    uint8_t hour_0:4;
    uint8_t hour_1:2;
    uint8_t unuse:1;
	uint8_t disable:1;	
  };
} RTC_HOUR_t;
typedef union RTC_DATE{
  uint8_t Value;
  struct{
    uint8_t date_0:4;
    uint8_t date_1:2;
    uint8_t unuse:1; 
	uint8_t disable:1;
  };
} RTC_DATE_t;
typedef union RTC_WEEKDAY{
  uint8_t Value;
  struct{
    uint8_t weekday:3;
    uint8_t unuse:4;  
	uint8_t disable:1;
  };
} RTC_WEEKDAY_t;
typedef union RTC_MONTH{
  uint8_t Value;
  struct{
    uint8_t month_0:4;
    uint8_t month_1:1;
	uint8_t unuse:2;
	uint8_t centry:1;
  };
} RTC_MONTH_t;
typedef union RTC_YEAR{
  uint8_t Value;
  struct{
    uint8_t year_0:4;
    uint8_t year_1:4;
  };
} RTC_YEAR_t;
typedef union RTC_TIMER{
  uint8_t Value;
  struct{
    uint8_t type:2;
    uint8_t unuse:5;
	uint8_t enable:1;
  };
} RTC_TIMER_t;
typedef union RTC_DATA{
	uint8_t trdata[7];
	struct{
		RTC_SECOND_t  sec;
		RTC_MINUTE_t  min;
		RTC_HOUR_t    hour;
		RTC_DATE_t    date;
		RTC_WEEKDAY_t wday;
		RTC_MONTH_t   mon;
		RTC_YEAR_t    year;
	};
} RTC_DATA_t;
typedef union RTC_BCD{
	uint8_t Value;
	struct{
		uint8_t L:4;
		uint8_t H:4;
	};
} RTC_BCD_t;

class RTC
{
public:
    RTC();

    void     begin(void);
    void     WriteReg(uint8_t reg, uint8_t data);
    uint8_t  ReadReg(uint8_t reg);
	void     update();
	
    void     SetTime(RTC_Time_t *RTC_TimeStruct);
    void     SetDate(RTC_Date_t *RTC_DateStruct);

    void     GetTime(RTC_Time_t *RTC_TimeStruct);
    void     GetDate(RTC_Date_t *RTC_DateStruct);

    int      SetAlarmIRQ(int afterSeconds);
    int      SetAlarmIRQ(int8_t Hr, int8_t Mt);
    int      SetAlarmIRQ(int Dy, int Hr, int Mt, int Wd);
    
    void     clearIRQ();
    void     disableIRQ();
	
	uint32_t getDayOfWeek(uint32_t day, uint32_t month, uint32_t year);
	bool     isWakeup();
	
    uint8_t  Second;
    uint8_t  Minute;
    uint8_t  Hour;
    uint8_t  Weekday;
    uint8_t  Day;
    uint8_t  Month;
    uint16_t Year;

private:
    uint8_t ByteToBcd(uint8_t Value);
	RTC_DATA_t rtcDATA;
	RTC_STATUS_t status;
};

#endif
