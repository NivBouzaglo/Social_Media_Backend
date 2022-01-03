//
// Created by nivbo on 12/29/2021.
//

#include <iostream>
#include <boost/thread.hpp>
#include <string>
#include <boost/asio.hpp>
#include "../include/connectionHandler.h"
#include "../include/keyboardInput.h"

using namespace std;

keyboardInput::keyboardInput(connectionHandler *connectionHandler1) : _connectionHandler(connectionHandler1) {}

void keyboardInput::run() {
        while (_connectionHandler->getTerminate() == 0) {
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            _connectionHandler->sendLine(line);
            if (line == "LOGOUT") {
                while (1) {
                    if (_connectionHandler->getTerminate() == 1)
                        break;
                    else if (_connectionHandler->getTerminate() == -1) {
                        _connectionHandler->setTerminate(0);
                        break;
                    }
                }
            }
    }
}