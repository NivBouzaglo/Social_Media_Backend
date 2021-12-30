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
    char opcodeBytes[2];
    // Stop when we encounter the null character. 
    // Notice that the null character is not appended to the frame string.
    try {
        getBytes(&opcodeBytes , 2);
        short opcode = bytesToShort(opcodeBytes);
        switch (opcode) {
            case 9:
                frame.append("NOTIFICATION ");
                getBytes(&ch,1);
                if (ch == '\1')
                    frame.append("Public ");
                else
                    frame.append("Pm ");
                while (true){
                    getBytes(&ch , 1);
                    if (ch == ';')
                        break;
                    frame.append(ch);
                }
            case 10:
                frame.append("ACK ");
                char messageOpcode[2];
                getBytes(&messageOpcode, 2);
                short mOp = bytesToShort(messageOpcode);
                frame.append(std::to_string(mOp));
                switch (mOp) {
                    case 3:
                        terminate = 1;
                    case 4:
                        char letter;
                        getBytes(&letter,1);
                        if(letter == '\0')
                            frame.append(" 0 ");
                        else
                            frame.append(" 1 ");
                        while (true) {
                            getBytes(&letter, 1);
                            if (letter == ';')
                                break;
                            frame.append(letter);
                        }
                    case 7:
                        char age[2];
                        getBytes(&age, 2);
                        frame.append(" "+std::to_string(age));
                        char NumPosts[2];
                        getBytes(&NumPosts, 2);
                        frame.append(" "+std::to_string(NumPosts));
                        char NumFollowers[2];
                        getBytes(&NumFollowers, 2);
                        frame.append(" "+std::to_string(NumFollowers));
                        char NumFollowing[2];
                        getBytes(&NumFollowing, 2);
                        frame.append(" "+std::to_string(NumFollowing));
                    case 8:
                        char age[2];
                        getBytes(&age, 2);
                        frame.append(" "+std::to_string(age));
                        char NumPosts[2];
                        getBytes(&NumPosts, 2);
                        frame.append(" "+std::to_string(NumPosts));
                        char NumFollowers[2];
                        getBytes(&NumFollowers, 2);
                        frame.append(" "+std::to_string(NumFollowers));
                        char NumFollowing[2];
                        getBytes(&NumFollowing, 2);
                        frame.append(" "+std::to_string(NumFollowing));
                }
            case 11:
                char error[2];
                getBytes(&error , 2);
                short mistake = bytesToShort(error);
                frame.append("ERROR " + std::to_string(mistake));
                if (mistake == 3)
                    terminate = -1;
        }
        return true;
    } catch (std::exception &e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return false;
}

bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
    std::vector<char> output = encode(frame);
    int size = output.size();
    char send[size];
    for (int i = 0; i < size; ++i) {
        send[i] = output[i];
    }
    return sendBytes(send, size);
}

std::vector<char> ConnectionHandler::encode(std::string msg) {
    std::vector<char> output;
    std::istringstream iss(msg);
    std::string word;
    char opcode[2];
    int i = 0;
    getline(iss, word, ' ')
    switch (word) {
        case "REGISTER":
            shortToBytes((short) 1, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            while (getline(iss, word, ' ') && i < 3) {
                for (int k = 0; k < word.length(); k++) {
                    output.push_back(word[k]);
                }
                output.push_back('\0');
                i++;
            }
        case "LOGIN":
            shortToBytes((short) 2, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            while (getline(iss, word, ' ') && i < 3) {
                if (i != 2) {
                    for (int k = 0; k < word.length(); k++) {
                        output.push_back(word[k]);
                    }
                    output.push_back('\0');
                }
                else{
                    if (word == "1")
                        output.push_back('\1');
                    else if (word == "0")
                        output.push_back('\0');
                }
                i++;
            }
        case "LOGOUT":
            shortToBytes((short) 3, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
        case "FOLLOW":
            shortToBytes((short) 4, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            getline(iss, word, ' ');
            if (word == "1")
                output.push_back('\1');
            else
                output.push_back('\0');
            getline(iss, word, ' ');
            for (int j = 0; j < word.length; ++j) {
                output.push_back(word[j]);
            }
        case "POST":
            shortToBytes((short)5, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            while (getline(iss, word, ' ')) {
                for (int j = 0; j < word.length; ++j) {
                    output.push_back(word[j]);
                }
            }
        case "PM":
            shortToBytes((short)6, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            getline(iss, word, ' ');
            for (int j = 0; j < word.length; ++j) {
                output.push_back(word[j]);
            }
            output.push_back('\0');
            while (getline(iss, word, ' ')) {
                for (int j = 0; j < word.length; ++j) {
                    output.push_back(word[j]);
                }
            }
            std::time_t t = std::time(0);   // get time now
            std::tm* now = std::localtime(&t);
            char time[];
            shortToBytes((short)now, time);
            for(int i=0; i<time.length(); i++)
                output.push_back(time[i]);
            //time
        case "LOGSTAT":
            shortToBytes((short)7, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
        case "STAT":
            shortToBytes((short)8, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            getline(iss, word, ' ');
            for (int j = 0; j < word.length; ++j) {
                output.push_back(word[j]);
            }
        case "BLOCK":
            shortToBytes((short)12, opcode);
            output.push_back(opcode[0]);
            output.push_back(opcode[1]);
            getline(iss, word, ' ');
            for (int j = 0; j < word.length; ++j) {
                output.push_back(word[j]);
            }
    }
    output.push_back(';');
    return output;
}

void ConnectionHandler::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

short ConnectionHandler::bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}


// Close down the connection properly.
void ConnectionHandler::close() {
    try {
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}