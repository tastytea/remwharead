include_directories(${PROJECT_SOURCE_DIR}/src/cli)

file(GLOB sources_cli *.cpp)

add_executable(${PROJECT_NAME}-cli ${sources_cli})
target_link_libraries(${PROJECT_NAME}-cli ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-cli
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}-cli DESTINATION ${CMAKE_INSTALL_BINDIR})
