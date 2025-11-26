/**
 * @file User.cpp
 * @brief Реалізація методів класу користувача.
 *
 * Містить реалізацію конструкторів, деструктора, операторів присвоєння
 * та методів доступу (геттерів/сеттерів) для полів користувача.
 */

#include "User.h"
#include <iostream>

/**
 * @brief Конструктор за замовчуванням.
 * Створює порожній об'єкт користувача.
 */
User::User() = default;

/**
 * @brief Параметризований конструктор.
 * @param username Логін користувача.
 * @param password Пароль користувача.
 * @param role Роль у системі ("admin" або "user").
 */
User::User(const std::string &username,
           const std::string &password,
           const std::string &role)
        : username(username), password(password), role(role) {}

/**
 * @brief Конструктор копіювання.
 * @param other Об'єкт, з якого копіюються дані.
 */
User::User(const User &other)
        : username(other.username), password(other.password), role(other.role) {}

/**
 * @brief Конструктор переміщення.
 * @param other Об'єкт, дані якого переміщуються (r-value).
 */
User::User(User &&other) noexcept
        : username(std::move(other.username)),
          password(std::move(other.password)),
          role(std::move(other.role)) {}

/**
 * @brief Деструктор.
 */
User::~User() {
}

/**
 * @brief Оператор присвоєння (копіювання).
 * @param other Об'єкт-джерело.
 * @return Посилання на поточний об'єкт.
 */
User &User::operator=(const User &other) {
    if (this != &other) {
        username = other.username;
        password = other.password;
        role = other.role;
    }
    return *this;
}

/**
 * @brief Оператор присвоєння (переміщення).
 * @param other Об'єкт-джерело.
 * @return Посилання на поточний об'єкт.
 */
User &User::operator=(User &&other) noexcept {
    if (this != &other) {
        username = std::move(other.username);
        password = std::move(other.password);
        role = std::move(other.role);
    }
    return *this;
}

/**
 * @brief Отримує логін користувача.
 * @return Константне посилання на рядок.
 */
const std::string &User::GetUsername() const {
    return username;
}

/**
 * @brief Отримує пароль користувача.
 * @return Константне посилання на рядок.
 */
const std::string &User::GetPassword() const {
    return password;
}

/**
 * @brief Отримує роль користувача.
 * @return Константне посилання на рядок ("admin"/"user").
 */
const std::string &User::GetRole() const {
    return role;
}

/**
 * @brief Встановлює новий логін.
 * @param value Новий логін.
 */
void User::SetUsername(const std::string &value) {
    username = value;
}

/**
 * @brief Встановлює новий пароль.
 * @param value Новий пароль.
 */
void User::SetPassword(const std::string &value) {
    password = value;
}

/**
 * @brief Встановлює нову роль.
 * @param value Нова роль.
 */
void User::SetRole(const std::string &value) {
    role = value;
}