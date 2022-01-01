#include <stdlib.h>
#include "../include/connectionHandler.h"
#include <thread>
#include "../include/readFromServer.h"
#include "../include/keyboardInput.h"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
using namespace std;

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    connectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    keyboardInput task1(&connectionHandler);
    readFromServer task2(&connectionHandler);
    std::thread t(&keyboardInput::run , &task1);
    std::thread t1(&readFromServer::run , &task2);
    t.join();
    t1.join();

    return 0;
}
