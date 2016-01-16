#include <OneWire.h>
#include <LiquidCrystal.h>
 
#define BUTTON A0
 
int DS18B20_Pin = 9;
 
OneWire ds(DS18B20_Pin);
LiquidCrystal lcd(8,9,4,5,6,7);
 
void setup(void) {
  Serial.begin(9600);
 
  lcd.begin(16, 2);
 
  lcd.setCursor(0,0);
  lcd.print("  DS18B20 LCD  ");
  lcd.setCursor(0,1);
  lcd.print("mauroalfieri.it");
 
  delay( 3000 );
}
 
void loop(void) {
  float temperature = getTemp();
  Serial.println(temperature);
 
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Temperatura:   ");
  lcd.setCursor(0,1);
  lcd.print(temperature);
 
  delay(1000); //just here to slow down the output so it is easier to read
 
}
 
float getTemp(){
  //returns the temperature from one DS18B20 in DEG Celsius
 
  byte data[12];
  byte addr[8];
 
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
 
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
 
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
 
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
 
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
 
  ds.reset_search();
 
  byte MSB = data[1];
  byte LSB = data[0];
 
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
 
  return TemperatureSum;
 
}