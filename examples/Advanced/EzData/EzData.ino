#include <CoreInk.h>
#include "M5_EzData.h"

// Configure the name and password of the connected wifi and your token.  配置所连接wifi的名称、密码以及你的token
const char* ssid = "chenHouse2.0";
const char* password = "j5t2jj2p4zbr";
const char* token = "JT8H8eSd0A0pKajNzGm2tDL2H6mjIVLc";

void setup() {
  M5.begin();

  M5.Ink.clear();                         //clear the screen. 清屏
  delay(1000);

  setupWifi(ssid,password);
}

void loop() {
   //Save the data 20 to the top of the testData topic queue.  保存数据20至 testData 队列首位
  M5.Ink.setCursor(10,50);
  if(setData(token,"testData",20)){
    M5.Ink.print("Success sending data");
  }else{
    M5.Ink.print("Fail to save dat");
  }
  M5.Ink.update();

  
  for(int i=0;i<3;i++){
    M5.Ink.setCursor(10,65);
    if(addToList(token,"testList",i)){
      M5.Ink.print("Success sending data");
    }else{
      M5.Ink.print("Fail to save dat");
    }
    delay(100);
  }
  delay(10000);
  M5.Ink.update();
  //Get a piece of data from a topic and store the value in result.  从一个 topic中获取一个数据,并将值存储在 result
  int result=0;
  M5.Ink.setCursor(10,80);
  if(getData(token,"testData",result)){
    M5.Ink.print("Success sending data");
  }else{
    M5.Ink.print("Fail to save dat");
  }
  delay(10000);
  M5.Ink.update();

  //Remove data
  M5.Ink.setCursor(10,95);
  if(removeData(token,"testData"))
    M5.Ink.print("Success remove data");
  else
    M5.Ink.print("Fail to remove data");

  M5.Ink.setCursor(10,110);
  if(removeData(token,"testList"))
    M5.Ink.print("Success remove data");
  else
    M5.Ink.print("Fail to remove data");
  delay(10000);
  M5.Ink.update();
  delay(10000);
  M5.Ink.clear(); //clear the screen. 清屏
}
