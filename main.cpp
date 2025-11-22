#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "UserManager.h"
#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"

void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ShowHelp() {
    std::cout << "=== Допомога ===" << std::endl;
    std::cout << "Система керує базою термінів певної науки." << std::endl;
    std::cout << "Підтримується авторизація користувачів, додавання, редагування," << std::endl;
    std::cout << "видалення термінів, побудова ланцюжків від складних термінів" << std::endl;
    std::cout << "до первинних понять, сортування за термінами та визначеннями," << std::endl;
    std::cout << "пошук за фрагментом визначення, фільтрація за типом терміна." << std::endl;
    std::cout << "Перед завершенням роботи усі зміни зберігаються у файли." << std::endl;
}

void HandleAdminUserMenu(UserManager &userManager) {
    while (true) {
        std::cout << "\n=== Меню адміністратора (користувачі) ===" << std::endl;
        std::cout << "1. Переглянути список користувачів" << std::endl;
        std::cout << "2. Додати користувача" << std::endl;
        std::cout << "3. Видалити користувача" << std::endl;
        std::cout << "0. Повернутися до головного меню" << std::endl;
        std::cout << "Вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення, спробуйте ще раз." << std::endl;
            continue;
        }
        ClearCin();

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            const auto &users = userManager.GetUsers();
            std::cout << "=== Зареєстровані користувачі ===" << std::endl;
            for (const auto &u : users) {
                std::cout << "- " << u.GetUsername()
                          << " (роль: " << u.GetRole() << ")" << std::endl;
            }
        } else if (choice == 2) {
            std::string username, password, role;
            std::cout << "Введіть логін нового користувача: ";
            std::getline(std::cin, username);
            username = Utils::Trim(username);

            std::cout << "Введіть пароль: ";
            std::getline(std::cin, password);

            std::cout << "Введіть роль (admin/user): ";
            std::getline(std::cin, role);
            role = Utils::Trim(role);
            if (role != "admin" && role != "user") {
                std::cout << "Роль некоректна. Допустимі значення: admin або user." << std::endl;
                continue;
            }

            if (userManager.AddUser(username, password, role)) {
                std::cout << "Користувача успішно додано." << std::endl;
                userManager.Save();
            } else {
                std::cout << "Користувач з таким логіном уже існує." << std::endl;
            }
        } else if (choice == 3) {
            std::string username;
            std::cout << "Введіть логін користувача для видалення: ";
            std::getline(std::cin, username);
            username = Utils::Trim(username);

            if (userManager.RemoveUser(username)) {
                std::cout << "Користувача видалено." << std::endl;
                userManager.Save();
            } else {
                std::cout << "Користувача не знайдено або видалення заборонено." << std::endl;
            }
        } else {
            std::cout << "Невідомий пункт меню." << std::endl;
        }
    }
}

void HandleAddTerm(TermManager &termManager) {
    std::cout << "\n=== Додавання терміна ===" << std::endl;
    std::cout << "Оберіть тип терміна:" << std::endl;
    std::cout << "1. Первинне поняття" << std::endl;
    std::cout << "2. Складний термін (з посиланнями)" << std::endl;
    std::cout << "Вибір: ";

    int typeChoice;
    if (!(std::cin >> typeChoice)) {
        ClearCin();
        std::cout << "Некоректне введення." << std::endl;
        return;
    }
    ClearCin();

    std::string name, definition;
    std::cout << "Введіть назву терміна: ";
    std::getline(std::cin, name);
    name = Utils::Trim(name);

    std::cout << "Введіть визначення: ";
    std::getline(std::cin, definition);
    definition = Utils::Trim(definition);

    if (name.empty() || definition.empty()) {
        std::cout << "Назва та визначення не можуть бути порожніми." << std::endl;
        return;
    }

    if (typeChoice == 1) {
        auto t = std::make_shared<PrimitiveTerm>(name, definition);
        termManager.AddTerm(t);
        std::cout << "Первинний термін додано." << std::endl;
    } else if (typeChoice == 2) {
        std::cout << "Введіть через кому назви термінів, на які є посилання (можна залишити порожнім): ";
        std::string refsLine;
        std::getline(std::cin, refsLine);

        std::vector<std::string> refs;
        if (!refsLine.empty()) {
            auto rawRefs = Utils::Split(refsLine, ',');
            for (auto &r : rawRefs) {
                auto trimmed = Utils::Trim(r);
                if (!trimmed.empty()) {
                    refs.push_back(trimmed);
                }
            }
        }

        auto t = std::make_shared<Term>(name, definition, refs);
        termManager.AddTerm(t);
        std::cout << "Складний термін додано." << std::endl;
    } else {
        std::cout << "Невірний тип." << std::endl;
    }
}

