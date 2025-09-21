#include <catch2/catch_test_macros.hpp>
#include <ctime>
#include <string>
#include "../Materials/Functions.h"
#include "../Molecules/String.h"

using namespace Groundfloor;

TEST_CASE("TimestampToStr basic functionality", "[timestamp]") {
    __int64 timestamp = 1609459200; // 2021-01-01 00:00:00 UTC

    SECTION("Basic date format") {
        String* result = TimestampToStr("%Y-%m-%d", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->match("2021-01-01"));
        delete result;
    }

    SECTION("Full datetime format") {
        String* result = TimestampToStr("%Y-%m-%d %H:%M:%S", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->getLength() > 0);
        INFO("Full datetime: " << result->getValue());
        delete result;
    }
}

TEST_CASE("TimestampToStr various formats", "[timestamp]") {
    __int64 timestamp = time(nullptr);

    SECTION("Year only") {
        String* result = TimestampToStr("%Y", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->getLength() == 4);
        delete result;
    }

    SECTION("US date format") {
        String* result = TimestampToStr("%m/%d/%Y", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->getLength() >= 8);
        delete result;
    }

    SECTION("Time only") {
        String* result = TimestampToStr("%H:%M:%S", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->getLength() == 8);
        delete result;
    }

    SECTION("Full date with day name") {
        String* result = TimestampToStr("%a, %d %b %Y %H:%M:%S", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->getLength() > 0);
        delete result;
    }

    SECTION("ISO 8601 format") {
        String* result = TimestampToStr("%Y-%m-%d %H:%M:%S", timestamp);
        REQUIRE(result != nullptr);
        REQUIRE(result->getLength() == 19);
        delete result;
    }
}

TEST_CASE("TimestampToStr invalid format validation", "[timestamp][validation]") {
    __int64 timestamp = 1609459200;

    SECTION("Invalid format specifier %s") {
        REQUIRE_THROWS_AS(TimestampToStr("D:\\Log\\%ss.txt", timestamp), std::runtime_error);

        try {
            TimestampToStr("D:\\Log\\%ss.txt", timestamp);
            FAIL("Expected exception was not thrown");
        } catch (const std::runtime_error& e) {
            std::string msg = e.what();
            REQUIRE(msg.find("%s") != std::string::npos);
            INFO("Exception message: " << e.what());
        }
    }

    SECTION("Invalid format with modifier %Ek") {
        REQUIRE_THROWS_AS(TimestampToStr("%Ek", timestamp), std::runtime_error);

        try {
            TimestampToStr("%Ek", timestamp);
            FAIL("Expected exception was not thrown");
        } catch (const std::runtime_error& e) {
            std::string msg = e.what();
            REQUIRE(msg.find("%Ek") != std::string::npos);
            INFO("Exception message: " << e.what());
        }
    }

    SECTION("Incomplete format specifier at end") {
        REQUIRE_THROWS_AS(TimestampToStr("Test %", timestamp), std::runtime_error);
    }

    SECTION("Valid format specifiers should not throw") {
        REQUIRE_NOTHROW(TimestampToStr("%Y-%m-%d", timestamp));
        REQUIRE_NOTHROW(TimestampToStr("%%", timestamp)); // Literal %
        REQUIRE_NOTHROW(TimestampToStr("%n%t", timestamp)); // Newline and tab
    }
}