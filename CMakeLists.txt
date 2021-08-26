cmake_minimum_required(VERSION 3.17)
project(miniLasKa C)

set(CMAKE_C_STANDARD 99)

add_executable(miniLasKa main.c headers/board.h board.c headers/player.h player.c)