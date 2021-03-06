#include <EnableInterrupt.h>
#include "motor.h"

#define setScaling 	( (0.625*count) - 19 ) // Scale the count mode with encoder speed (linear relationship)

#define MotorPinA	7 // Arduino interrupt input pins (must share the same interrupt vectors)
#define MotorPinB	0
#define MotorPinC 	0

motorClass motorA(0,0), motorB(0,0), motorC(0,0); // Initialising motor constructors

void motorInitialise() {				// Initialising motors
 
	pinMode(MotorPinA, INPUT_PULLUP); // Initialise Motor A interrupt inputs
		enableInterrupt(MotorPinA, assignEncoderA, FALLING);

	pinMode(MotorPinB, INPUT_PULLUP); // Initialise Motor B interrupt inputs
		enableInterrupt(MotorPinB, assignEncoderB, FALLING);

	pinMode(MotorPinC, INPUT_PULLUP); // Initialise Motor C interrupt inputs
		enableInterrupt(MotorPinC, assignEncoderC, FALLING);	
}

void setMode(char mode) {				// Update desired motor position for chosen mode

	switch(mode) {
		case urban:
			motorA.increment = 1;
			motorB.increment = 1;
			motorC.increment = 1;
			break;

		case motorway:
			motorA.increment = 1;
			motorB.increment = 1;
			motorC.increment = 1;
			break;

		case underground:
			motorA.increment = 1;
			motorB.increment = 1;
			motorC.increment = 1;
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

void motorClass::DebugDisplay() { 		// Print out chosen variable to serial monitor

	Serial.print(in); Serial.print(',');
	Serial.print(set); Serial.print(',');
	Serial.println(out);
}

void assignEncoderA() { motorA.Encoder(); } // Assign Motor A encoder class functions

void assignEncoderB() { motorB.Encoder(); } // Assign Motor B encoder class functions

void assignEncoderC() { motorC.Encoder(); } // Assign Motor C encoder class functions