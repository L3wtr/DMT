#include <EnableInterrupt.h>
#include "motor.h"

#define threshold 	2

motorClass motorA(7,6,3), motorB(0,0,0), motorC(0,0,0); // Defining motor classes

void assignMotorA() {					// Direct encoder pulse function to motor A
	
	motorA.EncoderPulse();
}

void motorClass::Initialise() {			// Initialising motors

	set = 200; // MotorA setpoint
	highFlag = 1;

	switch (outPin){
		case 3: // Initialise MotorA interrupt inputs
			pinMode(risePin, INPUT_PULLUP);
			enableInterrupt(risePin, assignMotorA, RISING);
			pinMode(fallPin, INPUT_PULLUP);
			enableInterrupt(fallPin, assignMotorA, FALLING);
		break;
	}
}

void motorClass::EncoderPulse() {		// Evaluate encoder pulse width using interrupts
	current = millis();

	if (highFlag == 0) {
		previous = current;
		highFlag = 1;
	}
	else {
		pulse = current - previous;
		PWMarray[0] = PWMarray[1];
		PWMarray[1] = pulse/8;
		highFlag = 0;
	}
}

void motorClass::Debouncer() { 			// Ignore small changes in signal (below a threshold)
	int variation = ((PWMarray[1] + PWMarray[0]) / 2) - PWMarray[1];
	Serial.print(pulse); 
  	Serial.println();

	if (abs(variation) > threshold) { // Trigger if the change in pulse width is greater than the threshold
		in = PWMarray[1];
	}
}

char motorClass::ReturnOutPin() {		// Return motor output pin from private class
	return outPin;
}