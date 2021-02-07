cmake_minimum_required(VERSION 3.17)
project(dataarmor_test)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        src/Server
        src/Client
        src/bytes
        src/Exception
        src/ThreadPool
        )

add_executable(dataarmor_test
        main.cpp
        src/Server/Server.cpp
        src/Client/Client.cpp
        src/bytes/bytes.cpp
        src/Exception/Exception.cpp
        src/ThreadPool/ThreadPool.cpp
        src/workers.cpp
        )

target_link_libraries(dataarmor_test pthread)
