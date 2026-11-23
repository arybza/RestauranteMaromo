cmake_minimum_required(VERSION 3.20)
project(Cardapio C)

set(CMAKE_C_STANDARD 11)

add_executable(Cardapio main.c cardapio.c cardapio.h)
