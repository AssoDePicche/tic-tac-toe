cmake_minimum_required(VERSION 3.16)

project(TicTacToe C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SOURCES game/source/*.c)

add_executable(${PROJECT_NAME} main/main.c ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE game/include)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib m)
