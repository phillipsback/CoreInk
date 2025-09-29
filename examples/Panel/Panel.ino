#include "M5CoreInk.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

HTTPClient http;
const char* ssid="chenhouse";
const char* password = "j5t2-jj2p-4zbr";
DynamicJsonDocument doc(2048);
Ink_Sprite fbuffer(&M5.M5Ink);
RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;
JsonObject obj;
JsonArray  arr;
bool getInfo = false;

uint8_t AvgTemp[16];
uint8_t AvgHum[16];
uint8_t DataCount;

String cityInfo;
String tempInfo = "溫度: ";


char timeStrbuff[64];
char* WDStr[7];
char* D0 = "(週日)";
char* D1 = "(週一)";
char* D2 = "(週二)";
char* D3 = "(週三)";
char* D4 = "(週四)";
char* D5 = "(週五)";
char* D6 = "(週六)";

void initWD(){
  WDStr[0] = D0;
  WDStr[1] = D1;
  WDStr[2] = D2;
  WDStr[3] = D3;
  WDStr[4] = D4;
  WDStr[5] = D5;
  WDStr[6] = D6;
}

void flushTime() {
    M5.rtc.GetTime(&RTCtime); 
    M5.rtc.GetDate(&RTCDate);  
    sprintf(timeStrbuff, "%d年%02d月%02d日%s   %02d:%02d:%02d", RTCDate.Year,
            RTCDate.Month, RTCDate.Date, WDStr[RTCDate.WeekDay] , RTCtime.Hours, RTCtime.Minutes,
            RTCtime.Seconds);
    fbuffer.clear(); 
    fbuffer.cSimPrint(timeStrbuff, 5, 5, 0);
    fbuffer.cSimPrint((char*)cityInfo.c_str(), 5, 25, 0);
    fbuffer.cSimPrint((int32_t)DataCount, 50, 40, 0);
    fbuffer.drawLine(3,  20,  197,  20,  0);
    for(int i=0; i<DataCount; i++){
      fbuffer.cSimPrint((int32_t)AvgTemp[i], 5, 40+i*15, 0);
    }
    fbuffer.pushSprite();  // push the sprite. 推送图片
}

void updateData(){
  getWeatherData();
  getLocate();
  getWEData();
}

void getWEData(){
  JsonObject map;
  JsonArray temparr;
  String Temp = arr[0];
  deserializeJson(doc, Temp);
  map = doc.as<JsonObject>();
  String temps = map["time"];
  deserializeJson(doc, temps);
  temparr = doc.as<JsonArray>();
  Serial.println(Temp);
  DataCount = temparr.size();
  for(int i=0; i<DataCount; i++){
    JsonObject emap;
    String s = temparr[i];
    deserializeJson(doc, s);
    emap = doc.as<JsonObject>();
    String val = emap["elementValue"][0];
    deserializeJson(doc, val);
    emap = doc.as<JsonObject>();
    AvgTemp[i] = emap["value"];
  }
}

void getLocate(){
  String Temp = obj["locations"][0];
  deserializeJson(doc, Temp);
  obj = doc.as<JsonObject>();
  Serial.println(Temp);
  String A = obj["locationsName"];
  String B = "地點: " + A;
  String Temp1 = obj["location"][0];
  deserializeJson(doc, Temp1);
  obj = doc.as<JsonObject>();
  String C = obj["locationName"];
  String D = B + C;
  cityInfo = D;
  String Node = obj["weatherElement"][0];
  deserializeJson(doc, Node);
  arr = doc.as<JsonArray>();
  Serial.println(Node);
}

void getWeatherData(){
    http.begin("https://opendata.cwb.gov.tw/api/v1/rest/datastore/F-D0047-093?Authorization=CWB-5676E533-A873-4FC7-8FF3-13F5C83EAF5E&format=JSON&locationId=F-D0047-071&locationName=%E6%9D%BF%E6%A9%8B%E5%8D%80&elementName=T,RH&sort=time");
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK){
      String getData = http.getString();
      deserializeJson(doc, getData);
      obj = doc.as<JsonObject>();
      String Rec = obj["records"];
      deserializeJson(doc, Rec);
      obj = doc.as<JsonObject>();
    }
    http.end();
}

void printLocalWeather(){
    http.begin("https://opendata.cwb.gov.tw/api/v1/rest/datastore/F-D0047-093?Authorization=CWB-5676E533-A873-4FC7-8FF3-13F5C83EAF5E&format=JSON&locationId=F-D0047-071&locationName=%E6%9D%BF%E6%A9%8B%E5%8D%80&elementName=Wx,MinT,MaxT&sort=time");
    int httpCode = http.GET();
    Serial.printf("%d\n", httpCode);
    if(httpCode == HTTP_CODE_OK){
      String getData = http.getString();
      deserializeJson(doc, getData);
      JsonObject obj = doc.as<JsonObject>();
      
      String Temp1 = obj["records"];
      deserializeJson(doc, Temp1);
      obj = doc.as<JsonObject>();

      String Temp3 = obj["locations"][0];
      deserializeJson(doc, Temp3);
      obj = doc.as<JsonObject>();
      
      String city = obj["locationsName"];
      
      String Temp2 = obj["weatherElement"][0];
      deserializeJson(doc, Temp2);
      obj = doc.as<JsonObject>();
      
      String temp1 = obj["time"];
      String temp2 = obj["temp2"];
      String weather = obj["weather"];
      String cityInfo = "地點: " + city;
      String tempInfo = "溫度: " + temp1 + "～" + temp2;
      String cityWeatherInfo = "天氣: " + weather;
      fbuffer.clear();
      fbuffer.cSimPrint((char*)city.c_str(), 2, 2);
      fbuffer.cSimPrint((char*)tempInfo.c_str(), 2, 24);
      fbuffer.cSimPrint((char*)cityWeatherInfo.c_str(), 2, 40);
      fbuffer.pushSprite();
    }else{
      Serial.println("GET ERR");
    }
  http.end();
}

void setup() {
  M5.begin();
  M5.M5Ink.clear();
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  initWD();
  updateData();
  fbuffer.creatSprite(0, 0, 200, 200, true);
  fbuffer.InitClib(0,1);
    //fbuffer.clear(); 
    //fbuffer.fillArc(50, 50, 0, 15, 40, 40, 5, 0);
    //fbuffer.SetFx(FX_HOLLOW);
    //fbuffer.drawRoundRect(5, 100, 50, 20, 5, 0);
    //fbuffer.cSimPrint("陳進昇", 10, 105);
    //fbuffer.pushSprite();
}

void loop() {
  if(!getInfo){
    if(WiFi.status() == WL_CONNECTED){
      getWeatherData();
      getInfo = true;
    }else{
      Serial.println("WIFI Disconnect");
    }
  }
  flushTime();
  delay(15000);
}
