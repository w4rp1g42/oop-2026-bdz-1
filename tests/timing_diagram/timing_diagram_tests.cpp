#include <catch2/catch_all.hpp>

#include "TimingDiagram.hpp"

TEST_CASE("Timing Diagram Constructors") {
    SECTION("Empty") {
        const TimingDiagram timing_diagram{};
        const auto signals = timing_diagram.getSignals();
        REQUIRE(signals.empty() == true);
    }
    SECTION("State And Duration: normal") {
        const TimingDiagram timing_diagram{SIGNAL_ZERO, 15};
        const auto signals = timing_diagram.getSignals();
        REQUIRE(signals.size() == 1);

        TernarySignal ternary_signal = signals.front();
        REQUIRE(ternary_signal.getState() == SIGNAL_ZERO);
        REQUIRE(ternary_signal.getDuration() == 15);
    }
    SECTION("State And Duration: zero duration") {
        REQUIRE_THROWS_AS(TimingDiagram(SIGNAL_ZERO, 0), std::logic_error);
    }
    SECTION("Signal String: 1 state") {
        const TimingDiagram timing_diagram{"11111"};
        const auto signals = timing_diagram.getSignals();
        REQUIRE(signals.size() == 1);

        TernarySignal ternary_signal = signals.front();
        REQUIRE(ternary_signal.getState() == SIGNAL_ONE);
        REQUIRE(ternary_signal.getDuration() == 5);
    }
    SECTION("Signal String: >1 states") {
        const TimingDiagram timing_diagram{"111110000XXX"};
        const auto signals = timing_diagram.getSignals();
        REQUIRE(signals.size() == 3);

        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(signals[2].getDuration() == 3);
    }
    SECTION("Signal String: empty signal") {
        REQUIRE_THROWS_AS(TimingDiagram{std::string{}}, std::logic_error);
    }
}

TEST_CASE("Timing Diagram Addition") {
    SECTION("Addition: 1 + 1") {
        TimingDiagram timing_diagram1{"111110000XXX"};
        const TimingDiagram timing_diagram2{"0000XXX"};

        timing_diagram1 += timing_diagram2;
        const auto signals = timing_diagram1.getSignals();
        REQUIRE(signals.size() == 5);
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 3);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(signals[3].getDuration() == 4);
        REQUIRE(signals[3].getState() == SIGNAL_ZERO);
        REQUIRE(signals[4].getDuration() == 3);
        REQUIRE(signals[4].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Addition: 1 + empty") {
        TimingDiagram timing_diagram1{"111110000XXX"};
        const TimingDiagram timing_diagram2{};

        timing_diagram1 += timing_diagram2;
        const auto signals = timing_diagram1.getSignals();
        REQUIRE(signals.size() == 3);
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 3);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Addition: empty + empty") {
        TimingDiagram timing_diagram1{};
        const TimingDiagram timing_diagram2{};

        timing_diagram1 += timing_diagram2;
        REQUIRE(timing_diagram1.getSignals().empty());
    }
}

TEST_CASE("Timing Diagram Total Duration") {
    SECTION("Duration: normal") {
        const TimingDiagram timing_diagram1{"111110000XXX"};
        REQUIRE(timing_diagram1.getTotalDuration() == 12);
    }
    SECTION("Duration: empty") {
        const TimingDiagram timing_diagram1{};
        REQUIRE(timing_diagram1.getTotalDuration() == 0);
    }
    SECTION("Addition: same + same") {
        TimingDiagram timing_diagram1{"111110000XXX"};

        timing_diagram1 += timing_diagram1;
        const auto signals = timing_diagram1.getSignals();
        REQUIRE(signals.size() == 6);
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 3);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(signals[3].getDuration() == 5);
        REQUIRE(signals[3].getState() == SIGNAL_ONE);
        REQUIRE(signals[4].getDuration() == 4);
        REQUIRE(signals[4].getState() == SIGNAL_ZERO);
        REQUIRE(signals[5].getDuration() == 3);
        REQUIRE(signals[5].getState() == SIGNAL_UNDEFINED);
    }
}

TEST_CASE("Timing Diagram Multiplication") {
    SECTION("Multiplication: normal") {
        const TimingDiagram timing_diagram1{"111110000"};

        TimingDiagram timing_diagram2 = timing_diagram1 * 2;
        const auto signals = timing_diagram2.getSignals();
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
    }
    SECTION("Multiplication: diagram empty") {
        const TimingDiagram timing_diagram1{};

        TimingDiagram timing_diagram2 = timing_diagram1 * 2;
        const auto signals = timing_diagram2.getSignals();
        REQUIRE(signals.empty() == true);
    }
    SECTION("Multiplication: multiply by zero") {
        const TimingDiagram timing_diagram1{"111110000XXX"};

        TimingDiagram timing_diagram2 = timing_diagram1 * 0;
        const auto signals = timing_diagram2.getSignals();
        REQUIRE(signals.empty() == true);
    }
}

