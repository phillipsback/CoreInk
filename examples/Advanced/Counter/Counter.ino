#include <CoreInk.h>
#include <Preferences.h>

Preferences preferences;

char Strbuff[64];

void setup() {
  M5.begin();
  
  M5.Ink.clear();
  delay(1000);

  M5.Ink.setCursor(10,10);
  M5.Ink.print("Counter");
  preferences.begin("my-app", false); 

  //preferences.clear();  // Remove all preferences under the opened namespace.清除preferences中所有的空间
  //preferences.remove("counter");  // Or remove the counter key only.  只清除counter中的值

  unsigned int counter = preferences.getUInt("counter", 0); 
  counter++;
  sprintf(Strbuff,"Current counter value:%u",counter);
  M5.Ink.setCursor(5,40);
  M5.Ink.print(Strbuff);
  preferences.putUInt("counter", counter);
  preferences.end();
  M5.Ink.setCursor(10,75);
  M5.Ink.print("Restart in 15 s");
  M5.Ink.update();
  delay(15000);
  
  ESP.restart();
}
void loop() {}
