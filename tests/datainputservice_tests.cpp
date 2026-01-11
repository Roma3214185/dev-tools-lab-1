#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include "../DataInputService.h"
#include <cctype>
#include <locale>

using namespace std;
using namespace DataInputService;

TEST_CASE("Name validation - UTF-8 friendly", "[name]") {
    Config cfg;
    cfg.kMinLenOfName = 1;
    cfg.kMaxLenOfName = 10;

    SECTION("Empty name") {
        auto r = nameValidDetailed("", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Name is empty");
    }

    SECTION("Exact min length") {
        auto r = nameValidDetailed("A", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Exact max length") {
        auto r = nameValidDetailed("abcdefghij", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Too short") {
        auto r = nameValidDetailed("", cfg);
        REQUIRE(!r.valid);
    }

    SECTION("Too long") {
        auto r = nameValidDetailed("abcdefghijk", cfg);
        REQUIRE(!r.valid);
    }

    SECTION("Control character") {
        auto r = nameValidDetailed("John\nDoe", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Name contains invalid character");
    }

    SECTION("Leading/trailing space") {
        auto r = nameValidDetailed(" John", cfg);
        REQUIRE(r.valid);
        auto r2 = nameValidDetailed("John ", cfg);
        REQUIRE(r2.valid);
    }

    SECTION("Invalid symbol") {
        auto r = nameValidDetailed("John@", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Name contains invalid character");
    }
}

TEST_CASE("Password validation - boundaries, symbols, unicode, control chars", "[password]") {
    Config cfg;
    cfg.kMinPasswordLength = 8;
    cfg.kMaxPasswordLength = 12;

    SECTION("Exactly min length") {
        auto r = passwordValidDetailed("Abc12345", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Exactly max length") {
        auto r = passwordValidDetailed("Abc12345!@#", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Contains space") {
        auto r = passwordValidDetailed("Abc 12345", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Password contains space or control character");
    }

    SECTION("Contains control char") {
        string p = "Abc12345";
        p.push_back('\n');
        auto r = passwordValidDetailed(p, cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Password contains space or control character");
    }

    SECTION("Contains unicode characters") {
        auto r = passwordValidDetailed("Pässw0rd✨", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Contains invalid symbol") {
        auto r = passwordValidDetailed("Abc|12345", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Password contains invalid character");
    }

    SECTION("Only letters") {
        auto r = passwordValidDetailed("abcdefgh", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Only digits") {
        auto r = passwordValidDetailed("12345678", cfg);
        REQUIRE(r.valid);
    }
}

TEST_CASE("Tag validation - mixed patterns, boundaries, unicode", "[tag]") {
    Config cfg;
    cfg.kMinTagLength = 2;
    cfg.kMaxTagLength = 16;

    SECTION("Empty tag") {
        auto r = tagValidDetailed("", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Tag is empty");
    }

    SECTION("Starts with underscore") {
        auto r = tagValidDetailed("_abc", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "First character must be letter or number");
    }

    SECTION("Consecutive underscores") {
        auto r = tagValidDetailed("ab__cd", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Tag contains consecutive underscores");
    }

    SECTION("Ends with underscore") {
        auto r = tagValidDetailed("tag_", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Max length tag") {
        string s(cfg.kMaxTagLength, 'a');
        auto r = tagValidDetailed(s, cfg);
        REQUIRE(r.valid);
    }

    SECTION("Unicode tag") {
        auto r = tagValidDetailed("тег1", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Mixed valid/invalid pattern") {
        auto r = tagValidDetailed("tag_name-1.", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Invalid symbol") {
        auto r = tagValidDetailed("tag!name", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Tag contains invalid character");
    }
}

TEST_CASE("Email validation - aliases, subdomains, IP literals, quoted local, boundaries", "[email]") {
    Config cfg;
    cfg.kMinEmailLocalPartLength = 1;
    cfg.kMaxEmailLocalPartLength = 64;

    SECTION("Simple valid") {
        auto r = emailValidDetailed("user@example.com", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Plus alias") {
        auto r = emailValidDetailed("user+alias@sub.domain.com", cfg);
        REQUIRE(r.valid);
    }

    SECTION("IP literal") {
        auto r = emailValidDetailed("user@[192.168.1.1]", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Quoted local") {
        auto r = emailValidDetailed("\"john..doe\"@example.com", cfg);
        REQUIRE(r.valid);
    }

    SECTION("Local part too long") {
        string local(cfg.kMaxEmailLocalPartLength + 1, 'a');
        auto r = emailValidDetailed(local + "@example.com", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Local part too long");
    }

    SECTION("Local part too short") {
        auto r = emailValidDetailed("@example.com", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Local part is empty");
    }

    SECTION("Missing at") {
        auto r = emailValidDetailed("notanemail", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Email does not contain @");
    }

    SECTION("Empty domain") {
        auto r = emailValidDetailed("user@", cfg);
        REQUIRE(!r.valid);
        REQUIRE(r.message == "Domain is empty");
    }
}

TEST_CASE("Chained validation - form level", "[form]") {
    Config cfg;
    UserInput good{"John Doe", "john+dev@example.com", "GoodP@ss1", "dev_tag"};
    auto r = validateUserInput(good, cfg);
    REQUIRE(r.valid);

    UserInput badName{"", "john+dev@example.com", "GoodP@ss1", "dev_tag"};
    auto r2 = validateUserInput(badName, cfg);
    REQUIRE(!r2.valid);
    REQUIRE(r2.message == "Name is empty");

    UserInput badEmail{"John Doe", "invalidemail", "GoodP@ss1", "dev_tag"};
    auto r3 = validateUserInput(badEmail, cfg);
    REQUIRE(!r3.valid);
    REQUIRE(r3.message == "Email does not contain @");

    UserInput badPassword{"John Doe", "john@example.com", "short", "dev_tag"};
    auto r4 = validateUserInput(badPassword, cfg);
    REQUIRE(!r4.valid);
    REQUIRE(r4.message == "Password is too short");

    UserInput badTag{"John Doe", "john@example.com", "GoodP@ss1", "_tag"};
    auto r5 = validateUserInput(badTag, cfg);
    REQUIRE(!r5.valid);
    REQUIRE(r5.message == "First character must be letter or number");
}

TEST_CASE("Parameterized invalid passwords", "[password][param]") {
    Config cfg;
    cfg.kMinPasswordLength = 6;
    cfg.kMaxPasswordLength = 12;

    auto password_sample = GENERATE("short", "has space", "bad|char", "\nnewline");
    auto r = passwordValidDetailed(password_sample, cfg);
    REQUIRE(!r.valid);
}
