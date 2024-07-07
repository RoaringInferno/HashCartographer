cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hash-cartography)

add_library(hash-cartography INTERFACE)
target_include_directories(hash-cartography INTERFACE src/)