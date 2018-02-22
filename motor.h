#ifndef motor_h
#define motor_h

class motorClass {
	private:

		char risePin, fallPin; // Motor encoder pin inputs
		char outPin; // Motor output pin
		double setPWM; // Motor setpoint

		volatile bool highFlag; // Toggles between rising and falling states
		volatile int current; // Current clock time
		volatile int previous; // Previous clock time
		volatile int pulse; // Pulse width
		volatile int PWMarray[1]; // Stores last two calculated PWM values

	public:
		char Kp, Ki, Kd; // PID control constants
		double set, in, out; // PID control

		motorClass(char rise, char fall, char output): risePin(rise), fallPin(fall), outPin(output) {} // Motor class constructor

		void Initialise(); // Set up motor pins
		void EncoderPulse(); // Determine the length of the encoder pulse
		void Debouncer(); // Debounce encoder signal base on threshold
		char ReturnOutPin(); // Return motor output pin from private class

};

extern motorClass motorA; // Declaring motor A class
extern motorClass motorB; // Declaring motor B class
extern motorClass motorC; // Declaring motor C class

void assignMotorA(); // Assign the encoder pulse function to each motor when called

#endif