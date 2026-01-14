project(euix)

include_directories(./include)
aux_source_directory(./src SRC)

link_libraries(EasyX)

add_library(${PROJECT_NAME} STATIC ${SRC})
add_dependencies(${PROJECT_NAME} EasyX)