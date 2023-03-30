#include "util.h"
#include "battery.h"
#include "httposter.h"
#include "credentials.h"
#include "payload.h"

Battery battery;
Sender sender;
Payload payload;

void setup() {
  Serial.begin(115200);
  sender = Sender(SSID, PASSWORD, SERVER_ADDRESS, SERVER_PORT);
  battery = Battery(BATTERY_PIN);
  payload = Payload();
}

void loop(){
  payload.addField("battery", battery.getCharge());
  sender.sendPostRequest("/api/update", payload.toString());
  delay(1000);
  //clear after sending
  payload.clear();
}