void blynkSetup() {               // Setup all relevant Blynk settings

  // Blynk USB Serial
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  // Set Default Button Labels
  Blynk.virtualWrite(0, LOW);
  Blynk.virtualWrite(1, LOW);
  Blynk.virtualWrite(2, LOW);
  Blynk.virtualWrite(3, 0);
}

BLYNK_WRITE(0) {                  // Sets automatic mode on button press

  if (param.asInt()) {
    Blynk.virtualWrite(6, "Automatic Mode: Enabled");

    // Pause and disable manual mode if running
    Blynk.virtualWrite(1, LOW); // If no time set, prevent change
    timer.disable(counter);
    Blynk.virtualWrite(5, "Paused");
  }
  else {
    Blynk.virtualWrite(6, "Automatic Mode: Disabled");
  }
}

BLYNK_WRITE(1) {                  // Sets manual mode on button press

  if (param.asInt()) {
    if (countRemain) {
      timer.enable(counter);

      // Reset and disable automatic mode if running
      Blynk.virtualWrite(0, LOW); // If no time set, prevent change
      Blynk.virtualWrite(6, "Automatic Mode Disabled");
    }
    else {
      Blynk.virtualWrite(1, LOW); // If no time set, prevent change
      Blynk.virtualWrite(5, "Please set time in settings menu");
    }
  }
  else {
    timer.disable(counter); // Pause timer
    Blynk.virtualWrite(5, "Paused");
  }
}

BLYNK_WRITE(2) {                  // Enables notifications on button press

  if (param.asInt()) {
    notifEnable = 1; // Notifcations are enabled
  }
  else {
    notifEnable = 0; // Notifications are disabled
  }
}

BLYNK_WRITE(3) {                  // Sets manual timer setting on slider change

  if (timer.isEnabled(counter)) { // If timer is running, prevent change
    Blynk.virtualWrite(3, param.asInt());
  }
  else { // Update timer
    countRemainReset = param.asInt() * 60 + 1;
    countRemain = param.asInt() * 60;
    countformat(countRemain);
  }
}

BLYNK_WRITE(4) {                  // Sets motion mode on menu change

   char speedSetting = param.asInt();
  switch (speedSetting) {
    case 1: // Urban road mode
    Blynk.virtualWrite(7, 60);
    break;

    case 2: // Motorway mode
    Blynk.virtualWrite(7, 80);
    break;

    case 3: // Underground mode
    Blynk.virtualWrite(7, 30);
    break;

    default: // Defaults to urban road mode
    Blynk.virtualWrite(7, 60);
    break;
  }
}
