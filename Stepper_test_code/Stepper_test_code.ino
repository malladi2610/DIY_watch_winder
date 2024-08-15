// //Includes the Arduino Stepper Library
// #include <Stepper.h>

// // Defines the number of steps per rotation
// const int stepsPerRevolution = 2038;

// // Creates an instance of stepper class
// // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
// Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

// void setup() {
//     // Nothing to do (Stepper Library sets pins as outputs)
// }

// void loop() {
// 	// Rotate CW slowly at 5 RPM
// 	myStepper.setSpeed(5);
// 	myStepper.step(stepsPerRevolution);
// 	delay(1000);
	
// 	// Rotate CCW quickly at 10 RPM
// 	myStepper.setSpeed(10);
// 	myStepper.step(-stepsPerRevolution);
// 	delay(1000);
// }

/*****Accelstepper library*********/

// // Include the AccelStepper Library
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Creates an instance
// // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// void setup() {
// 	// set the maximum speed, acceleration factor,
// 	// initial speed and the target position
// 	myStepper.setMaxSpeed(1000.0);
// 	myStepper.setAcceleration(50.0);
// 	myStepper.setSpeed(200);
// 	myStepper.moveTo(2038);
// }

// void loop() {
// 	// Change direction once the motor reaches target position
// 	if (myStepper.distanceToGo() == 0) 
// 		myStepper.moveTo(-myStepper.currentPosition());

// 	// Move the motor one step
// 	myStepper.run();
// }

/******Chatgpt working code (V1.0)**************/
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per rotation
// const int stepsPerRevolution = 2038;

// // Define the number of cycles and pause duration (in milliseconds)
// const int cycles = 40;
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes in milliseconds

// // Creates an instance of the stepper
// // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // Variables to keep track of cycles and timing
// int currentCycle = 0;
// unsigned long lastPauseTime = 0;
// bool isPausing = false;

// void setup() {
//     // Set the maximum speed, acceleration factor, initial speed, and target position
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);
// }

// void loop() {
//     // Check if we are currently pausing between cycles
//     if (isPausing) {
//         // If the pause duration has passed, end the pause
//         if (millis() - lastPauseTime >= pauseDuration) {
//             isPausing = false;
//             currentCycle = 0; // Reset cycle count for the next full cycle
//         }
//     } else {
//         // Perform the winding cycle
//         if (myStepper.distanceToGo() == 0) {
//             if (currentCycle < cycles) {
//                 // Perform a full rotation (CW and CCW)
//                 myStepper.moveTo(currentCycle % 2 == 0 ? stepsPerRevolution : -stepsPerRevolution);
//                 currentCycle++;
//             } else {
//                 // After completing all cycles, start the pause
//                 lastPauseTime = millis();
//                 isPausing = true;
//             }
//         }

//         // Move the motor one step
//         myStepper.run();
//     }
// }


/***************Chatgpt working code v2.0 with serial statments***************************/

// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20;

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // Variables to keep track of the current cycle, number of turns, and timing
// int currentCycle = 0;
// int currentTurn = 0;
// unsigned long lastPauseTime = 0;
// bool isPausing = false;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     // Check if we are currently pausing between cycles
//     if (isPausing) {
//         // If the pause duration has passed, end the pause
//         if (millis() - lastPauseTime >= pauseDuration) {
//             isPausing = false;
//             currentCycle++; // Move to the next cycle
//             currentTurn = 0; // Reset turn count for the next cycle
//             if (currentCycle >= cyclesPerDay) {
//                 currentCycle = 0; // Reset cycle count for the next day
//             }
//             Serial.print("Starting Cycle: ");
//             Serial.println(currentCycle + 1);
//         }
//     } else {
//         // Perform the winding cycle
//         if (myStepper.distanceToGo() == 0) {
//             // Perform a full cycle: 10 turns CW and 10 turns CCW
//             if (currentCycle % 2 == 0) {
//                 myStepper.moveTo(myStepper.currentPosition() + (stepsPerTurn));
//                 currentTurn++;
//                 Serial.print("CW Turn: ");
//             } else {
//                 myStepper.moveTo(myStepper.currentPosition() - (stepsPerTurn));
//                 currentTurn++;
//                 Serial.print("CCW Turn: ");
//             }

//             // Print the current turn number
//             Serial.println(currentTurn);

//             // Check if the cycle is complete (20 turns)
//             if (currentTurn >= turnsPerCycle) {
//                 // After completing a full cycle, start the pause
//                 Serial.println("Cycle Complete. Pausing...");
//                 lastPauseTime = millis();
//                 isPausing = true;
//             }
//         }

