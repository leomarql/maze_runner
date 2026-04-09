cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(maze_runner)

set(SOURCE maze_runner.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)