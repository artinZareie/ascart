cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ascart)

set(SOURCES src/main.c)

include_directories(include/)

add_executable(ascart ${SOURCES})
