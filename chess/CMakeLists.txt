
cmake_minimum_required (VERSION 3.8)
project ("chess_project")

#set the C++ standard to be used
set (CMAKE_CXX_STANDARD 17)


set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(libs/sfml)

set (SOURCES src/chess.cpp src/chess.h)
add_executable(chess ${SOURCES})

target_link_libraries(chess sfml-graphics)

 

