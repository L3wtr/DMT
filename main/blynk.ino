// Definitions // ==========================================================

#define vInMode     0
#define vInStart    1
#define vInVehicle  2
#define vInTimer    3
#define vInLight    4
#define vInNotif    5

#define vOutText    6
#define vPlotAudio  7
#define vPlotMotion 8

// Blynk Main Functions // =================================================

void blynkSetup() { // Setup all relevant Blynk settings // ----------------

  // Set Default Button Labels
  Blynk.virtualWrite(vInStart, LOW);
  Blynk.virtualWrite(vInMode, LOW);
  Blynk.virtualWrite(vInTimer, 0);
  Blynk.virtualWrite(vInVehicle, 1); // Defaults to first menu entry (urban)

  Blynk.virtualWrite(vInLight, 2); // Defaults to disabled (option 2)
  Blynk.virtualWrite(vInNotif, 2);

  Blynk.virtualWrite(vOutText, "Please select mode:");
}

// Blynk Push Functions // =================================================

BLYNK_WRITE(vInMode) { // Sets automatic mode on button press // -----------

  pauseAll();
  motionReset(); // Reset motion sensor
  
  if (!selectFlag) {
    selectFlag = true; // Mode selected
  }

  if (param.asInt() == 1) {
    autoFlag = true; // Enable automatic mode flag

    // Pause and disable manual mode if running
    timer.disable(counter);
  }
  else if (param.asInt() == 2) {
    autoFlag = false; // Disable automatic mode flag
  }
}

BLYNK_WRITE(vInStart) { // Starts motion on button press // ----------------

  if (selectFlag == false) {
      Blynk.virtualWrite(vInStart, LOW); // Reset start button if no mode is selected
  }
  else 
    if (param.asInt() == 1) {

      if (autoFlag) {
        Blynk.virtualWrite(vOutText, "Automatic mode active");
      }
      else {

        if (countRemain) {
          timer.enable(counter);

          pauseFlag = false; // Disable pause flag
          enabled = true; // Start motion
        }
        else {
          Blynk.virtualWrite(vInStart, LOW);
          Blynk.virtualWrite(vOutText, "Set timer length in settings");
        }
      }
    }
    else {

      pauseAll();
    }
}

BLYNK_WRITE(vInNotif) { // Enables notifications on button press // --------

  if (param.asInt() == 1) {
    notifFlag = true; // Notifcations are enabled
  }
  else if (param.asInt() == 2){
    notifFlag = false; // Notifications are disabled
  }
}

BLYNK_WRITE(vInTimer) { // Sets manual timer setting on slider change // ---

  if (enabled) { // Pause motion if changing timer settings while enabled

    pauseAll();
  }

  if (timer.isEnabled(counter)) { // If timer is running, prevent change
    Blynk.virtualWrite(vInTimer, param.asInt());
  }
  else { // Update timer
    countRemainReset = param.asInt() * 60 + 1;
    countRemain = param.asInt() * 60;
    countformat(countRemain);
  }
}

BLYNK_WRITE(vInVehicle) { // Sets motion mode on menu change // ------------

  uint8_t selectVehicle = param.asInt();

  switch (selectVehicle) {
    case urban: // Urban road mode
      setVehicle(urban);
    break;

    case motorway: // Motorway mode
      setVehicle(motorway);
    break;

    case underground: // Underground mode
      setVehicle(underground);
    break;

    case bus: // Bus mode
      setVehicle(bus);
    break;
  }
}

BLYNK_WRITE(vInLight) { // Enables light sequence on button press // -------

  if (param.asInt() == 1) {
    lightFlag = true; // Lighting sequence is enabled
  }
  else if (param.asInt() == 2) {
    lightFlag = false; // Lighting sequence is disabled
  }
}

// User Interface Functions // =============================================

void pauseAll() { // Set all pause flags and reset interface

  timer.disable(counter); // Pause timer
  Blynk.virtualWrite(vInStart, LOW);
  Blynk.virtualWrite(vOutText, "Cradle motion paused");

  pauseFlag = true; // Enable pause flag
}