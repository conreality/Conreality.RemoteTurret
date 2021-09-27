#include "servo180.h"
#include "Arduino.h"
Servo180::Servo180(Adafruit_PWMServoDriver servo,int channel)
{
    _servo=&servo;
    _isStoped=false;
    _channel=channel;
    _angleF=1400;
    _angleFreal=1400;
    _servo->writeMicroseconds(_channel,(int)_angleFreal);
}
float Servo180::lerp(float a, float b, float t)
{
    return a+(b-a)*t;
}
void Servo180::turnLeft(int speed)
{
    
    if(_angleF>500)_angleF-=speed;
    _angleF=max(500,_angleF);
    
}

void Servo180::turnRight(int speed)
{
    
    if(_angleF<2500)_angleF+=speed;
    _angleF=min(2500,_angleF);
    
    
}

void Servo180::tick()
{
  
    _angleFreal=lerp(_angleFreal,_angleF,0.1);
    _servo->writeMicroseconds(_channel,(int)_angleFreal);
    
}