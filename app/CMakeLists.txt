add_executable(app
        src/main.cpp
        src/dialog.cpp
        src/dialog_options.cpp
        src/utils.cpp
)
target_include_directories(app PRIVATE include)
target_link_libraries(app PRIVATE timing_diagram)