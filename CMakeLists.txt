cmake_minimum_required (VERSION 3.2)
project(remwharead
  VERSION 0.6.2
  LANGUAGES CXX
)

set(WITH_MAN "YES" CACHE STRING "WITH_MAN defaults to \"YES\"")
set(WITH_TESTS "NO" CACHE STRING "WITH_TESTS defaults to \"NO\"")
set(WITH_MOZILLA "NO" CACHE STRING "WITH_MOZILLA defaults to \"NO\"")

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXDG_BASEDIR REQUIRED libxdg-basedir)
# sqlite3 is not a direct dependency, but vsqlite++ has no cmake- or pkg-config
# module. Since it installs in the same directories as sqlite3, I am adding the
# module here to add the include- and link directories below. It is not REQUIRED
# because the sqlite3 in Debian jessie doesn't come with a pkg-config module.
pkg_check_modules(SQLITE3 sqlite3)
pkg_check_modules(CURLPP REQUIRED curlpp)
find_package(ICU COMPONENTS uc i18n REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -ftrapv \
-fsanitize=undefined -g -Og -fno-omit-frame-pointer")

include_directories(${PROJECT_SOURCE_DIR}/src)
Include_directories(${PROJECT_BINARY_DIR})

include_directories(${LIBXDG_BASEDIR_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${CURLPP_INCLUDE_DIRS})
include_directories(${ICU_INCLUDE_DIRS})

link_directories(${LIBXDG_BASEDIR_LIBRARY_DIRS})
link_directories(${SQLITE3_LIBRARY_DIRS})
link_directories(${CURLPP_LIBRARY_DIRS})
link_directories(${ICU_LIBRARY_DIRS})

set(COMMON_LIBRARIES
  ${LIBXDG_BASEDIR_LIBRARIES} vsqlitepp stdc++fs ${CURLPP_LIBRARIES}
  ${ICU_LIBRARIES})

# Write version in header
configure_file(
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)

file(GLOB sources src/*.cpp)
add_executable(${PROJECT_NAME} "${sources}")
target_link_libraries(${PROJECT_NAME} ${COMMON_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

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
  add_library(${PROJECT_NAME}_testlib SHARED ${sources})
  target_link_libraries(${PROJECT_NAME}_testlib ${COMMON_LIBRARIES})
  add_subdirectory(tests)
endif()

include(packages.CMakeLists.txt)
