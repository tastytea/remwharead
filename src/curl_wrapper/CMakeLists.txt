# Support version 3.9 and above, but use policy settings up to 3.17.
cmake_minimum_required(VERSION 3.9...3.17)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(curl_wrapper
  VERSION 0.1.0
  DESCRIPTION "Light libcurl wrapper."
  LANGUAGES CXX)

option(WITH_CURL_WRAPPER_TESTS "Compile tests for curl_wrapper." NO)
option(WITH_CURL_WRAPPER_DOC "Compile API reference for curl_wrapper." NO)

find_package(CURL 7.52 REQUIRED)

add_subdirectory("src")

if(WITH_CURL_WRAPPER_TESTS)
  add_subdirectory("tests")
endif()

if(WITH_CURL_WRAPPER_DOC)
  include("cmake/Doxygen.cmake")
  enable_doxygen("src")
endif()