//         // Move the motor one step
//         myStepper.run();
//     }
// }


/*****************Chatgpt modified code V2.1***********************/
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20; // 10 CW + 10 CCW

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // Variables to keep track of the current cycle and timing
// int currentCycle = 0;
// unsigned long lastPauseTime = 0;
// bool isPausing = false;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     // Check if we are currently pausing between cycles
//     if (isPausing) {
//         // If the pause duration has passed, end the pause
//         if (millis() - lastPauseTime >= pauseDuration) {
//             isPausing = false;
//             currentCycle++; // Move to the next cycle
//             if (currentCycle >= cyclesPerDay) {
//                 currentCycle = 0; // Reset cycle count for the next day
//             }
//             Serial.print("Starting Cycle: ");
//             Serial.println(currentCycle + 1);
//         }
//     } else {
//         // Perform the winding cycle
//         if (myStepper.distanceToGo() == 0) {
//             if (currentCycle < cyclesPerDay) {
//                 // Perform 10 CW turns
//                 for (int i = 0; i < turnsPerCycle / 2; i++) {
//                     myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
//                     while (myStepper.distanceToGo() != 0) {
//                         myStepper.run();
//                     }
//                     Serial.print("CW Turn: ");
//                     Serial.println(i + 1);
//                 }

//                 // Perform 10 CCW turns
//                 for (int i = 0; i < turnsPerCycle / 2; i++) {
//                     myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
//                     while (myStepper.distanceToGo() != 0) {
//                         myStepper.run();
//                     }
//                     Serial.print("CCW Turn: ");
//                     Serial.println(i + 1);
//                 }

//                 // After completing a full cycle (10 CW + 10 CCW), start the pause
//                 Serial.println("Cycle Complete. Pausing...");
//                 lastPauseTime = millis();
//                 isPausing = true;
//             }
//         }
//     }
// }

/**************Chatgpt code v2.2 including the 35 minutes issue************************/
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20; // 10 CW + 10 CCW

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // Variables to keep track of the current cycle and timing
// int currentCycle = 0;
// unsigned long lastPauseTime = 0;
// bool isPausing = false;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     // Check if we are currently pausing between cycles
//     if (isPausing) {
//         // If the pause duration has passed, end the pause and start the next cycle
//         if (millis() - lastPauseTime >= pauseDuration) {
//             isPausing = false;
//             currentCycle++; // Move to the next cycle
//             if (currentCycle >= cyclesPerDay) {
//                 currentCycle = 0; // Reset cycle count for the next day
//             }
//             Serial.print("Starting Cycle: ");
//             Serial.println(currentCycle + 1);
//         } else {
//             // Still pausing, so do nothing
//             return;
//         }
//     } 

//     // Perform the winding cycle
//     if (myStepper.distanceToGo() == 0) {
//         if (currentCycle < cyclesPerDay) {
//             // Perform 10 CW turns
//             for (int i = 0; i < turnsPerCycle / 2; i++) {
//                 myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CW Turn: ");
//                 Serial.println(i + 1);
//             }

//             // Perform 10 CCW turns
//             for (int i = 0; i < turnsPerCycle / 2; i++) {
//                 myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CCW Turn: ");
//                 Serial.println(i + 1);
//             }

//             // After completing a full cycle (10 CW + 10 CCW), start the pause
//             Serial.println("Cycle Complete. Pausing...");
//             lastPauseTime = millis(); // Record the time when the pause starts
//             isPausing = true;
//         }
//     }
// }

/*******************Chatgpt code v3.0 including statemachine for solving the 35 mins issue*********************/
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20; // 10 CW + 10 CCW

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // State machine enumeration
// enum StateType {
//   W_IDLE,  // 0
//   W_CYCLE, // 1
//   W_PAUSE  // 2
// };

// StateType WState = W_IDLE;  // Initial state
// int currentCycle = 0;
// unsigned long lastPauseTime = 0;
// int currentTurn = 0;
// bool LastMinute = false;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     switch (WState) {
//         case W_IDLE:
//             // Start a new winding cycle
//             if (currentCycle < cyclesPerDay) {
//                 Serial.print("Starting Cycle: ");
//                 Serial.println(currentCycle + 1);
//                 currentTurn = 0;
//                 WState = W_CYCLE;
//             }
//             break;

