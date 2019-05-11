cmake_minimum_required (VERSION 3.2)
project(remwharead
  VERSION 0.0.0
  LANGUAGES CXX
)

set(WITH_MAN "YES" CACHE STRING "WITH_MAN defaults to \"YES\"")
set(WITH_TESTS "NO" CACHE STRING "WITH_TESTS defaults to \"NO\"")

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURLPP REQUIRED curlpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -ftrapv \
-fsanitize=undefined -g -Og -fno-omit-frame-pointer")

include_directories(${PROJECT_SOURCE_DIR}/src)
Include_directories(${PROJECT_BINARY_DIR})

include_directories(${CURLPP_INCLUDE_DIRS})

link_directories(${CURLPP_LIBRARY_DIRS})

# Write version in header
configure_file(
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)

file(GLOB sources src/*.cpp)
add_executable(${PROJECT_NAME} "${sources}")
target_link_libraries(${PROJECT_NAME} ${CURLPP_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

if (WITH_MAN)
  add_custom_command(
    OUTPUT "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.1"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    DEPENDS "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.1.adoc"
    COMMAND ${CMAKE_SOURCE_DIR}/build_manpage.sh
    ARGS ${PROJECT_VERSION})
  add_custom_target(run ALL DEPENDS "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.1")
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif()

if(WITH_TESTS)
  add_library(${PROJECT_NAME}_testlib SHARED ${sources})
  target_link_libraries(${PROJECT_NAME}_testlib ${CURLPP_LIBRARIES})
  include(tests.CMakeLists.txt)
endif()
