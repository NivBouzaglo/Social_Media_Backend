//
// Created by nivbo on 12/29/2021.
//

#include <iostream>
#include <boost/thread.hpp>
#include <string>
#include <boost/asio.hpp>
#include "../include/ConnectionHandler.h"

using namespace std;

class keyboardInput {
private:
    ConnectionHandler *connectionHandler;
public:
    keyboardInput(ConnectionHandler *connectionHandler1) : connectionHandler(connectionHandler1) {}

  void run() {
        while (connectionHandler->getTerminate() == 0) {
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            connectionHandler->sendLine(line);
            if (line == "LOGOUT") {
                while (1) {
                    if (connectionHandler->getTerminate() == 1)
                        break;
                    else if (connectionHandler->getTerminate() == -1) {
                        connectionHandler->setTerminate(0);
                        break;
                    }
                }
            }
        }
    }
};