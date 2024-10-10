cmake_minimum_required(VERSION 3.29)
project(snake)

set(CMAKE_CXX_STANDARD 20)

add_executable(snake main.cpp Grid.h CircularMatrix.h Game.cpp Game.h Renderer.h)
