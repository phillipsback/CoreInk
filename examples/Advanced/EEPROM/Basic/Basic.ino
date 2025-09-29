#include "M5CoreInk.h"
#include <EEPROM.h>
#include <Fonts/Org_01.h>

#define SIZE 16  

char Strbuff[64];
int addr = 0; 

/*
 * =======================================================================
 * bool     begin(size_t size);                // 要求一塊分區存取記憶體
 * uint8_t  read(int address);                 // 讀取指定位址資料
 * void     write(int arrdess, uint_t val);    // 在指定位址儲存資料
 * uint16_t length();                          // 取得此要求分區記憶體的大小
 * bool     commit();                          // 將資料從緩衝區存入flash
 * void     end();                             // 結束存取
 * =======================================================================
 */

void setup() {
  M5.begin();
  M5.Ink.setFont(&Org_01);
  M5.Ink.InitClib(0,1);
  M5.Ink.setTextColor(GxEPD_BLACK);
  M5.Ink.eraseDisplay();
  
  if (!EEPROM.begin(SIZE)){
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
    sprintf(Strbuff,"\n%d Bytes已寫入EEPROM.\n值為:\n",SIZE);
    M5.Ink.cSimPrint(Strbuff, 3, 150);
    M5.Ink.updateWindow(3, 150, GxEPD_WIDTH, 40, false);
    M5.Ink.fillRect(3, 80, GxEPD_WIDTH, 30, GxEPD_WHITE);
    M5.Ink.setCursor(3,90);
    for(int i=0;i<SIZE;i++){
      int val = random(256);
      EEPROM.write(addr, val);
      M5.Ink.printf("%d ",val);
      addr += 1;
    }
    EEPROM.commit();
    addr = 0;
    M5.Ink.updateWindow(3, 80, GxEPD_WIDTH, 30, false);
    //M5.Ink.update();
  }
  if(M5.BtnUP.isPressed()){ 
    M5.Ink.fillRect(3, 150, GxEPD_WIDTH, 40, GxEPD_WHITE);
    sprintf(Strbuff,"由EEPROM讀取。值為:\n",SIZE);
    M5.Ink.cSimPrint(Strbuff, 3, 150);
    M5.Ink.updateWindow(3, 150, GxEPD_WIDTH, 40, false);
    M5.Ink.fillRect(3, 80, GxEPD_WIDTH, 30, GxEPD_WHITE); 
    M5.Ink.setCursor(3,90);
    for (int i = 0; i < SIZE; i++){
      M5.Ink.printf("%d ",EEPROM.read(i));
    }
    M5.Ink.updateWindow(3, 80, GxEPD_WIDTH, 30, false);
    //M5.Ink.update();
  }
  delay(150);
}
