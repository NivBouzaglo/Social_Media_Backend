#include "../include/ConnectionHandler.h"
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <sstream>
#include <vector>
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace std;
ConnectionHandler::ConnectionHandler(string host, short port) : host_(host), port_(port), io_service_(),
                                                                socket_(io_service_), terminate(0) {}

ConnectionHandler::~ConnectionHandler() {
    close();
}

int ConnectionHandler::getTerminate() {
    return terminate;
}

void ConnectionHendler::setTerminate() {
    terminate = 1;
}

bool ConnectionHandler::connect() {
    std::cout << "Starting connect to "
              << host_ << ":" << port_ << std::endl;
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
        boost::system::error_code error;
        socket_.connect(endpoint, error);
        if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception &e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp) {
            tmp += socket_.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
        }
        if (error)
            throw boost::system::system_error(error);
    } catch (std::exception &e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp) {
            tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
        if (error)
            throw boost::system::system_error(error);
    } catch (std::exception &e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::getLine(std::string &line) {
    return getFrameAscii(line, '\n');
}

bool ConnectionHandler::sendLine(std::string &line) {
    return sendFrameAscii(line, '\n');
}

bool ConnectionHandler::getFrameAscii(std::string &frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character. 
    // Notice that the null character is not appended to the frame string.
    try {
        do {
            getBytes(&ch, 1);
            frame.append(1, ch);
        } while (delimiter != ch);
    } catch (std::exception &e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
    /*bool result=sendBytes(frame.c_str(),frame.length());
    if(!result) return false;
    return sendBytes(&delimiter,1);*/
    std:: vector<char> output = encode(frame);
}

std::vector<char> ConnectionHandler::encode(std::string msg) {
    std::vector<char> output;
    std::istringstream iss(msg);
    std::string word;
    char opcode[2];
    int i =2 ;
    getline(iss, word, ' ')
    switch (word) {
        case "REGISTER":
            shortToBytes((short)1 , opcode);
            output.push_back(opcode[0])
            output.push_back(opcode[1])
            while (getline(iss, part, ' '))
            {
                    for (int k = i; k < part.length(); k++) {
                        charVector.push_back(part[k]);
                        i++;
                    }
                    charVector.push_back('\0');
                    i++;
            }
        case "LOGIN":
            shortToBytes((short)2 , opcode);
            output.push_back(opcode[0])
            output.push_back(opcode[1])
            while (getline(iss, part, ' '))
            {
                for (int k = i; k < part.length(); k++) {
                    charVector.push_back(part[k]);
                    i++;
                }
                charVector.push_back('\0');
                i++;
            }
        case "LOGOUT":
        case "FOLLOW":
        case "POST":
        case "PM":
        case "LOGSTAT":
        case "STAT":
        case "BLOCK":
    }
}
void shortToBytes(short num, char* bytesArr)
{
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}


// Close down the connection properly.
void ConnectionHandler::close() {
    try {
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}