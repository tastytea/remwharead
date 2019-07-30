cmake_minimum_required (VERSION 3.2)
project(remwharead
  VERSION 0.6.3
  LANGUAGES CXX
  )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -ftrapv \
-fsanitize=undefined -g -Og -fno-omit-frame-pointer")

set(WITH_MAN "YES" CACHE STRING "WITH_MAN defaults to \"YES\"")
set(WITH_TESTS "NO" CACHE STRING "WITH_TESTS defaults to \"NO\"")
set(WITH_MOZILLA "NO" CACHE STRING "WITH_MOZILLA defaults to \"NO\"")

include(GNUInstallDirs)

include_directories(${PROJECT_SOURCE_DIR}/src/lib)
Include_directories(${PROJECT_BINARY_DIR})

# Write version in header
configure_file(
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)

add_subdirectory(src/lib)
add_subdirectory(src/cli)

if (WITH_MAN)
  add_custom_command(
    OUTPUT "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.1"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    DEPENDS "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.1.adoc"
    COMMAND ${CMAKE_SOURCE_DIR}/build_manpage.sh
    ARGS ${PROJECT_VERSION})
  add_custom_target(man ALL DEPENDS "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.1")

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif()

if (WITH_MOZILLA)
  add_subdirectory(browser-plugins/webextension/native-wrapper)
endif()

if(WITH_TESTS)
  include_directories(${PROJECT_SOURCE_DIR}/src/cli)
  add_library(${PROJECT_NAME}_testlib SHARED src/cli/parse_options.cpp)
  add_subdirectory(tests)
endif()

include(packages.CMakeLists.txt)
