#include "util.h"
#include "battery.h"
#include "httposter.h"
#include "credentials.h"
#include "payload.h"
#include "soil.h"
#include "pump.h"

Battery battery;
Sender sender;
Payload payload;
SoilMoistureSensor soil;
Pump pump;

void execution();

void setup() {
  Serial.begin(115200);
  Serial.println("[TIME] Starting up");

  delay(10);

  sender = Sender(SSID, PASSWORD, SERVER_ADDRESS, SERVER_PORT);
  battery = Battery(BATTERY_PIN);
  soil = SoilMoistureSensor(SOIL_PIN);
  payload = Payload();
  pump = Pump(PUMP_PIN);

  delay(1000);

  execution();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);
  Serial.println("[TIME] Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
}

void loop(){
}

void execution(){
  payload.addField("battery", battery.getCharge());
  payload.addField("moisture", soil.getMoisture());
  payload.addField("temperature", 25);
  payload.addField("humidity", 50);
  sender.sendPostRequest("/api/write", payload.toString());
  delay(1000);
  payload.clear();
  pump.irrigate(soil.getMoisture());
}