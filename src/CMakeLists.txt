add_library(coroutine coroutine.cpp schedule.cpp)
target_link_libraries(coroutine ${Boost_LIBRARIES})
