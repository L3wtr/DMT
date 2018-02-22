#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <PID_v1.h>
#include "motor.h"

#define PWM1	5

PID PIDA(&motorA.in, &motorA.out, &motorA.set, 1, 0, 0, DIRECT); // Declaring Motor A PID control

void setup() {					// Arduino setup

	DebugSerial.begin(9600);
	Serial.begin(9600);

	motorA.Initialise(); // Initialise motor A
	PIDA.SetMode(AUTOMATIC); // Setup PID controller

  	Serial.print("Setup Complete!"); 
  	Serial.println();
}

void loop() {

	analogWrite(PWM1,200);
	motorA.Debouncer();
	PIDA.Compute();
	analogWrite(3,motorA.out);
}