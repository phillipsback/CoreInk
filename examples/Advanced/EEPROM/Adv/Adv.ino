#include "M5CoreInk.h"
#include <EEPROM.h>
#include <Fonts/Org_01.h>

int addr = 0; 
uint8_t Age = 50;
char StrBuff[64];

/*
 * =======================================================================
 * bool     begin(size_t size);                // 要求一塊分區存取記憶體
 * uint8_t  read(int address);                 // 讀取指定位址資料
 * void     write(int arrdess, uint_t val);    // 在指定位址儲存資料
 * uint16_t length();                          // 取得此要求分區記憶體的大小
 * bool     commit();                          // 將資料從緩衝區存入flash
 * void     end();                             // 結束存取
 * 
 * uint8_t  readByte   (int address);
 * int8_t   readChar   (int address);
 * uint8_t  readUChar  (int address);
 * int16_t  readShort  (int address);
 * uint16_t readUShort (int address);
 * int32_t  readInt    (int address);
 * uint32_t readUInt   (int address);
 * int32_t  readLong   (int address);
 * uint32_t readULong  (int address);
 * int64_t  readLong64 (int address);
 * uint64_t readULong64(int address);
 * float_t  readFloat  (int address);
 * double_t readDouble (int address);
 * bool     readBool   (int address);
 * size_t   readString (int address, char* value, size_t maxLen);
 * String   readString (int address);
 * size_t   readBytes  (int address, void* value, size_t maxLen);
 * template <class T> T readAll(int address, T &);
 * 
 * size_t writeByte   (int address, uint8_t  value);
 * size_t writeChar   (int address, int8_t   value);
 * size_t writeUChar  (int address, uint8_t  value);
 * size_t writeShort  (int address, int16_t  value);
 * size_t writeUShort (int address, uint16_t value);
 * size_t writeInt    (int address, int32_t  value);
 * size_t writeUInt   (int address, uint32_t value);
 * size_t writeLong   (int address, int32_t  value);
 * size_t writeULong  (int address, uint32_t value);
 * size_t writeLong64 (int address, int64_t  value);
 * size_t writeULong64(int address, uint64_t value);
 * size_t writeFloat  (int address, float_t  value);
 * size_t writeDouble (int address, double_t value);
 * size_t writeBool   (int address, bool     value);
 * size_t writeString (int address, const char* value);
 * size_t writeString (int address, String   value);
 * size_t writeBytes  (int address, const void* value, size_t len);
 * template <class T>T writeAll(int address, const T &);
 * 
 * =======================================================================
 */

void setup() {
  M5.begin();
  M5.Ink.setFont(&Org_01);
  M5.Ink.InitClib(0,1);
  M5.Ink.setTextColor(GxEPD_BLACK);
  M5.Ink.eraseDisplay();
  M5.Ink.update();
  if (!EEPROM.begin(1000)){
    M5.Ink.cSimPrint("無法初始化EEPROM", 3, 15);
    M5.Ink.update();
    delay(3000);
    ESP.restart();
  }else{
    M5.Ink.cSimPrint("已初始化EEPROM", 3, 15);
    M5.Ink.update();
  }
  M5.Ink.cSimPrint("<上推>鍵可顯示舊EEPROM值", 3, 30);
  M5.Ink.cSimPrint("<下推>鍵可寫入新EEPROM值", 3, 45);
  M5.Ink.update();
}

void loop() {
  M5.update();
  if(M5.BtnDOWN.isPressed()){    
    M5.Ink.fillRect(3, 150, GxEPD_WIDTH, 40, GxEPD_WHITE);
    M5.Ink.cSimPrint("字串寫入EEPROM", 3, 150);
    M5.Ink.updateWindow(3, 150, GxEPD_WIDTH, 40, false);
    M5.Ink.fillRect(3, 80, GxEPD_WIDTH, 30, GxEPD_WHITE);
    String Str = "我是陳進昇";
    EEPROM.writeString(addr, Str);
    addr += Str.length()+1;
    EEPROM.writeByte(addr, Age);
    addr += sizeof(byte);
    EEPROM.commit();
    addr = 0;
    M5.Ink.updateWindow(3, 80, GxEPD_WIDTH, 30, false);
    //M5.Ink.update();
  }
  if(M5.BtnUP.isPressed()){ 
    M5.Ink.fillRect(3, 150, GxEPD_WIDTH, 40, GxEPD_WHITE);
    M5.Ink.cSimPrint("由EEPROM讀取。", 3, 150);
    M5.Ink.updateWindow(3, 150, GxEPD_WIDTH, 40, false);
    M5.Ink.fillRect(3, 80, GxEPD_WIDTH, 30, GxEPD_WHITE); 
    String getStr = EEPROM.readString(addr);
    addr += getStr.length()+1;
    uint8_t getAge = EEPROM.readByte(addr);
    addr=0;
    sprintf(StrBuff,"%s 我 %d 歳了", getStr.c_str(), getAge);
    M5.Ink.cSimPrint(StrBuff, 3, 90);
    M5.Ink.updateWindow(3, 80, GxEPD_WIDTH, 30, false);
    //M5.Ink.update();
  }
  delay(150);
}
