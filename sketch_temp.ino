#include <LiquidCrystal.h>

#define SECS 1000

#define TMP_INTERVAL 10

#define N (SECS/TMP_INTERVAL)


LiquidCrystal lcd(12,11,5,4,3,2);
/*

 */

// named constant for the pin the sensor is connected to
const int sensorPin = A0;
//const int switchPin= = 6;
//int prevSwitchState = 0;
float * tmpVector;
int tmpPosition = 0;
float temperature;
long previousMillis = 0;        // will store last time LED was updated

const float refVoltage=5;
const float refShift = 0;
float getTemp(){
   // read the value on AnalogIn pin 0 
  // and store it in a variable  
  float voltage;
  float sensorVal;
  sensorVal = analogRead(sensorPin);
  float tmp;
  // send the 10-bit sensor value out the serial port
  Serial.print("sensor Value: ");
  Serial.print(sensorVal); 

  // convert the ADC reading to voltage
  voltage = (sensorVal/1024.0) * refVoltage;

  // Send the voltage level out the Serial port
  Serial.print(", Volts: ");
  Serial.print(voltage);

  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((volatge - 500mV) times 100)
  
  Serial.print(", degrees C: "); 
  /****** HO MESSO 0.45 per TARARLO!!!*/
  tmp = (voltage - .5) * 100 + refShift;
  Serial.println(tmp);
  return tmp;
}

void addTmpMeasure (float tmp){
  tmpVector[tmpPosition] = tmp;
  tmpPosition = (tmpPosition+1)%N;
  Serial.print("tmp Position: ");
  Serial.println(tmpPosition);
}

void printLCD(float tmpAvg, float tmpStd){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature");
    lcd.setCursor(0,1);
    lcd.print(tmpAvg);
    lcd.print(" +- ");
    lcd.print(tmpStd);  
}



void setup(){
  float tmpInst;
  int timeSetupCount = 0;
  // Memory allocation
  tmpVector = (float*) malloc(sizeof(float)*N);
  // open a serial connection to display values
  Serial.begin(9600);
  lcd.begin(16,2);
  //pinMode(
  lcd.setCursor(0,0);
  lcd.print("Setup");
  lcd.setCursor(0,1);
  lcd.print("Initializing...");
  tmpInst = getTemp();
 while (timeSetupCount < N){
     addTmpMeasure( tmpInst);
     delay(TMP_INTERVAL);
     timeSetupCount++;
   }
  lcd.clear();
}

void loop(){
  int i;
  float tmpAvg= 0;
  float tmpStd = 0;
//  const long int refreshInterval = 1;
 // long int lastRefresh = 0;
  
  // the follow variables is a long because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  long interval = 1000;           // interval at which to blink (milliseconds)


  temperature=getTemp();
  addTmpMeasure(temperature);
  for (i=0; i<N;i++){
     tmpAvg += tmpVector[i];
  }
  tmpAvg /= (N*1.0);
  for (i=0; i<N;i++){
     tmpStd += (tmpVector[i] - tmpAvg)*(tmpVector[i] - tmpAvg);
  }
  tmpStd = sqrt(tmpStd/ (N*N*1.0));
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;    
    printLCD(tmpAvg,tmpStd);
  }
  delay(TMP_INTERVAL);  
}
