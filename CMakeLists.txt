# Preset
# --------------------------------------------------
cmake_minimum_required(VERSION 3.13)

# Default settings
option(COMPILER_GCC "Compile by gcc" ON)
option(COMPILER_CL "Compile by cl" OFF)
option(BUILD_X64 "Build to win64" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}debug/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug/archive)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release/archive)

if(BUILD_X64)
    message(STATUS "Build to x64")
else()
    message(STATUS "Build to x86")
endif()

if(COMPILER_CL)
    message(STATUS "Compile by cl...")
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Build to debug")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Ox")
        message(STATUS "Build to release")
    endif()
elseif(COMPILER_GCC)
    message(STATUS "Compile by gcc...")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Build to debug")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
        message(STATUS "Build to release")

        if(BUILD_X64)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        endif()
    endif()
else()
    message(FATAL_ERROR "No compiler definition(COMPILER_XX)!")
endif()
# --------------------------------------------------

project(EuiX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(thirdparty/EasyX)
# TODO 解决因为debug和release编译不同而产生的问题：检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项: 值“0”不匹配值“2”
add_subdirectory(EuiX) # TODO 同时进行头文件的生成
add_subdirectory(exp3)