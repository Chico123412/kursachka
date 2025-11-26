/**
 * @file UserManager.h
 * @brief Оголошення класу менеджера користувачів.
 */

#ifndef KURSOVA_USERMANAGER_H
#define KURSOVA_USERMANAGER_H

#include <vector>
#include <string>
#include "User.h"

/**
 * @class UserManager
 * @brief Клас для керування обліковими записами користувачів.
 *
 * Цей клас відповідає за:
 * - Завантаження та збереження списку користувачів у текстовий файл (users.txt).
 * - Автентифікацію (перевірку логіна та пароля).
 * - Адміністрування (додавання нових користувачів, видалення існуючих).
 * - Забезпечення наявності хоча б одного адміністратора в системі.
 */
class UserManager {
private:
    /**
     * @brief Список усіх зареєстрованих користувачів.
     */
    std::vector<User> users;

    /**
     * @brief Шлях до файлу з обліковими даними.
     */
    std::string filePath;

public:
    /**
     * @brief Конструктор.
     * @param filePath Шлях до файлу користувачів.
     */
    explicit UserManager(const std::string &filePath);

    /**
     * @brief Завантажує дані користувачів із файлу.
     * Парсить формат "логін:пароль:роль".
     */
    void Load();

    /**
     * @brief Зберігає поточний список користувачів у файл.
     */
    void Save() const;

    /**
     * @brief Перевіряє наявність адміністратора.
     * Якщо база порожня або адміністратора видалено, створює дефолтного
     * користувача "admin" з паролем "admin".
     */
    void EnsureDefaultAdmin();

    /**
     * @brief Виконує вхід у систему.
     * @param login Введений логін.
     * @param password Введений пароль.
     * @param outUser Посилання, куди буде записано дані користувача у разі успіху.
     * @return true, якщо логін та пароль вірні.
     */
    bool Authenticate(const std::string &login,
                      const std::string &password,
                      User &outUser) const;

    /**
     * @brief Реєструє нового користувача.
     * @param username Бажаний логін.
     * @param password Бажаний пароль.
     * @param role Роль ("user" або "admin").
     * @return true, якщо користувача успішно додано (логін унікальний).
     */
    bool AddUser(const std::string &username,
                 const std::string &password,
                 const std::string &role);

    /**
     * @brief Видаляє користувача із системи.
     * @note Не дозволяє видалити користувача з логіном "admin".
     * @param username Логін користувача, якого треба видалити.
     * @return true, якщо видалення успішне.
     */
    bool RemoveUser(const std::string &username);

    /**
     * @brief Отримує список усіх користувачів.
     * Використовується для виводу таблиці користувачів в меню адміністратора.
     * @return Константне посилання на вектор користувачів.
     */
    const std::vector<User> &GetUsers() const;
};

#endif //KURSOVA_USERMANAGER_H