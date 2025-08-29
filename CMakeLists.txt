cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(AGP_DEBUG "Enable debug logging" OFF)
option(AGP_ERROR_FATAL "Enable debug logging" ON)
set(AGP_ERROR_LEVEL 2 CACHE STRING "Error logging level (0-2)")

if(AGP_DEBUG)
    add_compile_definitions(AGP_DEBUG)
	option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
	option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
	option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
	option(ENABLE_DEBUG "Enable debug symbols" OFF)
endif()
add_compile_definitions(AGP_ERROR_LEVEL=${AGP_ERROR_LEVEL})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
endif()

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native")

project(ascart)

set(SOURCES 
	src/main.c
	src/helpers.c
	src/image.c
	src/pgm.c
	src/ascii_engine.c
)

include_directories(include/)

find_package(OpenMP REQUIRED)
find_package(Curses REQUIRED)

add_executable(ascart ${SOURCES})

target_link_libraries(ascart PRIVATE m)
target_link_libraries(ascart PRIVATE OpenMP::OpenMP_C)
target_link_libraries(ascart PRIVATE ${CURSES_LIBRARIES})
