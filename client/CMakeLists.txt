make_minimum_required(VERSION 3.20)
project(client)

set(CMAKE_CXX_STANDARD 14)

AUX_SOURCE_DIRECTORY(include INCLUDE)
AUX_SOURCE_DIRECTORY(src SRC)

add_executable(client ${SRC} ${INCLUDE})
