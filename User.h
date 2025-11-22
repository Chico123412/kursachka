#ifndef KURSOVA_USER_H
#define KURSOVA_USER_H

#include <string>

/**
 * Клас користувача системи.
 * Підтримуються ролі: "admin" та "user".
 */
class User {
private:
    std::string username;
    std::string password;
    std::string role;

public:
    User();
    User(const std::string &username,
         const std::string &password,
         const std::string &role);

    User(const User &other);
    User(User &&other) noexcept;
    ~User();

    User &operator=(const User &other);
    User &operator=(User &&other) noexcept;

    const std::string &GetUsername() const;
    const std::string &GetPassword() const;
    const std::string &GetRole() const;

    void SetUsername(const std::string &value);
    void SetPassword(const std::string &value);
    void SetRole(const std::string &value);
};

#endif //KURSOVA_USER_H
