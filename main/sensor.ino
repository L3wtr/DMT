// Definitions // ==========================================================

#define audioPin  0 // Sensor pins (analog input)
#define motionPin 5
      
#define motionThreshold 3 // Trigger threshold for motion


// Sensors Main Functions // ===============================================

void sensorInitialise() { // Initialising sensors // -----------------------

	pinMode(audioPin, INPUT_PULLUP); // Setup audio analog pin
	pinMode(motionPin, INPUT_PULLUP); // Setup motion analog pin

  	timer.setInterval(1000, pushSensor); // Trigger sensor function every second
}

void pushSensor() { // Send sound level to Blynk SuperChart // -------------

  audioIn = analogRead(audioPin); // Evaluate live sensor data
  motionIn = digitalRead(motionPin);

  Blynk.virtualWrite(vPlotAudio, audioIn); // Display mean data to SuperChart
  Blynk.virtualWrite(vPlotMotion, motionIn);

  timeout++; // Increment timeout count

  if (motionIn != prevMotionIn) { // Trigger edge

    if (motionIn = HIGH) { // Trigger rising edge

      motionCount++; // Increment motion count
    }
  }

  prevMotionIn = motionIn; // Store last measured motion state
}

void motionCheck() { // Check if motion sensor trigger is satisfied --------

	if (timeout = 60) { // Trigger after 60 seconds

		if (motionCount >= motionThreshold) { // If motion is detected more than 2 times
			timeout++; // Increment timeout and trigger automatic motion

			enabled = true; // Start motion
		}
		else { // Reset if motion threshold is not reached
			motionReset();
		}
	}
	else if (timeout > 120) { // Reset automatic mode after 60 seconds
		motionReset();
		pauseFlag = true; // Pause motion at the end of a cycle
	}
}

void motionReset() { // Reset motion triggers ------------------------------

	timeout = 0; // Reset timeout count
	motionCount = 0; // Reset motion count
}