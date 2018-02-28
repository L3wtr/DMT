#include <EnableInterrupt.h>
#include "motor.h"

#define pulseScaleA ( pulseFiltered[2] ) // Calibrated pulse length to PWM value linear scaling functions
#define pulseScaleB ( pulseFiltered[1] )
#define pulseScaleC ( pulseFiltered[1] )

#define alpha		0.5 // First order signal smoothing constant

#define risePinA	7 // Motor A Arduino interrupt input pins (must share the same interrupt vectors)
#define fallPinA	6

#define risePinB	0 // Motor B Arduino interrupt input pins (must share the same interrupt vectors)
#define fallPinB	0

#define risePinC	0 // Motor C Arduino interrupt input pins (must share the same interrupt vectors)
#define fallPinC	0	

motorClass motorA('A',100,LOW), motorB('B',200,HIGH), motorC('C',200,HIGH); // Defining motor classes

void motorInitialise() {				// Initialising motors
 
	pinMode(risePinA, INPUT_PULLUP); // Initialise motor A interrupt inputs
		enableInterrupt(risePinA, assignMotorArise, RISING);
	pinMode(fallPinA, INPUT_PULLUP);
		enableInterrupt(fallPinA, assignMotorAfall, FALLING);

	pinMode(risePinB, INPUT_PULLUP); // Initialise motor B interrupt inputs
		enableInterrupt(risePinB, assignMotorBrise, RISING);
	pinMode(fallPinB, INPUT_PULLUP);
		enableInterrupt(fallPinB, assignMotorBfall, FALLING);		

	pinMode(risePinC, INPUT_PULLUP); // Initialise motor C interrupt inputs
		enableInterrupt(risePinC, assignMotorCrise, RISING);
	pinMode(fallPinC, INPUT_PULLUP);
		enableInterrupt(fallPinC, assignMotorCfall, FALLING);		
}

void motorClass::PulseRise() {			// Evaluate a previous time triggered on rising signal

	current = micros(); // Read current clock time in microseconds
	previous = current;
	highFlag = HIGH;
}

void motorClass::PulseFall() {			// Evaluate a smoothed PWM value triggered on falling signal

	if (highFlag == HIGH) {
		current = micros(); // Read current clock time in microseconds
		pulse = current - previous; // Calculate pulse length
		pulseFiltered[1] = pulseFiltered[2]; // Stores previous filtered pulse term
		pulseFiltered[2] = pulseFiltered[1]*(1 - alpha) + alpha*pulse; // First order smoothing function

		switch(motorLabel) { // Apply linear relationship to scale the pulse length (ms) to the set PWM value (0 - 255)
			case 'A': 
				in = pulseScaleA;
				break;
			case 'B': 
				in = pulseScaleB;
				break;
			case 'C': 
				in = pulseScaleC;
				break;
		}

		highFlag = LOW;
	}
}

void motorClass::DebugDisplay() { // Print out chosen varial to serial monitor

  	Serial.println(out);
}

void assignMotorArise() { motorA.PulseRise(); } // Assign motor A rise and fall class functions
void assignMotorAfall() { motorA.PulseFall(); }

void assignMotorBrise() { motorB.PulseRise(); } // Assign motor B rise and fall class functions
void assignMotorBfall() { motorB.PulseFall(); }

void assignMotorCrise() { motorC.PulseRise(); } // Assign motor C rise and fall class functions
void assignMotorCfall() { motorC.PulseFall(); }