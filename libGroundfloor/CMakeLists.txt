cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-nostdlib")

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
