#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "UserManager.h"
#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"

// Пауза після операції
void Pause() {
    std::cout << "\n(Натисніть Enter, щоб продовжити...)";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Очищення вводу після помилки
void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Красива шапка розділу
void Banner(const std::string &title) {
    std::cout << "\n===============================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "===============================================" << std::endl;
}

// Допомога
void ShowHelp() {
    Banner("ДОПОМОГА");
    std::cout
        << "Це консольна система для керування базою термінів.\n"
        << "Підтримуються первинні та складні терміни, пошук,\n"
        << "сортування, редагування, побудова ланцюжків.\n\n"
        << "Файли:\n"
        << " • terms.csv — база термінів (PRIM/TERM)\n"
        << " • users.txt — список користувачів\n\n"
        << "Стартове меню:\n"
        << "  1. Увійти\n"
        << "  2. Створити нового користувача\n"
        << "  3. Допомога\n"
        << "  0. Вихід\n\n"
        << "Головне меню після входу:\n"
        << "  1. Список термінів (коротко)\n"
        << "  2. Список термінів (повний)\n"
        << "  3. Пошук за назвою\n"
        << "  4. Пошук у визначеннях\n"
        << "  5. Додати термін\n"
        << "  6. Редагувати визначення\n"
        << "  7. Видалити термін\n"
        << "  8. Сортування за назвою\n"
        << "  9. Сортування за визначенням\n"
        << " 10. Первинні терміни\n"
        << " 11. Складні терміни\n"
        << " 12. Ланцюжок терміна\n"
        << " 13. Допомога\n"
        << " 14. Статистика\n"
        << " 20. Користувачі (для admin)\n";
    Pause();
}

// Меню адміністрування користувачів
void HandleAdminUserMenu(UserManager &userManager) {
    while (true) {
        Banner("МЕНЮ АДМІНІСТРАТОРА — КОРИСТУВАЧІВ");
        std::cout
            << "1. Переглянути користувачів\n"
            << "2. Додати користувача\n"
            << "3. Видалити користувача\n"
            << "0. Назад\n"
            << "-----------------------------------------------\n"
            << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення.\n";
            Pause();
            continue;
        }
        ClearCin();

        if (choice == 0) return;

        if (choice == 1) {
            Banner("СПИСОК КОРИСТУВАЧІВ");
            const auto &users = userManager.GetUsers();
            if (users.empty()) {
                std::cout << "Користувачів поки немає.\n";
            } else {
                for (const auto &u : users) {
                    std::cout << " • " << u.GetUsername()
                              << " (роль: " << u.GetRole() << ")\n";
                }
            }
            Pause();
        }
        else if (choice == 2) {
            Banner("ДОДАВАННЯ КОРИСТУВАЧА");

            std::string username, password, role;
            std::cout << "Логін: ";
            std::getline(std::cin, username);
            username = Utils::Trim(username);

            std::cout << "Пароль: ";
            std::getline(std::cin, password);
            password = Utils::Trim(password);

            std::string password2;
            std::cout << "Повторіть пароль: ";
            std::getline(std::cin, password2);
            password2 = Utils::Trim(password2);

            if (username.empty() || password.empty() || password2.empty()) {
                std::cout << "Помилка: логін і паролі не можуть бути порожніми.\n";
                Pause();
                continue;
            }

            if (password != password2) {
                std::cout << "Паролі не збігаються.\n";
                Pause();
                continue;
            }

            std::cout << "Роль (admin/user): ";
            std::getline(std::cin, role);
            role = Utils::Trim(role);

            if (role != "admin" && role != "user") {
                std::cout << "Некоректна роль. Дозволено: admin або user.\n";
                Pause();
                continue;
            }

            if (userManager.AddUser(username, password, role)) {
                std::cout << "Користувача додано.\n";
                userManager.Save();
            } else {
                std::cout << "Користувач із таким логіном уже існує.\n";
            }
            Pause();
        }
        else if (choice == 3) {
            Banner("ВИДАЛЕННЯ КОРИСТУВАЧА");

            std::string username;
            std::cout << "Введіть логін користувача: ";
            std::getline(std::cin, username);
            username = Utils::Trim(username);

            std::string confirm;
            std::cout << "Видалити '" << username << "'? (y/n): ";
            std::getline(std::cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                if (userManager.RemoveUser(username)) {
                    std::cout << "Користувача видалено.\n";
                    userManager.Save();
                } else {
                    std::cout << "Користувача не знайдено або видалення заборонено.\n";
                }
            } else {
                std::cout << "Операцію скасовано.\n";
            }
            Pause();
        }
        else {
            std::cout << "Невідомий пункт меню.\n";
            Pause();
        }
    }
}

