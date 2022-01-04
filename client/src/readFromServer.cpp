//
// Created by nivbo on 12/29/2021.
//
#include <iostream>
#include <boost/thread.hpp>
#include <sstream>
#include <fstream>
#include <string>
#include "../include/connectionHandler.h"
#include "../include/readFromServer.h"

using namespace std;

readFromServer::readFromServer(connectionHandler *connectionHandler1) : connectionHandler1(connectionHandler1) {}

void readFromServer::run() {
    while (1) {
        std::string answer = connectionHandler1->translateMessage();
        std::cout << answer << std::endl;
        if (answer == "ACK 3")
            break;
    }
}



