#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  EEPROM.begin(4);
  

}

void loop() {
  int x = EEPROM.read(1);
  x = x + 1;
  Serial.println(EEPROM.read(1));
  EEPROM.write(1, x); 
  EEPROM.commit();
  delay(1500);
  ESP.reset();

}