// Додавання терміна
void HandleAddTerm(TermManager &termManager) {
    Banner("ДОДАВАННЯ ТЕРМІНА");

    std::cout << "Оберіть тип:\n"
              << "  1. Первинне поняття\n"
              << "  2. Складний термін (з посиланнями)\n"
              << "Ваш вибір: ";

    int type;
    if (!(std::cin >> type)) {
        ClearCin();
        std::cout << "Некоректне введення.\n";
        Pause();
        return;
    }
    ClearCin();

    std::string name, definition;
    std::cout << "Назва терміна: ";
    std::getline(std::cin, name);
    name = Utils::Trim(name);

    std::cout << "Визначення:\n> ";
    std::getline(std::cin, definition);
    definition = Utils::Trim(definition);

    if (name.empty() || definition.empty()) {
        std::cout << "Назва та визначення не можуть бути порожніми.\n";
        Pause();
        return;
    }

    // Заборона термін-дублікатів (із урахуванням регістру/мови)
    if (termManager.FindByName(name) != nullptr) {
        std::cout << "Термін \"" << name << "\" уже існує у базі.\n";
        Pause();
        return;
    }

    // Визначення не повинно повністю збігатися з назвою
    if (Utils::ToLowerUTF8(name) == Utils::ToLowerUTF8(definition)) {
        std::cout << "Визначення не може повністю збігатися з назвою терміна.\n";
        Pause();
        return;
    }

    if (type == 1) {
        termManager.AddTerm(std::make_shared<PrimitiveTerm>(name, definition));
        termManager.Save();
        std::cout << "Первинний термін додано.\n";
        Pause();
    }
    else if (type == 2) {
        std::cout << "Введіть назви термінів, на які є посилання (через кому):\n> ";
        std::string line;
        std::getline(std::cin, line);

        std::vector<std::string> refs;
        if (!line.empty()) {
            auto raw = Utils::Split(line, ',');
            for (auto &r : raw) {
                r = Utils::Trim(r);
                if (!r.empty()) {
                    refs.push_back(r);
                }
            }
        }

        // Перевірка на самопосилання
        bool selfRef = false;
        for (const auto &r : refs) {
            if (Utils::ToLowerUTF8(r) == Utils::ToLowerUTF8(name)) {
                selfRef = true;
                break;
            }
        }

        if (selfRef) {
            std::cout << "Помилка: термін не може посилатися сам на себе.\n";
            Pause();
            return;
        }

        // Попередження про відсутні посилання
        bool anyMissing = false;
        for (const auto &r : refs) {
            if (!termManager.FindByName(r)) {
                std::cout << "[ПОПЕРЕДЖЕННЯ] Посилання \"" << r
                          << "\" відсутнє в базі термінів.\n";
                anyMissing = true;
            }
        }

        if (anyMissing) {
            std::cout << "Термін буде збережено, але частина посилань поки не існує.\n";
        }

        termManager.AddTerm(std::make_shared<Term>(name, definition, refs));
        termManager.Save();
        std::cout << "Складний термін додано.\n";
        Pause();
    }
    else {
        std::cout << "Некоректний тип терміна.\n";
        Pause();
    }
}

