# Support version 3.9 and above, but use policy settings up to 3.14.
# 3.9 is needed for project description.
cmake_minimum_required(VERSION 3.9...3.14)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Global build options.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build.")
option(BUILD_SHARED_LIBS "Build shared libraries." YES)
option(HUNTER_ENABLED "Use the Hunter package manager" NO)

if(${HUNTER_ENABLED})
  set(BUILD_SHARED_LIBS "NO")
  set(HUNTER_PocoCpp_CMAKE_ARGS
    ENABLE_CRYPTO=ON
    ENABLE_DATA=ON
    ENABLE_JSON=ON
    ENABLE_NET=ON
    ENABLE_NETSSL=ON
    ENABLE_SQL_SQLITE=ON
    ENABLE_UTIL=ON
    ENABLE_XML=ON)

  # FetchContent_MakeAvailable needs 3.14.
  if(NOT (${CMAKE_VERSION} VERSION_LESS 3.14))
    set(HUNTER_Boost_COMPONENTS locale)
    set(HUNTER_PACKAGES PocoCpp Boost)
    include(FetchContent)
    FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
    FetchContent_MakeAvailable(SetupHunter)
  else()
    include("cmake/HunterGate.cmake")
    HunterGate(
      URL "https://github.com/cpp-pm/hunter/archive/v0.23.225.tar.gz"
      SHA1 "4b018ec7b673817475c1fb9f0bd4dfb7c5e7f343"
      LOCAL)                    # Load cmake/Hunter/config.cmake.
  endif()
endif()

project(remwharead
  VERSION 0.9.2
  DESCRIPTION "Saves URIs of things you want to remember in a database.")

if(${HUNTER_ENABLED} AND ${CMAKE_VERSION} VERSION_LESS 3.14)
  hunter_add_package(PocoCpp)
  hunter_add_package(Boost COMPONENTS locale)
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

# Project build options.
option(WITH_MAN "Compile and install manpage." YES)
option(WITH_TESTS "Compile tests." NO)
option(WITH_MOZILLA "Build and install wrapper for Mozilla browsers." YES)
set(MOZILLA_NMH_DIR "${CMAKE_INSTALL_LIBDIR}/mozilla/native-messaging-hosts"
  CACHE STRING "Directory for the Mozilla extension wrapper.")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(debug_flags)

add_subdirectory(src)
add_subdirectory(src/lib)
add_subdirectory(include)
add_subdirectory(src/cli)
add_subdirectory(pkg-config)
add_subdirectory(cmake)

if(WITH_MAN)
  add_subdirectory(man)
endif()

if(WITH_MOZILLA)
  add_subdirectory(browser-plugins/webextension/native-wrapper)
endif()

if(WITH_TESTS)
  add_subdirectory(tests)
endif()

include(cmake/packages.cmake)
