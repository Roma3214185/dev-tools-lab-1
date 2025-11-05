#include "DataInputService.h"
#include <algorithm>
#include <cctype>
#include <regex>

using namespace std;

namespace DataInputService {

    static bool isAsciiLetterOrNumber(char ch) {
        return std::isalnum(static_cast<unsigned char>(ch));
    }

    static bool isControlOrSpace(char ch) {
        unsigned char uch = static_cast<unsigned char>(ch);
        return std::isspace(uch) || std::iscntrl(uch);
    }

    ValidationResult nameValidDetailed(const string& name, const Config& cfg) {
        if (name.empty()) return {false,  "Name is empty"};
        if (name.size() < cfg.kMinLenOfName) return {false,  "Name is shorter than minimum"};
        if (name.size() > cfg.kMaxLenOfName) return {false,  "Name is longer than maximum"};

        for (unsigned char uch : name) {
            if (uch >= 0x80) continue;
            if (isControlOrSpace(static_cast<char>(uch)) && uch != ' ') return {false, "Name contains control characters"};
            if (std::ispunct(uch) && uch != '\'' && uch != '-' && uch != ' ') return {false,  "Name contains invalid punctuation"};
        }
        return {true,  "Name is valid"};
    }

    ValidationResult emailValidDetailed(const string& email, const Config& cfg) {
        if (email.empty()) return {false,  "Email is empty"};

        auto atPos = email.find('@');
        if (atPos == string::npos) return {false,  "Email does not contain @"};
        string local = email.substr(0, atPos);
        string domain = email.substr(atPos + 1);

        if (local.empty()) return {false, "Local part is empty"};
        if (local.size() < cfg.kMinEmailLocalPartLength) return {false,  "Local part too short"};
        if (local.size() > cfg.kMaxEmailLocalPartLength) return {false,  "Local part too long"};

        bool isQuoted = (local.size() >= 2 && local.front() == '"' && local.back() == '"');
        if (!isQuoted) {
            if (local.front() == '.' || local.back() == '.') return {false,  "Local part starts/ends with dot"};
            for (size_t i = 0; i < local.size(); ++i) {
                unsigned char c = static_cast<unsigned char>(local[i]);
                if (c < 0x20) return {false,  "Local part contains control characters"};
                if (local[i] == '.') {
                    if (i + 1 < local.size() && local[i + 1] == '.') return {false,  "Local part has consecutive dots"};
                    continue;
                }
                const string allowedSymbols = "!#$%&'*+-/=?^_`{|}~";
                if (std::isalnum(c) || (allowedSymbols.find(local[i]) != string::npos) || local[i] == '+') continue;
                return {false, "Local part contains invalid character"};
            }
        }


        if (domain.empty()) return {false,  "Domain is empty"};

        if (domain.front() == '[' && domain.back() == ']') {
            string inside = domain.substr(1, domain.size() - 2);
            if (inside.empty()) return {false,  "Empty IP literal"};
            for (char c : inside) {
                if (!(std::isdigit(static_cast<unsigned char>(c)) || c == '.' || c == ':'))
                    return {false,  "IP literal contains invalid char"};
            }
            return {true,  "Email is valid"};
        }

        if (domain.size() > 255) return {false,  "Domain too long"};
        size_t last = 0;
        size_t labelLen = 0;
        for (size_t i = 0; i <= domain.size(); ++i) {
            if (i == domain.size() || domain[i] == '.') {
                if (labelLen == 0) return {false,  "Domain contains empty label"};

                char first = domain[last];
                char lastc = domain[i - 1];
                if (static_cast<unsigned char>(first) < 0x80 && first == '-') return {false,  "Label starts with -"};
                if (static_cast<unsigned char>(lastc) < 0x80 && lastc == '-') return {false,  "Label ends with -"};
                last = i + 1;
                labelLen = 0;
            } else {
                ++labelLen;
            }
        }
        return {true,  "Email is valid"};
    }

    ValidationResult passwordValidDetailed(const string& password, const Config& cfg) {
        if (password.empty()) return {false,  "Password is empty"};
        if (password.size() < cfg.kMinPasswordLength) return {false,  "Password is too short"};
        if (password.size() > cfg.kMaxPasswordLength) return {false,  "Password is too long"};


        const string allowedSymbols = "!$_+@#%&*-";
        for (size_t i = 0; i < password.size(); ++i) {
            unsigned char c = static_cast<unsigned char>(password[i]);
            if (c >= 0x80) continue;
            if (isControlOrSpace(static_cast<char>(c))) return {false, "Password contains space or control character"};
            if (std::isalnum(c)) continue;
            if (allowedSymbols.find(password[i]) != string::npos) continue;
            return {false,  "Password contains invalid character"};
        }
        return {true, "Password is valid"};
    }

    ValidationResult tagValidDetailed(const string& tag, const Config& cfg) {
        if (tag.empty()) return {false,  "Tag is empty"};
        if (tag.size() < cfg.kMinTagLength) return {false, "Tag too short"};
        if (tag.size() > cfg.kMaxTagLength) return {false,  "Tag too long"};

        unsigned char first = static_cast<unsigned char>(tag.front());
        if (first < 0x80 && !std::isalnum(first)) return {false,  "First character must be letter or number"};

        bool prevWasUnderscore = false;
        for (size_t i = 0; i < tag.size(); ++i) {
            unsigned char c = static_cast<unsigned char>(tag[i]);
            if (c >= 0x80) { prevWasUnderscore = false; continue; }
            if (std::isalnum(c)) { prevWasUnderscore = false; continue; }
            if (c == '_') {
                if (prevWasUnderscore) return {false,  "Tag contains consecutive underscores"};
                prevWasUnderscore = true;
                continue;
            }

            if (c == '-' || c == '.') { prevWasUnderscore = false; continue; }
            return {false,  "Tag contains invalid character"};
        }
        return {true, "Tag is valid"};
    }

}
