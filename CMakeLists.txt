# Support version 3.6 and above, but use policy settings up to 3.14.
# 3.6 is needed because of IMPORTED_TARGET in pkg_check_modules().
cmake_minimum_required(VERSION 3.6...3.14)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(remwharead
  VERSION 0.8.5
  LANGUAGES CXX)
# DESCRIPTION was introduced in version 3.9.
if(NOT (${CMAKE_VERSION} VERSION_LESS 3.9))
  set(PROJECT_DESCRIPTION
    "Saves URIs of things you want to remember in a database.")
endif()

include(GNUInstallDirs)

# All custom build switches.
option(WITH_MAN "Compile and install manpage." YES)
option(WITH_TESTS "Compile tests." NO)
option(WITH_MOZILLA "Build and install wrapper for Mozilla browsers." YES)
option(BUILD_SHARED_LIBS "Build shared libraries." YES)
set(MOZILLA_NMH_DIR "${CMAKE_INSTALL_LIBDIR}/mozilla/native-messaging-hosts"
  CACHE STRING "Directory for the Mozilla extension wrapper.")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEBUG_CXXFLAGS "")
# GCC >= 5.0 or Clang >= 5.0 is assumed.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND DEBUG_CXXFLAGS
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "-Wuninitialized"
        "-Wshadow"
        "-Wnon-virtual-dtor"
        "-Wconversion"
        "-Wsign-conversion"
        "-Wold-style-cast"
        "-Wzero-as-null-pointer-constant"
        # "-Wmissing-declarations"
        "-Wcast-align"
        "-Wunused"
        "-Woverloaded-virtual"
        "-Wdouble-promotion"
        "-Wformat=2"
        "-ftrapv"
        "-fsanitize=undefined"
        "-g"
        "-Og"
        "-fno-omit-frame-pointer")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        list(APPEND DEBUG_CXXFLAGS
            "-Wlogical-op"
            "-Wuseless-cast")
        if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6")
            list(APPEND DEBUG_CXXFLAGS
                "-Wmisleading-indentation"
                "-Wduplicated-cond"
                "-Wnull-dereference")
            if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7")
                list(APPEND DEBUG_CXXFLAGS
                    "-Wduplicated-branches")
            endif()
        endif()
    endif()

    set(DEBUG_LDFLAGS
        "-fsanitize=undefined")
    add_compile_options("$<$<CONFIG:Debug>:${DEBUG_CXXFLAGS}>")
    # add_link_options was introduced in version 3.13.
    if(${CMAKE_VERSION} VERSION_LESS 3.13)
        set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${DEBUG_LDFLAGS}")
    else()
        add_link_options("$<$<CONFIG:Debug>:${DEBUG_LDFLAGS}>")
    endif()
endif()

add_subdirectory(src)
add_subdirectory(src/lib)
add_subdirectory(include)
add_subdirectory(src/cli)
add_subdirectory(pkg-config)
add_subdirectory(cmake)

if (WITH_MAN)
  add_subdirectory(man)
endif()

if (WITH_MOZILLA)
  add_subdirectory(browser-plugins/webextension/native-wrapper)
endif()

if(WITH_TESTS)
  add_subdirectory(tests)
endif()

include(cmake/packages.cmake)
