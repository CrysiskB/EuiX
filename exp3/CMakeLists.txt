project(exp3)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(./include)

link_libraries(
    EuiX
)

add_executable(
    exp3-1
    "./src/sim3.1.cpp"
    "./src/exp3.1.cpp"
)

add_dependencies(exp3-1 EuiX)

add_executable(
    exp3-2
    "./src/sim3.2.cpp"
    "./src/exp3.2.cpp"
)

add_dependencies(exp3-2 EuiX)

Set_Target_Properties(exp3-1 exp3-2 PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")