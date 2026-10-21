#include "TernarySignal.hpp"

#include <iostream>
#include <format>
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
    if (duration == 0) throw std::logic_error("can't split empty signal");
    if (moment == 0 or moment >= duration) throw std::logic_error("split moment must be inside duration");

    const TernarySignal second_part{state, duration - moment};
    this->duration = moment;

    return second_part;
}

void TernarySignal::increase_duration(const size_t increase_amount) {
    duration += increase_amount;
}

void TernarySignal::decrease_duration(const size_t decrease_amount) {
    if (duration == 0) throw std::logic_error("can't decrease duration of empty signal");
    if (decrease_amount >= duration) throw std::logic_error("decrease can't be more than duration");
    duration -= decrease_amount;
}

std::string TernarySignal::draw() const {
    std::string signal_visual{};
    if (duration == 0) return signal_visual;

    signal_visual.reserve(duration);

    for (int i = 0; i < duration; i++) {
        signal_visual.push_back(get_state_symbol(state));
    }

    return signal_visual;
}

std::ostream &operator <<(std::ostream &stream, const TernarySignal &signal) {
    stream << signal.draw() << std::endl;
    return stream;
}

std::istream &operator >>(std::istream &stream, TernarySignal &signal) {
    std::string signal_string;
    stream >> signal_string;
    if (!stream) return stream;

    if (signal_string.empty()) {
        stream.setstate(std::ios::failbit);
        return stream;
    }

    TernarySignalState new_state{};
    std::string same_state_chars{};
    switch (signal_string.front()) {
        case '0':
        case '_':
            new_state = SIGNAL_ZERO;
            same_state_chars = "0_";
            break;
        case '1':
        case '\'':
            new_state = SIGNAL_ONE;
            same_state_chars = "1'";
            break;
        case 'X':
        case '-':
            new_state = SIGNAL_UNDEFINED;
            same_state_chars = "X-";
            break;
        default:
            stream.setstate(std::ios::failbit);
            return stream;
    }

    if (signal_string.find_first_not_of(same_state_chars) != std::string::npos) {
        stream.setstate(std::ios::failbit);
        return stream;
    }

    signal.setState(new_state);
    signal.setDuration(signal_string.length());
    return stream;
}
