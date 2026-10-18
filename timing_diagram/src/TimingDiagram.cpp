#include <ranges>
#include <numeric>

#include "TimingDiagram.hpp"

TimingDiagram::TimingDiagram(const std::string &signal_string) {
    if (signal_string.empty()) throw std::logic_error("can not make diagram with empty string");
    size_t start = 0, end = 0;
    end = signal_string.find_first_not_of(signal_string.front());
    while (end != std::string::npos) {
        signals.emplace_back(signal_string.substr(start, end));
        start = end;
        end = signal_string.find_first_not_of(signal_string[start], start);
    }
    signals.emplace_back(signal_string.substr(start));
}

size_t TimingDiagram::getTotalDuration() const {
    return std::accumulate(signals.begin(), signals.end(), 0,
                           [](const size_t total, const TernarySignal signal) {
                               return total + signal.getDuration();
                           });
}

void TimingDiagram::replaceWithOther(const TimingDiagram &other, const size_t start, const size_t end) {
    if (end <= start) throw std::invalid_argument("end can not be <= start");
    if (other.getTotalDuration() != end - start)
        throw std::logic_error(
            "diagram to insert can't be larger than insert section");
    if (end > getTotalDuration()) throw std::logic_error("range exceeds duration");

    // copy for strong exception safety
    std::vector signals_copy{signals};

    size_t time_marker = 0;
    size_t idx = 0;

    for (size_t i = 0; i < signals_copy.size(); ++i) {
        if (signals_copy[i].getDuration() <= start - time_marker) {
            // skip if before area
            time_marker += signals_copy[i].getDuration();
            continue;
        }
        // split if on border of area, remove second part, which is in area
        if (start > time_marker) {
            const TernarySignal second_part = signals_copy[i].split(start - time_marker);
            time_marker += signals_copy[i].getDuration();
            signals_copy.insert(signals_copy.begin() + i + 1, second_part);
            idx = i + 1;
        } else {
            idx = i;
        }

        break;
    }

    size_t insert_idx = idx;

    // remove everything from area
    while (true) {
        if (idx < signals_copy.size() && signals_copy[idx].getDuration() <= end - time_marker) {
            // remove if in area
            time_marker += signals_copy[idx].getDuration();
            signals_copy.erase(signals_copy.begin() + idx);
            continue;
        }

        if (idx < signals_copy.size() && end > time_marker) {
            const TernarySignal last = signals_copy[idx].split(end - time_marker);
            signals_copy.erase(signals_copy.begin() + idx);
            signals_copy.insert(signals_copy.begin() + idx, last);
        }

        break;
    }

    // add from other
    for (auto signal: other.getSignals()) {
        signals_copy.insert(signals_copy.begin() + insert_idx, signal);
        insert_idx++;
    }

    std::swap(signals, signals_copy);
}

TimingDiagram &TimingDiagram::operator+=(const TimingDiagram &other) {
    signals.insert(signals.end(), other.signals.begin(), other.signals.end());
    return *this;
}

TimingDiagram TimingDiagram::operator*(const size_t times) const {
    TimingDiagram new_timing_diagram{};
    for (int i = 0; i < times; i++) {
        new_timing_diagram.signals.insert(new_timing_diagram.signals.end(), signals.begin(), signals.end());
    }
    return new_timing_diagram;
}

TimingDiagram TimingDiagram::operator<<(const size_t shift) const {
    size_t shifted = 0;

    TimingDiagram diagram_copy = *this;

    while (shifted < shift && !diagram_copy.signals.empty()) {
        TernarySignal &signal = diagram_copy.signals.front();
        const size_t shift_per_signal = shift - shifted;
        if (signal.getDuration() <= shift_per_signal) {
            shifted += signal.getDuration();
            diagram_copy.signals.erase(diagram_copy.signals.begin());
        } else {
            shifted += shift_per_signal;
            signal.decrease_duration(shift_per_signal);
        }
    }
    if (!diagram_copy.signals.empty()) diagram_copy.signals.back().increase_duration(shift);
    return diagram_copy;
}

TimingDiagram TimingDiagram::operator>>(const size_t shift) const {
    size_t shifted = 0;

    TimingDiagram diagram_copy = *this;

    while (shifted < shift && !diagram_copy.signals.empty()) {
        TernarySignal &signal = diagram_copy.signals.back();
        const size_t shift_per_signal = shift - shifted;
        if (signal.getDuration() <= shift_per_signal) {
            shifted += signal.getDuration();
            diagram_copy.signals.pop_back();
        } else {
            shifted += shift_per_signal;
            signal.decrease_duration(shift_per_signal);
        }
    }
    if (!diagram_copy.signals.empty()) diagram_copy.signals.front().increase_duration(shift);
    return diagram_copy;
}
