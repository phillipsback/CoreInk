#include <CoreInk.h>
#include <Fonts/TomThumb.h>

void setup() {
    M5.begin(true, true, false);
    M5.Ink.InitClib(GxEPD_BLACK, GxEPD_WHITE);
    M5.Ink.clear();
    M5.Ink.cSimPrint("I2C檢測",3,2, GxEPD_BLACK);
    M5.Ink.drawLine(3,16, 197, 16, GxEPD_BLACK);
    M5.Ink.update();
    delay(3000);
    i2cScan();
//    M5.Ink.firstPage();
//    do{
////      delay(10);
//    }while(M5.Ink.nextPage());
}

void i2cScan() {
    int address;
    int error;
    int Cellw = 10;
    int Cellh = 10;

    M5.Ink.setFont(&TomThumb);
    for (int h=0; h<8; h++){
      M5.Ink.setCursor(3, h*Cellh+50);
      M5.Ink.print(h, HEX);
    }
    for(int l=0; l<16; l++){
      M5.Ink.setCursor(l*Cellw+12, 40);
      M5.Ink.print(l, HEX);
    }
    
    for (int h=0; h<8; h++){
      for(int l=0; l<16; l++){
        address = h*16 + l;
        if(address>0){
          Wire.beginTransmission(address);  // 开始向指定的设备地址进行传输数据
          error = Wire.endTransmission();   // 停止与从机的数据传输
          /* 0: 成功 
             1: 数据量超过传送缓存容纳限制
             2: 传送地址时收到 NACK 
             3: 传送数据时收到 NACK
             4: 其它错误 */
          if (error == 0)
            M5.Ink.fillRect(l*Cellw+10, h*Cellh+45, Cellw-1, Cellh-1, GxEPD_BLACK);
          else
            M5.Ink.drawRect(l*Cellw+10, h*Cellh+45, Cellw-1, Cellh-1, GxEPD_BLACK);
          delay(10);
        }
      }
    }
    M5.Ink.update();
}

void loop() {
  
}
