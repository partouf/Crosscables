cmake_minimum_required(VERSION 3.26)
project(Groundfloor VERSION 1.0.0 LANGUAGES CXX)

# Enable CTest
include(CTest)

# Fetch Catch2 if testing is enabled
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

add_library(Groundfloor STATIC
    Atoms/Freeable.cpp
    Molecules/String.cpp
    Molecules/Vector.cpp
    Materials/Functions.cpp
    Materials/Thread.cpp
    Materials/CallbackThread.cpp
    Atoms/Callable.cpp
    Atoms/Lockable.cpp
    Materials/FifoVector.cpp
    Materials/Logger.cpp
    Materials/ThreadedBuffer.cpp
    Materials/Directory.cpp
    Materials/GarbageCollector.cpp
    Materials/StringLookup.cpp
    Molecules/Callbacks.cpp
    Molecules/Property.cpp
    Bookshelfs/BFields.cpp
    Bookshelfs/BField.cpp
    Bookshelfs/BValue.cpp
    Bookshelfs/BRecords.cpp
    Bookshelfs/BRecord.cpp
    Bookshelfs/BTable.cpp
    Materials/FileWriter.cpp
    Materials/CsvSettings.cpp
    Bookshelfs/BFunctions.cpp
    Molecules/StringVector.cpp
    Molecules/BaseCommunicator.cpp
    Materials/FileCommunicator.cpp
    Atoms/MemFuncs.cpp
    Atoms/Initialize.cpp
    Bookshelfs/BAttribute.cpp
    Bookshelfs/BNode.cpp
    Bookshelfs/RemoteSquirrel.cpp)

file(GLOB gheaders Atoms/**.h Molecules/**.h Materials/**.h Bookshelfs/**.h)

target_sources(
  Groundfloor
  PUBLIC
    FILE_SET publicheaders
    TYPE HEADERS
    FILES
    ${gheaders}
)

install(TARGETS Groundfloor DESTINATION lib)
install(TARGETS Groundfloor FILE_SET publicheaders DESTINATION include/Groundfloor)

# Testing configuration
if(BUILD_TESTING)
    # Add test executable with all test files
    add_executable(groundfloor_tests
        tests/test_timestamp.cpp
    )

    # Link libraries
    target_link_libraries(groundfloor_tests
        PRIVATE
        Groundfloor
        Catch2::Catch2WithMain
    )

    # Include Catch2's CMake helpers
    include(Catch)

    # Discover tests for CTest
    catch_discover_tests(groundfloor_tests)
endif()
