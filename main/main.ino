#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <BlynkSimpleStream.h>
#include <PID_v1.h>

#include "motor.h"

// Declaring timer object and globals
BlynkTimer timer;
int countRemain, countRemainReset, counter;

// Declaring notifications global
bool notifEnable;

// Declaring Motor A PID control global
PID PIDA(&motorA.in, &motorA.out, &motorA.set, 20, 0, 0, DIRECT);

char auth[] = "6f20249fa4f84963ab71e22826a97068"; // Blynk authorize token

void setup() {                  // Main Arduino setup

  DebugSerial.begin(9600); // Debug console

  blynkSetup(); // Setup Blynk modes
  countdownSetup(); // Setup countdown mode

  motorSetup(); // Setup motor and PID control

  Serial.println("Setup Complete!");
}

void loop() {                   // Main Arduino loop

  Blynk.run(); // Enables Blynk
  timer.run(); // Enables Blynk countdown
  motorLoop(); // Motor loop functions
}
