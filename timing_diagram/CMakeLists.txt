add_library(timing_diagram
        include/TernarySignal.hpp
        src/TernarySignal.cpp
)
target_include_directories(timing_diagram PUBLIC include)