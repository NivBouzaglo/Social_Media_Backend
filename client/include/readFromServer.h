//
// Created by barse on 1/1/22.
//

#ifndef CLIENT_READFROMSERVER_H
#define CLIENT_READFROMSERVER_H
#include "../include/connectionHandler.h"

class readFromServer {
private:
    connectionHandler *connectionHandler1;
public:
    readFromServer(connectionHandler *connectionHandler1);
    void run();
};
#endif //CLIENT_READFROMSERVER_H
