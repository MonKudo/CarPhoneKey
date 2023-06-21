#include <Arduino.h>
#include "variables.h"

// Timer variables
unsigned long CurrentTime = 0UL;
unsigned long SerialPreviousTime = 0UL;
unsigned long SerialInterval = 5000UL;

unsigned long LockingPreviousTime = 0UL;
unsigned long LockingInterval = 1000UL;
bool LockingTimerStarted = false;

unsigned long UnlockingPreviousTime = 0UL;
unsigned long UnlockingInterval = 1000UL;
bool UnlockingTimerStarted = false;

unsigned long StartingPreviousTime = 0UL;
unsigned long StartingInterval = 1500UL;
bool StartingTimerStarted = false;

unsigned long DetectedPreviousTime = 0UL;
unsigned long DetectionInterval = 10000UL;

// Car status varibles
int CarStatus = 0;
int LastCarStatus = CarStatus;
// 0 = Everything Off
// 1 = ACC ON
// 2 = ACC ON + IGNITION 1 & 2 ON (ENGINE OFF)
// 3 = ACC ON + IGNITION 1 & 2 ON + STARTER WIRE ON (CRANKING)
// 4 = ACC ON + IGNITION 1 & 2 ON (ENGINE ON)

//Other variables
bool StartButtonReset = true;
bool Detected = true; // set to false
bool LastDetected = Detected;
