
#include "Servo.h"

#ifndef SERVO360_H
#define SERVO360_H

enum BrakeMode { Active, Passive };

class Servo360{
    public:
        Servo360(Servo servo);
        
        void turnLeft();
        void turnRight();

        void brake(BrakeMode mode);
        void activeBrake();
        void passiveBrake();
        
    private:
        int _lastRotation;
        unsigned long _brakeTime;
        Servo* _servo;
        
};


#endif
