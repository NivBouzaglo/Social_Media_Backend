//
// Created by nivbo on 12/29/2021.
//
#include <iostream>
#include "../include/ConnectionHandler.h"
#include <sstream>
#include <fstream>
#include <string>

using namespace std;
class readFromServer{
private:
    ConnectionHandler *connectionHandler1;
public:
    readFromServer(ConnectionHandler *connectionHandler1): connectionHandler1(connectionHandler1){}

   void run(){
        while(1){
            std::string answer="";
            if (connectionHandler1->getLine(answer)){
                std::cout << answer.c_str() << endl;
                if (answer == "ACK 3")
                    break;
            }

        }
    }
};
