#include <SoftwareSerial.h> 
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <BlynkSimpleStream.h>
#include <PID_v1.h>
#include "motor.h"

// Global Declarations // ==================================================

char auth[] = "6f20249fa4f84963ab71e22826a97068"; // Blynk authorize token

BlynkTimer timer; // Declaring timer object and globals
int countRemain, countRemainReset, counter;
int LEDtimer, FADEtimer;

int audioIn, motionIn; // ADC sensor values
int prevMotionIn, motionCount, timeout; // Motion trigger variables

bool notifFlag = false; // TRUE - notifications enabled; FALSE - notifications disabled
bool lightFlag = false; // TRUE - lighting sequence enabled; FALSE - lighting sequence disabled
bool selectFlag = false; // TRUE - mode selected; FALSE - mode not selected
bool autoFlag = true; // TRUE - automatic mode enabled; FALSE - automatic mode disabled
bool pauseFlag = false; // TRUE - pause motion; FALSE - continue
bool enabled = false; // TRUE - in motion; FALSE - stop motion

// Declaring Motor PID control globals
PID PIDA(&motorA.in, &motorA.out, &motorA.set, 20, 0, 0, DIRECT);
PID PIDB(&motorB.in, &motorB.out, &motorB.set, 20, 0, 0, DIRECT);
PID PIDC(&motorC.in, &motorC.out, &motorC.set, 20, 0, 0, DIRECT);

// Main // =================================================================

void setup() { // Main Arduino setup // ------------------------------------

  DebugSerial.begin(9600); // Debug console
  Serial.begin(9600); // Blynk USB Serial
  Blynk.begin(Serial, auth);

  motorSetup(); // Setup motor and PID control
  stop();

  blynkSetup(); // Setup Blynk modes
  countdownSetup(); // Setup countdown mode
  lightSetup(); // Setup lighting mode

  sensorInitialise(); // Setup sensor pins 
}

void loop() { // Main Arduino loop // -------------------------------------

  Blynk.run(); // Enables Blynk
  timer.run(); // Enables Blynk countdown

  if (autoFlag) { // If check for motion sensor trigger when automatic mode is enabled
    motionCheck();
  }

  if (enabled) { // If manual mode is activated
   	motorLoop(); // Motor loop functions
  }
  else {
    stop();
  }

  if (lightFlag) {
    timer.enable(LEDtimer); // Start LED loop
  }
  else {
    timer.disable(LEDtimer);
    LEDsetall(0); // Set all LEDs to off
  }
}