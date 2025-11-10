#ifndef _SERIALCOMMUNICATION_H_
#define _SERIALCOMMUNICATION_H_

#include <Arduino.h>

class SerialCommunication {
    static SerialCommunication* instance;

    char data[32] = { '\0' };
    bool recvInProgress;
    int i = 0;
    const char start = '*';
    const char end = '#';
    SerialCommunication();
    void init();
public:
    static SerialCommunication* get();
    void serialEvent();
    void parseData(char* data);
};

#endif