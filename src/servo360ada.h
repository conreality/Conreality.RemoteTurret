
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#ifndef SERVO360ADA_H
#define SERVO360ADA_H



class Servo360Ada{
    public:
        Servo360Ada(Adafruit_PWMServoDriver servo,int channel, int midPoint);
        
        void turnLeft(int speed);
        void turnRight(int speed);

        
        void brake();
        
    private:
        int _lastRotation;
        int _midPoint;
        int _channel;
        unsigned long _brakeTime;
        Adafruit_PWMServoDriver* _servo;
        
};


#endif
