// When Called, Decrements By 1 Second
void countdownTimer() {
  countRemain--; // Decrements by 1 when called
  countformat(countRemain);
  if (!countRemain) { // When countdown reaches 0
    timer.disable(counter);
    Blynk.virtualWrite(1, LOW); // Reset manual start/stop button
    Blynk.virtualWrite(5, "Complete");
  }
}

// On Manual Button Press
BLYNK_WRITE(1) {
  if (param.asInt()){
    if (countRemain) {
      timer.enable(counter);
      // Disable and reset automatic mode
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

// On Changing Time Slider
BLYNK_WRITE(3) {
  if (timer.isEnabled(counter)) { // If timer is running, prevent change
    Blynk.virtualWrite(3, param.asInt()); 
  }
  else { // Update timer
    countRemainReset = param.asInt() * 60 + 1;
    countRemain = param.asInt() * 60;
    countformat(countRemain);
  }
}

void countformat(int seconds) {
  long hours = 0;
  long mins = 0;
  long secs = 0;
  String mins_sep = ":";
  String secs_sep = ":";

  // Converting seconds to hours:minutes:seconds
  secs = seconds;
  mins = secs / 60;
  hours = mins / 60;

  // Formatting to display :59 maximum
  secs = secs - (mins * 60);
  mins = mins - (hours * 60);
  if (secs < 10) {
    secs_sep = ":0";
  }
  if (mins < 10) {
    mins_sep = ":0";
  }

  // Writing to Blynk
  Blynk.virtualWrite(5, hours + mins_sep + mins + secs_sep +secs);
}

// On Automatic Button Press
BLYNK_WRITE(0) {
  if (param.asInt()) {
    Blynk.virtualWrite(6, "Automatic Mode: Enabled");
    // Disable and pause manual mode if running
    Blynk.virtualWrite(1, LOW); // If no time set, prevent change
    timer.disable(counter);
    Blynk.virtualWrite(5, "Paused");
  }
  else {
    Blynk.virtualWrite(6, "Automatic Mode: Disabled");
  }
}

// Notifications Flag
BLYNK_WRITE(2) {
  if (param.asInt()) {
    notifEnable = 1; // Notifcations are enabled
  }
  else {
    notifEnable = 0; // Notifications are disabled
  }
}

// Speed Setting
BLYNK_WRITE(4) {
  speedSetting = param.asInt();
  switch(speedSetting) {
    case 1: // Urban road
      Blynk.virtualWrite(7, 60);
      break;
    case 2: // Motorway
      Blynk.virtualWrite(7, 80);
      break;
    case 3: // Underground
      Blynk.virtualWrite(7, 30);
      break;
    default: // Defaults to gentle car ride
      Blynk.virtualWrite(7, 60);
  }
}