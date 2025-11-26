/**
 * @file Utils.h
 * @brief Оголошення допоміжних функцій.
 */

#ifndef KURSOVA_UTILS_H
#define KURSOVA_UTILS_H

#include <string>
#include <vector>

/**
 * @namespace Utils
 * @brief Простір імен для утиліт обробки рядків та CSV.
 *
 * Містить функції для:
 * - Зміни регістру (з підтримкою української мови).
 * - Очищення рядків від пробілів.
 * - Розбиття та об'єднання рядків.
 * - Екранування спецсимволів для коректного запису у файл.
 */
namespace Utils {

    /**
     * @brief Перетворює рядок у нижній регістр (тільки ASCII).
     * @note Використовується для службових цілей або сумісності.
     * @param s Вхідний рядок.
     * @return Рядок у нижньому регістрі.
     */
    std::string ToLower(const std::string &s);

    /**
     * @brief Перетворює рядок у нижній регістр з повною підтримкою UTF-8.
     * @details Коректно обробляє українські літери (наприклад, "І" -> "і", "Є" -> "є").
     * @param s Вхідний рядок (UTF-8).
     * @return Рядок у нижньому регістрі.
     */
    std::string ToLowerUTF8(const std::string &s);

    /**
     * @brief Видаляє пробіли з початку та кінця рядка.
     * @param s Вхідний рядок.
     * @return Очищений рядок.
     */
    std::string Trim(const std::string &s);

    /**
     * @brief Розбиває рядок на частини за вказаним роздільником.
     * @details Функція враховує екранування: якщо перед роздільником стоїть '\',
     * він ігнорується і вважається частиною тексту.
     * @param s Вхідний рядок.
     * @param delim Символ-роздільник.
     * @return Вектор рядків.
     */
    std::vector<std::string> Split(const std::string &s, char delim);

    /**
     * @brief Об'єднує вектор рядків в один рядок.
     * @param parts Вектор частин.
     * @param delim Символ-роздільник.
     * @return З'єднаний рядок.
     */
    std::string Join(const std::vector<std::string> &parts, char delim);

    /**
     * @brief Екранує спецсимволи для формату CSV.
     * @details Додає символ '\' перед ';', ',' та '\'.
     * @param s Вхідний рядок.
     * @return Безпечний рядок для запису у файл.
     */
    std::string Escape(const std::string &s);

    /**
     * @brief Прибирає екранування (відновлює початковий рядок).
     * @param s Екранований рядок з файлу.
     * @return Оригінальний текст.
     */
    std::string Unescape(const std::string &s);

}

#endif //KURSOVA_UTILS_H