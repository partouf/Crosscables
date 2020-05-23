cmake_minimum_required(VERSION 3.10)

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
