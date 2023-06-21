#include <Arduino.h>
#include "actions.h"
#include "pins.h"
#include "variables.h"

void StartButtonActions() {
  // Start Button Reset :: Prevents from registerering button click multiples times.
  if (digitalRead(START_BUTTON_PIN) == LOW && StartButtonReset == false) {
    StartButtonReset = true;
  }
  // START BUTTON when ENGINE OFF without BRAKE
  if (digitalRead(TACHOMETER_PIN) == LOW &&
      digitalRead(START_BUTTON_PIN) == HIGH &&
      digitalRead(BRAKE_PIN) == LOW &&
      StartButtonReset == true &&
      StartingTimerStarted == false) {
    // Turn OFF ignition if it is ON.
    if (digitalRead(IGNITION_1_PIN) == HIGH &&
        digitalRead(IGNITION_2_PIN) == HIGH) {
      CarStatus = 1;
      Serial.println("Ignition OFF");
      StartButtonReset = false;
    }
    // Or turn ON ignition if it is OFF.
    else if (digitalRead(IGNITION_1_PIN) == LOW) {
      CarStatus = 2;
      Serial.println("Ignition ON");
      StartButtonReset = false;
    }
  }
  // START BUTTON + BRAKE when ENGINE OFF
  if (digitalRead(TACHOMETER_PIN) == LOW &&
      digitalRead(START_BUTTON_PIN) == HIGH &&
      digitalRead(BRAKE_PIN) == HIGH &&
      StartButtonReset == true &&
      StartingTimerStarted == false) { // If pushing start button and brake is pressed, and it is the first time.
    CarStatus = 3;
    Serial.println("ACC ON, Ignition ON, Starting/Cranking engine...");
    StartingTimerStarted = true;
    StartingPreviousTime = millis();
    StartButtonReset = false;
  }
  // START BUTTON + BRAKE when ENGINE ON
  if (digitalRead(TACHOMETER_PIN) == HIGH &&
      digitalRead(START_BUTTON_PIN) == HIGH &&
      digitalRead(BRAKE_PIN) == HIGH &&
      StartButtonReset == true &&
      StartingTimerStarted == false) {
    CarStatus = 2;
    Serial.println("Ignition OFF");
    StartButtonReset = false;
  } else if (StartingTimerStarted == true && CurrentTime - StartingPreviousTime > StartingInterval) { // If cranking timer has been reached: continue cranking if starting button is still pressed OR stop cranking if starting button is not pressed. // If start button has been pressed, starting the time, then...
    if (digitalRead(START_BUTTON_PIN) == HIGH) { // Continue cranking if start button is still pressed...
      if ((CurrentTime - StartingPreviousTime) % 1000 == 0) {
        Serial.println("Continuing to crank...");
      }
    } else if (digitalRead(START_BUTTON_PIN) == LOW && digitalRead(TACHOMETER_PIN) == LOW) { // Or stop cranking if the start button is not pressed. Cranking unsuccessful, engine is ON.
      CarStatus = 2;
      StartingTimerStarted = false;
      Serial.println("Cranking stopped. Engine's still off.");
    } else if (digitalRead(START_BUTTON_PIN) == LOW && digitalRead(TACHOMETER_PIN) == HIGH) { // Or stop cranking if the start button is not pressed. Cranking successful, engine is OFF.
      CarStatus = 4;
      StartingTimerStarted = false;
      Serial.println("Cranking stopped. Engine started.");
    }
  }
}

void CarActions() {
  if (LastCarStatus != CarStatus && Detected == true)
  {
    if (CarStatus == 0) {
      digitalWrite(ACC_PIN, LOW);
      digitalWrite(IGNITION_1_PIN, LOW);
      digitalWrite(IGNITION_2_PIN, LOW);
      digitalWrite(STARTER_PIN, LOW);
      // LED OFF
      setLed(0,0,0);
    } else if (CarStatus == 1) {
      digitalWrite(ACC_PIN, HIGH);
      digitalWrite(IGNITION_1_PIN, LOW);
      digitalWrite(IGNITION_2_PIN, LOW);
      digitalWrite(STARTER_PIN, LOW);
      // Blue LED
      setLed(0,0,255);
      Serial.println("BLUE");
    } else if (CarStatus == 2) {
      digitalWrite(ACC_PIN, HIGH);
      digitalWrite(IGNITION_1_PIN, HIGH);
      digitalWrite(IGNITION_2_PIN, HIGH);
      digitalWrite(STARTER_PIN, LOW);
      // Orange LED
      setLed(255,50,0);
    } else if (CarStatus == 3) {
      digitalWrite(ACC_PIN, HIGH);
      digitalWrite(IGNITION_1_PIN, HIGH);
      digitalWrite(IGNITION_2_PIN, HIGH);
      digitalWrite(STARTER_PIN, HIGH);
      // Yellow LED
      setLed(255,255,0);
    } else if (CarStatus == 4) {
      digitalWrite(ACC_PIN, HIGH);
      digitalWrite(IGNITION_1_PIN, HIGH);
      digitalWrite(IGNITION_2_PIN, HIGH);
      digitalWrite(STARTER_PIN, LOW);
      // Red LED
      setLed(255,0,0);
    }
    LastCarStatus = CarStatus;
  }
}

void LockingActions() {
  if (LastDetected != Detected) { // If detection changes.
    if (Detected == true && UnlockingTimerStarted == false) { // If detected.
      digitalWrite(UNLOCK_PIN, HIGH); // Unlock
      digitalWrite(LOCK_PIN, LOW);
      digitalWrite(ACC_PIN, HIGH); // Accessory ON
      digitalWrite(STARTER_PIN, LOW);
      digitalWrite(IGNITION_1_PIN, LOW);
      digitalWrite(IGNITION_2_PIN, LOW);
      //Start Timer
      UnlockingPreviousTime = millis();
      UnlockingTimerStarted = true;
    }
    else if (Detected == false && LockingTimerStarted == false && CarStatus != 3 && CarStatus != 4) { // If undetected. Turn off everything and lock the car.
      digitalWrite(UNLOCK_PIN, LOW);
      digitalWrite(LOCK_PIN, HIGH); // Lock
      digitalWrite(ACC_PIN, LOW);
      digitalWrite(STARTER_PIN, LOW);
      digitalWrite(IGNITION_1_PIN, LOW);
      digitalWrite(IGNITION_2_PIN, LOW);
      // Start Timer
      LockingPreviousTime = millis();
      LockingTimerStarted = true;
    }
  }

  if (UnlockingTimerStarted == true && CurrentTime - UnlockingPreviousTime >= UnlockingInterval) { // Turn off unlock relay and timervariable.
    digitalWrite(UNLOCK_PIN, LOW); // Turn off unlock relay.
    UnlockingTimerStarted == false;
    UnlockingPreviousTime = CurrentTime;
  } else if (LockingTimerStarted == true && CurrentTime - LockingPreviousTime >= LockingInterval) { // Turn off lock relay and timervariable.
    digitalWrite(LOCK_PIN, LOW); // Turn off lock relay.
    LockingTimerStarted == false;
  }
}

void setLed(int r, int g, int b){
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
