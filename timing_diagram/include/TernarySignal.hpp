#ifndef TERNARY_SIGNAL_H
#define TERNARY_SIGNAL_H

#include <stdexcept>
#include <string>

enum TernarySignalState {
    SIGNAL_ZERO = 0,
    SIGNAL_ONE = 1,
    SIGNAL_UNDEFINED = 2
};

class TernarySignal {
    TernarySignalState state;
    size_t duration;

    static char get_state_symbol(TernarySignalState raw_state);

public:
    TernarySignal() = default;

    TernarySignal(const TernarySignalState state, const size_t duration) : state(state), duration(duration) {
        if (duration == 0) throw std::logic_error("can not make signal with duration of 0");
    }

    explicit TernarySignal(const std::string &signal_string);

    TernarySignal split(size_t moment);

    void increase_duration(size_t increase_amount);

    void decrease_duration(size_t decrease_amount);

    std::string draw() const;

    TernarySignalState getState() const { return state; }
    size_t getDuration() const { return duration; }
};

#endif //TERNARY_SIGNAL_H
