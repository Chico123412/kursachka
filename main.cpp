#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "UserManager.h"
#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"

// Пауза після кожної операції
void Pause() {
    std::cout << "\n(Натисніть Enter, щоб продовжити...)";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Очищення вводу
void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Красива шапка
void Banner(const std::string &title) {
    std::cout << "\n===============================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "===============================================" << std::endl;
}

// Допомога
void ShowHelp() {
    Banner("ДОПОМОГА");
    std::cout << "Це консольна система для керування базою термінів.\n"
              << "Підтримуються первинні та складні терміни, пошук,\n"
              << "сортування, редагування, побудова ланцюжків.\n\n"
              << "Файли:\n"
              << " • terms.csv — база термінів (PRIM/TERM)\n"
              << " • users.txt — список користувачів\n\n"
              << "Основні можливості:\n"
              << " • Додавання термінів\n"
              << " • Редагування та видалення\n"
              << " • Сортування\n"
              << " • Ланцюжок термінів до первинних понять\n"
              << " • Пошук у назвах і визначеннях\n"
              << " • Адмін-меню керування користувачами\n";

    Pause();
}

// Меню адміністрування користувачів
void HandleAdminUserMenu(UserManager &userManager) {
    while (true) {
        Banner("МЕНЮ АДМІНІСТРАТОРА — КОРИСТУВАЧІВ");
        std::cout << "1. Переглянути користувачів\n"
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
            for (auto &u : userManager.GetUsers()) {
                std::cout << " • " << u.GetUsername() << " (роль: " << u.GetRole() << ")\n";
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

            std::cout << "Роль (admin/user): ";
            std::getline(std::cin, role);
            role = Utils::Trim(role);

            if (userManager.AddUser(username, password, role)) {
                std::cout << "Користувача додано.\n";
                userManager.Save();
            } else {
                std::cout << "Користувач вже існує.\n";
            }
            Pause();
        }
        else if (choice == 3) {
            Banner("ВИДАЛЕННЯ КОРИСТУВАЧА");

            std::string username;
            std::cout << "Введіть логін: ";
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
                    std::cout << "Не знайдено.\n";
                }
            } else {
                std::cout << "Скасовано.\n";
            }
            Pause();
        }
        else {
            std::cout << "Невідомий пункт.\n";
            Pause();
        }
    }
}

// Додавання терміна
void HandleAddTerm(TermManager &termManager) {
    Banner("ДОДАВАННЯ ТЕРМІНА");

    std::cout << "1. Первинне поняття\n"
              << "2. Складний термін\n"
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
    std::cout << "Назва: ";
    std::getline(std::cin, name);
    name = Utils::Trim(name);

    std::cout << "Визначення:\n> ";
    std::getline(std::cin, definition);
    definition = Utils::Trim(definition);

    if (name.empty() || definition.empty()) {
        std::cout << "Поля не можуть бути порожніми.\n";
        Pause();
        return;
    }

    if (type == 1) {
        termManager.AddTerm(std::make_shared<PrimitiveTerm>(name, definition));
        termManager.Save();
        std::cout << "Первинний термін додано.\n";
    }
    else if (type == 2) {
        std::cout << "Посилання через кому: ";
        std::string line;
        std::getline(std::cin, line);

        std::vector<std::string> refs;
        if (!line.empty()) {
            auto raw = Utils::Split(line, ',');
            for (auto &r : raw) {
                r = Utils::Trim(r);
                if (!r.empty()) refs.push_back(r);
            }
        }

        termManager.AddTerm(std::make_shared<Term>(name, definition, refs));
        termManager.Save();
        std::cout << "Складний термін додано.\n";
    }

    Pause();
}

