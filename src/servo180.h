
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#ifndef SERVO180_H
#define SERVO180_H

class Servo180
{
private:
    
    Adafruit_PWMServoDriver* _servo;
    bool _isStoped;
    int _channel;
    float _angleF;
    float _angleFreal;
    float lerp(float a, float b, float t);
public:
    Servo180(Adafruit_PWMServoDriver servo,int channel);
    void turnLeft(int speed);
    void turnRight(int speed);
    void tick();
};

#endif