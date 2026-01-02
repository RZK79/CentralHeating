#ifndef _SERIALCOMMUNICATION_H_
#define _SERIALCOMMUNICATION_H_

#include <Arduino.h>

class SerialCommunication {
    char data[32] = { '\0' };
    bool recvInProgress;
    int i;
    const char start = '*';
    const char end = '#';
public:
    void init();
    void serialEvent();
    void parseData(char* data);
};

#endif