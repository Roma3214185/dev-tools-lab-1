#ifndef DATAINPUTSERVICE_H
#define DATAINPUTSERVICE_H

#include <string>

namespace DataInputService {
    namespace detail {
        inline constexpr int kMinPasswordLength = 8;
        inline constexpr int kMaxPasswordLength = 22;
        inline constexpr int kMinTagLength = 4;
        inline constexpr int kMaxTagLength = 11;
        inline constexpr int kMinLenOfName = 4;
        inline constexpr int kMaxLenOfName = 20;
        inline constexpr int kMinEmailLocalPartLength = 4;
        inline constexpr int kMaxEmailLocalPartLength = 26;
        inline const std::string kEmailDomain = "@gmail.com";
    }

    bool emailValid(const std::string& login);

    bool passwordValid(const std::string& password);
    bool passwordValidLength(const std::string& password);
    bool passwordValidCharacters(const std::string& password);

    bool tagValid(const std::string& tag);
    bool tagValidCharacters(const std::string& tag);

    bool nameValid(const std::string& name);

} // namespace DataInputService

#endif // DATAINPUTSERVICE_H

