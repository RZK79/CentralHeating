#ifndef _SERIALCOMMUNICATION_H_
#define _SERIALCOMMUNICATION_H_

#include <Arduino.h>

#define SC_START '*'
#define SC_END '#'

class SerialCommunication {
    char data[32] = { '\0' };
    bool recvInProgress;
    int i = 0;
    void init();
public:
    SerialCommunication();
    void serialEvent();
    void parseData(char* data);
};

#endif