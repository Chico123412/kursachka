#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "UserManager.h"
#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"

// ----------------------------------------------------------
// СЛУЖБОВІ ФУНКЦІЇ
// ----------------------------------------------------------

void Pause() {
    std::cout << "\n(Натисніть Enter, щоб продовжити...)";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Banner(const std::string &title) {
    std::cout << "\n===============================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "===============================================" << std::endl;
}

// ----------------------------------------------------------
// ДОПОМОГА
// ----------------------------------------------------------

void ShowHelp() {
    Banner("ДОПОМОГА");

    std::cout
        << "Це консольна система керування базою термінів.\n"
        << "Підтримуються первинні та складні терміни, пошук,\n"
        << "сортування, редагування, побудова ланцюжків.\n\n"
        << "Файли:\n"
        << " • terms.csv — база термінів\n"
        << " • users.txt — користувачі\n\n"
        << "Стартове меню:\n"
        << "  1. Увійти\n"
        << "  2. Створити користувача\n"
        << "  3. Допомога\n"
        << "  0. Вихід\n\n"
        << "Права доступу:\n"
        << "  Користувач — може тільки переглядати.\n"
        << "  Адмін — може додавати, редагувати, видаляти терміни та\n"
        << "           керувати користувачами.\n";

    Pause();
}

// ----------------------------------------------------------
// МЕНЮ АДМІНІСТРАТОРА
// ----------------------------------------------------------

void HandleAdminUserMenu(UserManager &userManager) {
    while (true) {
        Banner("МЕНЮ АДМІНА — КОРИСТУВАЧІ");
        std::cout
            << "1. Переглянути користувачів\n"
            << "2. Додати користувача\n"
            << "3. Додати адміністратора\n"
            << "4. Видалити користувача\n"
            << "0. Назад\n"
            << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення.\n";
            continue;
        }
        ClearCin();

        if (choice == 0) return;

        if (choice == 1) {
            Banner("СПИСОК КОРИСТУВАЧІВ");
            for (auto &u : userManager.GetUsers()) {
                std::cout << "- " << u.GetUsername()
                          << "  (роль: " << u.GetRole() << ")\n";
            }
            Pause();
        }
        else if (choice == 2) {
            std::string login, pass;
            std::cout << "Логін нового користувача: ";
            std::getline(std::cin, login);
            login = Utils::Trim(login);

            std::cout << "Пароль: ";
            std::getline(std::cin, pass);

            if (userManager.AddUser(login, pass, "user")) {
                std::cout << "Користувача додано.\n";
                userManager.Save();
            } else {
                std::cout << "Такий користувач вже існує.\n";
            }
            Pause();
        }
        else if (choice == 3) {
            std::string login, pass;
            std::cout << "Логін нового адміністратора: ";
            std::getline(std::cin, login);
            login = Utils::Trim(login);

            std::cout << "Пароль: ";
            std::getline(std::cin, pass);

            if (userManager.AddUser(login, pass, "admin")) {
                std::cout << "Адміна додано.\n";
                userManager.Save();
            } else {
                std::cout << "Такий користувач вже існує.\n";
            }
            Pause();
        }
        else if (choice == 4) {
            std::string login;
            std::cout << "Кого видалити (логін): ";
            std::getline(std::cin, login);

            if (userManager.RemoveUser(login)) {
                std::cout << "Видалено.\n";
                userManager.Save();
            } else {
                std::cout << "Користувача не знайдено.\n";
            }
            Pause();
        }
        else {
            std::cout << "Невідомий пункт.\n";
            Pause();
        }
    }
}

// ----------------------------------------------------------
// ДОДАВАННЯ ТЕРМІНА
// ----------------------------------------------------------

void HandleAddTerm(TermManager &termManager) {
    Banner("ДОДАВАННЯ ТЕРМІНА");

    std::cout << "1. Первинний термін\n"
              << "2. Складний термін\n"
              << "Ваш вибір: ";

    int t;
    if (!(std::cin >> t)) {
        ClearCin();
        std::cout << "Помилка.\n";
        Pause();
        return;
    }
    ClearCin();

    std::string name, def;
    std::cout << "Назва: ";
    std::getline(std::cin, name);

    std::cout << "Визначення: ";
    std::getline(std::cin, def);

    if (name.empty() || def.empty()) {
        std::cout << "Поля не можуть бути порожніми.\n";
        Pause();
        return;
    }

    if (t == 1) {
        termManager.AddTerm(std::make_shared<PrimitiveTerm>(name, def));
        termManager.Save();
        std::cout << "Додано первинний термін.\n";
    }
    else if (t == 2) {
        std::cout << "Посилання через кому: ";
        std::string rline;
        std::getline(std::cin, rline);

        std::vector<std::string> refs;
        if (!rline.empty()) {
            for (auto &x : Utils::Split(rline, ',')) {
                if (!Utils::Trim(x).empty())
                    refs.push_back(Utils::Trim(x));
            }
        }

        termManager.AddTerm(std::make_shared<Term>(name, def, refs));
        termManager.Save();
        std::cout << "Додано складний термін.\n";
    }

    Pause();
}

// ----------------------------------------------------------
// ГОЛОВНЕ МЕНЮ
// ----------------------------------------------------------

