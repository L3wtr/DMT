#include <EnableInterrupt.h>
#include "motor.h"

// Global Declarations // ==================================================

// Assigning motor constructors (default 0)
motorClass motorA(0, 0, dirPinA), motorB(0, 0, dirPinB), motorC(0, 0, dirPinC);

int orderIndex = 1; // Initial order index

int rotTbl[] = {Zero, ZeroFive, ZeroFive + FiveTen, ZeroFive + FiveTen + TenFift, ZeroFive + FiveTen + TenFift + FiftTwenty}; // Assigning rotation lookup table [0, 0-5, 0-10, 0-15, 0-20]
int actTbl[] = {ZeroFive + ZeroFive, ZeroFive + FiveTen, FiveTen + TenFift, TenFift + FiftTwenty}; // Assigning actuation lookup table [0-5, 5-10, 10-15, 15-20]


int motionOrder[20]; // Mode motion order order arrays
int urbanOrder[] = {1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1}; // Urban mode motion order

void motorInitialise() {          // Initialising motors

  pinMode(MotorPinA, INPUT_PULLUP); // Initialise Motor A interrupt inputs
  enableInterrupt(MotorPinA, assignEncoderA, FALLING);

  pinMode(MotorPinB, INPUT_PULLUP); // Initialise Motor B interrupt inputs
  enableInterrupt(MotorPinB, assignEncoderB, FALLING);

  pinMode(MotorPinC, INPUT_PULLUP); // Initialise Motor C interrupt inputs
  enableInterrupt(MotorPinC, assignEncoderC, FALLING);
}

// External Functions // ===================================================

void setMode(char mode) {         // Update desired motor position for chosen mode

  switch (mode) {
    case urban:
      motorA.increment = 1; motorB.increment = 1; motorC.increment = 1; // Mode speed (### Dynamic Update TBD ####)
      memcpy(motionOrder, urbanOrder, sizeof motionOrder); // Assign the current motion order with urban preset

      break;

    case motorway:
      motorA.increment = 1; motorB.increment = 1; motorC.increment = 1;

      break;

    case underground:
      motorA.increment = 1; motorB.increment = 1; motorC.increment = 1;

      break;
  }
}

// Internal Functions // ===================================================

void motorClass::Reset() {        // Reset encoder and set positions

  encoderPos = 0; // Reset and update motor PID input
  count = 0; 
  set = setScaling; // Reset setpoint
  analogWrite(dirPin, dir); // Update rotation directions
}

void motorClass::UpdateSet() {    // Update PID control input parameters

  count += increment; // Running total of motor position

  if (count < 0 ) { // Reset in case of overflow
    Reset(); // Reset encoder values
  }

  in = encoderPos; // Update motor PID input
}

void motorClass::StartEnd(bool &cycleFlag, int cycleCurrent) {    // Assign target count values from the rotation lookup table

// Offset initial target position by -5 mm
  if (dir == forward) {
    targetCount = rotTbl[cycleIndex - 1];
  }
  else if (dir == reverse) {
    targetCount = rotTbl[cycleIndex + 1];
  }

  Reset(); // Reset encoder values

  cycleMode = cycleCurrent; // Update current cycle mode
  cycleFlag = false; // Set the current flag value to false
}

void motorClass::Actuation(bool &cycleFlag, int cycleCurrent) {   // Assign target count values from the actuation lookup table

  dir = forward; // Set synchronous forward actuation
  targetCount = actTbl[cycleIndex]; // Change encoder target count

  cycleMode = cycleCurrent; // Update current cycle mode
  cycleFlag = false; // Set the current flag value to false
}

void motorClass::Encoder() {      // Encoder count of the number of motor shaft rotations (pre-gearing)

  encoderPos++; // Increment rotation count (pre-gearing)

  if (encoderPos > targetCount && cycleMode == FromNeutral) { // Triggers when in position from neutral
    Reset();
    cycleInPos = true; // Set the next flag to true
  }

  else if (encoderPos > targetCount && cycleMode == NeutralAct && dir == forward) { // Triggers at the top of the actuation cycle
    Reset();
    dir = reverse; // Reverse direction
  }

  else if (encoderPos > targetCount && cycleMode == NeutralAct && dir == reverse) { // Triggers at the bottom of the actuation cycle
      Reset();
      cycleEnd = true; // Set the next flag to true
  }

  else if (encoderPos > targetCount && cycleMode == ToNeutral) { // Triggers when back to neutral
    Reset();
    cycleReset = true; // Set the next flag to true
  }
}

// Assigning Internal Functions To Interrupts // ===========================

void assignEncoderA() {           // Assign Motor A encoder class functions

  motorA.Encoder();
}

void assignEncoderB() {           // Assign Motor B encoder class functions

  motorB.Encoder();  
}

void assignEncoderC() {           // Assign Motor C encoder class functions

  motorC.Encoder();  
}