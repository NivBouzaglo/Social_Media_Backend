//
// Created by barse on 1/1/22.
//
#ifndef CLIENT_KEYBOARDINPUT_H
#define CLIENT_KEYBOARDINPUT_H
#include "../include/connectionHandler.h"
class keyboardInput {
private:
    connectionHandler *_connectionHandler;
public:
    keyboardInput(connectionHandler *connectionHandler1);
    void run();
};
#endif //CLIENT_KEYBOARDINPUT_H
