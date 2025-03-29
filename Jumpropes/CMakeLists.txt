cmake_minimum_required(VERSION 3.26)

if(NOT DISABLE_JUMPROPES)
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

  include_directories(../)

  # add_dependencies(Jumpropes devmake)
  add_dependencies(Jumpropes Groundfloor)

  file(GLOB gheaders **.h Common/**.h)

  target_sources(
    Jumpropes
    PUBLIC
      FILE_SET publicheaders
      TYPE HEADERS
      FILES
      ${gheaders}
  )

  install(TARGETS Jumpropes DESTINATION lib)
  install(TARGETS Jumpropes FILE_SET publicheaders DESTINATION include/Jumpropes)
endif()