// Головне меню
void ShowMainMenu(const User &currentUser, TermManager &termManager, UserManager &userManager) {
    while (true) {
        Banner("ГОЛОВНЕ МЕНЮ");
        std::cout
            << "1. Список термінів (коротко)\n"
            << "2. Список термінів (повний)\n"
            << "3. Пошук за назвою\n"
            << "4. Пошук у визначеннях\n"
            << "5. Додати термін\n"
            << "6. Редагувати визначення\n"
            << "7. Видалити термін\n"
            << "8. Сортування за назвою\n"
            << "9. Сортування за визначенням\n"
            << "10. Первинні терміни\n"
            << "11. Складні терміни\n"
            << "12. Ланцюжок терміна\n"
            << "13. Допомога\n"
            << "14. Статистика\n";

        if (currentUser.GetRole() == "admin") {
            std::cout << "20. Користувачі (адмін)\n";
        }

        std::cout << "0. Вихід\n"
                  << "-----------------------------------------------\n"
                  << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення.\n";
            Pause();
            continue;
        }
        ClearCin();

        if (choice == 0) return;

        switch (choice) {
            case 1:
                Banner("КОРОТКИЙ СПИСОК ТЕРМІНІВ");
                termManager.PrintAllShort();
                Pause();
                break;
            case 2:
                Banner("ПОВНИЙ СПИСОК ТЕРМІНІВ");
                termManager.PrintAllFull();
                Pause();
                break;
            case 3: {
                Banner("ПОШУК ЗА НАЗВОЮ");
                std::string name;
                std::cout << "Назва терміна: ";
                std::getline(std::cin, name);
                name = Utils::Trim(name);

                auto t = termManager.FindByName(name);
                if (!t) {
                    std::cout << "Термін не знайдено.\n";
                } else {
                    std::cout << "\n=== ТЕРМІН: " << t->GetName() << " ===\n";
                    std::cout << "Визначення:\n    " << t->GetDefinition() << "\n";
                }
                Pause();
                break;
            }
            case 4: {
                Banner("ПОШУК У ВИЗНАЧЕННЯХ");
                std::string frag;
                std::cout << "Фрагмент: ";
                std::getline(std::cin, frag);
                termManager.SearchByDefinition(frag);
                Pause();
                break;
            }
            case 5:
                HandleAddTerm(termManager);
                break;
            case 6: {
                Banner("РЕДАГУВАННЯ ВИЗНАЧЕННЯ");
                std::string name, def;
                std::cout << "Назва терміна: ";
                std::getline(std::cin, name);
                name = Utils::Trim(name);

                std::cout << "Нове визначення:\n> ";
                std::getline(std::cin, def);
                def = Utils::Trim(def);

                if (def.empty()) {
                    std::cout << "Нове визначення не може бути порожнім.\n";
                    Pause();
                    break;
                }

                if (termManager.EditDefinition(name, def)) {
                    termManager.Save();
                    std::cout << "Визначення оновлено.\n";
                } else {
                    std::cout << "Термін не знайдено.\n";
                }
                Pause();
                break;
            }
            case 7: {
                Banner("ВИДАЛЕННЯ ТЕРМІНА");
                std::string name;
                std::cout << "Назва терміна: ";
                std::getline(std::cin, name);
                name = Utils::Trim(name);

                std::string confirm;
                std::cout << "Видалити \"" << name << "\"? (y/n): ";
                std::getline(std::cin, confirm);

                if (confirm == "y" || confirm == "Y") {
                    if (termManager.RemoveTerm(name)) {
                        termManager.Save();
                        std::cout << "Термін видалено.\n";
                    } else {
                        std::cout << "Термін не знайдено або видалення заборонено.\n";
                    }
                } else {
                    std::cout << "Операцію скасовано.\n";
                }
                Pause();
                break;
            }
            case 8:
                Banner("СОРТУВАННЯ ЗА НАЗВОЮ");
                termManager.SortByName();
                termManager.Save();
                std::cout << "Список відсортовано.\n";
                Pause();
                break;
            case 9:
                Banner("СОРТУВАННЯ ЗА ВИЗНАЧЕННЯМ");
                termManager.SortByDefinition();
                termManager.Save();
                std::cout << "Список відсортовано.\n";
                Pause();
                break;
            case 10:
                Banner("ПЕРВИННІ ТЕРМІНИ");
                termManager.PrintFilteredByPrimitive(true);
                Pause();
                break;
            case 11:
                Banner("СКЛАДНІ ТЕРМІНИ");
                termManager.PrintFilteredByPrimitive(false);
                Pause();
                break;
            case 12: {
                Banner("ЛАНЦЮЖОК ТЕРМІНА");
                std::string name;
                std::cout << "Назва терміна: ";
                std::getline(std::cin, name);
                name = Utils::Trim(name);
                termManager.PrintChainFrom(name);
                Pause();
                break;
            }
            case 13:
                ShowHelp();
                break;
            case 14:
                Banner("СТАТИСТИКА БАЗИ ТЕРМІНІВ");
                termManager.PrintStats();
                Pause();
                break;
            case 20:
                if (currentUser.GetRole() == "admin") {
                    HandleAdminUserMenu(userManager);
                } else {
                    std::cout << "Недостатньо прав для доступу до цього розділу.\n";
                    Pause();
                }
                break;
            default:
                std::cout << "Невідомий пункт меню.\n";
                Pause();
                break;
        }
    }
}

