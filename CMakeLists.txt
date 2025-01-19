cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("BUILD_MSVC")

set(CMAKE_C_COMPILER cl)
set(CMAKE_CXX_COMPILER cl)

project(all)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("BUILD_DEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Ox")
    message("BUILD_RELEASE")
endif()

link_directories(./lib/msvc)

add_subdirectory(EuiX)
add_subdirectory(exp3)