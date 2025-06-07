#include <Arduino.h>
 #include <SPI.h>
#include <DS18B20.hpp>
#include <DHTesp.h>
#include <Wire.h>
#include <Adafruit_TSL2591.h>
#include <MoistureSensor.hpp>
//PIN CONFIG
#define GROUND_TEMPERATURE_PIN 4
#define AIR_TEMPERATURE_PIN 0
//CONST
#define SLEEP_TIME 5000

//GLOBALS
DS18B20 ground_sensor(GROUND_TEMPERATURE_PIN);
DHTesp air_sensor;
Adafruit_TSL2591 light_sensor = Adafruit_TSL2591(2591);
MoistureSensor moisture_sensor = MoistureSensor(32);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing device");

  air_sensor.setup(AIR_TEMPERATURE_PIN, DHTesp::AUTO_DETECT);

  light_sensor.begin();
  light_sensor.setGain(TSL2591_GAIN_MED);  
  light_sensor.setTiming(TSL2591_INTEGRATIONTIME_400MS);
}

void loop() {
  printf("Ground temperature is: %g\n",ground_sensor.read());
  printf("Air temperature is: %g\n",air_sensor.getTemperature());
  printf("Air humidity is: %g\n",air_sensor.getHumidity());
  printf("Luminosity is: %d\n",light_sensor.getLuminosity(TSL2591_VISIBLE));
  printf("Moisture is: %g\n",moisture_sensor.read());
  delay(SLEEP_TIME);
}