#include "Utils.h"
#include <algorithm>
#include <locale>
#include <codecvt>

namespace Utils {

    // -----------------------------------------------------------
    //  ASCII ToLower (старі виклики, щоб не ламався лінкер)
    // -----------------------------------------------------------
    std::string ToLower(const std::string &s) {
        std::string res = s;
        std::transform(res.begin(), res.end(), res.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return res;
    }

    // -----------------------------------------------------------
    //  UTF-8 ToLower (повна підтримка української мови)
    // -----------------------------------------------------------
    std::string ToLowerUTF8(const std::string &s) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring ws = conv.from_bytes(s);

        // системна локаль (macOS автоматично підтягує uk_UA)
        std::locale loc("");

        for (auto &ch : ws) {
            ch = std::tolower(ch, loc);
        }

        return conv.to_bytes(ws);
    }

    // -----------------------------------------------------------
    //  Trim
    // -----------------------------------------------------------
    std::string Trim(const std::string &s) {
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
            start++;

        if (start == s.size()) return "";

        size_t end = s.size() - 1;
        while (end > start && std::isspace(static_cast<unsigned char>(s[end])))
            end--;

        return s.substr(start, end - start + 1);
    }

    // -----------------------------------------------------------
    //  Split
    // -----------------------------------------------------------
    std::vector<std::string> Split(const std::string &s, char delim) {
        std::vector<std::string> result;
        std::string current;

        for (char ch : s) {
            if (ch == delim) {
                result.push_back(current);
                current.clear();
            } else {
                current.push_back(ch);
            }
        }

        result.push_back(current);
        return result;
    }

    // -----------------------------------------------------------
    //  Join
    // -----------------------------------------------------------
    std::string Join(const std::vector<std::string> &parts, char delim) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result.push_back(delim);
            result += parts[i];
        }
        return result;
    }

    // -----------------------------------------------------------
    //  Escape CSV
    // -----------------------------------------------------------
    std::string Escape(const std::string &s) {
        std::string res;
        res.reserve(s.size());

        for (char ch : s) {
            if (ch == ';' || ch == ',' || ch == '\\') {
                res.push_back('\\');
            }
            res.push_back(ch);
        }
        return res;
    }

    // -----------------------------------------------------------
    //  Unescape CSV
    // -----------------------------------------------------------
    std::string Unescape(const std::string &s) {
        std::string res;
        res.reserve(s.size());

        bool esc = false;
        for (char ch : s) {
            if (esc) {
                res.push_back(ch);
                esc = false;
            } else if (ch == '\\') {
                esc = true;
            } else {
                res.push_back(ch);
            }
        }

        return res;
    }

}
