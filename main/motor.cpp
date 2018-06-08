#include <EnableInterrupt.h>
#include "motor.h"

// Global Declarations // ==================================================

// Assigning motor constructors (default 0)
motorClass motorA(0, 0, 0, dirPinA), motorB(0, 0, 0, dirPinB), motorC(0, 0, 0, dirPinC);

uint8_t orderIndex = 0; // Initial order index

double rotTbl[] = {Zero, ZeroFive, ZeroFive + FiveTen, ZeroFive + FiveTen + TenFift, ZeroFive + FiveTen + TenFift + FiftTwenty}; // Assigning rotation lookup table [0, 0-5, 0-10, 0-15, 0-20]
double actTbl[] = {ZeroFive + ZeroFive, ZeroFive + FiveTen, FiveTen + TenFift, TenFift + FiftTwenty}; // Assigning actuation lookup table [-5-5, 0-10, 5-15, 10-20]

uint8_t motionOrder[20]; // Mode motion order order arrays
//uint8_t urbanOrder[] = {5,5,5,5,5, 5,5,5,5,5, 5,5,5,5,5, 5,5,5,5,5}; // Urban mode motion order
uint8_t urbanOrder[] = {7,7,7,7,7, 4,5,5,5,5, 5,6,5,5,5, 5,5,5,5,5}; // Urban mode motion order
uint8_t motorwayOrder[] = {7,3,4,5,5, 5,8,5,5,8, 8,6,6,3,6, 3,3,2,2,4}; // Urban mode motion order
uint8_t undergroundOrder[] = {5,5,8,7,9, 6,6,6,5,5, 2,2,5,5,4, 4,5,2,2,2}; // Urban mode motion order
uint8_t busOrder[] = {2,2,3,6,9, 6,5,4,7,4, 1,1,4,5,5, 5,5,5,6,9}; // Urban mode motion order

// External Functions // ===================================================

void motorInitialise() { // Initialising motors // -------------------------

  pinMode(intPinA, INPUT_PULLUP); // Initialise Motor A interrupt inputs
  enableInterrupt(intPinA, assignEncoderA, FALLING);

  pinMode(intPinB, INPUT_PULLUP); // Initialise Motor B interrupt inputs
  enableInterrupt(intPinB, assignEncoderB, FALLING);

  pinMode(intPinC, INPUT_PULLUP); // Initialise Motor C interrupt inputs
  enableInterrupt(intPinC, assignEncoderC, FALLING);

  pinMode(dirPinA, OUTPUT); // Initialise motor direction pins
  pinMode(dirPinB, OUTPUT);
  pinMode(dirPinC, OUTPUT);
}

// External Functions // ===================================================

void setVehicle(uint8_t vehicle) { // Update desired motor position for chosen vehicle case //

  switch (vehicle) {
    case urban:
      motorA.increment = 0.05; motorB.increment = 0.05; motorC.increment = 0.2; // Mode speed (### Dynamic Update TBD ####)
      memcpy(motionOrder, urbanOrder, sizeof motionOrder); // Assign the current motion order with urban preset

      break;

    case motorway:
      motorA.increment = 0.05; motorB.increment = 0.05; motorC.increment = 0.2;
      memcpy(motionOrder, motorwayOrder, sizeof motionOrder); // Assign the current motion order with urban preset

      break;

    case underground:
      motorA.increment = 0.05; motorB.increment = 0.05; motorC.increment = 0.2;
      memcpy(motionOrder, undergroundOrder, sizeof motionOrder); // Assign the current motion order with urban preset

      break;

    case bus:
      motorA.increment = 0.05; motorB.increment = 0.05; motorC.increment = 0.2;
      memcpy(motionOrder, busOrder, sizeof motionOrder); // Assign the current motion order with urban preset

      break;
  }
}

// Internal Functions // ===================================================

void motorClass::SetScaling(uint8_t motorLabel) { // Assigns set scaling based on direction pin passed //

  switch(motorLabel) {
    case dirPinA:
      set = scalingA;
    break;

    case dirPinB:
      set = scalingB;
    break;

    case dirPinC:
      set = scalingC;
    break;
  }
}

void motorClass::Reset() { // Reset encoder and set positions // -----------

  encoderCount = 0; // Reset and update motor PID input
  count = 0; 
  SetScaling(dirPin); // Reset setpoint
  digitalWrite(dirPin, dir); // Update respective rotation directions
}

void motorClass::UpdateSet() { // Update PID control input parameters // ---

  count += increment; // Running total of motor position

  if (count < 0 ) { // Reset in case of overflow
    Reset(); // Reset encoder values
  }

  SetScaling(dirPin); // Reset setpoint
  in = encoderCount; // Update motor PID input
}

void motorClass::StartEnd(bool &cycleFlag, uint8_t cycleCurrent) { // Assign target count values from the rotation lookup table //

  // Offset initial target position by -5 mm
  if (dir ^ cycleCurrent) { // Bitwise XOR to identify forward/reverse, start/end combinations
    targetCount = rotTbl[cycleIndex - 1];
  }
  else {
    targetCount = rotTbl[cycleIndex + 1];
  }

  Reset(); // Reset encoder values

  cycleMode = cycleCurrent; // Update current cycle mode
  cycleFlag = false; // Set the current flag value to false

  //delay(100);
}

void motorClass::Actuation(bool &cycleFlag, uint8_t cycleCurrent) { // Assign target count values from the actuation lookup table //

  dir = forward; // Set synchronous forward actuation
  targetCount = actTbl[cycleIndex]; // Change encoder target count

  Reset(); // Reset encoder values

  cycleMode = cycleCurrent; // Update current cycle mode
  cycleFlag = false; // Set the current flag value to false

  //delay(100);
}

void motorClass::FlipDir() { // Flip motor direction // --------------------
  if (dir) {
    dir = LOW;
  }
  else {
    dir = HIGH;
  }
}

void motorClass::Encoder() { // Encoder count of the number of motor shaft rotations (pre-gearing) //

  encoderCount++; // Increment rotation count (pre-gearing)

  if (encoderCount > targetCount && cycleMode == FromNeutral) { // Triggers when in position from neutral
    cycleInPos = true; // Set the next flag to true
  }

  else if (encoderCount > targetCount && cycleMode == NeutralAct && dir == forward) { // Triggers at the top of the actuation cycle
    dir = reverse; // Reverse direction
    Reset();
  }

  else if (encoderCount > targetCount && cycleMode == NeutralAct && dir == reverse) { // Triggers at the bottom of the actuation cycle
    cycleEnd = true; // Set the next flag to true
  }

  else if (encoderCount > targetCount && cycleMode == ToNeutral) { // Triggers when back to neutral
    cycleReset = true; // Set the next flag to true
  }
}

// Assigning Internal Functions To Interrupts // ===========================

void assignEncoderA() { // Assign Motor A encoder class functions // -------

  motorA.Encoder();
}

void assignEncoderB() { // Assign Motor B encoder class functions // -------

  motorB.Encoder();  
}

void assignEncoderC() { // Assign Motor C encoder class functions // -------

  motorC.Encoder();  
}