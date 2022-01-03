#include "../include/connectionHandler.h"
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

connectionHandler::connectionHandler(string host, short port) : host_(host), port_(port), io_service_(),
                                                                socket_(io_service_), terminate(0) {}

connectionHandler::~connectionHandler() {
    close();
}

int connectionHandler::getTerminate() {
    return terminate;
}

void connectionHandler::setTerminate(int i) {
    terminate = i;
}

bool connectionHandler::connect() {
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

bool connectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
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

bool connectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
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

bool connectionHandler::getLine(std::string &line) {
    return getFrameAscii(line, '\n');
}

bool connectionHandler::sendLine(std::string &line) {
    return sendFrameAscii(line, '\n');
}

bool connectionHandler::getFrameAscii(std::string &frame, char delimiter) {
    char ch;
    char opcodeBytes[2];
    // Stop when we encounter the null character. 
    // Notice that the null character is not appended to the frame string.
    getBytes(&opcodeBytes[0], 1);
    getBytes(&opcodeBytes[1], 1);
    short opcode = bytesToShort(opcodeBytes);
    if (opcode == 9) {
        frame.append("NOTIFICATION ");
        getBytes(&ch, 1);
        if (ch == '\1')
            frame.append("Public ");
        else
            frame.append("Pm ");
        while (true) {
            getBytes(&ch, 1);
            if (ch == ';')
                break;
            frame.append(&ch);
        }
    } else if (opcode == 10) {
        frame.append("ACK ");
        char messageOpcode[2];
        getBytes(&messageOpcode[0], 1);
        getBytes(&messageOpcode[1], 1);
        short mOp = bytesToShort(messageOpcode);
        frame.append(std::to_string(mOp));
        if (mOp == 3) {
            terminate = 1;
        } else if (mOp == 4) {
            char letter;
            getBytes(&letter, 1);
            if (letter == '\0')
                frame.append(" 0 ");
            else
                frame.append(" 1 ");
            while (true) {
                getBytes(&letter, 1);
                if (letter == ';')
                    break;
                frame.append(&letter);
            }
        } else if (mOp == 7) {
            char age[2];
            getBytes(age, 2);
            short a = bytesToShort(age);
            frame.append(" " + std::to_string(a));
            char NumPosts[2];
            getBytes(NumPosts, 2);
            short n = bytesToShort(NumPosts);
            frame.append(" " + std::to_string(n));
            char NumFollowers[2];
            getBytes(NumFollowers, 2);
            short f = bytesToShort(NumFollowers);
            frame.append(" " + std::to_string(f));
            char NumFollowing[2];
            getBytes(NumFollowing, 2);
            short l = bytesToShort(NumFollowing);
            frame.append(" " + std::to_string(l));
        } else if (mOp == 8) {
            char age[2];
            getBytes(age, 2);
            short a = bytesToShort(age);
            frame.append(" " + std::to_string(a));
            char NumPosts[2];
            getBytes(NumPosts, 2);
            short n1 = bytesToShort(NumPosts);
            frame.append(" " + std::to_string(n1));
            char NumFollowers[2];
            getBytes(NumFollowers, 2);
            short f1 = bytesToShort(NumFollowers);
            frame.append(" " + std::to_string(f1));
            char NumFollowing[2];
            getBytes(NumFollowing, 2);
            short l1 = bytesToShort(NumFollowing);
            frame.append(" " + std::to_string(l1));
        }
    } else if (opcode == 11) {
        char error[2];
        getBytes(error, 2);
        short mistake = bytesToShort(error);
        frame.append("ERROR " + std::to_string(mistake));
        if (mistake == 3)
            terminate = -1;
    }
    return true;
}

bool connectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
    std::vector<char> output = encode(frame);
    int size = output.size();
    char send[size];
    for (int i = 0; i < size; ++i) {
        send[i] = output[i];
    }
    return sendBytes(send, size);
}

