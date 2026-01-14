project(EasyX)

message(STATUS "Linking EasyX...")

add_library(EasyX INTERFACE IMPORTED GLOBAL)

target_include_directories(EasyX INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_X64)
    message(STATUS "Link lib64")
    target_link_directories(EasyX INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib64)
else()
    message(STATUS "Link lib32")
    target_link_directories(EasyX INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib32)
endif()
if(COMPILER_CL)
    message(STATUS "Linking EasyXw")
    target_link_libraries(EasyX INTERFACE EasyXw)
elseif(COMPILER_GCC)
    message(STATUS "Linking libeasyx")
    target_link_libraries(EasyX INTERFACE libeasyx)
else()
    message(FATAL_ERROR "No compiler definition(COMPILER_XX)!")
endif()
