#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <string>
#include "../DataInputService.h"

using namespace std;
namespace DISC = DataInputService::detail;

TEST_CASE("Testing password checking in datainputservice", "[auth][password]") {
    SECTION("ValidPasswordExpectedTrue") {
        string password = "12345678";
        REQUIRE(DataInputService::passwordValid(password) == true);
    }

    SECTION("ValidPasswordExpectedTrue") {
        string password = "123455760";
        REQUIRE(DataInputService::passwordValid(password) == true);
    }

    SECTION("PasswordWithInvalidCharacterExpectedFalse") {
        string password = "12345&6789";
        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }

    SECTION("PasswordWithTwoInvalidCharacterExpectedFalse") {
        string password = "12#345&6789";
        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }

    SECTION("PasswordWithEmptyCharacterExpectedFalse") {
        string password = "12345 36789";
        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }

    SECTION("PasswordWithEmpyCharacterInfrontExpectedFalse") {
        string password = " 1234536789";
        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }

    SECTION("PasswordWithEmpyCharactersExpectedFalse") {
        string password = " 12345    367893";
        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }

    SECTION("PasswordWithNotPermittedCharacterExpectedFalse") {
        string password = "12345&36789";
        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }

    SECTION("PasswordMoreThanMaxValidLengthCharactersExpectedFalse") {
        const int maxValidLength = DISC::kMaxPasswordLength;
        string password = string(maxValidLength + 2, 'b');

        REQUIRE(DataInputService::passwordValid(password)
                == false);
    }
}