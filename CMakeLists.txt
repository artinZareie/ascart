cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(AGP_DEBUG "Enable debug logging" OFF)
set(AGP_ERROR_LEVEL 2 CACHE STRING "Error logging level (0-2)")

if(AGP_DEBUG)
    add_compile_definitions(AGP_DEBUG)
endif()
add_compile_definitions(AGP_ERROR_LEVEL=${AGP_ERROR_LEVEL})

project(ascart)

set(SOURCES 
	src/main.c
	src/helpers.c
	src/image.c
	src/pgm.c
)

include_directories(include/)

add_executable(ascart ${SOURCES})