// Головне меню
void ShowMainMenu(const User &currentUser, TermManager &termManager, UserManager &userManager) {
    while (true) {
        Banner("ГОЛОВНЕ МЕНЮ");
        std::cout << "1. Список термінів (коротко)\n"
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

        if (currentUser.GetRole() == "admin")
            std::cout << "20. Користувачі (адмін)\n";

        std::cout << "0. Вихід\n-----------------------------------------------\nВаш вибір: ";

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
                Banner("КОРОТКИЙ СПИСОК");
                termManager.PrintAllShort();
                Pause();
                break;
            case 2:
                Banner("ПОВНИЙ СПИСОК");
                termManager.PrintAllFull();
                Pause();
                break;

            case 3: {
                Banner("ПОШУК ЗА НАЗВОЮ");
                std::string name;
                std::cout << "Назва: ";
                std::getline(std::cin, name);
                auto t = termManager.FindByName(Utils::Trim(name));

                if (!t)
                    std::cout << "Термін не знайдено.\n";
                else {
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
                Banner("РЕДАГУВАННЯ ТЕРМІНА");
                std::string name, def;
                std::cout << "Назва: ";
                std::getline(std::cin, name);

                std::cout << "Нове визначення:\n> ";
                std::getline(std::cin, def);

                if (termManager.EditDefinition(name, def)) {
                    termManager.Save();
                    std::cout << "Визначення оновлено.\n";
                } else
                    std::cout << "Термін не знайдено.\n";

                Pause();
                break;
            }

            case 7: {
                Banner("ВИДАЛЕННЯ ТЕРМІНА");
                std::string name, confirm;

                std::cout << "Назва: ";
                std::getline(std::cin, name);

                std::cout << "Видалити '" << name << "'? (y/n): ";
                std::getline(std::cin, confirm);

                if (confirm == "y" || confirm == "Y") {
                    if (termManager.RemoveTerm(name)) {
                        termManager.Save();
                        std::cout << "Термін видалено.\n";
                    } else
                        std::cout << "Термін не знайдено.\n";
                } else {
                    std::cout << "Скасовано.\n";
                }

                Pause();
                break;
            }

            case 8:
                termManager.SortByName();
                termManager.Save();
                std::cout << "Відсортовано.\n";
                Pause();
                break;

            case 9:
                termManager.SortByDefinition();
                termManager.Save();
                std::cout << "Відсортовано.\n";
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
                std::cout << "Назва: ";
                std::getline(std::cin, name);
                termManager.PrintChainFrom(name);
                Pause();
                break;
            }

            case 13:
                ShowHelp();
                break;

            case 14:
                Banner("СТАТИСТИКА");
                termManager.PrintStats();
                Pause();
                break;

            case 20:
                if (currentUser.GetRole() == "admin")
                    HandleAdminUserMenu(userManager);
                else {
                    std::cout << "Недостатньо прав.\n";
                    Pause();
                }
                break;

            default:
                std::cout << "Невідомий пункт.\n";
                Pause();
                break;
        }
    }
}


// -------------------------------------------------------------
//                       MAIN
// -------------------------------------------------------------
int main() {
    UserManager userManager("users.txt");
    TermManager termManager("terms.csv");

    userManager.Load();
    userManager.EnsureDefaultAdmin();
    termManager.Load();

    while (true) {
        Banner("СИСТЕМА КЕРУВАННЯ ТЕРМІНАМИ");
        std::cout << "1. Увійти\n"
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
            Banner("РЕЄСТРАЦІЯ КОРИСТУВАЧА");

            std::string login, pass;
            std::cout << "Новий логін: ";
            std::getline(std::cin, login);
            login = Utils::Trim(login);

            std::cout << "Новий пароль: ";
            std::getline(std::cin, pass);
            pass = Utils::Trim(pass);

            if (login.empty() || pass.empty()) {
                std::cout << "Помилка: поля порожні.\n";
                Pause();
                continue;
            }

            if (userManager.AddUser(login, pass, "user")) {
                userManager.Save();
                std::cout << "Користувача зареєстровано!\n";
            } else {
                std::cout << "Такий логін вже існує.\n";
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

                std::cout << "Завершено.\n";
                return 0;
            } else {
                std::cout << "Невірний логін або пароль.\n";
                Pause();
            }

            continue;
        }

        std::cout << "Невідомий пункт.\n";
        Pause();
    }
}
