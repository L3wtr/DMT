#ifndef motor_h
#define motor_h

// Definitions // ==========================================================

#define urban		1 // Speed mode label
#define motorway	2
#define underground	3
#define bus         4
#define train       5

#define outMin 		20 // Minimum PID output value
#define outMax 		50 // Maximum PID output value

#define outPinA		3 // Arduino motor output pins (must be PWM output)
#define outPinB		0
#define outPinC		0

#define intPinA	    7 // Arduino interrupt input pins (must share the same interrupt vectors)
#define intPinB	    0
#define intPinC 	0

#define dirPinA     3 // Direction pin number associated with each motor
#define dirPinB     5
#define dirPinC     9

#define forward     HIGH // Forward and reverse boolean
#define reverse     LOW

#define FromNeutral 0 // Associated cycle modes
#define ToNeutral   1
#define NeutralAct  2

#define Zero        0 // Rotation count values corresponding to specified linear actuation
#define ZeroFive    11.2
#define FiveTen     12.1
#define TenFift     14.5
#define FiftTwenty  32.2

#define scalingA    (0.1 * count) // Scaling increment speed calibrated to motor speed
#define scalingB    (0.1 * count)
#define scalingC    (0.1 * count)

// Internal Declarations // ================================================

class motorClass {
  private:
    volatile double encoderCount; // Encoder cycle count
    volatile double count; // Running total set cycle count

    uint8_t cycleMode; // Current cycle mode

    void Reset(); // Reset PID control input parameters

  public:
    volatile double increment; // Scale of count increment
    volatile bool dir; // Motor rotation direction
    volatile uint8_t dirPin; // Associated motor direction pin. Also used as unique object identifier
    volatile double targetCount; // trigger next cycle when target count is met

    double set, in, out; // PID control

    bool cycleReset; // Flag the end of a cycle; to be reset
    bool cycleInPos; // Flag the end of initial position; to start actuation
    bool cycleEnd; // Flag the end of the actuation cycle; to start reverse positioning
    uint8_t cycleIndex; // Cycle motion index

    motorClass(double initialSet, double initialEncoder, double initialCount, uint8_t initialDirPin):
                set(initialSet), encoderCount(initialEncoder), count(initialCount), dirPin(initialDirPin) {} // Motor class constructor

    void UpdateSet(); // Update PID control input parameters
    void Encoder(); // Increment the encoder count when triggered
    void FlipDir(); // Flip motor direction
    void StartEnd(bool &cycleFlag, uint8_t cycleCurrent); // Start and end positioning cycle modes
    void Actuation(bool &cycleFlag, uint8_t cycleCurrent); // Actuation cycle (forward and reverse) modes
    void SetScaling(uint8_t motorLabel); // Assigns set scaling based on motor
};

// External Declarations // ================================================

extern motorClass motorA; // Declaring motor A class
extern motorClass motorB; // Declaring motor B class
extern motorClass motorC; // Declaring motor C class

extern uint8_t orderIndex; // Mode array order index
extern double rotTbl[5]; // Rotation lookup table array
extern double actTbl[4]; // Actuation lookup table array
extern uint8_t motionOrder[20]; // Current motion order
extern uint8_t urbanOrder[20]; // Urban motion preset order
extern uint8_t motorwayOrder[20]; // Motorway motion preset order
extern uint8_t undergroundOrder[20]; // Underground motion preset order
extern uint8_t busOrder[20]; // Bus motion preset order
extern uint8_t trainOrder[20]; // Train motion preset order

void motorInitialise(); // Set up motor pins and interrupt functions
void setMode(uint8_t mode); // Select 1 of 9 motor positioning combination modes

void assignEncoderA(); // Assign the encoder increment function to Motor A when called
void assignEncoderB(); // Assign the encoder increment function to Motor B when called
void assignEncoderC(); // Assign the encoder increment function to Motor C when called

#endif