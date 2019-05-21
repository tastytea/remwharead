set(MOZILLA_NMH_DIR
  "${CMAKE_INSTALL_LIBDIR}/mozilla/native-messaging-hosts")
set(INSTALL_MOZILLA_NMH_DIR
  "${CMAKE_INSTALL_PREFIX}/${MOZILLA_NMH_DIR}")

add_executable(${PROJECT_NAME}_wrapper ${PROJECT_NAME}_wrapper.cpp)
install(TARGETS ${PROJECT_NAME}_wrapper DESTINATION ${MOZILLA_NMH_DIR})

add_custom_command(
  OUTPUT "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.json"
  WORKING_DIRECTORY
  "${CMAKE_SOURCE_DIR}/browser-plugins/webextension/native-wrapper"
  DEPENDS "${PROJECT_NAME}.json"
  COMMAND "./replace_nmh_dir.sh"
  ARGS "${INSTALL_MOZILLA_NMH_DIR}"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.json")
add_custom_target(nmh_replace ALL
  DEPENDS "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.json")
install(
  FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.json
  DESTINATION ${MOZILLA_NMH_DIR})
