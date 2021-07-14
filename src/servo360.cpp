#include "servo360.h"
#include "Arduino.h"

#define SERVO_ACTIVE_BRAKE_TIME 30
#define SERVO_ZERO_POSITION 1500
#define SERVO_LEFT_ROTATION 500
#define SERVO_RIGHT_ROTATION 2000

Servo360::Servo360(Servo servo)
{
    _servo = &servo;
    _brakeTime=0;
}

void Servo360::turnLeft()
{
    _servo->writeMicroseconds(SERVO_LEFT_ROTATION);
    _lastRotation=-1;
}

void Servo360::turnRight()
{
    _servo->writeMicroseconds(SERVO_RIGHT_ROTATION);
    _lastRotation=1;
}

void Servo360::brake(BrakeMode mode)
{
    if (mode==Active)
    {
        activeBrake();
    }
    if (mode==Passive)
    {
        passiveBrake();
    }
}

void Servo360::activeBrake()
{
    if(_lastRotation==-1)
    {
        _lastRotation=0;
        _servo->writeMicroseconds(SERVO_RIGHT_ROTATION);
        _brakeTime=millis()+SERVO_ACTIVE_BRAKE_TIME;
    }
    if(_lastRotation==1)
    {
        _lastRotation=0;
        _servo->writeMicroseconds(SERVO_LEFT_ROTATION);
        _brakeTime=millis()+SERVO_ACTIVE_BRAKE_TIME;
        
    }

    if(millis()>_brakeTime)
    {
        _servo->writeMicroseconds(SERVO_ZERO_POSITION);
    }

}

void Servo360::passiveBrake()
{
    _lastRotation=0;
    _servo->writeMicroseconds(SERVO_ZERO_POSITION);

}