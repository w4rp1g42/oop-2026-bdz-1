cmake_minimum_required(VERSION 4.4)
project(bdz1)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(timing_diagram)
add_subdirectory(app)

enable_testing()
add_subdirectory(tests)
