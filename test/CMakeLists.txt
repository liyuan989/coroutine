set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_executable(test test.cpp)
target_link_libraries(test coroutine)
