include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG        v3.16.0
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

add_executable(tests timing_diagram/ternary_signal_tests.cpp
        timing_diagram/timing_diagram_tests.cpp)
target_link_libraries(tests PRIVATE timing_diagram Catch2::Catch2WithMain)

add_compile_options(--coverage)
add_link_options(--coverage)

include(CTest)
include(Catch)
catch_discover_tests(tests)