// -------------------------------------------------------------
//                           MAIN
// -------------------------------------------------------------
int main() {
    UserManager userManager("users.txt");
    TermManager termManager("terms.csv");

    userManager.Load();
    userManager.EnsureDefaultAdmin();
    termManager.Load();

    while (true) {
        Banner("СИСТЕМА КЕРУВАННЯ БАЗОЮ ТЕРМІНІВ");
        std::cout
            << "1. Увійти\n"
            << "2. Створити нового користувача\n"
            << "3. Допомога\n"
            << "0. Вихід\n"
            << "-----------------------------------------------\n"
            << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення.\n";
            Pause();
            continue;
        }
        ClearCin();

        if (choice == 0) {
            std::cout << "Завершення роботи.\n";
            return 0;
        }

        if (choice == 3) {
            ShowHelp();
            continue;
        }

        if (choice == 2) {
            Banner("РЕЄСТРАЦІЯ НОВОГО КОРИСТУВАЧА");
            std::string login, pass, pass2;

            std::cout << "Новий логін: ";
            std::getline(std::cin, login);
            login = Utils::Trim(login);

            std::cout << "Новий пароль: ";
            std::getline(std::cin, pass);
            pass = Utils::Trim(pass);

            std::cout << "Повторіть пароль: ";
            std::getline(std::cin, pass2);
            pass2 = Utils::Trim(pass2);

            if (login.empty() || pass.empty() || pass2.empty()) {
                std::cout << "Помилка: логін і паролі не можуть бути порожніми.\n";
                Pause();
                continue;
            }

            if (pass != pass2) {
                std::cout << "Паролі не збігаються.\n";
                Pause();
                continue;
            }

            if (userManager.AddUser(login, pass, "user")) {
                userManager.Save();
                std::cout << "Користувача успішно зареєстровано.\n";
            } else {
                std::cout << "Користувач із таким логіном уже існує.\n";
            }
            Pause();
            continue;
        }

        if (choice == 1) {
            Banner("АВТОРИЗАЦІЯ");
            std::string login, pass;

            std::cout << "Логін: ";
            std::getline(std::cin, login);
            login = Utils::Trim(login);

            std::cout << "Пароль: ";
            std::getline(std::cin, pass);
            pass = Utils::Trim(pass);

            User currentUser;
            if (userManager.Authenticate(login, pass, currentUser)) {
                std::cout << "Авторизація успішна.\n";
                Pause();

                ShowMainMenu(currentUser, termManager, userManager);

                std::cout << "Збереження даних...\n";
                userManager.Save();
                termManager.Save();
                std::cout << "Роботу завершено.\n";
                return 0;
            } else {
                std::cout << "Невірний логін або пароль.\n";
                Pause();
                continue;
            }
        }

        std::cout << "Невідомий пункт меню.\n";
        Pause();
    }
}
