include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libxdg-basedir REQUIRED IMPORTED_TARGET libxdg-basedir)
find_package(ICU COMPONENTS uc i18n REQUIRED)
find_package(Poco COMPONENTS Net NetSSL Util CONFIG REQUIRED)

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
  PRIVATE
  PkgConfig::libxdg-basedir ICU::uc ICU::i18n Poco::Net Poco::NetSSL Poco::Util
  PUBLIC vsqlitepp stdc++fs)

install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