//         case W_CYCLE:
//             // Perform 10 CW turns
//             if (currentTurn < turnsPerCycle / 2) {
//                 myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CW Turn: ");
//                 Serial.println(currentTurn + 1);
//                 currentTurn++;
//             }
//             // Perform 10 CCW turns
//             else if (currentTurn < turnsPerCycle) {
//                 myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CCW Turn: ");
//                 Serial.println(currentTurn - 9);
//                 currentTurn++;
//             }
//             // After completing a full cycle, start the pause
//             else {
//                 Serial.println("Cycle Complete. Pausing...");
//                 lastPauseTime = millis(); // Record the time when the pause starts
//                 WState = W_PAUSE;
//             }
//             break;

//         case W_PAUSE:
//             // Check if the pause duration has passed
//             if (millis() - lastPauseTime >= pauseDuration) {
//                 currentCycle++;
//                 if (currentCycle >= cyclesPerDay) {
//                     currentCycle = 0; // Reset cycle count for the next day
//                 }
//                 Serial.println("Pause Complete. Resuming next cycle...");
//                 WState = W_IDLE; // Go back to idle to start the next cycle
//             } else if (!LastMinute && (millis() - lastPauseTime >= pauseDuration - 60000)) {
//                 Serial.println("Restarting in 1 minute...");
//                 LastMinute = true;
//             }
//             break;

//         default:
//             // Fallback to disable motor outputs if something goes wrong
//             myStepper.disableOutputs();
//             break;
//     }
// }

/************************Chatgpt v3.1 code with the modified 35 mins logic*********************/

// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20; // 10 CW + 10 CCW

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // State machine enumeration
// enum StateType {
//   W_IDLE,  // 0
//   W_CYCLE, // 1
//   W_PAUSE  // 2
// };

// StateType WState = W_IDLE;  // Initial state
// int currentCycle = 0;
// unsigned long lastPauseTime = 0;
// int currentTurn = 0;
// bool LastMinute = false;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     switch (WState) {
//         case W_IDLE:
//             // Start a new winding cycle
//             if (currentCycle < cyclesPerDay) {
//                 Serial.print("Starting Cycle: ");
//                 Serial.println(currentCycle + 1);
//                 currentTurn = 0;
//                 WState = W_CYCLE;
//             }
//             break;

//         case W_CYCLE:
//             // Perform 10 CW turns
//             if (currentTurn < turnsPerCycle / 2) {
//                 myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CW Turn: ");
//                 Serial.println(currentTurn + 1);
//                 currentTurn++;
//             }
//             // Perform 10 CCW turns
//             else if (currentTurn < turnsPerCycle) {
//                 myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CCW Turn: ");
//                 Serial.println(currentTurn - 9);
//                 currentTurn++;
//             }
//             // After completing a full cycle, start the pause
//             else {
//                 Serial.println("Cycle Complete. Pausing...");
//                 lastPauseTime = millis(); // Record the time when the pause starts
//                 LastMinute = false; // Reset last minute flag
//                 WState = W_PAUSE;
//             }
//             break;

//         case W_PAUSE:
//             // Check if the pause duration has passed
//             if (millis() - lastPauseTime >= pauseDuration) {
//                 currentCycle++;
//                 if (currentCycle >= cyclesPerDay) {
//                     currentCycle = 0; // Reset cycle count for the next day
//                 }
//                 Serial.println("Pause Complete. Resuming next cycle...");
//                 WState = W_IDLE; // Go back to idle to start the next cycle
//             } else if (!LastMinute && (millis() - lastPauseTime >= pauseDuration - 60000)) {
//                 Serial.println("Restarting in 1 minute...");
//                 LastMinute = true;
//             }
//             break;

//         default:
//             // Fallback to disable motor outputs if something goes wrong
//             myStepper.disableOutputs();
//             break;
//     }
// }


/*******************Chatgpt v3.3 changes for the 35 mins delay*********************/
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20; // 10 CW + 10 CCW

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // State machine enumeration
// enum StateType {
//   W_IDLE,  // 0
//   W_CYCLE, // 1
//   W_PAUSE  // 2
// };

// StateType WState = W_IDLE;  // Initial state
// int currentCycle = 0;
// unsigned long lastPauseTime = 0;
// int currentTurn = 0;
// bool LastMinute = false;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     switch (WState) {
//         case W_IDLE:
//             // Start a new winding cycle
//             if (currentCycle < cyclesPerDay) {
//                 Serial.print("Starting Cycle: ");
//                 Serial.println(currentCycle + 1);
//                 currentTurn = 0;
//                 WState = W_CYCLE;
//             }
//             break;