std::vector<char> connectionHandler::encode(std::string msg) {
    std::vector<char> output;
    std::istringstream iss(msg);
    std::string word;
    char opcode[2];
    int i = 0;
    iss >> word;
    if (word == "REGISTER") {
        shortToBytes((short) 1, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        while (i < 3) {
            iss >> word;
            for (char &k: word) {
                output.push_back(k);
            }
            if (i != 2)
                output.push_back('\0');
            i++;
        }
    } else if (word == "LOGIN") {
        shortToBytes((short) 2, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        while (i < 3) {
            iss >> word;
            if (i != 2) {
                for (char &k: word) {
                    output.push_back(k);
                }
                output.push_back('\0');
            } else {
                if (word == "1")
                    output.push_back('\1');
                else if (word == "0")
                    output.push_back('\0');
            }
            i++;
        }
    } else if (word == "LOGOUT") {
        shortToBytes((short) 3, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
    } else if (word == "FOLLOW") {
        shortToBytes((short) 4, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        iss >> word;
        if (word == "1")
            output.push_back('\1');
        else
            output.push_back('\0');
        iss >> word;
        for (char &j: word) {
            output.push_back(j);
        }
    } else if (word == "POST") {
        shortToBytes((short) 5, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        while (getline(iss, word, ' ')) {
            for (char &j: word) {
                output.push_back(j);
            }
        }
    } else if (word == "PM") {
        shortToBytes((short) 6, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        while (getline(iss, word, ' ')) {
            for (char &j: word) {
                output.push_back(j);
            }
        }
        /*std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        char time[2];
        shortToBytes(now, time);
        for (int i = 0; i < time.length(); i++)
            output.push_back(time[i]);*/
    } else if (word == "LOGSTAT") {
        shortToBytes((short) 7, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
    } else if (word == "STAT") {
        shortToBytes((short) 8, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        while (getline(iss, word, ' ')) {
            for (char &j: word) {
                output.push_back(j);
            }
        }
    } else if (word == "BLOCK") {
        shortToBytes((short) 12, opcode);
        output.push_back(opcode[0]);
        output.push_back(opcode[1]);
        while (getline(iss, word, ' ')) {
            for (char &j: word) {
                output.push_back(j);
            }
        }
    }
    output.push_back('\n');
    return output;
}

void connectionHandler::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

short connectionHandler::bytesToShort(char *bytesArr) {
    short result = (short) ((bytesArr[0] & 0xff) << 8);
    result += (short) (bytesArr[1] & 0xff);
    return result;
}


// Close down the connection properly.
void connectionHandler::close() {
    try {
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

std::string connectionHandler::translateMessage() {
    std::string frame;
    char ch;
    char opcodeBytes[2];
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    getBytes(&opcodeBytes[0], 1);
    getBytes(&opcodeBytes[1], 1);
    short opcode = bytesToShort(opcodeBytes);
    if (opcode == 9) {
        frame.append("NOTIFICATION ");
        getBytes(&ch, 1);
        if (ch == '\1')
            frame.append("Public ");
        else
            frame.append("Pm ");
        while (true) {
            getBytes(&ch, 1);
            if (ch == ';')
                break;
            frame.append(&ch);
        }
    } else if (opcode == 10) {
        frame.append("ACK ");
        char messageOpcode[2];
        getBytes(&messageOpcode[0], 1);
        getBytes(&messageOpcode[1], 1);
        short mOp = bytesToShort(messageOpcode);
        frame.append(std::to_string(mOp));
        if (mOp == 3) {
            terminate = 1;
        } else if (mOp == 4) {
            char letter;
            getBytes(&letter, 1);
            while (true) {
                getBytes(&letter, 1);
                if (letter == ';')
                    break;
                frame.append(&letter);
            }
        } else if (mOp == 7) {
            char age[2];
            getBytes(age, 2);
            short a = bytesToShort(age);
            frame.append(" " + std::to_string(a));
            char NumPosts[2];
            getBytes(NumPosts, 2);
            short n = bytesToShort(NumPosts);
            frame.append(" " + std::to_string(n));
            char NumFollowers[2];
            getBytes(NumFollowers, 2);
            short f = bytesToShort(NumFollowers);
            frame.append(" " + std::to_string(f));
            char NumFollowing[2];
            getBytes(NumFollowing, 2);
            short l = bytesToShort(NumFollowing);
            frame.append(" " + std::to_string(l));
        } else if (mOp == 8) {
            char age[2];
            getBytes(age, 2);
            short a = bytesToShort(age);
            frame.append(" " + std::to_string(a));
            char NumPosts[2];
            getBytes(NumPosts, 2);
            short n1 = bytesToShort(NumPosts);
            frame.append(" " + std::to_string(n1));
            char NumFollowers[2];
            getBytes(NumFollowers, 2);
            short f1 = bytesToShort(NumFollowers);
            frame.append(" " + std::to_string(f1));
            char NumFollowing[2];
            getBytes(NumFollowing, 2);
            short l1 = bytesToShort(NumFollowing);
            frame.append(" " + std::to_string(l1));
        }
    } else if (opcode == 11) {
        char error[2];
        getBytes(error, 2);
        short mistake = bytesToShort(error);
        frame.append("ERROR " + std::to_string(mistake));
        if (mistake == 3)
            terminate = -1;
    }
    return frame;
}
