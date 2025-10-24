#include "DataInputService.h"
#include <vector>
#include <algorithm>
#include <cctype>

using namespace DataInputService::detail;
using namespace std;

namespace {

    bool isLetterOrNumber(char ch) {
        return std::isalnum(static_cast<unsigned char>(ch));
    }

    bool firstElementIsLetterOrNumber(const string &tag) {
        return !tag.empty() && isLetterOrNumber(tag.front());
    }

    bool isAllowedSymbol(char ch) {
        return ch == '!' || ch == '$' || ch == '_' || ch == '+';
    }

    bool hasConsecutiveUnderscores(char ch, char prev) {
        return ch == '_' && prev == '_';
    }

} // namespace

namespace DataInputService {

    bool nameValid(const string &name) {
        return name.size() >= kMinLenOfName && name.size() <= kMaxLenOfName;
    }

    bool emailValid(const string &login) {
        if (!login.ends_with(kEmailDomain)) return false;

        const string localPart = login.substr(0, login.size() - kEmailDomain.size());
        if (localPart.size() < kMinEmailLocalPartLength || localPart.size() > kMaxEmailLocalPartLength) return false;

        return std::all_of(localPart.begin(), localPart.end(), isLetterOrNumber);
    }

    bool passwordValidLength(const string &password) {
        return password.size() >= kMinPasswordLength && password.size() <= kMaxPasswordLength;
    }

    bool passwordValidCharacters(const string &password) {
        return std::all_of(password.begin(), password.end(), [](char ch) {
            return isLetterOrNumber(ch) || isAllowedSymbol(ch);
        });
    }

    bool passwordValid(const string &password) {
        return passwordValidLength(password) && passwordValidCharacters(password);
    }

    bool tagValidCharacters(const string &tag) {
        if (!firstElementIsLetterOrNumber(tag)) return false;

        char prevChar = '\n';
        return std::all_of(tag.begin(), tag.end(), [&prevChar](char ch) {
            if (isLetterOrNumber(ch)) {
                prevChar = ch;
                return true;
            }
            if (ch == '_' && !hasConsecutiveUnderscores(ch, prevChar)) {
                prevChar = ch;
                return true;
            }
            return false;
        });
    }

    bool tagValid(const string &tag) {
        return tagValidCharacters(tag) && tag.size() >= kMinTagLength && tag.size() <= kMaxTagLength;
    }

} // namespace DataInputService
