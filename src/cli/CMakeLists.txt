include(GNUInstallDirs)

find_package(Poco COMPONENTS Util CONFIG)

file(GLOB sources_cli *.cpp)

add_executable(${PROJECT_NAME}-cli ${sources_cli})

set_target_properties(${PROJECT_NAME}-cli
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}-cli
  PRIVATE "${PROJECT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME}-cli
  PRIVATE ${PROJECT_NAME} Poco::Util)

install(TARGETS ${PROJECT_NAME}-cli
  DESTINATION ${CMAKE_INSTALL_BINDIR})
