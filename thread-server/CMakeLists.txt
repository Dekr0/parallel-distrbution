cmake_minimum_required(VERSION 3.24)
project(thread_server C)

set(CMAKE_C_STANDARD 11)

add_executable(attacker attacker.c)
add_executable(client client.c)
add_executable(main1 main1.c)
add_executable(main2 main2.c)
add_executable(main4 main4.c)

target_link_libraries(attacker m)
target_link_libraries(client m)
target_link_libraries(main1 m)
target_link_libraries(main2 m)
