/*
The code print the data in the following order:
Time(sec),Infrared Light(in candelas(cd)),Visible Light(in candelas(cd)),Heater On/Off(1/0),Air Temperature(°C),Air Humidity(%),Water Temperature(°C),PH(0-14),Electrical Conductivity(micro siemens),CO2(parts per million(ppm))
*/

//libraries:
#include "DHT.h"
#define DHTPIN1 2 //Yellow
#define DHTPIN2 3 //Green
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h" 
#include "Adafruit_CCS811.h"    

//DHT Sensors Setup:
DHT dht[] = {
  {DHTPIN1, DHT22},
  {DHTPIN2, DHT11}
};

//Gas Sensor Initiation:
Adafruit_CCS811 ccs;

//PH values:
int PHpin = A2;
float rawCountPH; 
float voltagePH; 
float sensorValuePH;

//pump:
int pumpPin = 8; 

//time:
long double timeStep = 60000; //60000 <- 1 minute
long double totalTime = 0;

//Conductivity values:
int Cpin = A3;
float rawCountC; 
float voltageC; 
float sensorValueC;
float slopeC = 3257.7;
float interceptC = -2659.0;
float prevCon = 0;

//DHT Sensor values:
int tempPin = 9;
int humPin = 10;
float humidity[2];
float temperature[2];
float lastTemp = 0;
float lastHumidity = 0;

//Light Sensor Initiation:
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

//Configuration/Calibration of Light Sensor:
void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW); // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED); // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH); // 428x gain
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); // shortest integration time (bright light)
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS); // longest integration time (dim light)
}

//Light Sensor Reading Method:
void advancedRead(void)
{
// More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
// That way you can do whatever math and comparisons you want!
uint32_t lum = tsl.getFullLuminosity();
uint16_t ir, full;
ir = lum >> 16;
full = lum & 0xFFFF;
Serial.print(ir); Serial.print(",");
Serial.print(full - ir); Serial.print(",");
}

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.print(frac,DEC) ;
}

void setup() {
  Serial.begin(9600);
  
  //DHT Sensor Startup:
  for (auto& sensor : dht) {
    sensor.begin();
  }
  
  //Light Sensor Startup:
  tsl.begin();
  configureSensor();

  //Gas Sensor Startup:
  ccs.begin();

  //Outputs:
  pinMode(tempPin, OUTPUT);
  pinMode(humPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);      
  
  //Inputs:
  pinMode(PHpin, INPUT);
  pinMode(Cpin, INPUT);
}

void loop() {
  //Signal for pump:
  digitalWrite(pumpPin, HIGH);
  
  //Mandatory delay (don't remove):
  delay(timeStep);
  totalTime += (timeStep/1000);
  printDouble(totalTime, 0); Serial.print(",");

  //Light Sensor Read:
  advancedRead();

  //Reading DHT Sensor:
  for (int i = 0; i < 2; i++) {
    temperature[i] = dht[i].readTemperature();
    humidity[i] = dht[i].readHumidity();
  }

  //DHT Logic \/
  //test code temp:
  /*
  if(true){
    analogWrite(tempPin, 255);
  }
  */
  //Actual code temp:
  if(lastTemp<18.0){
    digitalWrite(tempPin, HIGH);
    Serial.print(1); Serial.print(","); //print ligical 1 for Heater ON
  }
  else{
    digitalWrite(tempPin, LOW);
    Serial.print(0); Serial.print(","); //print ligical 0 for Heater OFF
  }
  //test code hum:
  /*
  if(true){
    analogWrite(humPin, 255);
  }
  */
  //Actual code hum:
  if(lastHumidity<50.0 || lastHumidity>70.0){
    analogWrite(humPin, 255);
  }
  else if(lastHumidity>50.0 && lastHumidity<60.0){
    int voltage = (60-lastHumidity)*25.5;
    analogWrite(humPin, voltage);
  }
  else if(lastHumidity>60.0 && lastHumidity<70.0){
    int voltage = (70-lastHumidity)*25.5;
    digitalWrite(humPin, voltage);
  }
  
  //Check if Air Temperature is a digit:
  if (isnan(temperature[0])) {  //tests if the temperature is a digit
    Serial.print(temperature[0]); Serial.print(",");
    lastTemp = temperature[0];
  }
  else {
    Serial.print(temperature[0]); Serial.print(",");
    temperature[0] = lastTemp; //save last uncorrupted temperature
  }
  
  //Check if Air Humididty is a digit:
  if (isnan(humidity[0])) {  //tests if humidity is a digit
    Serial.print(humidity[0]); Serial.print(",");
    lastHumidity = humidity[0];
  }
  else {
    Serial.print(humidity[0]); Serial.print(",");
    humidity[0] = lastHumidity; //save last uncorrupted humidity
  }
  
  Serial.print(temperature[1]); Serial.print(",");
  //DHT Logic /\

  //PH Logic:
  rawCountPH =analogRead(PHpin); 
  voltagePH=rawCountPH/1023*5; 
  sensorValuePH = -3.75*voltagePH + 14; //convert voltage to PH (0-14)
  Serial.print(sensorValuePH); Serial.print(",");
  
  //Conductivity Logic:
  rawCountC=analogRead(Cpin); 
  voltageC=rawCountC/1023*5; 
  sensorValueC=voltageC*7532; //convert voltage to micro siemens
  if(sensorValueC<0){
    sensorValueC = prevCon;
  }
  else{
    prevCon = sensorValueC;
  }
  Serial.print(sensorValueC); Serial.print(",");

  //Gas Sensor Logic:
  if(!ccs.readData()){
    Serial.print(ccs.geteCO2());
  }
  else{
    Serial.println("ERROR!");
    while(1);
  }

  //Mandatory new line (don't remove):
  Serial.println();
}