//         case W_CYCLE:
//             // Perform 10 CW turns
//             if (currentTurn < turnsPerCycle / 2) {
//                 myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CW Turn: ");
//                 Serial.println(currentTurn + 1);
//                 currentTurn++;
//             }
//             // Perform 10 CCW turns
//             else if (currentTurn < turnsPerCycle) {
//                 myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
//                 while (myStepper.distanceToGo() != 0) {
//                     myStepper.run();
//                 }
//                 Serial.print("CCW Turn: ");
//                 Serial.println(currentTurn - 9);
//                 currentTurn++;
//             }
//             // After completing a full cycle, start the pause
//             else {
//                 Serial.println("Cycle Complete. Pausing...");
//                 lastPauseTime = millis(); // Record the time when the pause starts
//                 LastMinute = false; // Reset last minute flag
//                 WState = W_PAUSE;
//             }
//             break;

//         case W_PAUSE:
//             // Calculate time remaining for the pause
//             unsigned long timeElapsed = millis() - lastPauseTime;
//             unsigned long timeRemaining = pauseDuration - timeElapsed;

//             // Check if the pause duration has passed
//             if (timeElapsed >= pauseDuration) {
//                 currentCycle++;
//                 if (currentCycle >= cyclesPerDay) {
//                     currentCycle = 0; // Reset cycle count for the next day
//                 }
//                 Serial.println("Pause Complete. Resuming next cycle...");
//                 WState = W_IDLE; // Go back to idle to start the next cycle
//             } 
//             // Notify when 1 minute remains
//             else if (!LastMinute && timeRemaining <= 60000) {
//                 Serial.println("Restarting in 1 minute...");
//                 LastMinute = true;
//             }
//             break;

//         default:
//             // Fallback to disable motor outputs if something goes wrong
//             myStepper.disableOutputs();
//             break;
//     }
// }

/*******************Chatgpt v4.0 code simiplified with delay to solve the 35 mins issue******************************/
// #include <AccelStepper.h>

// // Define step constant
// #define MotorInterfaceType 4

// // Define the number of steps per full revolution of the motor
// const int stepsPerRevolution = 2038;

// // Define the number of steps per turn
// const int stepsPerTurn = stepsPerRevolution / 2; // Adjust this if your motor requires fewer steps per turn

// // Define the number of turns per cycle
// const int turnsPerCycle = 20; // 10 CW + 10 CCW

// // Define the number of cycles per day
// const int cyclesPerDay = 40;

// // Define the pause duration between cycles (35 minutes in milliseconds)
// const unsigned long pauseDuration = 35 * 60 * 1000; // 35 minutes

// // Create an instance of the stepper motor
// AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// // Variables to track cycles and turns
// int currentCycle = 0;
// int currentTurn = 0;

// void setup() {
//     // Initialize Serial communication for debugging
//     Serial.begin(9600);

//     // Set the maximum speed, acceleration, and initial speed
//     myStepper.setMaxSpeed(1000.0);
//     myStepper.setAcceleration(50.0);
//     myStepper.setSpeed(200);

//     // Move to the home position (0 steps) when powered up
//     myStepper.setCurrentPosition(0);
//     myStepper.moveTo(0);

//     // Initial debug message
//     Serial.println("Watch Winder Initialized.");
// }

// void loop() {
//     if (currentCycle < cyclesPerDay) {
//         Serial.print("Starting Cycle: ");
//         Serial.println(currentCycle + 1);

//         // Perform 10 CW turns
//         for (int i = 0; i < turnsPerCycle / 2; i++) {
//             myStepper.moveTo(myStepper.currentPosition() + stepsPerTurn);
//             while (myStepper.distanceToGo() != 0) {
//                 myStepper.run();
//             }
//             Serial.print("CW Turn: ");
//             Serial.println(i + 1);
//         }

//         // Perform 10 CCW turns
//         for (int i = 0; i < turnsPerCycle / 2; i++) {
//             myStepper.moveTo(myStepper.currentPosition() - stepsPerTurn);
//             while (myStepper.distanceToGo() != 0) {
//                 myStepper.run();
//             }
//             Serial.print("CCW Turn: ");
//             Serial.println(i + 1);
//         }

//         // After completing a full cycle, start the pause
//         Serial.println("Cycle Complete. Pausing for 35 minutes...");
//         delay(pauseDuration); // Pause for 35 minutes (2100000 milliseconds)

//         currentCycle++; // Increment cycle count for the next cycle
//     }

//     // After all cycles are completed for the day, reset the counter
//     if (currentCycle >= cyclesPerDay) {
//         Serial.println("All cycles complete for the day.");
//         currentCycle = 0; // Reset cycle count for the next day
//     }
// }

/********************Chatgpt, gemini v5.0 code with additional function to stop the motor from funcitoning during millis************************/
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


