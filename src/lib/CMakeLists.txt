include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libxdg-basedir REQUIRED IMPORTED_TARGET libxdg-basedir)
find_package(ICU COMPONENTS uc i18n REQUIRED)
find_package(Poco COMPONENTS Net NetSSL Util CONFIG) # Not required, see below.

file(GLOB_RECURSE sources_lib *.cpp)
file(GLOB_RECURSE headers_lib ../../include/*.hpp)

add_library(${PROJECT_NAME} "${sources_lib}" "${headers_lib}")

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

target_include_directories(${PROJECT_NAME}
  PRIVATE
  "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>" # version.hpp
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${PROJECT_NAME}
  PRIVATE PkgConfig::libxdg-basedir ICU::uc ICU::i18n
  PUBLIC vsqlitepp stdc++fs)

# Debian Stretch does not contain Poco*Config.cmake recipes.
if(PocoNetSSL_FOUND)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE Poco::Net Poco::NetSSL Poco::Util)
else()
  find_file(Poco_h NAMES "Poco/Poco.h"
    PATHS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")

  if("${Poco_h}" STREQUAL "Poco_h-NOTFOUND")
    message(FATAL_ERROR "Could not find POCO.")
  else()
    message(WARNING
      "Your distribution of POCO doesn't contain the *Config.cmake recipes, "
      "but the files seem to be in the standard directories. "
      "Let's hope this works.")
    target_link_libraries(${PROJECT_NAME}
      PRIVATE PocoNet PocoNetSSL PocoUtil)
  endif()
endif()

install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

