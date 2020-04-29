cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-nostdlib")

add_library(Jumpropes STATIC
    BaseSocket.cpp
    ClientSocket.cpp
    ServerSocket.cpp
    ThreadedConnection.cpp
    ThreadedServer.cpp
    ThreadedSender.cpp
    Initialize.cpp
    URI.cpp
    Common/HttpConnection.cpp
    Common/HttpClient.cpp
    Functions.cpp
    NetAddress.cpp
    LookupCache.cpp)

include_directories(../include)

add_dependencies(Jumpropes devmake)
add_dependencies(Jumpropes Groundfloor)
