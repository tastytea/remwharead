include(GNUInstallDirs)

# Some distributions do not contain Poco*Config.cmake recipes.
find_package(Poco CONFIG
  COMPONENTS Foundation Net NetSSL Data DataSQLite JSON XML)

find_package(Boost 1.48.0 REQUIRED COMPONENTS locale)

file(GLOB_RECURSE sources_lib *.cpp)
file(GLOB_RECURSE headers_lib ../../include/*.hpp)

add_library(${PROJECT_NAME} "${sources_lib}" "${headers_lib}")

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

target_include_directories(${PROJECT_NAME}
  PRIVATE
  "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>" # version.hpp
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${PROJECT_NAME}
  PRIVATE pthread Boost::locale
  PUBLIC stdc++fs)

# If no Poco*Config.cmake recipes are found, look for headers in standard dirs.
if(PocoNetSSL_FOUND)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE Poco::Foundation Poco::Net Poco::NetSSL Poco::DataSQLite
    Poco::JSON Poco::XML
    PUBLIC Poco::Data)
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
      PRIVATE PocoFoundation PocoNet PocoNetSSL PocoDataSQLite PocoJSON PocoXML
      PUBLIC PocoData)
  endif()
endif()

install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
