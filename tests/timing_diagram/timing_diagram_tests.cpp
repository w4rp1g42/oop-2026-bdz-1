#include <catch2/catch_all.hpp>

#include "TimingDiagram.hpp"

TEST_CASE("Timing Diagram Constructors") {
    SECTION("Empty") {
        const TimingDiagram timing_diagram{};
        REQUIRE(timing_diagram.getSignals().empty() == true);
    }
    SECTION("State And Duration: normal") {
        const TimingDiagram timing_diagram{SIGNAL_ZERO, 15};
        REQUIRE(timing_diagram.getSignals().size() == 1);

        TernarySignal ternary_signal = timing_diagram.getSignals().front();
        REQUIRE(ternary_signal.getState() == SIGNAL_ZERO);
        REQUIRE(ternary_signal.getDuration() == 15);
    }
    SECTION("State And Duration: zero duration") {
        REQUIRE_THROWS_AS(TimingDiagram(SIGNAL_ZERO, 0), std::logic_error);
    }
    SECTION("Signal String: 1 state") {
        const TimingDiagram timing_diagram{"11111"};
        REQUIRE(timing_diagram.getSignals().size() == 1);

        TernarySignal ternary_signal = timing_diagram.getSignals().front();
        REQUIRE(ternary_signal.getState() == SIGNAL_ONE);
        REQUIRE(ternary_signal.getDuration() == 5);
    }
    SECTION("Signal String: >1 states") {
        const TimingDiagram timing_diagram{"111110000XXX"};
        REQUIRE(timing_diagram.getSignals().size() == 3);

        REQUIRE(timing_diagram.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram.getSignals()[0].getDuration() == 5);
        REQUIRE(timing_diagram.getSignals()[1].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram.getSignals()[1].getDuration() == 4);
        REQUIRE(timing_diagram.getSignals()[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(timing_diagram.getSignals()[2].getDuration() == 3);
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
        REQUIRE(timing_diagram1.getSignals().size() == 5);
        REQUIRE(timing_diagram1.getSignals()[0].getDuration() == 5);
        REQUIRE(timing_diagram1.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram1.getSignals()[1].getDuration() == 4);
        REQUIRE(timing_diagram1.getSignals()[1].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram1.getSignals()[2].getDuration() == 3);
        REQUIRE(timing_diagram1.getSignals()[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(timing_diagram1.getSignals()[3].getDuration() == 4);
        REQUIRE(timing_diagram1.getSignals()[3].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram1.getSignals()[4].getDuration() == 3);
        REQUIRE(timing_diagram1.getSignals()[4].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Addition: 1 + empty") {
        TimingDiagram timing_diagram1{"111110000XXX"};
        const TimingDiagram timing_diagram2{};

        timing_diagram1 += timing_diagram2;
        REQUIRE(timing_diagram1.getSignals().size() == 3);
        REQUIRE(timing_diagram1.getSignals()[0].getDuration() == 5);
        REQUIRE(timing_diagram1.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram1.getSignals()[1].getDuration() == 4);
        REQUIRE(timing_diagram1.getSignals()[1].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram1.getSignals()[2].getDuration() == 3);
        REQUIRE(timing_diagram1.getSignals()[2].getState() == SIGNAL_UNDEFINED);
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
}

TEST_CASE("Timing Diagram Multiplication") {
    SECTION("Multiplication: normal") {
        const TimingDiagram timing_diagram1{"111110000"};

        TimingDiagram timing_diagram2 = timing_diagram1 * 2;
        REQUIRE(timing_diagram2.getSignals()[0].getDuration() == 5);
        REQUIRE(timing_diagram2.getSignals()[1].getDuration() == 4);
        REQUIRE(timing_diagram2.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram2.getSignals()[1].getState() == SIGNAL_ZERO);
    }
    SECTION("Multiplication: diagram empty") {
        const TimingDiagram timing_diagram1{};

        TimingDiagram timing_diagram2 = timing_diagram1 * 2;
        REQUIRE(timing_diagram2.getSignals().empty() == true);
    }
    SECTION("Multiplication: multiply by zero") {
        const TimingDiagram timing_diagram1{"111110000XXX"};

        TimingDiagram timing_diagram2 = timing_diagram1 * 0;
        REQUIRE(timing_diagram2.getSignals().empty() == true);
    }
}

TEST_CASE("Timing Diagram Replace With Other") {
    SECTION("Replace: normal") {
        TimingDiagram timing_diagram1{"111100XXX11"};
        const TimingDiagram timing_diagram2{"11111"};

        timing_diagram1.replaceWithOther(timing_diagram2, 5, 10);
        REQUIRE(timing_diagram1.getSignals()[0].getDuration() == 4);
        REQUIRE(timing_diagram1.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram1.getSignals()[1].getDuration() == 1);
        REQUIRE(timing_diagram1.getSignals()[1].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram1.getSignals()[2].getDuration() == 5);
        REQUIRE(timing_diagram1.getSignals()[2].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram1.getSignals()[3].getDuration() == 1);
        REQUIRE(timing_diagram1.getSignals()[3].getState() == SIGNAL_ONE);
    }
    SECTION("Replace: beginning") {
        TimingDiagram timing_diagram1{"111100XXX11"};
        const TimingDiagram timing_diagram2{"11111"};

        timing_diagram1.replaceWithOther(timing_diagram2, 0, 5);
        REQUIRE(timing_diagram1.getSignals()[0].getDuration() == 5);
        REQUIRE(timing_diagram1.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram1.getSignals()[1].getDuration() == 1);
        REQUIRE(timing_diagram1.getSignals()[1].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram1.getSignals()[2].getDuration() == 3);
        REQUIRE(timing_diagram1.getSignals()[2].getState() == SIGNAL_UNDEFINED);
        REQUIRE(timing_diagram1.getSignals()[3].getDuration() == 2);
        REQUIRE(timing_diagram1.getSignals()[3].getState() == SIGNAL_ONE);
    }
    SECTION("Replace: invalid area") {
        TimingDiagram timing_diagram1{"111100XXX11"};
        const TimingDiagram timing_diagram2{"11111"};

        REQUIRE_THROWS(timing_diagram1.replaceWithOther(timing_diagram2, 50, 55));
        REQUIRE_THROWS(timing_diagram1.replaceWithOther(timing_diagram2, 3, 1));
        REQUIRE_THROWS(timing_diagram1.replaceWithOther(timing_diagram2, 0, 0));
        REQUIRE_THROWS(timing_diagram1.replaceWithOther(timing_diagram2, 1, 8));
    }
}

TEST_CASE("Timing Diagram Shifts") {
    SECTION("Shift >>: normal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        timing_diagram >> 5;
        REQUIRE(timing_diagram.getSignals()[0].getDuration() == 10);
        REQUIRE(timing_diagram.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram.getSignals()[1].getDuration() == 2);
        REQUIRE(timing_diagram.getSignals()[1].getState() == SIGNAL_ZERO);
    }
    SECTION("Shift >>: remove signal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        timing_diagram >> 3;
        REQUIRE(timing_diagram.getSignals()[0].getDuration() == 8);
        REQUIRE(timing_diagram.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram.getSignals()[1].getDuration() == 4);
        REQUIRE(timing_diagram.getSignals()[1].getState() == SIGNAL_ZERO);
    }
    SECTION("Shift <<: normal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        timing_diagram << 3;
        REQUIRE(timing_diagram.getSignals()[0].getDuration() == 2);
        REQUIRE(timing_diagram.getSignals()[0].getState() == SIGNAL_ONE);
        REQUIRE(timing_diagram.getSignals()[1].getDuration() == 4);
        REQUIRE(timing_diagram.getSignals()[1].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram.getSignals()[2].getDuration() == 6);
        REQUIRE(timing_diagram.getSignals()[2].getState() == SIGNAL_UNDEFINED);
    }
    SECTION("Shift <<: remove signal") {
        TimingDiagram timing_diagram{"111110000XXX"};
        timing_diagram << 5;
        REQUIRE(timing_diagram.getSignals()[0].getDuration() == 4);
        REQUIRE(timing_diagram.getSignals()[0].getState() == SIGNAL_ZERO);
        REQUIRE(timing_diagram.getSignals()[1].getDuration() == 8);
        REQUIRE(timing_diagram.getSignals()[1].getState() == SIGNAL_UNDEFINED);
    }
}
