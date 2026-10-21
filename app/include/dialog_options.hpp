#ifndef BDZ1_DIALOG_OPTIONS_HPP
#define BDZ1_DIALOG_OPTIONS_HPP

#include "TernarySignal.hpp"
#include "TimingDiagram.hpp"

void dialog_ternary_signal_create_empty(TernarySignal &signal);

void dialog_ternary_signal_create_state_duration(TernarySignal &signal);

void dialog_ternary_signal_create_string(TernarySignal &signal);

void dialog_ternary_signal_split(TernarySignal &signal);

void dialog_ternary_signal_increase_duration(TernarySignal &signal);

void dialog_ternary_signal_decrease_duration(TernarySignal &signal);

void dialog_ternary_signal_draw(TernarySignal &signal);

void dialog_ternary_signal_set_state(TernarySignal &signal);

void dialog_ternary_signal_set_duration(TernarySignal &signal);

void dialog_ternary_signal_stream_output(TernarySignal &signal);

void dialog_ternary_signal_stream_input(TernarySignal &signal);

void dialog_timing_diagram_create_empty(TimingDiagram &diagram);

void dialog_timing_diagram_create_state_duration(TimingDiagram &diagram);

void dialog_timing_diagram_create_string(TimingDiagram &diagram);

void dialog_timing_diagram_replace(TimingDiagram &diagram);

void dialog_timing_diagram_add(TimingDiagram &diagram);

void dialog_timing_diagram_multiply(TimingDiagram &diagram);

void dialog_timing_diagram_shift_left(TimingDiagram &diagram);

void dialog_timing_diagram_shift_right(TimingDiagram &diagram);

void dialog_timing_diagram_show_signals(TimingDiagram &diagram);

void dialog_timing_diagram_show_total_duration(TimingDiagram &diagram);

void dialog_timing_diagram_stream_output(TimingDiagram &diagram);

void dialog_timing_diagram_stream_input(TimingDiagram &diagram);

#endif //BDZ1_DIALOG_OPTIONS_HPP
