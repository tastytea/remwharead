find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXDG_BASEDIR REQUIRED libxdg-basedir)
# sqlite3 is not a direct dependency, but vsqlite++ has no cmake- or pkg-config
# module. Since it installs in the same directories as sqlite3, I am adding the
# module here to add the include- and link directories below. It is not REQUIRED
# because the sqlite3 in Debian jessie doesn't come with a pkg-config module.
pkg_check_modules(SQLITE3 sqlite3)
pkg_check_modules(CURLPP REQUIRED curlpp)
find_package(ICU COMPONENTS uc i18n REQUIRED)

include_directories(${LIBXDG_BASEDIR_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${CURLPP_INCLUDE_DIRS})
include_directories(${ICU_INCLUDE_DIRS})

link_directories(${LIBXDG_BASEDIR_LIBRARY_DIRS})
link_directories(${SQLITE3_LIBRARY_DIRS})
link_directories(${CURLPP_LIBRARY_DIRS})
link_directories(${ICU_LIBRARY_DIRS})

file(GLOB sources_lib *.cpp export/*.cpp)
file(GLOB headers_lib *.hpp)
file(GLOB headers_lib_export export/*.hpp)

add_library(${PROJECT_NAME} SHARED ${sources_lib})
target_link_libraries(${PROJECT_NAME} ${LIBXDG_BASEDIR_LIBRARIES}
  vsqlitepp stdc++fs ${CURLPP_LIBRARIES} ${ICU_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
  )

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${headers_lib}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${headers_lib_export}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/export)

configure_file("${PROJECT_SOURCE_DIR}/remwharead.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/remwharead.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/remwharead.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
