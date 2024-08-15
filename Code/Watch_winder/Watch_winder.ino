#include <AccelStepper.h>

// Define step constant
#define MotorInterfaceType 4

// Define the number of steps per full revolution of the motor
 const int stepsPerRevolution = 2038;

//const int stepsPerRevolution = 4096;

// Define the number of steps per turn
const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// Define the number of turns per cycle
const int turnsPerCycle = 20; // 10 CW + 10 CCW

// Define the number of cycles per day
const int cyclesPerDay = 40;

// Define the pause duration between cycles (35 minutes in milliseconds)
const unsigned long pauseDuration = 15UL * 60UL * 1000UL; // 35 minutes

// Create an instance of the stepper motor
AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// Variables to track cycles and turns
int currentCycle = 0;
int currentTurn = 0;

void setup() {
    // Initialize Serial communication for debugging
    Serial.begin(9600);

    // Set the maximum speed, acceleration, and initial speed
    myStepper.setMaxSpeed(1000.0);
    // myStepper.setAcceleration(50.0);
     myStepper.setAcceleration(800.0);
    myStepper.setSpeed(1000);

    // Move to the home position (0 steps) when powered up
    myStepper.setCurrentPosition(0);
    myStepper.moveTo(0);

    // Initial debug message
    Serial.println("Watch Winder Initialized.");
}

void loop() {
    if (currentCycle < cyclesPerDay) {
        Serial.print("Starting Cycle: ");
        Serial.println(currentCycle + 1);

        // Perform 10 CW turns
        for (int i = 0; i < turnsPerCycle / 2; i++) {
            myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
            while (myStepper.distanceToGo() != 0) {
                myStepper.run();
            }
            Serial.print("CW Turn: ");
            Serial.println(i + 1);
        }

        // Perform 10 CCW turns
        for (int i = 0; i < turnsPerCycle / 2; i++) {
            myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
            while (myStepper.distanceToGo() != 0) {
                myStepper.run();
            }
            Serial.print("CCW Turn: ");
            Serial.println(i + 1);
        }

        // After completing a full cycle, start the pause
        Serial.println("Cycle Complete. Pausing for 35 minutes...");

        // Disable the stepper motor outputs during the pause
        myStepper.disableOutputs();

        // Pause for 35 minutes
        unsigned long startTime = millis();
        while ((unsigned long)(millis() - startTime) < pauseDuration) {
            // Do nothing during the pause
        }

        // Re-enable the stepper motor outputs after the pause
        myStepper.enableOutputs();

        currentCycle++; // Increment cycle count for the next cycle
    }

    // After all cycles are completed for the day, reset the counter
    if (currentCycle >= cyclesPerDay) {
        Serial.println("All cycles complete for the day.");
        currentCycle = 0; // Reset cycle count for the next day
    }
}


