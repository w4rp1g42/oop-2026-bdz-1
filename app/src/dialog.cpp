#include "dialog.hpp"

#include <iostream>

#include "dialog_options.hpp"
#include "utils.hpp"

static size_t dialog(const char *options[], const size_t options_count) {
    const char *error_message = "";
    while (true) {
        std::cout << error_message;
        error_message = "Incorrect\n";

        for (size_t i = 0; i < options_count; i++)
            std::cout << options[i] << "\n";

        std::cout << "Enter command: ";
        const size_t option_index = input_size_t();
        if (option_index < options_count) return option_index;
    }
}

void start_dialog() {
    const char *options[] = {
        "0. Exit",
        "1. TernarySignal dialog",
        "2. TimingDiagram dialog"
    };
    const size_t options_count = sizeof(options) / sizeof(options[0]);

    void (*operations[])() = {
        nullptr,
        start_ternary_signal_dialog,
        start_timing_diagram_dialog
    };

    try {
        while (true) {
            const size_t option_index = dialog(options, options_count);
            if (option_index == 0) break;
            operations[option_index]();
        }
    } catch (const QuitSignal &) {
        std::cout << "\nGoodbye!\n";
    }
}

void start_ternary_signal_dialog() {
    const char *options[] = {
        "0. Back",
        "1. Create empty signal",
        "2. Create signal from state and duration",
        "3. Create signal from string",
        "4. Split signal (modifies current signal)",
        "5. Increase duration",
        "6. Decrease duration",
        "7. Draw signal",
        "8. Set state",
        "9. Set duration",
        "10. Print (stream <<)",
        "11. Read (stream >>)"
    };
    const size_t options_count = sizeof(options) / sizeof(options[0]);

    void (*operations[])(TernarySignal &) = {
        nullptr,
        dialog_ternary_signal_create_empty,
        dialog_ternary_signal_create_state_duration,
        dialog_ternary_signal_create_string,
        dialog_ternary_signal_split,
        dialog_ternary_signal_increase_duration,
        dialog_ternary_signal_decrease_duration,
        dialog_ternary_signal_draw,
        dialog_ternary_signal_set_state,
        dialog_ternary_signal_set_duration,
        dialog_ternary_signal_stream_output,
        dialog_ternary_signal_stream_input
    };

    TernarySignal signal{};

    while (true) {
        const size_t option_index = dialog(options, options_count);
        if (option_index == 0) return;
        try {
            operations[option_index](signal);
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

void start_timing_diagram_dialog() {
    const char *options[] = {
        "0. Back",
        "1. Create empty diagram",
        "2. Create diagram from state and duration",
        "3. Create diagram from string",
        "4. Replace a range with another diagram",
        "5. Append another diagram (operator+=)",
        "6. Multiply diagram (operator*)",
        "7. Shift left (operator<<)",
        "8. Shift right (operator>>)",
        "9. Show signals",
        "10. Show total duration",
        "11. Print (stream <<)",
        "12. Read (stream >>)"
    };
    const size_t options_count = sizeof(options) / sizeof(options[0]);

    void (*operations[])(TimingDiagram &) = {
        nullptr,
        dialog_timing_diagram_create_empty,
        dialog_timing_diagram_create_state_duration,
        dialog_timing_diagram_create_string,
        dialog_timing_diagram_replace,
        dialog_timing_diagram_add,
        dialog_timing_diagram_multiply,
        dialog_timing_diagram_shift_left,
        dialog_timing_diagram_shift_right,
        dialog_timing_diagram_show_signals,
        dialog_timing_diagram_show_total_duration,
        dialog_timing_diagram_stream_output,
        dialog_timing_diagram_stream_input
    };

    TimingDiagram diagram{};

    while (true) {
        const size_t option_index = dialog(options, options_count);
        if (option_index == 0) return;
        try {
            operations[option_index](diagram);
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}
