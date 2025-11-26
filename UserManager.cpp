/**
 * @file UserManager.cpp
 * @brief Реалізація методів менеджера користувачів.
 *
 * Містить логіку для читання/запису бази користувачів, автентифікації
 * та керування обліковими записами (додавання, видалення).
 */

#include "UserManager.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// -------------------------------------------------------------
//                     CONSTRUCTOR
// -------------------------------------------------------------

/**
 * @brief Конструктор.
 * @param filePath Шлях до файлу з даними користувачів (наприклад, "users.txt").
 */
UserManager::UserManager(const std::string &filePath)
        : filePath(filePath) {}

// -------------------------------------------------------------
//                     LOAD
// -------------------------------------------------------------

/**
 * @brief Завантажує список користувачів з файлу.
 *
 * Формат файлу: логін:пароль:роль
 * Якщо файл не знайдено, виводиться повідомлення, але помилка не кидається
 * (файл буде створено пізніше при збереженні).
 */
void UserManager::Load() {
    users.clear();
    std::ifstream in(filePath);
    if (!in.is_open()) {
        std::cout << "[INFO] Файл користувачів не знайдено, буде створено новий." << std::endl;
        return;
    }

    try {
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) {
                continue;
            }
            // Використовуємо роздільник ':' для формату users.txt
            auto parts = Utils::Split(line, ':');
            if (parts.size() < 3) {
                continue;
            }
            std::string username = Utils::Trim(parts[0]);
            std::string password = Utils::Trim(parts[1]);
            std::string role = Utils::Trim(parts[2]);
            users.emplace_back(username, password, role);
        }
    } catch (const std::exception &ex) {
        std::cerr << "[ERROR] Помилка читання користувачів: " << ex.what() << std::endl;
    }
}

// -------------------------------------------------------------
//                     SAVE
// -------------------------------------------------------------

/**
 * @brief Зберігає поточний список користувачів у файл.
 *
 * Перезаписує файл повністю. Дані зберігаються у форматі:
 * username:password:role
 */
void UserManager::Save() const {
    std::ofstream out(filePath);
    if (!out.is_open()) {
        std::cerr << "[ERROR] Не вдалося відкрити users.txt для запису." << std::endl;
        return;
    }

    try {
        for (const auto &u : users) {
            out << u.GetUsername() << ":" << u.GetPassword() << ":" << u.GetRole() << "\n";
        }
    } catch (const std::exception &ex) {
        std::cerr << "[ERROR] Помилка запису users.txt: " << ex.what() << std::endl;
    }
}

// -------------------------------------------------------------
//                 ENSURE DEFAULT ADMIN
// -------------------------------------------------------------

/**
 * @brief Перевіряє наявність адміністратора в базі.
 *
 * Якщо користувача з логіном "admin" не існує, створює його
 * зі стандартним паролем "admin" і зберігає базу.
 * Це гарантує, що в систему завжди можна увійти.
 */
void UserManager::EnsureDefaultAdmin() {
    bool hasAdmin = false;
    for (const auto &u : users) {
        if (u.GetUsername() == "admin") {
            hasAdmin = true;
            break;
        }
    }
    if (!hasAdmin) {
        std::cout << "[INFO] Створено адміністратора за замовчуванням: admin/admin" << std::endl;
        users.emplace_back("admin", "admin", "admin");
        Save();
    }
}

// -------------------------------------------------------------
//                     AUTHENTICATE
// -------------------------------------------------------------

/**
 * @brief Перевіряє правильність логіна та пароля.
 * @param login Введений логін.
 * @param password Введений пароль.
 * @param outUser Посилання на об'єкт User, куди будуть записані дані знайденого користувача.
 * @return true, якщо аутентифікація успішна (дані збігаються).
 */
bool UserManager::Authenticate(const std::string &login,
                               const std::string &password,
                               User &outUser) const {
    for (const auto &u : users) {
        if (u.GetUsername() == login && u.GetPassword() == password) {
            outUser = u;
            return true;
        }
    }
    return false;
}

// -------------------------------------------------------------
//                     ADD USER
// -------------------------------------------------------------

/**
 * @brief Додає нового користувача.
 * @param username Логін.
 * @param password Пароль.
 * @param role Роль ("user" або "admin").
 * @return true, якщо користувача успішно додано; false, якщо такий логін вже існує.
 */
bool UserManager::AddUser(const std::string &username,
                          const std::string &password,
                          const std::string &role) {
    // Перевірка на унікальність логіна
    auto it = std::find_if(users.begin(), users.end(),
                           [&username](const User &u) {
                               return u.GetUsername() == username;
                           });
    if (it != users.end()) {
        return false;
    }
    users.emplace_back(username, password, role);
    return true;
}

// -------------------------------------------------------------
//                     REMOVE USER
// -------------------------------------------------------------

/**
 * @brief Видаляє користувача за логіном.
 *
 * @note Заборонено видаляти користувача з логіном "admin" (super-admin).
 * @param username Логін користувача.
 * @return true, якщо користувача знайдено і видалено.
 */
bool UserManager::RemoveUser(const std::string &username) {
    if (username == "admin") {
        std::cout << "Видалення адміністратора заборонено." << std::endl;
        return false;
    }
    auto it = std::remove_if(users.begin(), users.end(),
                             [&username](const User &u) {
                                 return u.GetUsername() == username;
                             });
    bool removed = it != users.end();
    users.erase(it, users.end());
    return removed;
}

// -------------------------------------------------------------
//                     GETTERS
// -------------------------------------------------------------

/**
 * @brief Отримує список усіх користувачів.
 * @return Константне посилання на вектор користувачів.
 */
const std::vector<User> &UserManager::GetUsers() const {
    return users;
}