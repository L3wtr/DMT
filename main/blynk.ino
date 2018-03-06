#define vInAuto     0
#define vInManual   1
#define vInNotif    2
#define vInTimer    3
#define vInMode     4

#define vOutTimer   5
#define vOutAuto    6
#define vOutMode    7

void blynkSetup() {               // Setup all relevant Blynk settings

  // Set Default Button Labels
  Blynk.virtualWrite(vInAuto, LOW);
  Blynk.virtualWrite(vInManual, LOW);
  Blynk.virtualWrite(vInNotif, LOW);
  Blynk.virtualWrite(vInTimer, 0);
}

BLYNK_WRITE(vInAuto) {          // Sets automatic mode on button press

  if (param.asInt()) {
    Blynk.virtualWrite(vOutAuto, "Automatic Mode: Enabled");

    // Pause and disable manual mode if running
    Blynk.virtualWrite(vInManual, LOW); // If no time set, prevent change
    timer.disable(counter);
    Blynk.virtualWrite(vOutTimer, "Paused");
  }
  else {
    Blynk.virtualWrite(vOutAuto, "Automatic Mode: Disabled");
  }
}

BLYNK_WRITE(vInManual) {         // Sets manual mode on button press

  if (param.asInt()) {
    if (countRemain) {
      timer.enable(counter);

      // Reset and disable automatic mode if running
      Blynk.virtualWrite(vInAuto, LOW); // If no time set, prevent change
      Blynk.virtualWrite(vOutAuto, "Automatic Mode Disabled");
    }
    else {
      Blynk.virtualWrite(vInManual, LOW); // If no time set, prevent change
      Blynk.virtualWrite(vOutTimer, "Please set time in settings menu");
    }
  }
  else {
    timer.disable(counter); // Pause timer
    Blynk.virtualWrite(vOutTimer, "Paused");
  }
}

BLYNK_WRITE(vInNotif) {          // Enables notifications on button press

  if (param.asInt()) {
    notifEnable = 1; // Notifcations are enabled
  }
  else {
    notifEnable = 0; // Notifications are disabled
  }
}

BLYNK_WRITE(vInTimer) {          // Sets manual timer setting on slider change

  if (timer.isEnabled(counter)) { // If timer is running, prevent change
    Blynk.virtualWrite(vInTimer, param.asInt());
  }
  else { // Update timer
    countRemainReset = param.asInt() * 60 + 1;
    countRemain = param.asInt() * 60;
    countformat(countRemain);
  }
}

BLYNK_WRITE(vInMode) {           // Sets motion mode on menu change

   char speedSetting = param.asInt();
  switch (speedSetting) {
    case 1: // Urban road mode
    Blynk.virtualWrite(vOutMode, 60);
    break;

    case 2: // Motorway mode
    Blynk.virtualWrite(vOutMode, 80);
    break;

    case 3: // Underground mode
    Blynk.virtualWrite(vOutMode, 30);
    break;

    default: // Defaults to urban road mode
    Blynk.virtualWrite(vOutMode, 60);
    break;
  }
}