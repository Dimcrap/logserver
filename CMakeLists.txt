cmake_minimum_required(VERSION 3.15)

project(logserver  VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(HEADERS)

set(UDP_server_resources UDPserver.cpp threadpool.cpp logsmanager.cpp)
set(UDP_client_resources UDPclient.cpp)
set(UDP_repeater_resources lorem.c UDPrepeaterClient.cpp )  

add_executable(UDPs)
target_sources(UDPs  PRIVATE ${UDP_server_resources})


add_executable(UDPc)
target_sources(UDPc PRIVATE ${UDP_client_resources})


add_executable(UDPrc)
target_sources(UDPrc PRIVATE ${UDP_repeater_resources})


add_compile_options(
    -fno-pretty-templates
    -fdiagnostics-show-template-tree
    -fmax-errors=3
)
add_compile_options(-Wall -Wextra -Wpedantic)