cmake_minimum_required(VERSION 3.16)

project(
  FastDDSHelloWorld
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fastrtps REQUIRED)
find_package(fastcdr REQUIRED)

# ------------------------------------------------------------------
# Generated sources from fastddsgen
# ------------------------------------------------------------------

set(IDL_SOURCES
  idl/srcs/HelloWorld.cxx
  idl/srcs/HelloWorldPubSubTypes.cxx
)

set(IDL_HEADERS
  idl/srcs/HelloWorld.h
  idl/srcs/HelloWorldPubSubTypes.h
)

# ------------------------------------------------------------------
# Publisher
# ------------------------------------------------------------------

add_executable(
  publisher
  srcs/publisher.cpp
  ${IDL_SOURCES}
)

target_include_directories(
  publisher
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/idl
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs
)

target_link_libraries(
  publisher
  PRIVATE
    fastrtps
    fastcdr
)
