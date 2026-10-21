#ifndef TIMINGDIAGRAM_H
#define TIMINGDIAGRAM_H

#include <vector>

#include "TernarySignal.hpp"

class TimingDiagram {
    std::vector<TernarySignal> signals;
public:
    TimingDiagram() = default;

    TimingDiagram(TernarySignalState state, size_t duration) {
        signals.emplace_back(state, duration);
    }

    explicit TimingDiagram(const std::string &signal_string);

    void replaceWithOther(const TimingDiagram &other, size_t start, size_t end);

    TimingDiagram &operator +=(const TimingDiagram &other);

    TimingDiagram operator *(size_t times) const;

    TimingDiagram operator <<(size_t shift) const;

    TimingDiagram operator >>(size_t shift) const;

    std::vector<TernarySignal> getSignals() const {
        return signals;
    }

    size_t getTotalDuration() const;

    friend std::ostream &operator <<(std::ostream &stream, const TimingDiagram &diagram);
    friend std::istream &operator >>(std::istream &stream, TimingDiagram &diagram);
};

#endif //TIMINGDIAGRAM_H
