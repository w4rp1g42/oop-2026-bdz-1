#include "dialog_options.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "utils.hpp"

static const char *state_name(const TernarySignalState state) {
    switch (state) {
        case SIGNAL_ZERO: return "ZERO";
        case SIGNAL_ONE: return "ONE";
        case SIGNAL_UNDEFINED: return "UNDEFINED";
    }
    return "UNKNOWN";
}

static TernarySignalState input_state() {
    std::cout << "State (0=ZERO, 1=ONE, 2=UNDEFINED): ";
    const size_t state_index = input_size_t();
    if (state_index > 2) throw std::invalid_argument("invalid state");
    return static_cast<TernarySignalState>(state_index);
}

static void print_signal(const TernarySignal &signal) {
    std::cout << "state=" << state_name(signal.getState())
            << ", duration=" << signal.getDuration() << "\n";
}

void dialog_ternary_signal_create_empty(TernarySignal &signal) {
    signal = TernarySignal{};
    std::cout << "Signal reset to empty\n";
}

void dialog_ternary_signal_create_state_duration(TernarySignal &signal) {
    const TernarySignalState state = input_state();
    std::cout << "Enter duration: ";
    signal = TernarySignal{state, input_size_t()};
    print_signal(signal);
}

void dialog_ternary_signal_create_string(TernarySignal &signal) {
    signal = TernarySignal{input_line("Enter signal string: ")};
    print_signal(signal);
}

void dialog_ternary_signal_split(TernarySignal &signal) {
    std::cout << "Enter split moment: ";
    const TernarySignal second_part = signal.split(input_size_t());
    std::cout << "First part:  ";
    print_signal(signal);
    std::cout << "Second part: ";
    print_signal(second_part);
}

void dialog_ternary_signal_increase_duration(TernarySignal &signal) {
    std::cout << "Enter increase amount: ";
    signal.increase_duration(input_size_t());
    print_signal(signal);
}

void dialog_ternary_signal_decrease_duration(TernarySignal &signal) {
    std::cout << "Enter decrease amount: ";
    signal.decrease_duration(input_size_t());
    print_signal(signal);
}

void dialog_ternary_signal_draw(TernarySignal &signal) {
    std::cout << "\"" << signal.draw() << "\"\n";
}

void dialog_ternary_signal_set_state(TernarySignal &signal) {
    signal.setState(input_state());
    print_signal(signal);
}

void dialog_ternary_signal_set_duration(TernarySignal &signal) {
    std::cout << "Enter new duration: ";
    signal.setDuration(input_size_t());
    print_signal(signal);
}

void dialog_ternary_signal_stream_output(TernarySignal &signal) {
    std::cout << signal;
}

void dialog_ternary_signal_stream_input(TernarySignal &signal) {
    std::istringstream stream{input_line("Enter signal string: ")};
    stream >> signal;
    if (stream.fail()) throw std::runtime_error("invalid signal input");
    print_signal(signal);
}

static void print_diagram(const TimingDiagram &diagram) {
    const std::vector<TernarySignal> signals = diagram.getSignals();
    std::cout << "Total duration: " << diagram.getTotalDuration() << "\n";
    for (size_t i = 0; i < signals.size(); i++) {
        std::cout << "  [" << i << "] state=" << state_name(signals[i].getState())
                << ", duration=" << signals[i].getDuration() << "\n";
    }
}

void dialog_timing_diagram_create_empty(TimingDiagram &diagram) {
    diagram = TimingDiagram{};
    std::cout << "Diagram reset to empty\n";
}

void dialog_timing_diagram_create_state_duration(TimingDiagram &diagram) {
    const TernarySignalState state = input_state();
    std::cout << "Enter duration: ";
    diagram = TimingDiagram{state, input_size_t()};
    print_diagram(diagram);
}

void dialog_timing_diagram_create_string(TimingDiagram &diagram) {
    diagram = TimingDiagram{input_line("Enter diagram string: ")};
    print_diagram(diagram);
}

void dialog_timing_diagram_replace(TimingDiagram &diagram) {
    const TimingDiagram other{input_line("Enter replacement diagram string: ")};
    std::cout << "Enter start: ";
    const size_t start = input_size_t();
    std::cout << "Enter end: ";
    const size_t end = input_size_t();
    diagram.replaceWithOther(other, start, end);
    print_diagram(diagram);
}

void dialog_timing_diagram_add(TimingDiagram &diagram) {
    const TimingDiagram other{input_line("Enter diagram string to append: ")};
    diagram += other;
    print_diagram(diagram);
}

void dialog_timing_diagram_multiply(TimingDiagram &diagram) {
    std::cout << "Enter times: ";
    diagram = diagram * input_size_t();
    print_diagram(diagram);
}

void dialog_timing_diagram_shift_left(TimingDiagram &diagram) {
    std::cout << "Enter shift: ";
    diagram = diagram << input_size_t();
    print_diagram(diagram);
}

void dialog_timing_diagram_shift_right(TimingDiagram &diagram) {
    std::cout << "Enter shift: ";
    diagram = diagram >> input_size_t();
    print_diagram(diagram);
}

void dialog_timing_diagram_show_signals(TimingDiagram &diagram) {
    print_diagram(diagram);
}

void dialog_timing_diagram_show_total_duration(TimingDiagram &diagram) {
    std::cout << "Total duration: " << diagram.getTotalDuration() << "\n";
}

void dialog_timing_diagram_stream_output(TimingDiagram &diagram) {
    std::cout << diagram;
}

void dialog_timing_diagram_stream_input(TimingDiagram &diagram) {
    std::istringstream stream{input_line("Enter diagram string: ")};
    stream >> diagram;
    if (stream.fail()) throw std::runtime_error("invalid diagram input");
    print_diagram(diagram);
}
