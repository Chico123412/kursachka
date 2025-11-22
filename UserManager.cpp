#include "UserManager.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

UserManager::UserManager(const std::string &filePath)
        : filePath(filePath) {}

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

bool UserManager::AddUser(const std::string &username,
                          const std::string &password,
                          const std::string &role) {
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

const std::vector<User> &UserManager::GetUsers() const {
    return users;
}
