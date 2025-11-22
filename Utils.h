#ifndef KURSOVA_UTILS_H
#define KURSOVA_UTILS_H

#include <string>
#include <vector>

namespace Utils {

    // ASCII-only lowercase (для старих викликів)
    std::string ToLower(const std::string &s);

    // Правильна версія для української/UTF-8
    std::string ToLowerUTF8(const std::string &s);

    // Обрізання пробілів
    std::string Trim(const std::string &s);

    // Розбиття рядка
    std::vector<std::string> Split(const std::string &s, char delim);

    // Об’єднання рядків
    std::string Join(const std::vector<std::string> &parts, char delim);

    // CSV escape
    std::string Escape(const std::string &s);

    // CSV unescape
    std::string Unescape(const std::string &s);

}

#endif //KURSOVA_UTILS_H
