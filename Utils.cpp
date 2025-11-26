/**
 * @file Utils.cpp
 * @brief Реалізація допоміжних утиліт для обробки рядків.
 *
 * Містить функції для конвертації регістру (з підтримкою UTF-8),
 * обрізання пробілів, розбиття та об'єднання рядків,
 * а також екранування спецсимволів для формату CSV.
 */

#include "Utils.h"
#include <algorithm>
#include <locale>
#include <codecvt>

namespace Utils {

    // -----------------------------------------------------------
    //  ASCII ToLower
    // -----------------------------------------------------------

    /**
     * @brief Перетворює рядок у нижній регістр (тільки ASCII символи).
     * @note Використовується для сумісності або роботи з англійським текстом.
     * @param s Вхідний рядок.
     * @return Рядок у нижньому регістрі.
     */
    std::string ToLower(const std::string &s) {
        std::string res = s;
        std::transform(res.begin(), res.end(), res.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return res;
    }

    // -----------------------------------------------------------
    //  UTF-8 ToLower
    // -----------------------------------------------------------

    /**
     * @brief Перетворює рядок у нижній регістр з підтримкою UTF-8.
     *
     * Використовує std::codecvt_utf8 та системну локаль для коректної
     * обробки українських літер (кирилиці).
     *
     * @param s Вхідний рядок у кодуванні UTF-8.
     * @return Перетворений рядок.
     */
    std::string ToLowerUTF8(const std::string &s) {
        // УВАГА: std::codecvt_utf8 є deprecated у C++17, але це стандартний
        // спосіб для навчальних проєктів без підключення важких бібліотек типу ICU.
        // Для приглушення warning-ів можна використовувати компіляцію з прапором -Wno-deprecated.
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring ws = conv.from_bytes(s);

        // Системна локаль (автоматично підтягує налаштування ОС, наприклад uk_UA)
        std::locale loc("");

        for (auto &ch : ws) {
            ch = std::tolower(ch, loc);
        }

        return conv.to_bytes(ws);
    }

    // -----------------------------------------------------------
    //  Trim
    // -----------------------------------------------------------

    /**
     * @brief Видаляє пробільні символи з початку та кінця рядка.
     * @param s Вхідний рядок.
     * @return Очищений рядок.
     */
    std::string Trim(const std::string &s) {
        size_t start = 0;
        // Шукаємо перший непробільний символ
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
            start++;

        // Якщо весь рядок складається з пробілів
        if (start == s.size()) return "";

        size_t end = s.size() - 1;
        // Шукаємо останній непробільний символ
        while (end > start && std::isspace(static_cast<unsigned char>(s[end])))
            end--;

        return s.substr(start, end - start + 1);
    }

    // -----------------------------------------------------------
    //  Split
    // -----------------------------------------------------------

    /**
     * @brief Розбиває рядок на частини за роздільником.
     *
     * Враховує механізм екранування: якщо перед роздільником стоїть '\',
     * він вважається частиною тексту, а не роздільником.
     *
     * @param s Вхідний рядок.
     * @param delim Символ-роздільник.
     * @return Вектор рядків.
     */
    std::vector<std::string> Split(const std::string &s, char delim) {
        std::vector<std::string> result;
        std::string current;
        bool escaped = false; // Прапорець стану екранування

        for (char ch : s) {
            if (escaped) {
                // Якщо попередній символ був '\', то поточний додаємо як звичайний символ
                current.push_back(ch);
                escaped = false;
            } else if (ch == '\\') {
                // Знайшли екрануючий символ, встановлюємо прапорець
                escaped = true;
                // Додаємо сам слеш, щоб Unescape міг його потім обробити
                current.push_back(ch);
            } else if (ch == delim) {
                // Знайшли роздільник (не екранований) -> завершуємо поточну частину
                result.push_back(current);
                current.clear();
            } else {
                // Звичайний символ
                current.push_back(ch);
            }
        }

        // Додаємо останню частину (навіть якщо вона порожня)
        result.push_back(current);
        return result;
    }

    // -----------------------------------------------------------
    //  Join
    // -----------------------------------------------------------

    /**
     * @brief Об'єднує вектор рядків в один рядок.
     * @param parts Частини рядка.
     * @param delim Роздільник.
     * @return З'єднаний рядок.
     */
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

    /**
     * @brief Екранує спецсимволи для CSV.
     *
     * Додає символ '\' перед ';', ',' та самим '\', щоб зберегти структуру даних.
     * @param s Вхідний рядок.
     * @return Безпечний для CSV рядок.
     */
    std::string Escape(const std::string &s) {
        std::string res;
        res.reserve(s.size() + 5); // Резервуємо трохи більше пам'яті для оптимізації

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

    /**
     * @brief Відновлює початковий вигляд екранованого рядка.
     *
     * Замінює "\;" на ";", "\," на "," тощо.
     * @param s Екранований рядок з файлу.
     * @return Оригінальний текст.
     */
    std::string Unescape(const std::string &s) {
        std::string res;
        res.reserve(s.size());

        bool esc = false;
        for (char ch : s) {
            if (esc) {
                // Якщо був прапорець escape, просто додаємо символ (пропускаючи слеш)
                res.push_back(ch);
                esc = false;
            } else if (ch == '\\') {
                // Знайшли слеш, вмикаємо режим escape, але сам слеш не пишемо
                esc = true;
            } else {
                // Звичайний символ
                res.push_back(ch);
            }
        }
        return res;
    }

}