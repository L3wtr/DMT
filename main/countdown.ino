void countdownSetup() {				// Setup Countdown Timer

  counter = timer.setInterval(1000, countdownTimer);
  timer.disable(counter);
}

void countdownTimer() {           	// Decrements counter by 1 second and formats time

  countRemain--; // Decrements by 1 when called
  countformat(countRemain);

  if (!countRemain) { // When countdown reaches 0
    timer.disable(counter);
    Blynk.virtualWrite(vInManual, LOW); // Reset manual start/stop button
    Blynk.virtualWrite(vOutTimer, "Complete");
  }
}

void countformat(int seconds) {		// Formats timer count to hr:min:sec display string

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

  Blynk.virtualWrite(vOutTimer, hours + mins_sep + mins + secs_sep + secs); // Writing to Blynk
}