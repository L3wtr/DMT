void motorSetup() {				// Setup motor and PID control

  motorInitialise(); // Initialise motors

  setMode(urban); // Set initial motor positioning mode

  PIDA.SetMode(AUTOMATIC); // Setup PID controller
  PIDA.SetOutputLimits(outMin, outMax); // Set min and max PWM output (to conserve PWM signal)
}

void motorLoop() {

  motorA.UpdateSet(); // Update PID control input parameters
  motorB.UpdateSet();
  motorC.UpdateSet();

  PIDA.Compute(); // Evaluate new PID output signal
  analogWrite(outPinA, motorA.out); // Set output PWM signal to motor A

  //motorA.DebugDisplay(); // Output debug display
}