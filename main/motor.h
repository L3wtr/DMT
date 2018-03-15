#ifndef motor_h
#define motor_h

// Motor Related Defines // ================================================

#define urban		1 // Speed mode label
#define motorway	2
#define underground	3

#define outMin 		0 // Minimum PID output value
#define outMax 		200 // Maximum PID output value

#define outPinA		3 // Arduino motor output pins (must be PWM output)
#define outPinB		0
#define outPinC		0

#define setScaling	( (0.625*count) - 19 ) // Scale the count mode with encoder speed (linear relationship)

#define MotorPinA	7 // Arduino interrupt input pins (must share the same interrupt vectors)
#define MotorPinB	0
#define MotorPinC 	0

#define dirPinA     0 // Direction pin number associated with each motor
#define dirPinB     0
#define dirPinC     0

#define forward     true // Forward and reverse boolean
#define reverse     false

#define FromNeutral 1 // Associated cycle modes
#define NeutralAct  2
#define ToNeutral   3

#define Zero        0 // Rotation count values corresponding to specified linear actuation
#define ZeroFive    0
#define FiveTen     0
#define TenFift     0
#define FiftTwenty  0

// Internal Declarations // ================================================

class motorClass {
  private:
    volatile double encoderPos; // Encoder cycle count
    volatile double count; // Running total set cycle count

    int cycleMode; // Current cycle mode

    void Reset(); // Reset PID control input parameters

  public:
    volatile double increment; // Scale of count increment
    volatile bool dir; // Motor rotation direction
    volatile int dirPin; // Associated motor direction pin
    volatile int targetCount; // trigger next cycle when target count is met

    int Kp, Ki, Kd; // PID control constants
    double set, in, out; // PID control

    bool cycleReset; // Flag the end of a cycle; to be reset
    bool cycleInPos; // Flag the end of initial position; to start actuation
    bool cycleEnd; // Flag the end of the actuation cycle; to start reverse positioning
    int cycleIndex; // Cycle motion index

    motorClass(double initialSet, double initialPos, int initialDirPin):
                set(initialSet), encoderPos(initialPos), dirPin(initialDirPin) {} // Motor class constructor

    void UpdateSet(); // Update PID control input parameters
    void Encoder(); // Increment the encoder count when triggered
    void StartEnd(bool &cycleFlag, int cycleCurrent); // Start and end positioning cycle modes
    void Actuation(bool &cycleFlag, int cycleCurrent); // Actuation cycle (forward and reverse) modes
};

// External Declarations // ================================================

extern motorClass motorA; // Declaring motor A class
extern motorClass motorB; // Declaring motor B class
extern motorClass motorC; // Declaring motor C class

extern int orderIndex; // Mode array order index

extern int rotTbl[5]; // Rotation lookup table array
extern int actTbl[4]; // Actuation lookup table array
extern int motionOrder[20]; // Current motion order
extern int urbanOrder[20]; // Urban rotation preset order

void motorInitialise(); // Set up motor pins and interrupt functions
void setMode(char mode); // Select 1 of 9 motor positioning combination modes

void assignEncoderA(); // Assign the encoder increment function to Motor A when called
void assignEncoderB(); // Assign the encoder increment function to Motor B when called
void assignEncoderC(); // Assign the encoder increment function to Motor C when called

#endif