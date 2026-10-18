add_library(timing_diagram STATIC
        include/TernarySignal.hpp
        src/TernarySignal.cpp
        src/TimingDiagram.cpp
        include/TimingDiagram.hpp
)
target_include_directories(timing_diagram PUBLIC include)