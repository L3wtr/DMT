#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <PID_v1.h>
#include "motor.h"

#define PWM1	5

#define outPinA		3 // Arduino motor output pins (must be PWM output)
#define outPinB		0
#define outPinC		0

PID PIDA(&motorA.in, &motorA.out, &motorA.set, 1, 0, 0, DIRECT); // Declaring Motor A PID control

void setup() {					// Arduino setup

	DebugSerial.begin(9600); // Debug serial baud rate
	Serial.begin(9600);

	motorInitialise(); // Initialise motors
	PIDA.SetMode(AUTOMATIC); // Setup PID controller

  	Serial.println("Setup Complete!");
}

void loop() {					// Arduino loop function

	analogWrite(PWM1,100); // Output PWM signal (Duty Cycle: 100/255)
	PIDA.Compute(); // Evaluate new PID output signal
	analogWrite(outPinA,motorA.out); // Set output PWM signal to motor A
	motorA.DebugDisplay();
}