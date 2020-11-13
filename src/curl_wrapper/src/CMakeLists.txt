file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")
add_library(${PROJECT_NAME} STATIC ${sources} ${headers})
unset(sources)
unset(headers)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON)

# FindCURL provides an IMPORTED target since CMake 3.12.
if(NOT ${CMAKE_VERSION} VERSION_LESS 3.12)
  target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl)
else()
  target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CURL_LIBRARIES})
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>")
