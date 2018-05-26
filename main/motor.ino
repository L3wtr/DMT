void motorSetup() {	// Setup motor and PID control // ----------------------

  motorInitialise(); // Initialise motors

  setVehicle(urban); // Set initial motor positioning vehicle case

  PIDA.SetMode(AUTOMATIC); // Disable PID controller
  PIDB.SetMode(AUTOMATIC);
  PIDC.SetMode(AUTOMATIC);

  PIDA.SetOutputLimits(outMin, outMax); // Set min and max PWM output (to conserve PWM signal)
  PIDB.SetOutputLimits(outMin, outMax);
  PIDC.SetOutputLimits(outMin, outMax);

  motorA.cycleReset = true; // Setting intial position flag
  motorB.cycleReset = true;
  motorC.cycleReset = true;
}

void motorLoop() { // Motor code to be run every Arduino loop // -----------

  // Initiate the next cycle when each motor has reached a flagged position
  if (motorA.cycleReset /*&& motorB.cycleReset && motorC.cycleReset*/) {

    if (orderIndex > sizeof(motionOrder)) { // Reset order index after 20 samples to loop motion
      orderIndex = 0;
    }

    rotate(motionOrder[orderIndex]); // Set values for current rotation order

    motorA.StartEnd(motorA.cycleReset, FromNeutral); // Move to initial position and toggle flags
    motorB.StartEnd(motorB.cycleReset, FromNeutral); 
    motorC.StartEnd(motorC.cycleReset, FromNeutral); 
  }

  else if (motorA.cycleInPos /*&& motorB.cycleInPos && motorC.cycleInPos*/) {

    motorA.Actuation(motorA.cycleInPos, NeutralAct); // Apply actuation motion and toggle flags
    motorB.Actuation(motorB.cycleInPos, NeutralAct);
    motorC.Actuation(motorC.cycleInPos, NeutralAct);
  }

  else if (motorA.cycleEnd /*&& motorB.cycleEnd && motorC.cycleEnd*/) {

    motorA.FlipDir(); motorB.FlipDir(); motorC.FlipDir(); // Reverse direction

    motorA.StartEnd(motorA.cycleEnd, ToNeutral); // Return to neutral position and toggle flags
    motorB.StartEnd(motorB.cycleEnd, ToNeutral); 
    motorC.StartEnd(motorC.cycleEnd, ToNeutral);

    orderIndex++; // Increment order index

    if (pauseFlag) {
      enabled = false; // Pause motion at the end of cycle if selected
    }
  }

  motorA.UpdateSet(); motorB.UpdateSet(); motorC.UpdateSet(); // Update PID control input parameters
  
  PIDA.Compute(); PIDB.Compute(); PIDC.Compute(); // Evaluate new PID output signal

  analogWrite(outPinA, motorA.out); // Set output PWM signal to pin
  analogWrite(outPinB, motorB.out);
  analogWrite(outPinC, motorC.out);
}

void stop() { // Stop platform motion // -----------------------------------

  analogWrite(outPinA, 0); // Set output PWM signal to pin
  analogWrite(outPinB, 0);
  analogWrite(outPinC, 0);
}

void rotate(uint8_t combo) { // Assign the cycle index and directions // -------

  switch (combo) { // See logbook for complete motion table
    case 1:
      motorA.cycleIndex = 2; motorA.dir = reverse;
      motorB.cycleIndex = 1; motorB.dir = forward;
      motorC.cycleIndex = 3; motorC.dir = forward;
    break;

    case 2:
      motorA.cycleIndex = 0; motorA.dir = reverse;
      motorB.cycleIndex = 1; motorB.dir = reverse;
      motorC.cycleIndex = 1; motorC.dir = forward;
    break;

    case 3:
      motorA.cycleIndex = 2; motorA.dir = forward;
      motorB.cycleIndex = 3; motorB.dir = reverse;
      motorC.cycleIndex = 1; motorC.dir = reverse;
    break;

    case 4:
      motorA.cycleIndex = 2; motorA.dir = reverse;
      motorB.cycleIndex = 2; motorB.dir = forward;
      motorC.cycleIndex = 2; motorC.dir = forward;
    break;

    case 5:
      motorA.cycleIndex = 0; motorA.dir = reverse;
      motorB.cycleIndex = 0; motorB.dir = reverse;
      motorC.cycleIndex = 0; motorC.dir = reverse;
    break;

    case 6:
      motorA.cycleIndex = 2; motorA.dir = forward;
      motorB.cycleIndex = 2; motorB.dir = reverse;
      motorC.cycleIndex = 2; motorC.dir = reverse;
    break;

    case 7:
      motorA.cycleIndex = 2; motorA.dir = reverse;
      motorB.cycleIndex = 3; motorB.dir = forward;
      motorC.cycleIndex = 1; motorC.dir = forward;
    break;

    case 8:
      motorA.cycleIndex = 0; motorA.dir = reverse;
      motorB.cycleIndex = 5; motorB.dir = forward;
      motorC.cycleIndex = 5; motorC.dir = reverse;
    break;

    case 9:
      motorA.cycleIndex = 2; motorA.dir = forward;
      motorB.cycleIndex = 1; motorB.dir = reverse;
      motorC.cycleIndex = 3; motorC.dir = reverse;
    break;
  }
}