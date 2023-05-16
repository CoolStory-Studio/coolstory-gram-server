cmake_minimum_required(VERSION 3.22)
project(coolstory_gram_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(coolstory_gram_server src/main.cpp)

