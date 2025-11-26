/**
 * @file User.h
 * @brief Оголошення класу користувача.
 */

#ifndef KURSOVA_USER_H
#define KURSOVA_USER_H

#include <string>

/**
 * @class User
 * @brief Клас для представлення користувача системи.
 *
 * Зберігає облікові дані (логін, пароль) та роль користувача в системі.
 * Підтримуються дві ролі:
 * - "admin": має повний доступ (додавання, редагування, видалення термінів та користувачів).
 * - "user": має обмежений доступ (тільки перегляд та пошук).
 */
class User {
private:
    /**
     * @brief Логін користувача.
     * Унікальний ідентифікатор для входу в систему.
     */
    std::string username;

    /**
     * @brief Пароль користувача.
     * Зберігається у відкритому вигляді (для навчальних цілей).
     */
    std::string password;

    /**
     * @brief Роль користувача.
     * Визначає рівень доступу ("admin" або "user").
     */
    std::string role;

public:
    /**
     * @brief Конструктор за замовчуванням.
     */
    User();

    /**
     * @brief Параметризований конструктор.
     * @param username Логін.
     * @param password Пароль.
     * @param role Роль доступу.
     */
    User(const std::string &username,
         const std::string &password,
         const std::string &role);

    /**
     * @brief Конструктор копіювання.
     * @param other Об'єкт для копіювання.
     */
    User(const User &other);

    /**
     * @brief Конструктор переміщення.
     * @param other Об'єкт для переміщення (r-value).
     */
    User(User &&other) noexcept;

    /**
     * @brief Деструктор.
     */
    ~User();

    /**
     * @brief Оператор присвоєння (копіювання).
     * @param other Об'єкт-джерело.
     * @return Посилання на поточний об'єкт.
     */
    User &operator=(const User &other);

    /**
     * @brief Оператор присвоєння (переміщення).
     * @param other Об'єкт-джерело.
     * @return Посилання на поточний об'єкт.
     */
    User &operator=(User &&other) noexcept;

    /**
     * @brief Отримує логін користувача.
     * @return Константне посилання на рядок.
     */
    const std::string &GetUsername() const;

    /**
     * @brief Отримує пароль користувача.
     * @return Константне посилання на рядок.
     */
    const std::string &GetPassword() const;

    /**
     * @brief Отримує роль користувача.
     * @return Константне посилання на рядок.
     */
    const std::string &GetRole() const;

    /**
     * @brief Встановлює новий логін.
     * @param value Нове значення логіна.
     */
    void SetUsername(const std::string &value);

    /**
     * @brief Встановлює новий пароль.
     * @param value Нове значення пароля.
     */
    void SetPassword(const std::string &value);

    /**
     * @brief Встановлює нову роль.
     * @param value Нова роль ("admin" або "user").
     */
    void SetRole(const std::string &value);
};

#endif //KURSOVA_USER_H