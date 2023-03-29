#include "util.h"
#include "battery.h"
#include "httposter.h"
#include "credentials.h"
#include "logger.h"
#include "payload.h"

Battery battery;
Sender sender;
Logger logger(&Serial, DEBUG);
Payload payload;

void setup() {
  Serial.begin(115200);
  sender = Sender(SSID, PASSWORD, SERVER_ADDRESS, SERVER_PORT);
  battery = Battery(BATTERY_PIN);
}

void loop(){
  payload.add_field("battery", battery.getCharge());
  sender.sendPostRequest("/api/update", payload.to_string());
  delay(1000);
  //clear after sending
  payload.clear();
}