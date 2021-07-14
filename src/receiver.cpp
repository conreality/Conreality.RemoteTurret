#include "receiver.h"
#include <RH_NRF24.h>
#include "nRF24L01.h"
#include "RF24.h"

//#define PURE
#ifndef PURE

Receiver::Receiver(int ce, int ss)
{

    _driver = new RH_NRF24(ce, ss);
    
    _manager = new RHReliableDatagram(*_driver, RECEIVER_ADDRESS);
    _buf = new uint8_t[RH_NRF24_MAX_MESSAGE_LEN];
    _buttons = new uint8_t[RH_NRF24_MAX_MESSAGE_LEN - 3]; //two axes + buttons count field = 3
    _pressedFlags = new uint8_t[RH_NRF24_MAX_MESSAGE_LEN - 3];
    for(int i=0;i<(RH_NRF24_MAX_MESSAGE_LEN - 3);i++)
    {
        _pressedFlags[i]=1;
    }
}

bool Receiver::init()
{
    
    bool res= _manager->init();
    if(Serial.availableForWrite())
    {
        if(res){
            Serial.println("radio initialized");
        }else{
            Serial.println("radio failed initialization");
        }
    }
    return res;
}

int Receiver::axisDirection(byte axisNumber, byte minDeadzone,byte maxDeadzone)
{
    uint8_t axisValue = 0;
    if (axisNumber == 0)
        axisValue = _axis0;
    if (axisNumber == 1)
        axisValue = _axis1;

    if(axisValue<=minDeadzone)
    {
        return -1;
    }
    if(axisValue>=maxDeadzone)
    {
        return 1;
    }
    return 0;
}

int Receiver::axisValue(byte axisNumber, byte minDeadzone,byte maxDeadzone)
{
    uint8_t axisValue = 0;
    if (axisNumber == 0)
        axisValue = _axis0;
    if (axisNumber == 1)
        axisValue = _axis1;

    if(axisValue>=minDeadzone&&axisValue<=maxDeadzone)
    {
        return 0;
    }

    

    return axisValue-127;
}

bool Receiver::update()
{
    if (_manager->available())
    {
        /*Serial.print("received");
        Serial.println(millis());*/
        uint8_t len = RH_NRF24_MAX_MESSAGE_LEN;
        uint8_t from;
        if (_manager->recvfromAck(_buf, &len, &from))
        {
            _axis0 = _buf[0];
            _axis1 = _buf[1];

            byte buttonsCount = _buf[2];

            for (int i = 0; i < buttonsCount; i++)
            {
                
                _buttons[i] = _buf[i + 3];
                if(_pressedFlags[i]==1&&_buttons[i]==0)
                    _pressedFlags[i]=0;
                if (_pressedFlags[i]==2&&_buttons[i]==1)//wait for release
                    _pressedFlags[i]=1;
                
                /*Serial.print(i);
                Serial.print(":");
                Serial.println(_pressedFlags[i]);*/
            }
        }
        return true;
    }
    return false;
}

bool Receiver::keyDown(int key)
{
    return _buttons[key]==0;
}

void Receiver::waitForConnection()
{
    while (!update()) {}
}

bool Receiver::keyPressed(int key)
{
    bool res= (_pressedFlags[key]==0);
    
    if(res)
        _pressedFlags[key]=2;
    
    return res;
}
#endif
#ifdef PURE
RF24 radio(10, 9);
byte addresses[][6] = {"1Node", "2Node"};
Receiver::Receiver(int ce, int ss)
{

   
    _buf = new uint8_t[32];
    _buttons = new uint8_t[RH_NRF24_MAX_MESSAGE_LEN - 3]; //two axes + buttons count field = 3
    _pressedFlags = new uint8_t[RH_NRF24_MAX_MESSAGE_LEN - 3];
    for(int i=0;i<(RH_NRF24_MAX_MESSAGE_LEN - 3);i++)
    {
        _pressedFlags[i]=1;
    }
}

bool Receiver::init()
{
    Serial.println("start");
    radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  //radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  //radio.openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  radio.startListening();                 // Start listening
  
  radio.powerUp();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
    //_manager = new RHReliableDatagram(*_driver, RECEIVER_ADDRESS);
    bool res = true;
    //bool res= _manager->init();
    if(Serial.availableForWrite())
    {
        if(res){
            Serial.println("radio initialized");
        }else{
            Serial.println("radio failed initialization");
        }
    }
    return res;
}

int Receiver::axisDirection(byte axisNumber, byte minDeadzone,byte maxDeadzone)
{
    uint8_t axisValue = 0;
    if (axisNumber == 0)
        axisValue = _axis0;
    if (axisNumber == 1)
        axisValue = _axis1;

    if(axisValue<=minDeadzone)
    {
        return -1;
    }
    if(axisValue>=maxDeadzone)
    {
        return 1;
    }
    return 0;
}

int Receiver::axisValue(byte axisNumber, byte minDeadzone,byte maxDeadzone)
{
    uint8_t axisValue = 0;
    if (axisNumber == 0)
        axisValue = _axis0;
    if (axisNumber == 1)
        axisValue = _axis1;

    if(axisValue>=minDeadzone&&axisValue<=maxDeadzone)
    {
        return 0;
    }

    

    return axisValue-127;
}

bool Receiver::update()
{
    if (radio.available())
    {

    
        
        if (radio.available())
        {
            radio.read( _buf, 32 );
            _axis0 = _buf[0];
            _axis1 = _buf[1];

            byte buttonsCount = _buf[2];

            for (int i = 0; i < buttonsCount; i++)
            {
                
                _buttons[i] = _buf[i + 3];
                if(_pressedFlags[i]==1&&_buttons[i]==0)
                    _pressedFlags[i]=0;
                if (_pressedFlags[i]==2&&_buttons[i]==1)//wait for release
                    _pressedFlags[i]=1;
                
                /*Serial.print(i);
                Serial.print(":");
                Serial.println(_pressedFlags[i]);*/
            }
        }
        return true;
    }
    return false;
}

bool Receiver::keyDown(int key)
{
    return _buttons[key]==0;
}

void Receiver::waitForConnection()
{
    while (!update()) {}
}

bool Receiver::keyPressed(int key)
{
    bool res= (_pressedFlags[key]==0);
    
    if(res)
        _pressedFlags[key]=2;
    
    return res;
}
#endif