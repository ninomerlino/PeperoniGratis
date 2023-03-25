#include "util.h"
#include "battery.h"
#include "httposter.h"
#include "credentials.h"
#include "logger.h"

Battery battery;
Sender sender;
Logger logger(&Serial, DEBUG);

void setup() {
  Serial.begin(115200);
  sender = Sender(SSID, PASSWORD, SERVER_ADDRESS, SERVER_PORT);
  battery = Battery(BATTERY_PIN);
}

void loop(){
  sender.send(battery.getCharge());
  delay(1000);
}