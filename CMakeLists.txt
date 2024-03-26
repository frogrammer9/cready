cmake_minimum_required(VERSION 3.21)
project(cready)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(crdy crdy.cpp funcs.cpp funcs.hpp)
