

#ifndef RECEIVER_H
#define RECEIVER_H

#include "Arduino.h"
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#ifndef RECEIVER_ADDRESS
#define RECEIVER_ADDRESS 2
#endif

class Receiver{
    public:
    Receiver(int ce,int ss);
    bool init();
    bool update();
    void waitForConnection();
    bool keyDown(int key);
    bool keyPressed(int key);
    int axisDirection(byte axisNumber,byte minDeadzone,byte maxDeadzone);
    int axisValue(byte axisNumber,byte minDeadzone,byte maxDeadzone);
    private:
    RH_NRF24* _driver;
    RHReliableDatagram* _manager;
    uint8_t* _buf;
    uint8_t* _buttons;
    uint8_t* _pressedFlags;
    uint8_t _axis0;
    uint8_t _axis1;

};


#endif