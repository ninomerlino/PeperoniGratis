#include "util.h"
#include "battery.h"
#include "httposter.h"
#include "credentials.h"

Battery battery;
Sender sender;

void setup() {
  Serial.begin(115200);
  sender = Sender(SSID, PASSWORD, SERVER_ADDRESS, 8000);
  battery = Battery(BATTERY_PIN);
}

void loop(){
  sender.send(battery.getCharge());
  delay(1000);
}