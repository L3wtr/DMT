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

  // Initiate the next cycle when each motor has reached a flagged position
  if (motorA.cycleReset && motorB.cycleReset && motorC.cycleReset) {
    
    orderIndex++; // Increment order index

    if (orderIndex > sizeof(motionOrder)) { // Reset order index after 20 samples to loop motion
      orderIndex = 1;
    }

    rotate(motionOrder[orderIndex]); // Set values for current rotation order

    motorA.StartEnd(motorA.cycleReset, FromNeutral); // Move to initial position and toggle flags
    motorB.StartEnd(motorB.cycleReset, FromNeutral); 
    motorC.StartEnd(motorC.cycleReset, FromNeutral); 
  }

  else if (motorA.cycleInPos && motorB.cycleInPos && motorC.cycleInPos) {

    motorA.Actuation(motorA.cycleInPos, NeutralAct); // Apply actuation motion and toggle flags
    motorB.Actuation(motorB.cycleInPos, NeutralAct);
    motorC.Actuation(motorC.cycleInPos, NeutralAct);
  }

  else if (motorA.cycleEnd && motorB.cycleEnd && motorC.cycleEnd) {

    motorA.dir = !motorA.dir; motorB.dir = !motorB.dir; motorC.dir = !motorC.dir; // Reverse direction

    motorA.StartEnd(motorA.cycleEnd, ToNeutral); // Return to neutral position and toggle flags
    motorB.StartEnd(motorB.cycleEnd, ToNeutral); 
    motorC.StartEnd(motorC.cycleEnd, ToNeutral); 
  }

  motorA.UpdateSet(); motorB.UpdateSet(); motorC.UpdateSet(); // Update PID control input parameters
  
  PIDA.Compute(); PIDB.Compute(); PIDC.Compute(); // Evaluate new PID output signal

  analogWrite(outPinA, motorA.out); // Set output PWM signal to pin
  analogWrite(outPinB, motorB.out);
  analogWrite(outPinC, motorC.out);
}

void rotate(int mode) {   // Assign the discrete rotation counts and directions for each of the nine combinations

  switch (mode) { // See logbook for complete motion table
    case 1:
      motorA.cycleIndex = 2; motorA.dir = reverse;
      motorB.cycleIndex = 1; motorB.dir = forward;
      motorC.cycleIndex = 3; motorC.dir = forward;
    break;

    case 2:
    break;

    case 3:
    break;

    case 4:
    break;

    case 5:
    break;

    case 6:
    break;

    case 7:
    break;

    case 8:
    break;

    case 9:
    break;
  }
}