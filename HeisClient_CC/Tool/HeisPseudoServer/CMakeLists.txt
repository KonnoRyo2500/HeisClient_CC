# heis 疑似サーバ治具ビルド用cmakeファイル
# Author: Ryo Konno

project(HeisPseudoServer)
cmake_minimum_required(VERSION 3.0)
file(GLOB SRCS src/*/*.cpp src/*.cpp ../../src/common/socket_base.cpp ../../src/common/server_socket.cpp ../../src/common/csv_setting_file_reader.cpp ../../src/common/log.cpp ../../src/common/common.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/com")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/scenario")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Isrc/setting")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I../../src/common/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D PS")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(HeisPseudoServer ${SRCS})
