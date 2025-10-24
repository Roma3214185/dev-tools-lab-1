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

TEST_CASE("Testing tag in datainputservice", "[auth][tag]") {

    SECTION("TagEmptyExpectedFalse") {
        string tag = "";
        REQUIRE(DataInputService::tagValid(tag) == false);
    }

    SECTION("TagLessThanMinValidTagLenExpectedFalse") {
        const int minValidTagLen = DISC::kMinTagLength;
        string tag = string(minValidTagLen - 1, 'a');
        REQUIRE(DataInputService::tagValid(tag) == false);
    }

    SECTION("TagStartsWithUnderlineExpectedFalse") {
        string tag = "_r4241";
        REQUIRE(DataInputService::tagValid(tag) == false);
    }

    SECTION("TagWithTwoUnderlineInARowExpectedFalse") {
        string tag = "r__r14";
        REQUIRE(DataInputService::tagValid(tag) == false);
    }

    SECTION("TagWithDotExpectedFalse") {
        string tag = "r.r13";
        REQUIRE(DataInputService::tagValid(tag) == false);
    }
}

TEST_CASE("Testing email in datainputservice", "[auth][email]") {
    SECTION("EmailWithoutDomeinExpectedFalse") {
        string email = "123456789";
        REQUIRE(DataInputService::emailValid(email) == false);
    }

    SECTION("ValidEmailExpectedTrue") {
        string email = "roma@gmail.com";
        REQUIRE(DataInputService::emailValid(email) == true);
    }

    SECTION("EmailWithOnlyDomeinExpectedFalse") {
        string email = "@gmail.com";
        REQUIRE(DataInputService::emailValid(email) == false);
    }

    SECTION("EmailWithInvalidCharacterExpectedFalse") {
        string email = "roma*@gmail.com";
        REQUIRE(DataInputService::emailValid(email) == false);
    }

    SECTION("EmailWithInvalidDomeinExpectedFalse") {
        string email = "roma*@gmailcom";
        REQUIRE(DataInputService::emailValid(email) == false);
    }

    SECTION("EmailIsLessThanMinValidLenExpectedFalse"){
        const int minValidEmailLen = DISC::kMinEmailLocalPartLength;
        string email = string(minValidEmailLen - 1, 'a') + DISC::kEmailDomain;

        REQUIRE(DataInputService::emailValid(email) == false);
    }

    SECTION("EmailIsMoreThanMaxValidLenExpectedFalse") {
        const int maxValidEmailLen = DISC::kMaxEmailLocalPartLength;
        string email = string(maxValidEmailLen + 1, 'a') + DISC::kEmailDomain;

        REQUIRE(DataInputService::emailValid(email) == false);
    }

    SECTION("EmailWithEmptyCharaverExpectedFalse") {
        string email = "rom a@gmailcom";
        REQUIRE(DataInputService::emailValid(email) == false);
    }
    SECTION("EmailWithEmptyCharaverInfrontExpectedFalse") {
        string email = "     roma@gmailcom";
        REQUIRE(DataInputService::emailValid(email) == false);
    }
}