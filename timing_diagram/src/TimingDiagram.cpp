#include <ranges>

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

    size_t time_marker = 0;
    std::vector<TernarySignal>::iterator it;
    for (auto cur = signals.begin(); cur != signals.end(); ++cur) {
        if (cur->getDuration() < start - time_marker) {
            // skip if before area
            time_marker += cur->getDuration();
            continue;
        }
        // split if on border of area, remove second part, which is in area
        if (start > time_marker) {
            const TernarySignal second_part = cur->split(start - time_marker);
            time_marker += cur->getDuration();
            it = signals.insert(cur + 1, second_part);
        } else {
            it = cur;
        }

        break;
    }

    auto insert_start = it;

    // remove everything from area
    while (true) {
        if (it != signals.end() and it->getDuration() < end - time_marker) {
            // remove if in area
            time_marker += it->getDuration();
            it = signals.erase(it);
            continue;
        }

        if (end > time_marker) {
            const TernarySignal last = it->split(end - time_marker);
            it = signals.erase(it);
            signals.insert(it, last);
        }

        break;
    }

    for (auto signal: other.getSignals()) {
        signals.insert(insert_start++, signal);
    }
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

TimingDiagram &TimingDiagram::operator<<(const size_t shift) {
    size_t shifted = 0;

    while (shifted < shift && !signals.empty()) {
        TernarySignal &signal = signals.front();
        const size_t shift_per_signal = shift - shifted;
        if (signal.getDuration() <= shift_per_signal) {
            shifted += signal.getDuration();
            signals.erase(signals.begin());
        } else {
            shifted += shift_per_signal;
            signal.decrease_duration(shift_per_signal);
        }
    }
    if (!signals.empty()) signals.back().increase_duration(shift);
    return *this;
}

TimingDiagram &TimingDiagram::operator>>(const size_t shift) {
    size_t shifted = 0;
    while (shifted < shift && !signals.empty()) {
        TernarySignal &signal = signals.back();
        const size_t shift_per_signal = shift - shifted;
        if (signal.getDuration() <= shift_per_signal) {
            shifted += signal.getDuration();
            signals.pop_back();
        } else {
            shifted += shift_per_signal;
            signal.decrease_duration(shift_per_signal);
        }
    }
    if (!signals.empty()) signals.front().increase_duration(shift);
    return *this;
}
