#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <BlynkSimpleStream.h>
#include <PID_v1.h>
#include "motor.h"

// Global Declarations // ==================================================

char auth[] = "6f20249fa4f84963ab71e22826a97068"; // Blynk authorize token

BlynkTimer timer; // Declaring timer object and globals
int countRemain, countRemainReset, counter;

bool notifFlag = false; // TRUE - notifications enabled; FALSE - notifications disabled
bool lightFlag = false; // TRUE - lighting sequence enabled; FALSE - lighting sequence disabled
bool soundFlag = false; // TRUE - ambient noise enabled; FALSE - ambient noise disabled
bool autoFlag = false; // TRUE - automatic mode enabled; FALSE - automatic mode disabled
bool pauseFlag = false; // TRUE - pause motion; FALSE - continue
bool enabled = false; // TRUE - in motion; FALSE - stop motion

// Declaring Motor A PID control global
PID PIDA(&motorA.in, &motorA.out, &motorA.set, 20, 0, 0, DIRECT);
PID PIDB(&motorB.in, &motorB.out, &motorB.set, 20, 0, 0, DIRECT);
PID PIDC(&motorC.in, &motorC.out, &motorC.set, 20, 0, 0, DIRECT);

// Main // =================================================================

void setup() { // Main Arduino setup // ------------------------------------

  DebugSerial.begin(9600); // Debug console
  Serial.begin(9600); // Blynk USB Serial
  Blynk.begin(Serial, auth);

  motorSetup(); // Setup motor and PID control

  blynkSetup(); // Setup Blynk modes
  countdownSetup(); // Setup countdown mode
}

void loop() { // Main Arduino loop // -------------------------------------

  Blynk.run(); // Enables Blynk
  timer.run(); // Enables Blynk countdown

  if (enabled){
  	motorLoop(); // Motor loop functions
  }
  else {
  	stop();
  }
}