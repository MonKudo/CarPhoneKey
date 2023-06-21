#include <Arduino.h>
#include "pins.h"

void PinSetup() {
  // Setting pin modes.
  pinMode(ACC_PIN, OUTPUT);
  pinMode(STARTER_PIN, OUTPUT);
  pinMode(IGNITION_1_PIN, OUTPUT);
  pinMode(IGNITION_2_PIN, OUTPUT);
  pinMode(LOCK_PIN, OUTPUT);
  pinMode(UNLOCK_PIN, OUTPUT);
  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(START_BUTTON_PIN, INPUT);
  pinMode(TACHOMETER_PIN, INPUT_PULLUP);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Setting pin voltages.
  digitalWrite(ACC_PIN, LOW);
  digitalWrite(STARTER_PIN, LOW);
  digitalWrite(IGNITION_1_PIN, LOW);
  digitalWrite(IGNITION_2_PIN, LOW);
  digitalWrite(LOCK_PIN, LOW);
  digitalWrite(UNLOCK_PIN, LOW);

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}
