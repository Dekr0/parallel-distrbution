cmake_minimum_required(VERSION 3.24)
project(matrix_multiplication C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} src/main.c src/main.h src/lab1_IO.c src/lab1_IO.h)
add_executable(matrixgen src/matrixgen.c src/util.h src/util.c)

target_link_libraries(${PROJECT_NAME} m)
target_link_libraries(matrixgen m)
