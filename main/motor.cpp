#include <EnableInterrupt.h>
#include "motor.h"

// Initialising motor constructors (default 0)
motorClass motorA(0, 0), motorB(0, 0), motorC(0, 0);

void motorInitialise() {				// Initialising motors

  pinMode(MotorPinA, INPUT_PULLUP); // Initialise Motor A interrupt inputs
  enableInterrupt(MotorPinA, assignEncoderA, FALLING);

  pinMode(MotorPinB, INPUT_PULLUP); // Initialise Motor B interrupt inputs
  enableInterrupt(MotorPinB, assignEncoderB, FALLING);

  pinMode(MotorPinC, INPUT_PULLUP); // Initialise Motor C interrupt inputs
  enableInterrupt(MotorPinC, assignEncoderC, FALLING);
}

void setMode(char mode) {				// Update desired motor position for chosen mode

  switch (mode) {
    case urban:
      motorA.increment = 1; motorB.increment = 1; motorC.increment = 1;
      motorA.offset = 100; motorB.offset = 100; motorC.offset = 100;
      break;

    case motorway:
      motorA.increment = 1; motorB.increment = 1; motorC.increment = 1;
      motorA.offset = 100; motorB.offset = 100; motorC.offset = 100;
      break;

    case underground:
      motorA.increment = 1; motorB.increment = 1; motorC.increment = 1;
      motorA.offset = 100; motorB.offset = 100; motorC.offset = 100;
      break;
  }
}

void motorClass::UpdateSet() {			// Update PID control input parameters

  count += increment; // Running total of motor position

  if (count < 0 ) { // Reset in case of overflow
    encoderPos = 0;
    count = 0;
  }

  set = setScaling; // Update motor PID setpoint
  in = encoderPos; // Update motor PID input
}

void motorClass::Encoder() { 			// Encoder count of the number of motor shaft rotations (pre-gearing)

  encoderPos++; // Increment rotation count (pre-gearing)
}

void assignEncoderA() {

  motorA.Encoder();  // Assign Motor A encoder class functions
}

void assignEncoderB() {

  motorB.Encoder();  // Assign Motor B encoder class functions
}

void assignEncoderC() {

  motorC.Encoder();  // Assign Motor C encoder class functions
}