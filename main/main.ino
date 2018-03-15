#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <BlynkSimpleStream.h>
#include <PID_v1.h>
#include "motor.h"

// Global Declarations // ==================================================

char auth[] = "6f20249fa4f84963ab71e22826a97068"; // Blynk authorize token

BlynkTimer timer; // Declaring timer object and globals
int countRemain, countRemainReset, counter, selectMode;

bool notifEnable; // Declaring notifications global

// Declaring Motor A PID control global
PID PIDA(&motorA.in, &motorA.out, &motorA.set, 20, 0, 0, DIRECT);
PID PIDB(&motorB.in, &motorB.out, &motorB.set, 20, 0, 0, DIRECT);
PID PIDC(&motorC.in, &motorC.out, &motorC.set, 20, 0, 0, DIRECT);

// Main // =================================================================

void setup() {                  // Main Arduino setup

  DebugSerial.begin(9600); // Debug console
  Serial.begin(9600); // Blynk USB Serial
  Blynk.begin(Serial, auth);

  blynkSetup(); // Setup Blynk modes
  countdownSetup(); // Setup countdown mode

  motorSetup(); // Setup motor and PID control
}

void loop() {                   // Main Arduino loop

  Blynk.run(); // Enables Blynk
  timer.run(); // Enables Blynk countdown
  motorLoop(); // Motor loop functions
}