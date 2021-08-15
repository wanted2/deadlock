cmake_minimum_required(VERSION 3.0)
project(x0035 C CXX)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(deadlock main.cpp copy.cpp)