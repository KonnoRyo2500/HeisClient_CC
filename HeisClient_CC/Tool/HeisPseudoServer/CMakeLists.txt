# heis 疑似サーバ治具ビルド用cmakeファイル
# Author: Ryo Konno

project(HeisPseudoServer)
cmake_minimum_required(VERSION 3.0)
file(GLOB_RECURSE SRCS src/*.cpp ../../src/utils/path.cpp ../../src/utils/string_utils.cpp ../../src/com/socket/socket.cpp ../../src/com/socket/linux_socket.cpp ../../src/com/socket/windows_socket.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/command")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/scenario")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/setting")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I../../src/utils")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I../../src/system/setting")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I../../src/com/socket")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=CP932")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D PS")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(HeisPseudoServer ${SRCS})
