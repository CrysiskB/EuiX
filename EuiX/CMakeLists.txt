project(EuiX)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(./include)
aux_source_directory(./src SRC)

add_library(EuiX STATIC ${SRC})

target_link_directories(EuiX PRIVATE ./lib/msvc)

target_link_libraries(EuiX PUBLIC EasyXw)