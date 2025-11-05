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
    ValidationResult emailValid(const std::string& login);

    ValidationResult passwordValid(const std::string& password);
    ValidationResult passwordValidLength(const std::string& password);
    ValidationResult passwordValidCharacters(const std::string& password);

    ValidationResult tagValid(const std::string& tag);
    ValidationResult tagValidCharacters(const std::string& tag);

    ValidationResult nameValid(const std::string& name);

} // namespace DataInputService

#endif // DATAINPUTSERVICE_H

