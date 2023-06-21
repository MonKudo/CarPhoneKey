// INCLUSIONS
#include <Arduino.h>
#include "variables.h"
#include "pins.h"
#include "wifi_mqtt.h"
#include "actions.h"
#include "config.h"

void setup() {

  Serial.begin(115200);

  loadConfigData();

  PinSetup();

  WifiSetup();

  MqttSetup();
}

void loop() {
  CurrentTime = millis(); // For timed functions.
  MqttLoop();

  handleClients();

  StartButtonActions();

  CarActions();

  LockingActions();

  // SERIAL INFORMATION
  if (CurrentTime - SerialPreviousTime > SerialInterval) {
    if (digitalRead(TACHOMETER_PIN) == HIGH) {
      Serial.println("Engine is on");
    } else if (digitalRead(TACHOMETER_PIN) == LOW) {
      Serial.println("Status: Engine is off");
    }
    SerialPreviousTime = CurrentTime;
  }
  LastDetected = Detected;
}
