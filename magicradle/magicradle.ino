#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <BlynkSimpleStream.h>
#include <PID_v1.h>
#include <EnableInterrupt.h>

// Unique authorize Token
char auth[] = "6f20249fa4f84963ab71e22826a97068";

// Declaring timer object and variables
BlynkTimer timer;
int countRemain, countRemainReset, counter;

// Declaring Notifications Variable
bool notifEnable;

void setup() {
  // Debug Console
  DebugSerial.begin(9600);

  // Blynk USB Serial
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  // Set Default Button Labels
  Blynk.virtualWrite(0, LOW);
  Blynk.virtualWrite(1, LOW);
  Blynk.virtualWrite(2, LOW);
  Blynk.virtualWrite(3, 0); 

  // Setup Countdown Timer
  counter = timer.setInterval(1000, countdownTimer);
  timer.disable(counter);
}

void loop() {
  Blynk.run();
  timer.run();
}