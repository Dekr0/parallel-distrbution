cmake_minimum_required(VERSION 3.24)
project(parallel_distrbution C)

set(CMAKE_C_STANDARD 11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(main "Code/Lab3IO.c" "Code/main.c")
add_executable(main_no_index_map "src/main_no_index_map.c" "src/Lab3IO.c")
add_executable(try "Code/Lab3IO.c" "Code/try.c")
add_executable(gen "Code/datagen.c" "Code/Lab3IO.c")
add_executable(test "Code/serialtester.c" "Code/Lab3IO.c")

target_link_libraries(test m)
target_link_libraries(main_no_index_map m)