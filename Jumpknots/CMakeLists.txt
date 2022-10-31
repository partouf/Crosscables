cmake_minimum_required(VERSION 3.10)

find_package(OpenSSL)

add_library(Jumpknots STATIC
    SSLInitialize.cpp
    SSLBaseSocket.cpp
    SSLClientSocket.cpp)

include_directories(../)

add_dependencies(Jumpknots Groundfloor)
add_dependencies(Jumpknots Jumpropes)
