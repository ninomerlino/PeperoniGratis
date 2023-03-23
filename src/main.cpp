#include "util.h"
#include "battery.h"
#include "httposter.h"
#include "credentials.h"

Battery battery;
Sender sender;

void setup() {
  Serial.begin(115200);
  sender = Sender(SSID, PASSWORD, SERVER_ADDRESS, SERVER_PORT);
  battery = Battery(BATTERY_PIN);
}

void loop(){
  battery.getCharge();
  sender.send();
  delay(1000);
}