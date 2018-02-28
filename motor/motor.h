#ifndef motor_h
#define motor_h

class motorClass {
	private:
		//double setPWM; // Motor setpoint
		char motorLabel; // Identify motor

		volatile bool highFlag; // Toggles between rising and falling states
		volatile int current; // Current clock time
		volatile int previous; // Previous clock time
		volatile int pulse; // Pulse width
		//volatile int PWMarray[1]; // Stores last two calculated PWM values
		volatile int pulseFiltered[2]; // First order signal smoothing filter array

	public:
		char Kp, Ki, Kd; // PID control constants
		double set, in, out; // PID control

		motorClass(char initialLabel, double initialSet, bool initialFlag):
			motorLabel(initialLabel), set(initialSet), highFlag(initialFlag), pulseFiltered({0,0}) {} // Motor class constructor

		void PulseRise(); // Store the clock time on the PWM rise trigger
		void PulseFall(); // Evaluate the pulse length on the PWM fall trigger
		//void Filter(); // Filter the pulse length signal using a first order smoothing filter

};
extern motorClass motorA; // Declaring motor A class
extern motorClass motorB; // Declaring motor B class
extern motorClass motorC; // Declaring motor C class

void motorInitialise(); // Set up motor pins and interrupt functions

void assignMotorArise(); // Assign the encoder rise function to motor A when called
void assignMotorAfall(); // Assign the encoder fall function to motor A when called
void assignMotorBrise(); // Assign the encoder rise function to motor B when called
void assignMotorBfall(); // Assign the encoder fall function to motor B when called
void assignMotorCrise(); // Assign the encoder rise function to motor C when called
void assignMotorCfall(); // Assign the encoder fall function to motor C when called

#endif
