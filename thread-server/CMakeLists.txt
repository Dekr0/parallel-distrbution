cmake_minimum_required(VERSION 3.24)
project(thread_server C)

set(CMAKE_C_STANDARD 11)

add_executable(main1 main1.c)
add_executable(attacker attacker.c)
add_executable(simple_client demos/simpleClient.c)
add_executable(simple_server demos/simpleServer.c)
add_executable(client client.c)

target_link_libraries(attacker m)