cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hash-cartography)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_executable(hashcart ${SOURCES})
target_include_directories(hashcart PRIVATE src include)