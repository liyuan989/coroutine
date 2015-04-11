add_library(coroutine Coroutine.cpp Schedule.cpp)
target_link_libraries(coroutine ${Boost_LIBRARIES})
