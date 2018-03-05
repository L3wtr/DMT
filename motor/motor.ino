#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <PID_v1.h>
#include "motor.h"

#define min 		35
#define max 		220

#define outPinA		3 // Arduino motor output pins (must be PWM output)
#define outPinB		0
#define outPinC		0

PID PIDA(&motorA.in, &motorA.out, &motorA.set, 20, 0, 0, DIRECT); // Declaring Motor A PID control

void setup() {					// Arduino setup

	DebugSerial.begin(9600); // Debug serial baud rate
	Serial.begin(9600);

	motorInitialise(); // Initialise motors
	PIDA.SetMode(AUTOMATIC); // Setup PID controller
	PIDA.SetOutputLimits(min, max); // Set min and max PWM output (to conserve PWM signal)

	setMode(urban); // Set initial motor positioning mode

  	Serial.println("Setup Complete!");
}

void loop() {					// Arduino loop function

	motorA.UpdateSet(); // Update PID control input parameters
	motorB.UpdateSet();
	motorC.UpdateSet();

	PIDA.Compute(); // Evaluate new PID output signal
	analogWrite(outPinA,motorA.out); // Set output PWM signal to motor A

	motorA.DebugDisplay();
}