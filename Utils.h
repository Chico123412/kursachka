#ifndef KURSOVA_UTILS_H
#define KURSOVA_UTILS_H

#include <string>
#include <vector>

namespace Utils {
    std::string ToLower(const std::string &s);

    /// Обрізання пробілів з початку і кінця рядка
    std::string Trim(const std::string &s);

    /// Розбиття рядка за заданим символом-роздільником
    std::vector<std::string> Split(const std::string &s, char delim);

    /// Об’єднання рядків у один з роздільником
    std::string Join(const std::vector<std::string> &parts, char delim);

    /// Проста екранування символів ';' і ',' для CSV
    std::string Escape(const std::string &s);

    /// Зворотне екранування
    std::string Unescape(const std::string &s);

}

#endif //KURSOVA_UTILS_H
