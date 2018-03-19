// Definitions // ==========================================================

#define vInAuto     0
#define vInManual   1
#define vInNotif    2
#define vInTimer    3
#define vInMode     4

#define vOutTimer   5
#define vOutAuto    6
#define vOutMode    7

#define vInLight    8
#define vInSound    9
#define vOutLight   10
#define vOutSound   11

bool test;

// Blynk Main Functions // =================================================

void blynkSetup() { // Setup all relevant Blynk settings // ----------------

  // Set Default Button Labels
  Blynk.virtualWrite(vInAuto, LOW);
  Blynk.virtualWrite(vInManual, LOW);
  Blynk.virtualWrite(vInNotif, LOW);
  Blynk.virtualWrite(vInTimer, 0);

  Blynk.virtualWrite(vInLight, LOW);
      Blynk.virtualWrite(vOutLight, "Press to enable lighting sequence");
  Blynk.virtualWrite(vInSound, LOW);
      Blynk.virtualWrite(vOutSound, "Press to enable ambient noise");
}

BLYNK_WRITE(vInAuto) { // Sets automatic mode on button press // -----------

  if (param.asInt()) {
    Blynk.virtualWrite(vOutAuto, "Automatic Mode: Enabled");

    autoFlag = true; // Enable automatic mode flag

    // Pause and disable manual mode if running
    Blynk.virtualWrite(vInManual, LOW); // If no time set, prevent change
    timer.disable(counter);
    Blynk.virtualWrite(vOutTimer, "Paused");
  }
  else {
    Blynk.virtualWrite(vOutAuto, "Automatic Mode: Disabled");

    autoFlag = false; // Disable automatic mode flag
  }
}

BLYNK_WRITE(vInManual) { // Sets manual mode on button press // -----------

  if (param.asInt()) {
    if (countRemain) {
      timer.enable(counter);

      autoFlag = false; // Disable automatic mode flag
      pauseFlag = false; // Disable pause flag
      enabled = true; // Start motion

      // Reset and disable automatic mode if running
      Blynk.virtualWrite(vInAuto, LOW); // If no time set, prevent change
      Blynk.virtualWrite(vOutAuto, "Automatic Mode: Disabled");
    }
    else {
      Blynk.virtualWrite(vInManual, LOW); // If no time set, prevent change
      Blynk.virtualWrite(vOutTimer, "Please set time in settings menu");
    }
  }
  else {
    timer.disable(counter); // Pause timer
    Blynk.virtualWrite(vOutTimer, "Paused");

    pauseFlag = true; // Enable pause flag
  }
}

BLYNK_WRITE(vInNotif) { // Enables notifications on button press // --------

  if (param.asInt()) {
    notifFlag = true; // Notifcations are enabled
  }
  else {
    notifFlag = false; // Notifications are disabled
  }
}

BLYNK_WRITE(vInTimer) { // Sets manual timer setting on slider change // ---

  if (timer.isEnabled(counter)) { // If timer is running, prevent change
    Blynk.virtualWrite(vInTimer, param.asInt());
  }
  else { // Update timer
    countRemainReset = param.asInt() * 60 + 1;
    countRemain = param.asInt() * 60;
    countformat(countRemain);
  }
}

BLYNK_WRITE(vInMode) { // Sets motion mode on menu change // ---------------

  uint8_t selectMode = param.asInt();

  switch (selectMode) {
    case urban: // Urban road mode
      Blynk.virtualWrite(vOutMode, 60);
      setMode(urban);
    break;

    case motorway: // Motorway mode
      Blynk.virtualWrite(vOutMode, 80);
      setMode(motorway);
    break;

    case underground: // Underground mode
      Blynk.virtualWrite(vOutMode, 20);
      setMode(underground);
    break;

    case bus: // Bus mode
      Blynk.virtualWrite(vOutMode, 40);
      setMode(bus);
    break;

    case train: // High speed train mode
      Blynk.virtualWrite(vOutMode, 100);
      setMode(train);
    break;
  }
}

BLYNK_WRITE(vInLight) { // Enables light sequence on button press // -------

  if (param.asInt()) {
    Blynk.virtualWrite(vOutLight, "Lighting sequence enabled");

    lightFlag = true; // Lighting sequence is enabled
  }
  else {
    Blynk.virtualWrite(vOutLight, "Press to enable lighting sequence");

    lightFlag = false; // Lighting sequence is disabled
  }
}

BLYNK_WRITE(vInSound) { // Enables ambient noise on button press // --------

  if (param.asInt()) {
    Blynk.virtualWrite(vOutSound, "Ambient noise enabled");

    soundFlag = true; // Ambient noise is enabled
  }
  else {
    Blynk.virtualWrite(vOutSound, "Press to enable ambient noise");

    soundFlag = false; // Ambient noise is disabled
  }
}