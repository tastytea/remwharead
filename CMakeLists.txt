# Support version 3.9 and above, but use policy settings up to 3.17.
# 3.9 is needed for project description.
cmake_minimum_required(VERSION 3.9...3.17)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Global build options.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build.")
option(BUILD_SHARED_LIBS "Build shared libraries." YES)

project(remwharead
  VERSION 0.10.0
  DESCRIPTION "Saves URIs of things you want to remember in a database.")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

# Project build options.
option(WITH_MAN "Compile and install manpage." YES)
option(WITH_TESTS "Compile tests." NO)
option(WITH_MOZILLA "Build and install wrapper for Mozilla browsers." YES)
set(MOZILLA_NMH_DIR "${CMAKE_INSTALL_LIBDIR}/mozilla/native-messaging-hosts"
  CACHE STRING "Directory for the Mozilla extension wrapper.")
option(WITH_CLANG-TIDY "Check sourcecode with clang-tidy while compiling." NO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(debug_flags)

if(WITH_CLANG-TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    "clang-tidy"
    "-header-filter=${PROJECT_SOURCE_DIR}"
    "-quiet")
endif()

add_subdirectory(src)
add_subdirectory(src/curl_wrapper)
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
