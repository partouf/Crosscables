cmake_minimum_required(VERSION 3.10)

find_package(OpenSSL)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-nostdlib")

add_library(Jumpknots STATIC
    SSLInitialize.cpp
    SSLBaseSocket.cpp
    SSLClientSocket.cpp)

include_directories(../include)

add_dependencies(Jumpknots Groundfloor)
add_dependencies(Jumpknots Jumpropes)
