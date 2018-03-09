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

void motorLoop() {

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