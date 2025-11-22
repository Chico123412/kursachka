#ifndef KURSOVA_USERMANAGER_H
#define KURSOVA_USERMANAGER_H

#include <vector>
#include <string>
#include "User.h"

/**
 * Менеджер користувачів.
 * Відповідає за файл users.txt, авторизацію та адміністрування акаунтів.
 */
class UserManager {
private:
    std::vector<User> users;
    std::string filePath;

public:
    explicit UserManager(const std::string &filePath);

    void Load();
    void Save() const;

    /// Перевірка наявності адміністратора, створення admin/admin за потреби
    void EnsureDefaultAdmin();

    bool Authenticate(const std::string &login,
                      const std::string &password,
                      User &outUser) const;

    bool AddUser(const std::string &username,
                 const std::string &password,
                 const std::string &role);

    bool RemoveUser(const std::string &username);

    const std::vector<User> &GetUsers() const;
};

#endif //KURSOVA_USERMANAGER_H
