include(GNUInstallDirs)

find_package(Poco COMPONENTS Util CONFIG)

file(GLOB sources_cli *.cpp)

add_executable(${PROJECT_NAME}-cli ${sources_cli})

set_target_properties(${PROJECT_NAME}-cli
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}-cli
  PRIVATE "${PROJECT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME}-cli
  PRIVATE ${PROJECT_NAME})

# If no Poco*Config.cmake recipes are found, look for headers in standard dirs.
if(PocoUtil_FOUND)
  target_link_libraries(${PROJECT_NAME}-cli
    PRIVATE Poco::Util)
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
    target_link_libraries(${PROJECT_NAME}-cli
      PRIVATE PocoUtil)
  endif()
endif()

install(TARGETS ${PROJECT_NAME}-cli
  DESTINATION ${CMAKE_INSTALL_BINDIR})
