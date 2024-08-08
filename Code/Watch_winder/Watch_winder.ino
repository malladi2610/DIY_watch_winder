// Configuration
#define CYCLES 5
#define ROT_R 2  //default 2
#define ROT_L 2  //default 2
#define PAUSE_MIN 35  //default 35

// Pins
#define LED_PIN 5

// Motor
#define ROT_SPEED 1000
#define ROT_ACCEL 800
#define ROT_PAUSE 4000
#define ROT_STEPS 4096

#include <jled.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper winder(8, 8, 10, 9, 11, false);
JLed pwr_led = JLed(LED_PIN).FadeOn(1000);

enum StateType
{
  W_HOME,  // 0
  W_CYCLE, // 1
  W_RIGHT, // 2
  W_LEFT,  // 3
  W_STOP,  // 4
  W_PAUSE  // 5
};

StateType WState = W_HOME;
int Rotations = CYCLES;
int TargetPos = 0;
long StartTime = 0;
bool LastMinute = false;

void setup()
{
  Serial.begin(115200);

  winder.setMaxSpeed(ROT_SPEED);
  winder.setAcceleration(ROT_ACCEL);
  WState = W_HOME;

  Serial.println("<< Winder: Initializing, moving to home position");
}

void loop()
{
  StateType old_state = WState;

  switch (WState)
  {
  case W_HOME:
    // Move to home position
    winder.moveTo(0);
    while (winder.distanceToGo() != 0)
    {
      winder.run();
    }
    winder.disableOutputs();
    WState = W_CYCLE;
    StartTime = millis();
    Rotations = CYCLES;
    break;

  case W_CYCLE:
    if ((Rotations--) > 0)
    {
      Serial.print("## Cycles until Pause: ");
      Serial.println(Rotations);
      WState = W_RIGHT;
      TargetPos -= (ROT_R * ROT_STEPS);
      winder.moveTo(TargetPos);
    }
    else
    {
      WState = W_STOP;
    }
    break; // case W_CYCLE

  case W_RIGHT:
    if (winder.distanceToGo() != 0)
    {
      winder.run();
    }
    else
    {
      WState = W_LEFT;
      TargetPos += (ROT_L * ROT_STEPS);
      winder.moveTo(TargetPos);
    }
    break; // case W_RIGHT

  case W_LEFT:
    if (winder.distanceToGo() != 0)
    {
      winder.run();
    }
    else
    {
      WState = W_CYCLE;
    }
    break; // case W_LEFT

  case W_STOP:
    winder.disableOutputs();
    StartTime = millis();
    Serial.println("<< Winder: Stopped, waiting for next cycle");
    WState = W_PAUSE;
    break; // case W_STOP

  case W_PAUSE:
    long temp = millis() - StartTime;
    long delta = 60L * 1000L;

    // run once, 1 minute before PAUSE_MIN elapses
    if ((!LastMinute) && (temp > (delta * long(PAUSE_MIN - 1))))
    {
      Serial.println("<< Winder: Restarting in 1 minute");
      LastMinute = true;
    }
    // PAUSE_MIN has elapsed
    else if (temp > (delta * long(PAUSE_MIN)))
    {
      LastMinute = false;
      WState = W_CYCLE;
      Rotations = CYCLES;
    }
    break; // W_PAUSE

  default:
    winder.disableOutputs();
    break;

  } // switch

  if (WState != old_state)
  {
    Serial.print("-- Changing State: ");
    Serial.print(old_state);
    Serial.print(" -> ");
    Serial.println(WState);
  }

  pwr_led.Update();
} // loop
