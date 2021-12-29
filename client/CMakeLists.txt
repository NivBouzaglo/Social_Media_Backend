make_minimum_required(VERSION 3.20)
project(spl3)

set(CMAKE_CXX_STANDARD 14)

AUX_SOURCE_DIRECTORY(include INCLUDE)
AUX_SOURCE_DIRECTORY(src SRC)

add_executable(spl3 ${SRC} ${INCLUDE})
