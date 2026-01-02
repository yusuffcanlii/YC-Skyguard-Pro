cmake_minimum_required(VERSION 3.10)

project(YC-Skyguard-Pro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Skyguard main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(Skyguard PRIVATE Threads::Threads)