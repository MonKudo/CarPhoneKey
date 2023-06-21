#include <Arduino.h>

// Connector 1
// 6-wire harness
#define ACC_PIN 12
#define STARTER_PIN 27
#define IGNITION_1_PIN 25
#define IGNITION_2_PIN 26
// 2-wire harness
#define BRAKE_PIN 34
#define TACHOMETER_PIN 22

// Connector 2
// 6-wire harness
#define START_BUTTON_PIN 13
#define undefined_1
#define undefined_2
#define undefined_3
// 2-wire harness
#define LOCK_PIN 5
#define UNLOCK_PIN 23

// RGB Led
#define RED_PIN 19
#define GREEN_PIN 18
#define BLUE_PIN 17

void PinSetup();
