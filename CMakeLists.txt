cmake_minimum_required(VERSION 3.0)
project(segmentation)
set(CMAKE_CXX_STANDARD 17)

add_executable(segmentation main.cpp Model.cpp Reader.cpp Writer.cpp)