void ShowMainMenu(const User &currentUser, TermManager &termManager, UserManager &userManager) {
    while (true) {

        Banner("ГОЛОВНЕ МЕНЮ");
        std::cout
            << "1. Список (коротко)\n"
            << "2. Список (повний)\n"
            << "3. Пошук за назвою\n"
            << "4. Пошук у визначеннях\n"
            << "5. Додати термін  (admin)\n"
            << "6. Редагувати     (admin)\n"
            << "7. Видалити       (admin)\n"
            << "8. Сортувати за назвою (admin)\n"
            << "9. Сортувати за визначенням (admin)\n"
            << "10. Первинні терміни\n"
            << "11. Складні терміни\n"
            << "12. Ланцюжок терміна\n"
            << "13. Допомога\n"
            << "14. Статистика\n";

        if (currentUser.GetRole() == "admin")
            std::cout << "20. Керування користувачами\n";

        std::cout << "0. Вихід\n"
                  << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Помилка.\n";
            Pause();
            continue;
        }
        ClearCin();

        if (choice == 0) return;

        switch (choice) {
            case 1:
                termManager.PrintAllShort();
                Pause();
                break;

            case 2:
                termManager.PrintAllFull();
                Pause();
                break;

            case 3: {
                std::string name;
                std::cout << "Назва: ";
                std::getline(std::cin, name);
                auto t = termManager.FindByName(name);
                if (!t) std::cout << "Не знайдено.\n";
                else std::cout << t->GetDefinition() << "\n";
                Pause();
                break;
            }

            case 4: {
                std::string s;
                std::cout << "Фрагмент: ";
                std::getline(std::cin, s);
                termManager.SearchByDefinition(s);
                Pause();
                break;
            }

            case 5:
                if (currentUser.GetRole() != "admin") {
                    std::cout << "Недостатньо прав.\n";
                    Pause();
                } else HandleAddTerm(termManager);
                break;

            case 6: {
                if (currentUser.GetRole() != "admin") {
                    std::cout << "Недостатньо прав.\n";
                    Pause();
                    break;
                }

                std::string name, def;
                std::cout << "Назва: ";
                std::getline(std::cin, name);
                std::cout << "Нове визначення: ";
                std::getline(std::cin, def);

                if (termManager.EditDefinition(name, def)) {
                    termManager.Save();
                    std::cout << "Оновлено.\n";
                } else std::cout << "Не знайдено.\n";

                Pause();
                break;
            }

            case 7: {
                if (currentUser.GetRole() != "admin") {
                    std::cout << "Недостатньо прав.\n";
                    Pause();
                    break;
                }

                std::string name;
                std::cout << "Назва: ";
                std::getline(std::cin, name);

                if (termManager.RemoveTerm(name)) {
                    termManager.Save();
                    std::cout << "Видалено.\n";
                } else std::cout << "Не знайдено.\n";

                Pause();
                break;
            }

            case 8:
                if (currentUser.GetRole() != "admin") {
                    std::cout << "Недостатньо прав.\n";
                    Pause();
                } else {
                    termManager.SortByName();
                    termManager.Save();
                    std::cout << "Відсортовано.\n";
                    Pause();
                }
                break;

            case 9:
                if (currentUser.GetRole() != "admin") {
                    std::cout << "Недостатньо прав.\n";
                    Pause();
                } else {
                    termManager.SortByDefinition();
                    termManager.Save();
                    std::cout << "Відсортовано.\n";
                    Pause();
                }
                break;

            case 10:
                termManager.PrintFilteredByPrimitive(true);
                Pause();
                break;

            case 11:
                termManager.PrintFilteredByPrimitive(false);
                Pause();
                break;

            case 12: {
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
                std::cout << "Невідомий пункт меню.\n";
                Pause();
                break;
        }
    }
}

// ----------------------------------------------------------
// ГОЛОВНИЙ ВХІД
// ----------------------------------------------------------

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
            << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Помилка.\n";
            Pause();
            continue;
        }
        ClearCin();

        if (choice == 0) {
            std::cout << "До побачення!\n";
            return 0;
        }

        if (choice == 3) {
            ShowHelp();
            continue;
        }

        if (choice == 2) {
            Banner("РЕЄСТРАЦІЯ");
            std::string login, pass1, pass2;

            std::cout << "Логін: ";
            std::getline(std::cin, login);

            std::cout << "Пароль: ";
            std::getline(std::cin, pass1);

            std::cout << "Повторіть пароль: ";
            std::getline(std::cin, pass2);

            if (pass1 != pass2) {
                std::cout << "Паролі не збігаються.\n";
                Pause();
                continue;
            }

            if (userManager.AddUser(login, pass1, "user")) {
                userManager.Save();
                std::cout << "Реєстрація успішна.\n";
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

            std::cout << "Пароль: ";
            std::getline(std::cin, pass);

            User currentUser;

            if (userManager.Authenticate(login, pass, currentUser)) {
                std::cout << "Вхід успішний!\n";
                Pause();
                ShowMainMenu(currentUser, termManager, userManager);

                userManager.Save();
                termManager.Save();
                return 0;
            } else {
                std::cout << "Невірний логін або пароль.\n";
                Pause();
                continue;
            }
        }

        std::cout << "Невідомий пункт.\n";
        Pause();
    }
}
