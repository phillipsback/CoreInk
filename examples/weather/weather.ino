#include <CoreInk.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <math.h>
#include <Fonts/TomThumb.h>
#include <Fonts/FreeSans9pt7b.h>

typedef struct{
  uint8_t x;
  uint8_t y;
  uint8_t w;
  uint8_t h;
} AREA_t;

HTTPClient http;
const char*    ssid           ="chenHouse2.0";
const char*    password       = "j5t2jj2p4zbr";
const char*    ntpServer      = "pool.ntp.org";
const uint16_t utcOffset      = 28800;
const uint8_t  daylightOffset = 0;
JsonDocument doc;

RTC_Time_t RTCtime;
RTC_Date_t RTCDate;
JsonObject obj;
JsonObject MAP;
bool getInfo = false;
std::map<String, int> DayTemp;
std::map<String, int> NitTemp;
std::vector<int> Humid;

String cityInfo = "";
int Yr=0, Mh, Dy;


char timeStrbuff[64];
char* WDStr[] ={"(週日)","(週一)","(週二)","(週三)","(週四)","(週五)","(週六)"};

void parseDate(const char* dateStr, int TVal, int HVal){
  int year, month, day, hour, minute, second;
  char Buff[16];
  sscanf(dateStr, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
  if(!Yr){
    Yr=year;
    Mh=month;
    Dy=day;
  }
  sprintf(Buff,"%04d%02d%02d",year, month, day);
  Serial.printf("%s:%d:%d\n", Buff, TVal, HVal);
  if(hour==18){
    NitTemp[Buff] = TVal;
  }else{
    DayTemp[Buff] = TVal;
  }
  Humid.push_back(HVal);

}

void flushTime() {
    AREA_t Area = {10, 50, 180, 130};
    
    M5.rtc.GetTime(&RTCtime); 
    M5.rtc.GetDate(&RTCDate);  
    sprintf(timeStrbuff, "%d年%02d月%02d日%s   %02d:%02d:%02d", RTCDate.Year,
            RTCDate.Month, RTCDate.Day, WDStr[RTCDate.WeekDay] , RTCtime.Hours, RTCtime.Minutes,
            RTCtime.Seconds);
    M5.Ink.clear(); 
    M5.Ink.cSimPrint(timeStrbuff, 5, 5, 0);
    M5.Ink.drawLine(3, 20, 197, 20, 0);     // Y軸
    M5.Ink.fillRect(5,23,85,15,0);
    M5.Ink.cSimPrint((char*)cityInfo.c_str(), 5, 25, 1);
    drawGraph(Area);
    M5.Ink.update();
}

void updateData(){
  Yr=0;
  DayTemp.clear();
  NitTemp.clear();
  Humid.clear();
  getWeatherData();
  getLocate();
  getList();
}

void NextDay(){
  int MDay[]={31,28,31,30,31,30,31,31,30,31,30,31};
  if(Mh==2){
    if((Yr%400)==0)  { MDay[1]=29; }
    else{
      if((Yr%100)==0){ MDay[1]=28; }
      else{
        if((Yr%4)==0){ MDay[1]=29; } 
      }
    }
  }
  if(Dy==MDay[Mh-1]){
    if(Mh==12){
      Yr++;
      Mh=1;
    }else{
      Mh++;
    }
    Dy=1;
  }else{
    Dy++;
  }
}

void getList(){
  JsonArray Tarr = obj["records"]["locations"][0]["location"][0]["weatherElement"][0]["time"];     // 氣溫
  JsonArray Harr = obj["records"]["locations"][0]["location"][0]["weatherElement"][1]["time"];     // 濕度
  uint8_t DataCount = Tarr.size();
  for(int i=0; i<DataCount; i++){
    String s = Tarr[i]["startTime"];
    int Tp = atoi(Tarr[i]["elementValue"][0]["value"].as<const char*>());
    int Hp = atoi(Harr[i]["elementValue"][0]["value"].as<const char*>());
    //Serial.printf("%s\n", s.c_str());
    parseDate(s.c_str(), Tp, Hp);
  }
}

void getLocate(){
  cityInfo = "";
  //cityInfo = cityInfo + "地點: ";
  cityInfo = cityInfo + obj["records"]["locations"][0]["locationsName"].as<const char*>(); 
  cityInfo = cityInfo + " ";
  cityInfo = cityInfo + obj["records"]["locations"][0]["location"][0]["locationName"].as<const char*>();
}

void getWeatherData(){
    http.begin("https://opendata.cwa.gov.tw/api/v1/rest/datastore/F-D0047-093?Authorization=CWB-5676E533-A873-4FC7-8FF3-13F5C83EAF5E&format=JSON&locationId=F-D0047-071&locationName=%E6%9D%BF%E6%A9%8B%E5%8D%80&elementName=T,RH&sort=time");
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK){
      String getData = http.getString();
      JsonDocument filter;
      filter["records"]["locations"][0]["locationsName"] = true;                                                                // 新北市
      filter["records"]["locations"][0]["location"][0]["locationName"] = true;                                                  // 板橋區
      filter["records"]["locations"][0]["location"][0]["weatherElement"][0]["time"][0]["startTime"] = true;                     // 開始時間
      filter["records"]["locations"][0]["location"][0]["weatherElement"][0]["time"][0]["elementValue"][0]["value"] = true;      // 溫度
      deserializeJson(doc, getData, DeserializationOption::Filter(filter),  DeserializationOption::NestingLimit(12));
      //serializeJsonPretty(doc, Serial);
      obj = doc.as<JsonObject>();
    }
    http.end();
}

