#include <CoreInk.h>
#include "time.h"

typedef struct{
  uint16_t x;
  uint16_t y;
} POS_t;
typedef struct{
  uint8_t w;
  uint8_t h;
} PAD_t;
typedef struct{
  uint16_t Year;
  uint8_t  Month;
  uint8_t  Day;
  bool     isLeap;    // 為閏月
} LDATE_t;

typedef struct{
  char*  date;
  bool   holiday;
  String name;
} HOLIDAY_t;

uint8_t conWeekend = 3;  // 星期六顏色顯示: 1=黑色, 2=綠色, 3=紅色, 4=隔週休

uint16_t lunarInfo[]={
0x4bd8,0x4ae0,0xa570,0x54d5,0xd260,0xd950,0x5554,0x56af,0x9ad0,0x55d2,
0x4ae0,0xa5b6,0xa4d0,0xd250,0xd295,0xb54f,0xd6a0,0xada2,0x95b0,0x4977,
0x497f,0xa4b0,0xb4b5,0x6a50,0x6d40,0xab54,0x2b6f,0x9570,0x52f2,0x4970,
0x6566,0xd4a0,0xea50,0x6a95,0x5adf,0x2b60,0x86e3,0x92ef,0xc8d7,0xc95f,
0xd4a0,0xd8a6,0xb55f,0x56a0,0xa5b4,0x25df,0x92d0,0xd2b2,0xa950,0xb557,
0x6ca0,0xb550,0x5355,0x4daf,0xa5b0,0x4573,0x52bf,0xa9a8,0xe950,0x6aa0,
0xaea6,0xab50,0x4b60,0xaae4,0xa570,0x5260,0xf263,0xd950,0x5b57,0x56a0,
0x96d0,0x4dd5,0x4ad0,0xa4d0,0xd4d4,0xd250,0xd558,0xb540,0xb6a0,0x95a6,
0x95bf,0x49b0,0xa974,0xa4b0,0xb27a,0x6a50,0x6d40,0xaf46,0xab60,0x9570,
0x4af5,0x4970,0x64b0,0x74a3,0xea50,0x6b58,0x5ac0,0xab60,0x96d5,0x92e0,
0xc960,0xd954,0xd4a0,0xda50,0x7552,0x56a0,0xabb7,0x25d0,0x92d0,0xcab5,
0xa950,0xb4a0,0xbaa4,0xad50,0x55d9,0x4ba0,0xa5b0,0x5176,0x52bf,0xa930,
0x7954,0x6aa0,0xad50,0x5b52,0x4b60,0xa6e6,0xa4e0,0xd260,0xea65,0xd530,
0x5aa0,0x76a3,0x96d0,0x4afb,0x4ad0,0xa4d0,0xd0b6,0xd25f,0xd520,0xdd45,
0xb5a0,0x56d0,0x55b2,0x49b0,0xa577,0xa4b0,0xaa50,0xb255,0x6d2f,0xada0,
0x4b63,0x937f,0x49f8,0x4970,0x64b0,0x68a6,0xea5f,0x6b20,0xa6c4,0xaaef,
0x92e0,0xd2e3,0xc960,0xd557,0xd4a0,0xda50,0x5d55,0x56a0,0xa6d0,0x55d4,
0x52d0,0xa9b8,0xa950,0xb4a0,0xb6a6,0xad50,0x55a0,0xaba4,0xa5b0,0x52b0,
0xb273,0x6930,0x7337,0x6aa0,0xad50,0x4b55,0x4b6f,0xa570,0x54e4,0xd260,
0xe968,0xd520,0xdaa0,0x6aa6,0x56df,0x4ae0,0xa9d4,0xa4d0,0xd150,0xf252,
0xd520};
uint8_t solarMonth = (31,28,31,30,31,30,31,31,30,31,30,31);
char* Gan[]={"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};
char* Zhi[]={"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};
char* Animals[]={"鼠","牛","虎","兔","龍","蛇","馬","羊","猴","雞","狗","豬"};
char* solarTerm[] = {"小寒","大寒","立春","雨水","驚蟄","春分","清明","穀雨","立夏","小滿","芒種","夏至","小暑","大暑","立秋","處暑","白露","秋分","寒露","霜降","立冬","小雪","大雪","冬至"};
char* nStr1[] = {"日","一","二","三","四","五","六","七","八","九","十"};
char* nStr2[] = {"初","十","廿","卅","卌"};
char* monthName[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

//國曆節日 *表示放假日
HOLIDAY_t TWMday[]={
  {"0101", true,  "元旦"},
  {"0214", false, "情人"},
  {"0228", true,  "228"},
  {"0308", false, "婦女"},
  {"0312", false, "植樹"},
  {"0329", false, "青年"},
  {"0401", false, "愚人"},
  {"0404", false, "婦幼"},
  {"0501", true,  "勞動"},
  {"0808", false, "父親"},
  {"0928", false, "教師"},
  {"1031", false, "萬聖"},
  {"1225", false, "聖誕"}};
//某月的第幾個星期幾。 5,6,7,8 表示到數第 1,2,3,4 個星期幾
HOLIDAY_t TWWday[]{
  {"0520", false, "母親"},
  {"1144", false, "感恩"}};
//農曆節日
HOLIDAY_t TWLday[]{
  {"0101", true,  "春節"},
  {"0109", false, "天公生"},
  {"0115", false, "元宵節"},
  {"0202", false, "土地公生"},
  {"0323", false, "媽祖生"},
  {"0408", false, "浴佛"},
  {"0505", true,  "端午"},
  {"0701", false, "開鬼門"},
  {"0707", false, "七夕"},
  {"0715", false, "中元"},
  {"0800", false, "關鬼門"},
  {"0815", true,  "中秋"},
  {"0909", false, "重陽"},
  {"1216", false, "尾牙"},
  {"1224", false, "送神"},
  {"0100", true,  "除夕"}};

uint8_t Remark(RTC_Date_t Date, char* name){
  LDATE_t LD = toLunar(Date);
  uint8_t tmp1  = sTerm(Date.Year,(Date.Month-1)*2  )-1;
  uint8_t tmp2  = sTerm(Date.Year,(Date.Month-1)*2+1)-1;
  char*   stmp1 = solarTerm[(Date.Month-1)*2];
  char*   stmp2 = solarTerm[(Date.Month-1)*2+1];
  char    strBuf[10];
  uint8_t MHlen = sizeof(TWMday)/sizeof(HOLIDAY_t);
  uint8_t LHlen = sizeof(TWLday)/sizeof(HOLIDAY_t);
  uint8_t RtnVal = 0;
  bool    HDEmpty = true;
  
  name[0]=0;
  if(Date.Day==tmp1){                                     // 節氣1
    strcpy(name, stmp1);
    RtnVal=1;
  }else{
    if(Date.Day==tmp2){                                   // 節氣2
      strcpy(name, stmp2);
      RtnVal=1;
    }
    else{
      sprintf(strBuf,"%02d%02d", Date.Month, Date.Day);   // 國曆
      for(int i=0; i<MHlen; i++){
        if(strcmp(strBuf, TWMday[i].date)==0){
          strcpy(name,TWMday[i].name.c_str());
          RtnVal = TWMday[i].holiday?2:0;
          HDEmpty = false;
          break;
        }
      }
      sprintf(strBuf,"%02d%02d", LD.Month, LD.Day);       // 農曆
      for(int i=0; i<LHlen; i++){
        if(strcmp(strBuf, TWLday[i].date)==0){
          strcpy(name,TWLday[i].name.c_str());
          RtnVal = TWLday[i].holiday?2:0;
          HDEmpty = false;
          break;
        }
      }
      if(HDEmpty){
        if(LD.Day==1){
          sprintf(name, "%s月", nStr1[LD.Month]);
        }else{
          sprintf(name, "%s%s", nStr2[LD.Day/10], nStr1[LD.Day%10]);
        }
      }
    }
  }
  return RtnVal;
}

uint8_t MonthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};
//====================================== 傳回農曆 y年的總天數
uint16_t lYearDays(uint16_t y) {
   uint16_t i, sum = 348;
   for(i=0x8000; i>0x8; i>>=1) sum += (lunarInfo[y-1900] & i)? 1: 0;
   return(sum+leapDays(y));
}
//====================================== 傳回農曆 y年閏月的天數
uint16_t leapDays(uint16_t y) {
   if(leapMonth(y)) return( (lunarInfo[y-1899]&0xf)==0xf? 30: 29);
   else return(0);
}
//====================================== 傳回農曆 y年閏哪個月 1-12 , 沒閏傳回 0
uint8_t leapMonth(uint16_t y) {
   uint16_t lm = lunarInfo[y-1900] & 0xf;
   return (lm==0xf?0:lm);
}
//====================================== 傳回農曆 y年m月的總天數
uint8_t monthDays(uint16_t y, uint8_t m) {
   return( (lunarInfo[y-1900] & (0x10000>>m))? 30: 29 );
}
//====================================== 傳回國曆 y年某m+1月的天數
uint8_t solarDays(uint16_t y, uint8_t m) {
   if(m==1)
      return(((y%4 == 0) && (y%100 != 0) || (y%400 == 0))? 29: 28);
   else
      return(MonthDays[m]);
}
//====================================== 傳入 offset 傳回干支, 0=甲子
String cyclical(uint16_t num) {
  String Temp = String(Gan[num%10]) + String(Zhi[num%12]);
  return Temp;
}
//====================================== 傳入國曆y年m月 傳回農曆年
String lYear(uint16_t y, uint8_t m) {
  String cY;
  
  if(m<3) cY=cyclical(y-1900+36-1);
  else cY=cyclical(y-1900+36);

  return cY;
}

//===== 某年的第n個節氣為幾日(從0小寒起算)
uint8_t solarTermBase[] = {4,19,3,18,4,19,4,19,4,20,4,20,6,22,6,22,6,22,7,22,6,21,6,21};
char solarTermIdx[] = "0123415341536789:;<9:=<>:=1>?012@015@015@015AB78CDE8CD=1FD01GH01GH01IH01IJ0KLMN;LMBEOPDQRST0RUH0RVH0RWH0RWM0XYMNZ[MB\\]PT^_ST`_WH`_WH`_WM`_WM`aYMbc[Mde]Sfe]gfh_gih_Wih_WjhaWjka[jkl[jmn]ope]qph_qrh_sth_W";
char solarTermOS[] = "211122112122112121222211221122122222212222222221222122222232222222222222222233223232223232222222322222112122112121222211222122222222222222222222322222112122112121222111211122122222212221222221221122122222222222222222222223222232222232222222222222112122112121122111211122122122212221222221221122122222222222222221211122112122212221222211222122222232222232222222222222112122112121111111222222112121112121111111222222111121112121111111211122112122112121122111222212111121111121111111111122112122112121122111211122112122212221222221222211111121111121111111222111111121111111111111111122112121112121111111222111111111111111111111111122111121112121111111221122122222212221222221222111011111111111111111111122111121111121111111211122112122112121122211221111011111101111111111111112111121111121111111211122112122112221222211221111011111101111111110111111111121111111111111111122112121112121122111111011111121111111111111111011111111112111111111111011111111111111111111221111011111101110111110111011011111111111111111221111011011101110111110111011011111101111111111211111001011101110111110110011011111101111111111211111001011001010111110110011011111101111111110211111001011001010111100110011011011101110111110211111001011001010011100110011001011101110111110211111001010001010011000100011001011001010111110111111001010001010011000111111111111111111111111100011001011001010111100111111001010001010000000111111000010000010000000100011001011001010011100110011001011001110111110100011001010001010011000110011001011001010111110111100000010000000000000000011001010001010011000111100000000000000000000000011001010001010000000111000000000000000000000000011001010000010000000";

uint8_t sTerm(uint16_t y, uint8_t n) {
   return (solarTermBase[n] +  solarTermOS[(solarTermIdx[y-1900] - 48) * 24 + n ]-47);
}
LDATE_t toLunar(RTC_Date_t Date) {
   LDATE_t RtnVal;
   int16_t i, leap=0, temp=0;
   struct tm tm_Date1 = {0,0,0,Date.Day, Date.Month-1, Date.Year-1900};
   time_t tDate1      = mktime(&tm_Date1);
   int32_t offset   = (tDate1/86400)-36;

   //Serial.printf("[%04d]",offset);
   for(i=1970; i<2100 && offset>0; i++) { temp=lYearDays(i); offset-=temp; }
   if(offset<0) { offset+=temp; i--; }
   RtnVal.Year = i;

   leap = leapMonth(i); //閏哪個月
   RtnVal.isLeap = false;

   for(i=1; i<13 && offset>0; i++) {
      //閏月
      if(leap>0 && i==(leap+1) && RtnVal.isLeap==false)
         { --i; RtnVal.isLeap = true; temp = leapDays(RtnVal.Year); }
      else
         { temp = monthDays(RtnVal.Year, i); }

      //解除閏月
      if(RtnVal.isLeap==true && i==(leap+1)) RtnVal.isLeap = false;

      offset -= temp;
   }

   if(offset==0 && leap>0 && i==leap+1)
      if(RtnVal.isLeap)
         { RtnVal.isLeap = false; }
      else
         { RtnVal.isLeap = true; --i; }

   if(offset<0){ offset += temp; --i; }

   RtnVal.Month = i;
   RtnVal.Day = offset + 1;

   return RtnVal;
}
bool isLeap(uint16_t Year){
  bool RtnVal = true;
  if(Year%400){
    if((Year%100)){
      if(Year%4){
        RtnVal=false;
      }else{
        RtnVal=true;
      }
    }else{
      RtnVal=false;
    }
  }else{
    RtnVal=true;
  }
  return RtnVal;
}

void ShowCal(POS_t Pos, PAD_t Sz, RTC_Date_t tgr){
    uint8_t Col= M5.rtc.getDayOfWeek(tgr.Day, tgr.Month, tgr.Year);
    uint8_t Row=0;
    uint8_t Offset= M5.rtc.getDayOfWeek(1, tgr.Month, tgr.Year);
    uint16_t Color;
    uint16_t RMColor;
    RTC_Date_t Today;
    RTC_Date_t tmpDay = tgr;
    LDATE_t LD; 
    bool Mark=false;
    char MarkStr[16];
    
    M5.rtc.GetDate(&Today); 
    MarkStr[0]=0;
    if((Today.Year==tgr.Year)&&(Today.Month==tgr.Month)){             // 指定月曆是本月份的話
      Mark=true;
    }
    
    if(tgr.Month==2){                                                 // 處理二月天數
      MonthDays[1]=isLeap(tgr.Year)?29:28;
    }

    M5.Ink.fillRect(0,0,200,14, GxEPD_BLACK);
    for(int i=0;i<7;i++){                                             // 標題列
      M5.Ink.cSimPrint(nStr1[i], Pos.x+i*Sz.w+7, Pos.y+2, GxEPD_WHITE);
      if(i!=0){
        M5.Ink.drawLine(Pos.x+i*Sz.w, 0, Pos.x+i*Sz.w, 200, GxEPD_BLACK);
      }
    }
    //M5.Ink.drawLine(Pos.x, Pos.y+(Sz.h/2)+1, Pos.x+Sz.w*7, Pos.y+(Sz.h/2)+1, GxEPD_BLACK);
    
    Pos.y+=15;
    RMColor = GxEPD_BLACK;
    for(int i=0;i<MonthDays[tgr.Month-1];i++){ 
      Col=(Offset+i)%7;
      Row=(Offset+i)/7;
      tmpDay.Day=(i+1);

      if (Col==0){
        M5.Ink.drawLine(0, Pos.y+Row*Sz.h-5, 200, Pos.y+Row*Sz.h-5, GxEPD_BLACK);
      }

      Remark(tmpDay, MarkStr);
      
      if(Mark&&(tgr.Day==(i+1))){
        M5.Ink.fillRect(Pos.x+Col*Sz.w, Pos.y+Row*Sz.h-4, Sz.w-1, Sz.h, GxEPD_BLACK);
        Color = GxEPD_WHITE;
        M5.Ink.setTextColor(GxEPD_WHITE);
      }else{
        Color = GxEPD_BLACK;
        M5.Ink.setTextColor(GxEPD_BLACK);
      }
      M5.Ink.setCursor(Pos.x+Col*Sz.w-2, Pos.y+Row*Sz.h+10);
      M5.Ink.printf("% 2d",i+1);
      //M5.Ink.cSimPrint(i+1,     Pos.x+Col*Sz.w, Pos.y+Row*Sz.h);
      M5.Ink.cSimPrint(MarkStr, Pos.x+Col*Sz.w, Pos.y+Row*Sz.h+12, Color);
    }
}

void setup() {
    M5.begin();
    M5.Ink.InitClib(GxEPD_BLACK, GxEPD_WHITE);

    RTC_Date_t Today;                  // 取得今天日期
    M5.rtc.GetDate(&Today);
    Serial.printf("%4d-%2d-%2d\n", Today.Year, Today.Month, Today.Day);
    M5.rtc.update();
    
    POS_t Pos      = {0, 0};
    PAD_t CellSize = {29, 32};    
    M5.Ink.clear();
    M5.Ink.display();
    ShowCal(Pos, CellSize, Today);
    M5.Ink.display();
    M5.Ink.powerOff();
    M5.shutdown();
}

void loop() {

    delay(500);
}
