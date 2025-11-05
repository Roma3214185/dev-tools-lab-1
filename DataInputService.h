#ifndef DATAINPUTSERVICE_H
#define DATAINPUTSERVICE_H

#include <string>

struct ValidationResult {
    bool valid;
    std::string message;
};

struct Config {
    size_t kMinLenOfName = 1;
    size_t kMaxLenOfName = 64;

    size_t kMinPasswordLength = 8;
    size_t kMaxPasswordLength = 64;

    size_t kMinTagLength = 2;
    size_t kMaxTagLength = 32;

    size_t kMinEmailLocalPartLength = 1;
    size_t kMaxEmailLocalPartLength = 64;
    std::string kEmailDomain = "@gmail.com";
};

struct UserInput {
    std::string name;
    std::string email;
    std::string password;
    std::string tag;
};

namespace DataInputService {
    ValidationResult nameValidDetailed(const std::string& name, const Config& cfg = Config());
    ValidationResult emailValidDetailed(const std::string& email, const Config& cfg = Config());
    ValidationResult passwordValidDetailed(const std::string& password, const Config& cfg = Config());
    ValidationResult tagValidDetailed(const std::string& tag, const Config& cfg = Config());

    bool nameValid(const std::string& name, const Config& cfg = Config());
    bool emailValid(const std::string& login, const Config& cfg = Config());
    bool passwordValid(const std::string& password, const Config& cfg = Config());
    bool tagValid(const std::string& tag, const Config& cfg = Config());

    ValidationResult validateUserInput(const UserInput& input, const Config& cfg = Config());

} // namespace DataInputService

#endif // DATAINPUTSERVICE_H

