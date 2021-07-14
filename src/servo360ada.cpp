#include "servo360ada.h"

Servo360Ada::Servo360Ada(Adafruit_PWMServoDriver servo,int channel, int midPoint)
{
    _channel=channel;
    _midPoint=midPoint;
    _servo=&servo;
    
}

void Servo360Ada::turnLeft(int speed)
{
    _servo->writeMicroseconds(_channel,_midPoint-speed);
}

void Servo360Ada::turnRight(int speed)
{
    _servo->writeMicroseconds(_channel,_midPoint+speed);
}



void Servo360Ada::brake()
{
    _servo->writeMicroseconds(_channel,_midPoint);
}