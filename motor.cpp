#include <EnableInterrupt.h>
#include "motor.h"

#define pulseScaleA ( 300 - pulseFiltered[1]/20 )
#define pulseScaleB ( pulseFiltered[1] )
#define pulseScaleC ( pulseFiltered[1] )

#define alpha		0.9

#define risePinA	7
#define fallPinA	6
#define outPinA		3

#define risePinB	0
#define fallPinB	0
#define outPinB		0

#define risePinC	0
#define fallPinC	0	
#define outPinC		0

motorClass motorA('A',200,LOW), motorB('B',200,HIGH), motorC('C',200,HIGH); // Defining motor classes

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

	current = micros(); // Read current clock time in milliseconds
	previous = current;
	highFlag = HIGH;
}

void motorClass::PulseFall() {			// Evaluate a smoothed PWM value triggered on falling signal

	if (highFlag == HIGH) {
		current = micros(); // Read current clock time in milliseconds
		pulse = current - previous;
		pulseFiltered[1] = pulseFiltered[2]; // Stores previous PWM value to evaluate an average
		pulseFiltered[2] = pulseFiltered[1]*(1 - alpha) + alpha*pulse; // First order smoothing function

		switch(motorLabel) { // Linear relationship to scale the pulse length (ms) to the set PWM value (0 - 255)
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
/*
void motorClass::Filter() { 			// Apply a moving first order signal filter to the pulse length
	int variation = ((PWMarray[1] + PWMarray[0]) / 2) - PWMarray[1];

	if (abs(variation) > threshold) { // Trigger if the change in pulse width is greater than the threshold
		in = PWMarray[1];
		Serial.print(PWMarray[1]); 
  		Serial.println();
	}
}
*/
void assignMotorArise() { motorA.PulseRise(); }
void assignMotorAfall() { motorA.PulseFall(); }

void assignMotorBrise() { motorB.PulseRise(); }
void assignMotorBfall() { motorB.PulseFall(); }

void assignMotorCrise() { motorC.PulseRise(); }
void assignMotorCfall() { motorC.PulseFall(); }