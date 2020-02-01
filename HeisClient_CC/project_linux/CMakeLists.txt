# heis CCビルド用cmakeファイル
# Author: Ryo Konno

project(HeisClient_CC)
cmake_minimum_required(VERSION 3.0)
file(GLOB SRCS ../src/*/*.cpp ../src/*.cpp)
set(CMAKE_CXX_FLAGS "-Wall -O2 -I../src/ -I../src/common -I../src/AI -I../src/field -I../src/JSON -I../src/local_mode -I../src/online_mode -I../src/picojson-master/picojson-master -I../src/unit")
add_executable(HeisClient_CC ${SRCS})
