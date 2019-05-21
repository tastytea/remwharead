set(MOZILLA_NMH_DIR "${CMAKE_INSTALL_LIBDIR}/mozilla/native-messaging-hosts")

add_executable(${PROJECT_NAME}_wrapper ${PROJECT_NAME}_wrapper.cpp)

install(TARGETS ${PROJECT_NAME}_wrapper DESTINATION ${MOZILLA_NMH_DIR})
install(FILES remwharead.json DESTINATION ${MOZILLA_NMH_DIR})
