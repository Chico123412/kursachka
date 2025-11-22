#include "User.h"
#include <iostream>

User::User() = default;

User::User(const std::string &username,
           const std::string &password,
           const std::string &role)
        : username(username), password(password), role(role) {}

User::User(const User &other)
        : username(other.username), password(other.password), role(other.role) {}

User::User(User &&other) noexcept
        : username(std::move(other.username)),
          password(std::move(other.password)),
          role(std::move(other.role)) {}

User::~User() {
    std::cout << "[DEBUG] Знищення користувача: " << username << std::endl;
}

User &User::operator=(const User &other) {
    if (this != &other) {
        username = other.username;
        password = other.password;
        role = other.role;
    }
    return *this;
}

User &User::operator=(User &&other) noexcept {
    if (this != &other) {
        username = std::move(other.username);
        password = std::move(other.password);
        role = std::move(other.role);
    }
    return *this;
}

const std::string &User::GetUsername() const {
    return username;
}

const std::string &User::GetPassword() const {
    return password;
}

const std::string &User::GetRole() const {
    return role;
}

void User::SetUsername(const std::string &value) {
    username = value;
}

void User::SetPassword(const std::string &value) {
    password = value;
}

void User::SetRole(const std::string &value) {
    role = value;
}
