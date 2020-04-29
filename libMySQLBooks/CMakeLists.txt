cmake_minimum_required(VERSION 3.10)

find_package(MySQL)

add_library(MySQLBooks STATIC
    MySQLSquirrel.cpp
    MySquirrelUtils.cpp)

include_directories(../include)

add_dependencies(MySQLBooks devmake)
add_dependencies(MySQLBooks Groundfloor)


