#ifndef VARIABLES_H
#define VARIABLES_H
#include <Arduino.h>

// Timer variables
extern unsigned long CurrentTime;
extern unsigned long SerialPreviousTime;
extern unsigned long SerialInterval;

extern unsigned long LockingPreviousTime;
extern unsigned long LockingInterval;
extern bool LockingTimerStarted;

extern unsigned long UnlockingPreviousTime;
extern unsigned long UnlockingInterval;
extern bool UnlockingTimerStarted;

extern unsigned long StartingPreviousTime;
extern unsigned long StartingInterval;
extern bool StartingTimerStarted;

extern unsigned long DetectedPreviousTime;
extern unsigned long DetectionInterval;

// Car status varibles
extern int CarStatus;
extern int LastCarStatus;
// 0 = Everything Off
// 1 = ACC ON
// 2 = ACC ON + IGNITION 1 & 2 ON (ENGINE OFF)
// 3 = ACC ON + IGNITION 1 & 2 ON + STARTER WIRE ON (CRANKING)
// 4 = ACC ON + IGNITION 1 & 2 ON (ENGINE ON)

//Other variables
extern bool StartButtonReset;
extern bool Detected; // set to false
extern bool LastDetected;

#endif