void ShowMainMenu(const User &currentUser,
                  TermManager &termManager,
                  UserManager &userManager) {
    while (true) {
        std::cout << "\n=== Головне меню ===" << std::endl;
        std::cout << "Поточний користувач: " << currentUser.GetUsername()
                  << " (роль: " << currentUser.GetRole() << ")" << std::endl;

        std::cout << "1. Переглянути список термінів (коротко)" << std::endl;
        std::cout << "2. Переглянути всі терміни (повна інформація)" << std::endl;
        std::cout << "3. Пошук терміна за назвою" << std::endl;
        std::cout << "4. Пошук за фрагментом визначення" << std::endl;
        std::cout << "5. Додати термін" << std::endl;
        std::cout << "6. Редагувати визначення терміна" << std::endl;
        std::cout << "7. Видалити термін" << std::endl;
        std::cout << "8. Сортувати за назвою" << std::endl;
        std::cout << "9. Сортувати за визначенням" << std::endl;
        std::cout << "10. Вивести первинні терміни" << std::endl;
        std::cout << "11. Вивести складні терміни" << std::endl;
        std::cout << "12. Показати ланцюжок від терміна до первинних понять" << std::endl;
        std::cout << "13. Допомога" << std::endl;
        if (currentUser.GetRole() == "admin") {
            std::cout << "20. Адміністрування користувачів" << std::endl;
        }
        std::cout << "0. Вихід" << std::endl;
        std::cout << "Вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення, спробуйте ще раз." << std::endl;
            continue;
        }
        ClearCin();

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            termManager.PrintAllShort();
        } else if (choice == 2) {
            termManager.PrintAllFull();
        } else if (choice == 3) {
            std::string name;
            std::cout << "Введіть назву терміна: ";
            std::getline(std::cin, name);
            name = Utils::Trim(name);
            auto term = termManager.FindByName(name);
            if (!term) {
                std::cout << "Термін не знайдено." << std::endl;
            } else {
                std::cout << "Термін: " << term->GetName() << std::endl;
                std::cout << "Визначення: " << term->GetDefinition() << std::endl;
                if (term->IsPrimitive()) {
                    std::cout << "Тип: первинний." << std::endl;
                } else {
                    auto tptr = std::dynamic_pointer_cast<Term>(term);
                    if (tptr) {
                        std::cout << "Посилання на: ";
                        const auto &refs = tptr->GetReferences();
                        if (refs.empty()) {
                            std::cout << "немає";
                        } else {
                            for (size_t i = 0; i < refs.size(); ++i) {
                                if (i > 0) {
                                    std::cout << ", ";
                                }
                                std::cout << refs[i];
                            }
                        }
                        std::cout << std::endl;
                    }
                }
            }
        } else if (choice == 4) {
            std::string fragment;
            std::cout << "Введіть фрагмент визначення: ";
            std::getline(std::cin, fragment);
            termManager.SearchByDefinition(fragment);
        } else if (choice == 5) {
            HandleAddTerm(termManager);
        } else if (choice == 6) {
            std::string name, newDef;
            std::cout << "Введіть назву терміна для редагування: ";
            std::getline(std::cin, name);
            name = Utils::Trim(name);

            std::cout << "Введіть нове визначення: ";
            std::getline(std::cin, newDef);
            newDef = Utils::Trim(newDef);

            if (termManager.EditDefinition(name, newDef)) {
                std::cout << "Визначення змінено." << std::endl;
            } else {
                std::cout << "Термін не знайдено." << std::endl;
            }
        } else if (choice == 7) {
            std::string name;
            std::cout << "Введіть назву терміна для видалення: ";
            std::getline(std::cin, name);
            name = Utils::Trim(name);

            if (termManager.RemoveTerm(name)) {
                std::cout << "Термін видалено." << std::endl;
            } else {
                std::cout << "Термін не знайдено." << std::endl;
            }
        } else if (choice == 8) {
            termManager.SortByName();
            std::cout << "Список відсортовано за назвою." << std::endl;
        } else if (choice == 9) {
            termManager.SortByDefinition();
            std::cout << "Список відсортовано за визначенням." << std::endl;
        } else if (choice == 10) {
            termManager.PrintFilteredByPrimitive(true);
        } else if (choice == 11) {
            termManager.PrintFilteredByPrimitive(false);
        } else if (choice == 12) {
            std::string name;
            std::cout << "Введіть назву терміна: ";
            std::getline(std::cin, name);
            name = Utils::Trim(name);
            termManager.PrintChainFrom(name);
        } else if (choice == 13) {
            ShowHelp();
        } else if (choice == 20 && currentUser.GetRole() == "admin") {
            HandleAdminUserMenu(userManager);
        } else {
            std::cout << "Невідомий пункт меню." << std::endl;
        }
    }
}

int main() {
    UserManager userManager("users.txt");
    TermManager termManager("terms.csv");

    userManager.Load();
    userManager.EnsureDefaultAdmin();

    termManager.Load();

    std::cout << "=== Система управління базою термінів ===" << std::endl;

    User currentUser;
    while (true) {
        std::string login, password;
        std::cout << "Логін: ";
        std::getline(std::cin, login);
        std::cout << "Пароль: ";
        std::getline(std::cin, password);

        if (userManager.Authenticate(login, password, currentUser)) {
            std::cout << "Авторизація успішна." << std::endl;
            break;
        } else {
            std::cout << "Невірний логін або пароль. Спробувати ще раз? (y/n): ";
            std::string ans;
            std::getline(std::cin, ans);
            if (!ans.empty() && (ans[0] == 'n' || ans[0] == 'N')) {
                std::cout << "Завершення роботи." << std::endl;
                return 0;
            }
        }
    }

    ShowMainMenu(currentUser, termManager, userManager);

    std::cout << "Збереження даних..." << std::endl;
    userManager.Save();
    termManager.Save();
    std::cout << "Роботу завершено." << std::endl;

    return 0;
}
