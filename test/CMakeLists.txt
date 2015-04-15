set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_executable(coroutine_test test.cpp)
target_link_libraries(coroutine_test coroutine)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark coroutine)
