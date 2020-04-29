cmake_minimum_required(VERSION 3.10)

find_package(MySQL)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-nostdlib")

add_library(MySQLBooks STATIC
    MySQLSquirrel.cpp
    MySquirrelUtils.cpp)

include_directories(../include)

add_dependencies(MySQLBooks devmake)
add_dependencies(MySQLBooks Groundfloor)


