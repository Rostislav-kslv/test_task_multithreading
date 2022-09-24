project(main LANGUAGES CXX)

set(CMAKE_CXX_FLAGS -pthread)

add_executable(sk_test main.cpp)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build_debug/bin)
