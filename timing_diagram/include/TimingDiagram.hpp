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

    explicit TimingDiagram(const std::string &signal_string) {
        signals.emplace_back(signal_string);
    }

    void replaceWithOther(const TimingDiagram &other, size_t start);

    TimingDiagram &operator +=(const TimingDiagram &other);

    TimingDiagram &operator *(const TimingDiagram &other) const;

    TimingDiagram &operator <<(size_t duration);

    TimingDiagram &operator >>(size_t duration);
};

#endif //TIMINGDIAGRAM_H
