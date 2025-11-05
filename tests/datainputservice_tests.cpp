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

TEST_CASE("Email validation - aliases, subdomains, IP literals, quoted local", "[email]") {
    Config cfg;
    cfg.kMinEmailLocalPartLength = 1;
    cfg.kMaxEmailLocalPartLength = 64;

    SECTION("Simple valid email") {
        auto r = emailValidDetailed("user@example.com", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Email with plus alias") {
        auto r = emailValidDetailed("user+alias@sub.domain.com", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Email with IP literal") {
        auto r = emailValidDetailed("user@[192.168.1.1]", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Quoted local part") {
        auto r = emailValidDetailed("\"john..doe\"@example.com", cfg);
        REQUIRE(r.valid == true);
    }

    SECTION("Local part too long") {
        string local(cfg.kMaxEmailLocalPartLength + 1, 'a');
        auto r = emailValidDetailed(local + "@example.com", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "local_too_long");
    }

    SECTION("Missing at") {
        auto r = emailValidDetailed("notanemail", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "no_at");
    }
}

TEST_CASE("Chained validation - form level", "[form]") {
    Config cfg;
    UserInput good{"John Doe", "john+dev@example.com", "GoodP@ss1", "dev_tag"};
    auto r = validateUserInput(good, cfg);
    REQUIRE(r.valid == true);

    UserInput badName{"", "john+dev@example.com", "GoodP@ss1", "dev_tag"};
    auto r2 = validateUserInput(badName, cfg);
    REQUIRE(r2.valid == false);
    REQUIRE(r2.message == "empty");
}

