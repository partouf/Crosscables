cmake_minimum_required(VERSION 3.10)

# set the project name
project(Crosscables
    LANGUAGES CXX)

add_subdirectory(libGroundfloor)
add_subdirectory(libJumpropes)

add_subdirectory(libJumpknots)
add_subdirectory(libMySQLBooks)

add_custom_target(
    devmake ALL
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/libGroundfloor && sh devmake.sh &&
    cd ${CMAKE_CURRENT_SOURCE_DIR}/libJumpropes && sh devmake.sh &&
    cd ${CMAKE_CURRENT_SOURCE_DIR}/libJumpknots && sh devmake.sh &&
    cd ${CMAKE_CURRENT_SOURCE_DIR}/libMySQLBooks && sh devmake.sh
)
