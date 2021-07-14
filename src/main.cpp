#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>
#include "servo360ada.h"
#include "servo180.h"

#include "receiver.h"

/*-------------------------------------------------------------------------------------*/

#include "config.h"

/*-------------------------------------------------------------------------------------*/

Receiver receiver(CE_PIN, SS_PIN);

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

#define AXIS_X_DIRECTION receiver.axisDirection(AXIS_X, X_LOWER_DEADZONE, X_UPPER_DEADZONE)
#define AXIS_X_VALUE receiver.axisValue(AXIS_X, X_LOWER_DEADZONE, X_UPPER_DEADZONE)
#define AXIS_Y_DIRECTION receiver.axisDirection(AXIS_Y, Y_LOWER_DEADZONE, Y_UPPER_DEADZONE)
#define AXIS_Y_VALUE receiver.axisValue(AXIS_Y, Y_LOWER_DEADZONE, Y_UPPER_DEADZONE)

/***
 *                                                             
 *    ,------.                     ,--.                        
 *    |  .--. ' ,---.  ,---. ,---. `--',--.  ,--.,---. ,--.--. 
 *    |  '--'.'| .-. :| .--'| .-. :,--. \  `'  /| .-. :|  .--' 
 *    |  |\  \ \   --.\ `--.\   --.|  |  \    / \   --.|  |    
 *    `--' '--' `----' `---' `----'`--'   `--'   `----'`--'    
 *                                                             
 */

Servo360Ada *servo;
Servo180 *servoVertical;
bool toggleStates[BUTTONS_COUNT];
void setup()
{

  Serial.begin(9600);

  pwm1.begin();
  pwm1.setOscillatorFrequency(26500000);
  pwm1.setPWMFreq(70); // This is the maximum PWM frequency

  servo = new Servo360Ada(pwm1, 0, SERVO360_MIDDLEPOINT);
  servoVertical = new Servo180(pwm1, 1);

  for (int i = 0; i < BUTTONS_COUNT; i++)
  {
    pinMode(outputPins[i], OUTPUT);
    toggleStates[i] = !onValues[i];
  }

  receiver.init();              //start receiver
  receiver.waitForConnection(); //and wait while first packet
}

#ifdef DEBUG_JOYSTICK
uint32_t __debug_timeout=0;
#endif

void loop()
{
  int valueX = map(abs(AXIS_X_VALUE), 0, 128, 0, 300);
  int valueY = map(abs(AXIS_Y_VALUE), 0, 89, 0, 10);

  if (AXIS_X_DIRECTION < 0)
    servo->turnRight(valueX); //turn left

  if (AXIS_X_DIRECTION > 0)
    servo->turnLeft(valueX); //turn right

  if (AXIS_X_DIRECTION == 0) //brake
    servo->brake();          //brake using Active or Passive braking

  if (AXIS_Y_DIRECTION > 0)
    servoVertical->turnRight(valueY); //turn left

  if (AXIS_Y_DIRECTION < 0)
    servoVertical->turnLeft(valueY); //turn right

  #if DEBUG_JOYSTICK == 1
  if(__debug_timeout<millis())
  {
    Serial.println("-----------------------------");
    Serial.print("X:");
    Serial.print(AXIS_X_VALUE);
    Serial.print(" Y:");
    Serial.println(AXIS_Y_VALUE);
  }
  #endif

  for (int i = 0; i < BUTTONS_COUNT; i++)
  {
    if (modes[i] == OnWhenPressed)
    {
      bool value = !onValues[i];
      if (receiver.keyDown(inputs[i]))
      {
        value=onValues[i];
      }
      digitalWrite(outputPins[i], value);
      #if DEBUG_JOYSTICK == 1
      if(__debug_timeout<millis())
  {
      Serial.print("Pin ");
      Serial.print(outputPins[i]);
      Serial.print(":");
      Serial.println(value);
  }
      #endif
    }
    if (modes[i] == SwitchWhenPressed)
    {
      if (receiver.keyPressed(inputs[i]))
      {
        toggleStates[i] = !toggleStates[i];
      }
      digitalWrite(outputPins[i], toggleStates[i]);
      #if DEBUG_JOYSTICK == 1
      if(__debug_timeout<millis())
  {
      Serial.print("pin ");
      Serial.print(outputPins[i]);
      Serial.print(":");
      Serial.println(toggleStates[i]);
  }
      #endif
    }
  }

  #if DEBUG_JOYSTICK == 1
  if(__debug_timeout<millis())
  {
  __debug_timeout=millis()+DEBUG_INTERVAL;
  }
  #endif

  receiver.update(); //receive new packet
  servoVertical->tick();
}