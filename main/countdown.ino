void countdownSetup() { // Setup Countdown Timer // ------------------------

  counter = timer.setInterval(1000, countdownTimer);
  timer.disable(counter);
}

void countdownTimer() { // Counts down by 1 second and formats time // -----

  countRemain--; // Decrements by 1 when called
  countformat(countRemain);

  if (!countRemain) { // When countdown reaches 0
    timer.disable(counter);
    Blynk.virtualWrite(vOutText, "Complete");

    pauseFlag = true; // Enable pause flag
  }
}

void countformat(int seconds) {	// Formats timer count to hr:min:sec display string

  int hours = 0;
  int mins = 0;
  int secs = 0;
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

  Blynk.virtualWrite(vOutText, hours + mins_sep + mins + secs_sep + secs); // Writing to Blynk
}