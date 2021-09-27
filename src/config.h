#include "Arduino.h"
#ifndef RC_CONFIG
#define RC_CONFIG



//RF pins
#define CE_PIN 10
#define SS_PIN 9

//FUNDUINO or custom joystick
#define FUNDUINO 0
#define CUSTOM_JOYSTICK 1

//X deadzones
#define X_LOWER_DEADZONE 110
#define X_UPPER_DEADZONE 140

//Y deadzones
#define Y_LOWER_DEADZONE 125
#define Y_UPPER_DEADZONE 137

//debug
#define DEBUG_JOYSTICK 1
#define DEBUG_INTERVAL 500

#define SERVO360_MIDDLEPOINT 1525

enum InputMode{
    OnWhenPressed=1,
    SwitchWhenPressed=2
};
//funduino configuration
#if FUNDUINO == 1

    #define AXIS_X 0
    #define AXIS_Y 1

    #define BTN_A 0
    #define BTN_B 1
    #define BTN_C 2
    #define BTN_D 3
    #define BTN_E 4
    #define BTN_F 5
    #define BTN_K 6

    #define BUTTONS_COUNT 7

    byte inputs[BUTTONS_COUNT]=     {   BTN_A,          BTN_B, BTN_C, BTN_D, BTN_E, BTN_F, BTN_K};
    byte modes[BUTTONS_COUNT]=      {   OnWhenPressed,  OnWhenPressed,OnWhenPressed,OnWhenPressed,OnWhenPressed,OnWhenPressed,OnWhenPressed};
    byte outputPins[BUTTONS_COUNT]= {   2,              3,4,5,6,7,8};
    byte onValues[BUTTONS_COUNT]=   {   0,              1,1,1,1,1,1};

    

#endif

//custom configuration
#if CUSTOM_JOYSTICK == 1

    #define AXIS_X 0
    #define AXIS_Y 1

    #define BTN_TRIGGER 0
    #define BTN_FLASHLIGHT 1
    #define BTN_LASER 2

    #define BUTTONS_COUNT 3

    byte inputs[BUTTONS_COUNT]=     {  BTN_TRIGGER,    BTN_FLASHLIGHT,     BTN_LASER           };
    byte modes[BUTTONS_COUNT]=      {  OnWhenPressed,  SwitchWhenPressed,  SwitchWhenPressed   };
    byte outputPins[BUTTONS_COUNT]= {  2,              3,                  4                   };
    bool onValues[BUTTONS_COUNT]=   {  1,              1,                  1                   };

    

#endif
#endif