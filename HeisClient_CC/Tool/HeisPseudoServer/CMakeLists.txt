# heis 疑似サーバ治具ビルド用cmakeファイル
# Author: Ryo Konno

project(HeisPseudoServer)
cmake_minimum_required(VERSION 3.0)
file(GLOB SRCS src/*/*.cpp src/*.cpp ../../src/common/server_socket.cpp ../../src/common/csv_setting_file_reader.cpp ../../src/common/heis_client_exception.cpp ../../src/common/path_generator.cpp ../../src/common/token_manager.cpp)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Isrc/ -Isrc/com -Isrc/scenario -Isrc/setting -I../../src/common/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(HeisPseudoServer ${SRCS})
