#include <catch2/catch_all.hpp>

#include "TernarySignal.hpp"

TEST_CASE("Ternary Signal Constructors") {
    SECTION("Empty") {
        const TernarySignal ternary_signal{};
        REQUIRE(ternary_signal.getDuration() == 0);
        REQUIRE(ternary_signal.getState() == SIGNAL_ZERO);
    }
    SECTION("State And Duration: normal") {
        const TernarySignal ternary_signal{SIGNAL_ZERO, 15};
        REQUIRE(ternary_signal.getState() == SIGNAL_ZERO);
        REQUIRE(ternary_signal.getDuration() == 15);
    }
    SECTION("State And Duration: zero duration") {
        REQUIRE_THROWS_AS(TernarySignal(SIGNAL_ZERO, 0), std::logic_error);
    }
    SECTION("Signal String: 1 state") {
        const TernarySignal ternary_signal{"11111"};
        REQUIRE(ternary_signal.getState() == SIGNAL_ONE);
        REQUIRE(ternary_signal.getDuration() == 5);
    }
    SECTION("Signal String: >1 states") {
        const TernarySignal ternary_signal{"111110000XXX"};
        REQUIRE(ternary_signal.getState() == SIGNAL_ONE);
        REQUIRE(ternary_signal.getDuration() == 5);
    }
    SECTION("Signal String: empty signal") {
        REQUIRE_THROWS_AS(TernarySignal {std::string{}}, std::logic_error);
    }
}

TEST_CASE("Ternary Signal Split") {
    SECTION("Split: normal") {
        TernarySignal first_part{"111110000XXX"};
        const TernarySignal second_part = first_part.split(3);

        REQUIRE(first_part.getState() == SIGNAL_ONE);
        REQUIRE(first_part.getDuration() == 3);

        REQUIRE(second_part.getState() == SIGNAL_ONE);
        REQUIRE(second_part.getDuration() == 2);
    }
    SECTION("Split: moment >= duration") {
        TernarySignal first_part{"111110000XXX"};
        REQUIRE_THROWS_AS(first_part.split(10), std::logic_error);
    }
}

TEST_CASE("Ternary Signal Duration") {
    SECTION("Increase: normal") {
        TernarySignal ternary_signal{"111110000XXX"};
        ternary_signal.increase_duration(7);
        REQUIRE(ternary_signal.getDuration() == 12);
    }
    SECTION("Increase: add zero") {
        TernarySignal ternary_signal{"111110000XXX"};
        ternary_signal.increase_duration(0);
        REQUIRE(ternary_signal.getDuration() == 5);
    }
    SECTION("Decrease: normal") {
        TernarySignal ternary_signal{"111110000XXX"};
        ternary_signal.decrease_duration(3);
        REQUIRE(ternary_signal.getDuration() == 2);
    }
    SECTION("Decrease: subtract zero") {
        TernarySignal ternary_signal{"111110000XXX"};
        ternary_signal.decrease_duration(0);
        REQUIRE(ternary_signal.getDuration() == 5);
    }
    SECTION("Decrease: subtract > duration") {
        TernarySignal ternary_signal{"111110000XXX"};
        REQUIRE_THROWS_AS(ternary_signal.decrease_duration(10), std::logic_error);
    }
}

TEST_CASE("Ternary Signal Draw") {
    SECTION("Draw: SIGNAL_ONE") {
        const TernarySignal ternary_signal{"111110000XXX"};
        REQUIRE(ternary_signal.draw() == "\'\'\'\'\'");
    }
    SECTION("Draw: SIGNAL_ZERO") {
        const TernarySignal ternary_signal{"0000XXX"};
        REQUIRE(ternary_signal.draw() == "____");
    }
    SECTION("Draw: SIGNAL_UNDEFINED") {
        const TernarySignal ternary_signal{"XXX"};
        REQUIRE(ternary_signal.draw() == "---");
    }
}

TEST_CASE("Ternary Signal Setters") {
    SECTION("Setter: duration, normal") {
        TernarySignal ternary_signal{"111110000XXX"};
        ternary_signal.setDuration(1);
        REQUIRE(ternary_signal.getDuration() == 1);
        REQUIRE(ternary_signal.getState() == SIGNAL_ONE);
    }
    SECTION("Setter: duration, zero duration") {
        TernarySignal ternary_signal{"111110000XXX"};
        REQUIRE_THROWS(ternary_signal.setDuration(0));
    }
    SECTION("Setter: state, normal") {
        TernarySignal ternary_signal{"111110000XXX"};
        ternary_signal.setState(SIGNAL_ZERO);
        REQUIRE(ternary_signal.getDuration() == 5);
        REQUIRE(ternary_signal.getState() == SIGNAL_ZERO);
    }
    SECTION("Setter: state, incorrect state") {
        TernarySignal ternary_signal{"111110000XXX"};
        REQUIRE_THROWS(ternary_signal.setState(static_cast<TernarySignalState>(5)));
    }
}

