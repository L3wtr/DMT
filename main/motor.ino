void motorSetup() {				// Setup motor and PID control

  motorInitialise(); // Initialise motors

  setMode(urban); // Set initial motor positioning mode

  PIDA.SetMode(AUTOMATIC); // Setup PID controller
  PIDB.SetMode(AUTOMATIC);
  PIDC.SetMode(AUTOMATIC);

  PIDA.SetOutputLimits(outMin, outMax); // Set min and max PWM output (to conserve PWM signal)
  PIDB.SetOutputLimits(outMin, outMax);
  PIDC.SetOutputLimits(outMin, outMax);
}

void motorLoop() {        // Motor code to be run every Arduino loop

  randomOffset(); // Roll to offset motors

  motorA.UpdateSet(); // Update PID control input parameters
  motorB.UpdateSet();
  motorC.UpdateSet();

  PIDA.Compute(); // Evaluate new PID output signal
  PIDB.Compute();
  PIDC.Compute();

  analogWrite(outPinA, motorA.out); // Set output PWM signal to pin
  analogWrite(outPinB, motorB.out);
  analogWrite(outPinC, motorC.out);
}

void randomOffset() {     // Chance to randomly offset a motor (without replacement)

  int change = random(3) + 1; // 1/3 chance of motor offset

  if (change == 1) {
    switch (order[motorIndex]) { // Select which motor to offset based on shuffled order
      case 1:
        motorA.UpdateSet();
        motorIndex++;
        break;
      case 2:
        motorB.UpdateSet();
        motorIndex++;
        break;
      case 3:
        motorC.UpdateSet();
        motorIndex++;
        break;
    }

    if (motorIndex > 2) { // Reset motor index when all motors have been offset

      motorIndex = 0;
      shuffle();
    }

    motorA.UpdateOffset(); // Update motor count offsets
    motorB.UpdateOffset();
    motorC.UpdateOffset();
  }
}

void shuffle() {          // Pseudo-random shuffle function for array of [1,2,3]

  int swapFrom = random(3); // Swap from random index (0, 1 or 2)
  int swapTo = random(3); // Swap to random index (0, 1 or 2)
  int temp = order[swapTo]; // Recall number to be replaced

  order[swapTo] = order[swapFrom];
  order[swapFrom] = temp; // Set new shuffled order
}