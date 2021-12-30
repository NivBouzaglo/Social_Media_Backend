cmake_minimum_required(VERSION 3.20)
project(client)

set(CMAKE_CXX_STANDARD 14)

AUX_SOURCE_DIRECTORY(include INCLUDE)
AUX_SOURCE_DIRECTORY(src SRC)
SET(LDFLAGS "-lboost_system -lboost_thread -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKR_CXX_FLASG}${LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${GCC_COVERAGE_LINK_FLAGS}")
add_executable(echoClient ${SRC} ${INCLUDE})
