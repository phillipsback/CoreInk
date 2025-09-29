#include <CoreInk.h>
#include <EEPROM.h>

int addr = 0;                     // EEPROM地址起始编号
#define SIZE 32                   // 定义EEPROM的大小(字节)

void setup() {
  M5.begin();                     // 初始化 CoreInk

  M5.Ink.clear();                 // 清屏
  delay(1000);
  // 创建图像区域
  M5.Ink.setCursor(10,10);
  M5.Ink.print("EEPROM");         // 在图像区域内绘制字符串
  M5.Ink.update();
  if (!EEPROM.begin(SIZE)){       //  申请SIZE大小的存储(成功返回1)
     Serial.println("\nFailed to initialise EEPROM!"); //串口输出格式化字符串.
    delay(1000000);
  }
   Serial.println("\nRead data from EEPROM. Values are:");
  for (int i = 0; i < SIZE; i++){
     Serial.printf("%d ",EEPROM.read(i)); // 读取EEPROM中从0到SIZE中的数据
  }
  Serial.println("\n\nPress BtnA to Write EEPROM");
}

void loop() {
  M5.update();                    // 检测按键按下状态
  if(M5.BtnEXT.isPressed()){      // 如果按键A按下
    Serial.printf("\n%d Bytes datas written on EEPROM.\nValues are:\n",SIZE);
    for(int i=0;i<SIZE;i++){
      int val = random(256);      // 将要存储于EEPROM的整数数值(EEPROM每一个存储地址可以储存一个字节，只能存储0-255的数.故如果要使用EEPROM存储模拟输入引脚所读取到的数值需要将该数值除以4)
      EEPROM.write(addr, val);    // 向指定地址写入指定数据
       Serial.printf("%d ",val);
      addr += 1;                  // 转入下一存储地址
    }
    // 当存储地址序列号达到EEPROM的存储空间结尾，返回到EEPROM开始地址
    addr = 0;
     Serial.println("\n\nRead form EEPROM. Values are:");
    for(int i=0;i<SIZE;i++){
       Serial.printf("%d ",EEPROM.read(i));
    }
     Serial.println("\n-------------------------------------\n");
  }
  delay(150);
}