TEST_CASE("Timing Diagram Replace With Other") {
    SECTION("Replace: normal") {
        TimingDiagram timing_diagram1{"111000XXX11"};
        const TimingDiagram timing_diagram2{"00111"};

        timing_diagram1.replaceWithOther(timing_diagram2, 6, 11);
        const auto signals = timing_diagram1.getSignals();
        REQUIRE(signals.size() == 4);
        REQUIRE(signals[0].getDuration() == 3);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 3);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 2);
        REQUIRE(signals[2].getState() == SIGNAL_ZERO);
        REQUIRE(signals[3].getDuration() == 3);
        REQUIRE(signals[3].getState() == SIGNAL_ONE);
    }

    SECTION("Replace: exact single signal boundary match") {
        TimingDiagram timing_diagram1{"111000111"};
        const TimingDiagram timing_diagram2{"XXX"};

        timing_diagram1.replaceWithOther(timing_diagram2, 3, 6);
        const auto signals = timing_diagram1.getSignals();
        REQUIRE(signals.size() == 3);
        REQUIRE(signals[0].getDuration() == 3);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 3);
        REQUIRE(signals[1].getState() == SIGNAL_UNDEFINED);
        REQUIRE(signals[2].getDuration() == 3);
        REQUIRE(signals[2].getState() == SIGNAL_ONE);
    }

    SECTION("Replace: entire diagram") {
        TimingDiagram timing_diagram1{"111000"};
        const TimingDiagram timing_diagram2{"000111"};

        timing_diagram1.replaceWithOther(timing_diagram2, 0, 6);
        const auto signals = timing_diagram1.getSignals();
        REQUIRE(signals.size() == 2);
        REQUIRE(signals[0].getDuration() == 3);
        REQUIRE(signals[0].getState() == SIGNAL_ZERO);
        REQUIRE(signals[1].getDuration() == 3);
        REQUIRE(signals[1].getState() == SIGNAL_ONE);
    }

    SECTION("Replace: invalid size mismatch throws") {
        TimingDiagram timing_diagram1{"111000111"};
        const TimingDiagram timing_diagram2{"1111"};

        REQUIRE_THROWS(timing_diagram1.replaceWithOther(timing_diagram2, 3, 6));
    }
}

TEST_CASE("Timing Diagram Shifts") {
    SECTION("Shift >>: normal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram >> 5;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 10);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 2);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
    }
    SECTION("Shift >>: remove signal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram >> 3;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 8);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
    }
    SECTION("Shift >>: shift > total duration") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram >> 20;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 12);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
    }
    SECTION("Shift >>: shift == total duration") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram >> 12;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 12);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
    }
    SECTION("Shift >>: shift == 0") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram >> 0;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 3);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Shift <<: normal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram << 3;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 2);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 6);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Shift <<: remove signal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram << 5;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 4);
        REQUIRE(signals[0].getState() == SIGNAL_ZERO);
        REQUIRE(signals[1].getDuration() == 8);
        REQUIRE(signals[1].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Shift <<: shift > total duration") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram << 20;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 12);
        REQUIRE(signals[0].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Shift <<: shift == total duration") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram << 12;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 12);
        REQUIRE(signals[0].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Shift <<: shift == 0") {
        TimingDiagram timing_diagram{"111110000XXX"};
        TimingDiagram result = timing_diagram << 0;

        const auto signals = result.getSignals();
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[2].getDuration() == 3);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
    }
}

TEST_CASE("Timing Diagram Stream Output") {
    SECTION("Output: normal") {
        std::ostringstream stream;
        stream << TimingDiagram{"111110000XXX"};
        REQUIRE(stream.str() == "\'\'\'\'\'____---\n");
    }
    SECTION("Output: empty") {
        std::ostringstream stream;
        stream << TimingDiagram{};
        REQUIRE(stream.str() == "\n");
    }
}

TEST_CASE("Timing Diagram Stream Input") {
    SECTION("Input: valid digits") {
        TimingDiagram timing_diagram{};
        std::istringstream stream{"111110000XXX"};
        stream >> timing_diagram;

        REQUIRE(stream.fail() == false);
        const auto signals = timing_diagram.getSignals();
        REQUIRE(signals.size() == 3);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(signals[2].getDuration() == 3);
    }
    SECTION("Input: valid symbols") {
        TimingDiagram timing_diagram{};
        std::istringstream stream{"\'\'\'\'\'____---"};
        stream >> timing_diagram;

        REQUIRE(stream.fail() == false);
        const auto signals = timing_diagram.getSignals();
        REQUIRE(signals.size() == 3);
        REQUIRE(signals[0].getState() == SIGNAL_ONE);
        REQUIRE(signals[0].getDuration() == 5);
        REQUIRE(signals[1].getState() == SIGNAL_ZERO);
        REQUIRE(signals[1].getDuration() == 4);
        REQUIRE(signals[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(signals[2].getDuration() == 3);
    }
    SECTION("Input: invalid character") {
        TimingDiagram timing_diagram{};
        std::istringstream stream{"111222"};
        stream >> timing_diagram;

        REQUIRE(stream.fail() == true);
    }
    SECTION("Input: empty") {
        TimingDiagram timing_diagram{};
        std::istringstream stream{std::string{}};
        stream >> timing_diagram;

        REQUIRE(stream.fail() == true);
    }
}