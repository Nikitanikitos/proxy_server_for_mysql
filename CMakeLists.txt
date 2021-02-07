cmake_minimum_required(VERSION 3.17)
project(dataarmor_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        src/Server
        src/Client
        src/bytes
        src/Exception
        )

add_executable(dataarmor_test
        main.cpp
        src/Server/Server.cpp
        src/Client/Client.cpp
        src/bytes/bytes.cpp
        src/Exception/Exception.cpp src/Exception/Exception.hpp)