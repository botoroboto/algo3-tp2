cmake_minimum_required(VERSION 3.0.0)
project(algo3-tp2 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(ejercicio4 ./4/main.cpp)

add_executable(ejercicio3 ./3/main.cpp)

add_executable(ejercicio2 ./2/main.cpp)

add_executable(ejercicio1 ./1/main.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
