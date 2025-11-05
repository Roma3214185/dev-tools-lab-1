#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include "../DataInputService.h"

using namespace std;
using namespace DataInputService;

TEST_CASE("Name validation - boundaries and message", "[name]") {
    Config cfg;
    cfg.kMinLenOfName = 1;
    cfg.kMaxLenOfName = 10;

    SECTION("Empty name") {
        auto r = nameValidDetailed("", cfg);
        REQUIRE(r.valid == false);
        REQUIRE(r.message == "Name is empty");
    }

    SECTION("Exact min length") {
        auto r = nameValidDetailed("A", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Exact max length") {
        string s(cfg.kMaxLenOfName, 'x');
        auto r = nameValidDetailed(s, cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Unicode name allowed") {
        auto r = nameValidDetailed("Іван Петренко", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Name with apostrophe and hyphen") {
        auto r = nameValidDetailed("O'Connor-Jr", cfg);
        REQUIRE(r.valid == true);
    }
}

TEST_CASE("Tag validation - mixed patterns and edge cases", "[tag]") {
    Config cfg;
    cfg.kMinTagLength = 2;
    cfg.kMaxTagLength = 16;

    SECTION("Empty tag") {
        auto r = tagValidDetailed("", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "empty");
    }

    SECTION("Starts with underscore -> invalid") {
        auto r = tagValidDetailed("_abc", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "first_invalid");
    }

    SECTION("Consecutive underscores -> invalid") {
        auto r = tagValidDetailed("ab__cd", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "consecutive_underscore");
    }

    SECTION("Allow hyphen and dot inside") {
        auto r = tagValidDetailed("tag-name.v1", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Unicode tag start") {
        auto r = tagValidDetailed("тег1", cfg);
        REQUIRE(r.valid == true);
    }
}

