#include "TernarySignal.hpp"

#include <csignal>
#include <stdexcept>

char TernarySignal::get_state_symbol(const TernarySignalState raw_state) {
    if (!is_valid_state(raw_state)) throw std::logic_error("state does not exist");
    switch (raw_state) {
        case SIGNAL_ZERO: return '_';
        case SIGNAL_ONE: return '\'';
        case SIGNAL_UNDEFINED: return '-';
    }
}

bool TernarySignal::is_valid_state(const TernarySignalState raw_state) {
    switch (raw_state) {
        case SIGNAL_ZERO:
        case SIGNAL_ONE:
        case SIGNAL_UNDEFINED: return true;
    }
    return false;
}

TernarySignal::TernarySignal(const std::string &signal_string) {
    if (signal_string.empty()) throw std::logic_error("signal can not be empty");
    const char signal_state = signal_string.front();

    switch (signal_state) {
        case '0':
            state = SIGNAL_ZERO;
            break;
        case '1':
            state = SIGNAL_ONE;
            break;
        case 'X':
            state = SIGNAL_UNDEFINED;
            break;
        default:
            throw std::invalid_argument("signal can be '0', '1' or 'X'");
    }

    duration = signal_string.find_first_not_of(signal_state);
    if (duration == std::string::npos) duration = signal_string.length();
}

TernarySignal TernarySignal::split(const size_t moment) {
    if (moment >= duration) throw std::logic_error("split moment must be inside duration");

    const TernarySignal second_part{state, duration - moment};
    this->duration = moment;

    return second_part;
}

void TernarySignal::increase_duration(const size_t increase_amount) {
    duration += increase_amount;
}

void TernarySignal::decrease_duration(const size_t decrease_amount) {
    if (decrease_amount >= duration) throw std::logic_error("decrease can't be more than duration");
    duration -= decrease_amount;
}

std::string TernarySignal::draw() const {
    std::string signal_visual{};
    signal_visual.reserve(duration);

    for (int i = 0; i < duration; i++) {
        signal_visual.push_back(get_state_symbol(state));
    }

    return signal_visual;
}
