// DS1820 Library
#include <OneWire.h> 
// LCD Library
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);




//Set digital pin number for the termometer
int DS18S20_Pin = 13; 

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2




void setup(void) {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("starting");  
}

void loop(void) {
  lcd.clear();
  float temperature = getTemp();
  Serial.println(temperature);
  lcd.print("Temperature is  ");
  lcd.setCursor(0,1);
  lcd.print(temperature);
  lcd.print ("  C");
  delay(300); //just here to slow down the output so it is easier to read
  
}


float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

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
