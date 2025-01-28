
cmake_minimum_required(VERSION 3.10)
project(SpaceInvaders)

set (CMAKE_CXX_STANDARD 17)


include_directories(include)

add_executable(SpaceInvaders
    src/main.cpp
    src/class.cpp
)
