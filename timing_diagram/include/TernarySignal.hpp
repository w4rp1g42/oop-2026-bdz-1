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
    TernarySignalState state = SIGNAL_ZERO;
    size_t duration = 0;

    static char get_state_symbol(TernarySignalState raw_state);
    static bool is_valid_state(TernarySignalState raw_state);

public:
    // the only way to get zero duration signal
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

    void setState(const TernarySignalState new_state) {
        if (!is_valid_state(new_state)) throw std::logic_error("invalid state");
        state = new_state;
    }

    void setDuration(const size_t new_duration) {
        if (new_duration == 0) throw std::logic_error("can not make signal with duration of 0");
        duration = new_duration;
    }

    friend std::ostream &operator <<(std::ostream &stream, const TernarySignal &signal);
    friend std::istream &operator >>(std::istream &stream, TernarySignal &signal);
};

#endif //TERNARY_SIGNAL_H