void drawDashLine(POINT Start, POINT End, uint8_t width){
  double s = (End.y-Start.y) / (End.x-Start.x);
  double l = sqrt((End.y-Start.y)*(End.y-Start.y) + (End.x-Start.x)*(End.x-Start.x));
  double dx= width*(End.x-Start.x)/l;
  int x1=Start.x, y1=Start.y, x2, y2;
  bool draw=true;

//  Serial.printf("%0.2f, %0.2f\n", dx, l);
  do{
    x2=x1+dx;
    y2=y1+dx*s;
    if(draw){
      M5.Ink.drawLine(x1, y1, x2, y2, 0);
    }
    x1=x2;
    y1=y2;
    draw=!draw;
  }while(x1<End.x);
}

void drawGraph(AREA_t Area){
  Area.w-=20;
  Area.h-=20;
  double rateX = Area.w/7;
  uint8_t x1=0, y1=0;
  uint8_t x2=0, y2=0;
  uint8_t x3=0, y3;
  long tx  , ty;
  char Buf[16];
  uint8_t idx=0;
  
  M5.Ink.drawLine(Area.x+15,     Area.y       , Area.x+15    , Area.y+Area.h, 0);     // Y軸
  M5.Ink.drawLine(Area.x+Area.w, Area.y,        Area.x+Area.w, Area.y+Area.h, 0);
  M5.Ink.drawLine(Area.x+15,     Area.y+Area.h, Area.x+Area.w, Area.y+Area.h, 0);
  M5.Ink.cSimPrint("℃", Area.x, Area.y+Area.h-4, 0);
  M5.Ink.cSimPrint("Rh%", Area.x+Area.w+3, Area.y+Area.h-4, 0);

  for(int i=1; i<5; i++){
    ty = Area.y+Area.h-Area.h*i/4;
    sprintf(Buf,"%01d", i*10);
    M5.Ink.cSimPrint(Buf, Area.x, ty-4, 0);
    M5.Ink.cSimPrint(i*25, Area.x+Area.w+3, ty-4, 0);
    drawDashLine((POINT){Area.x+15, ty}, (POINT){Area.x+Area.w, ty}, 5);
  }
  
  M5.Ink.setFont(&TomThumb);
  for(int i=0;i<7;i++){
    sprintf(Buf,"%04d%02d%02d", Yr, Mh, Dy);
    Serial.println(Buf);
    M5.Ink.cSimPrint(Dy, Area.x+rateX*(i+1)-3, Area.y+Area.h+2, 0);
    auto tmp1 = DayTemp.find(Buf);
    if(tmp1!=DayTemp.end()){
      tx = Area.x + rateX*(i+1);
      ty = Area.y + Area.h - Area.h*  tmp1->second/40;
      if(x1){
        M5.Ink.drawLine(x1, y1, tx, ty, 0);
      }
      M5.Ink.setCursor(tx-4, ty-8);
      M5.Ink.printf("%d",tmp1->second);
      x1 = tx;
      y1 = ty;
      ty = Area.y + Area.h - Area.h* Humid[idx++]/100;
      if(x3){
        M5.Ink.drawLine(x3, y3, tx, ty, 0);
      }
      x3 = tx;
      y3 = ty;
    }
//    auto tmp2 = NitTemp.find(Buf);
//    if(tmp2!=NitTemp.end()){
      tx = Area.x + rateX*(i+1);
//      ty = Area.y + Area.h - Area.h * tmp2->second/40;
//      if(x2){
//        M5.Ink.drawLine(x2, y2, tx, ty, 0);
//      }
//      x2 = tx;
//      y2 = ty;
      ty = Area.y + Area.h - Area.h* Humid[idx++]/100;
      if(x3&&(i<6)){
        M5.Ink.drawLine(x3, y3, tx + rateX/2, ty, 0);
      }
      x3 = tx+rateX/2;
      y3 = ty;
//    }

    NextDay();
  }
}

void updateTime(){
  struct tm now={};
  getLocalTime(&now);
  RTC_Time_t rtcTime;
  rtcTime.Hours = now.tm_hour;
  rtcTime.Minutes = now.tm_min;
  rtcTime.Seconds = now.tm_sec;
  M5.rtc.SetTime(&rtcTime);
  RTC_Date_t rtcDate;
  rtcDate.Year  = now.tm_year+1900;
  rtcDate.Month = now.tm_mon;
  rtcDate.Day  =  now.tm_mday;
  rtcDate.WeekDay = now.tm_wday;
  M5.rtc.SetDate(&rtcDate);  
}

void setup() {
  M5.begin();
  M5.Ink.clear();
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  configTime(utcOffset, daylightOffset, ntpServer);
  Serial.print("\n");
//  Serial.println("Connected");
  M5.Ink.InitClib(0,1);
  updateTime();

}

void loop() {
  if(!getInfo){
    if(WiFi.status() == WL_CONNECTED){
      updateData();
      getInfo = true;
    }else{
      Serial.println("WIFI Disconnect");
    }
  }
  flushTime();
  getInfo=false;
  delay(900000);
}
