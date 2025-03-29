cmake_minimum_required(VERSION 3.26)

# set the project name
project(Crosscables
    LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(Groundfloor)

if(NOT DISABLE_JUMPROPES)
    add_subdirectory(Jumpropes)
endif()

#add_subdirectory(Jumpknots)

# add_subdirectory(MySQLBooks)

# add_custom_target(
#     devmake ALL
#     COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/Groundfloor && sh devmake.sh &&
#     cd ${CMAKE_CURRENT_SOURCE_DIR}/Jumpropes && sh devmake.sh &&
#     cd ${CMAKE_CURRENT_SOURCE_DIR}/Jumpknots && sh devmake.sh &&
#     cd ${CMAKE_CURRENT_SOURCE_DIR}/MySQLBooks && sh devmake.sh
# )
