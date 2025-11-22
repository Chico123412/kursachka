#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "UserManager.h"
#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"

// Очищення вводу
void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Допомога
void ShowHelp() {
    std::cout << "=== Допомога ===" << std::endl;
    std::cout << "Система керує базою термінів певної науки." << std::endl;
    std::cout << "Підтримується авторизація користувачів, додавання, редагування," << std::endl;
    std::cout << "видалення термінів, побудова ланцюжків від складних термінів" << std::endl;
    std::cout << "до первинних понять, сортування за термінами та визначеннями," << std::endl;
    std::cout << "пошук за фрагментом визначення, фільтрація за типом терміна." << std::endl;
    std::cout << "Усі зміни зберігаються в файл terms.csv." << std::endl;
}

// Меню адміністрування користувачів
void HandleAdminUserMenu(UserManager &userManager) {
    while (true) {
        std::cout << "\n=== Меню адміністратора (користувачі) ===" << std::endl;
        std::cout << "1. Переглянути користувачів" << std::endl;
        std::cout << "2. Додати користувача" << std::endl;
        std::cout << "3. Видалити користувача" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення." << std::endl;
            continue;
        }
        ClearCin();

        if (choice == 0) return;

        if (choice == 1) {
            const auto &users = userManager.GetUsers();
            std::cout << "=== Користувачі ===" << std::endl;
            for (const auto &u : users) {
                std::cout << "- " << u.GetUsername() << " (роль: " << u.GetRole() << ")" << std::endl;
            }
        }
        else if (choice == 2) {
            std::string username, pass, role;

            std::cout << "Логін: ";
            std::getline(std::cin, username);

            std::cout << "Пароль: ";
            std::getline(std::cin, pass);

            std::cout << "Роль (admin/user): ";
            std::getline(std::cin, role);

            if (userManager.AddUser(username, pass, role)) {
                std::cout << "Користувача додано." << std::endl;
                userManager.Save();
            } else {
                std::cout << "Користувач з таким логіном існує." << std::endl;
            }
        }
        else if (choice == 3) {
            std::string username;
            std::cout << "Введіть логін: ";
            std::getline(std::cin, username);

            if (userManager.RemoveUser(username)) {
                std::cout << "Користувача видалено." << std::endl;
                userManager.Save();
            } else {
                std::cout << "Не знайдено." << std::endl;
            }
        }
        else {
            std::cout << "Невідомий пункт." << std::endl;
        }
    }
}

// Додавання термінів
void HandleAddTerm(TermManager &termManager) {
    std::cout << "\n=== Додавання терміна ===" << std::endl;
    std::cout << "1. Первинне поняття" << std::endl;
    std::cout << "2. Складний термін" << std::endl;
    std::cout << "Вибір: ";

    int typeChoice;
    if (!(std::cin >> typeChoice)) {
        ClearCin();
        std::cout << "Некоректне введення." << std::endl;
        return;
    }
    ClearCin();

    std::string name, definition;
    std::cout << "Назва: ";
    std::getline(std::cin, name);

    std::cout << "Визначення: ";
    std::getline(std::cin, definition);

    name = Utils::Trim(name);
    definition = Utils::Trim(definition);

    if (name.empty() || definition.empty()) {
        std::cout << "Поля не можуть бути порожніми." << std::endl;
        return;
    }

    if (typeChoice == 1) {
        termManager.AddTerm(std::make_shared<PrimitiveTerm>(name, definition));
        termManager.Save();   // ← ОБОВʼЯЗКОВЕ ЗБЕРЕЖЕННЯ
        std::cout << "Первинний термін додано." << std::endl;
    }
    else if (typeChoice == 2) {
        std::string refsLine;
        std::cout << "Посилання (через кому): ";
        std::getline(std::cin, refsLine);

        std::vector<std::string> refs;

        if (!refsLine.empty()) {
            auto raw = Utils::Split(refsLine, ',');
            for (auto &r : raw) {
                r = Utils::Trim(r);
                if (!r.empty()) refs.push_back(r);
            }
        }

        termManager.AddTerm(std::make_shared<Term>(name, definition, refs));
        termManager.Save();   // ← ОБОВʼЯЗКОВЕ ЗБЕРЕЖЕННЯ
        std::cout << "Складний термін додано." << std::endl;
    }
    else {
        std::cout << "Некоректний вибір." << std::endl;
    }
}

// Головне меню
void ShowMainMenu(const User &currentUser, TermManager &termManager, UserManager &userManager) {
    while (true) {
        std::cout << "\n=== Головне меню ===" << std::endl;

        std::cout << "1. Короткий список термінів" << std::endl;
        std::cout << "2. Повний список" << std::endl;
        std::cout << "3. Пошук за назвою" << std::endl;
        std::cout << "4. Пошук у визначеннях" << std::endl;
        std::cout << "5. Додати термін" << std::endl;
        std::cout << "6. Редагувати визначення" << std::endl;
        std::cout << "7. Видалити термін" << std::endl;
        std::cout << "8. Сортувати за назвою" << std::endl;
        std::cout << "9. Сортувати за визначенням" << std::endl;
        std::cout << "10. Первинні терміни" << std::endl;
        std::cout << "11. Складні терміни" << std::endl;
        std::cout << "12. Ланцюжок терміна" << std::endl;
        std::cout << "13. Допомога" << std::endl;
        std::cout << "14. Статистика бази" << std::endl;

        if (currentUser.GetRole() == "admin") {
            std::cout << "20. Користувачі (адмін)" << std::endl;
        }

        std::cout << "0. Вихід" << std::endl;
        std::cout << "Вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            ClearCin();
            std::cout << "Некоректне введення." << std::endl;
            continue;
        }
        ClearCin();

        if (choice == 0) return;

        if (choice == 1) termManager.PrintAllShort();
        else if (choice == 2) termManager.PrintAllFull();
        else if (choice == 3) {
            std::string name;
            std::cout << "Назва: ";
            std::getline(std::cin, name);

            auto t = termManager.FindByName(Utils::Trim(name));
            if (!t) std::cout << "Не знайдено." << std::endl;
            else {
                std::cout << "Термін: " << t->GetName() << std::endl;
                std::cout << "Визначення: " << t->GetDefinition() << std::endl;
            }
        }
        else if (choice == 4) {
            std::string frag;
            std::cout << "Фрагмент: ";
            std::getline(std::cin, frag);
            termManager.SearchByDefinition(frag);
        }
        else if (choice == 5) HandleAddTerm(termManager);
        else if (choice == 6) {
            std::string name, def;
            std::cout << "Назва: ";
            std::getline(std::cin, name);
            std::cout << "Нове визначення: ";
            std::getline(std::cin, def);

            if (termManager.EditDefinition(name, def)) {
                termManager.Save();
                std::cout << "Змінено." << std::endl;
            } else {
                std::cout << "Не знайдено." << std::endl;
            }
        }
        else if (choice == 7) {
            std::string name;
            std::cout << "Назва: ";
            std::getline(std::cin, name);

            if (termManager.RemoveTerm(name)) {
                termManager.Save();
                std::cout << "Видалено." << std::endl;
            } else {
                std::cout << "Не знайдено або заборонено." << std::endl;
            }
        }
        else if (choice == 8) {
            termManager.SortByName();
            termManager.Save();
            std::cout << "Відсортовано." << std::endl;
        }
        else if (choice == 9) {
            termManager.SortByDefinition();
            termManager.Save();
            std::cout << "Відсортовано." << std::endl;
        }
        else if (choice == 10) termManager.PrintFilteredByPrimitive(true);
        else if (choice == 11) termManager.PrintFilteredByPrimitive(false);
        else if (choice == 12) {
            std::string name;
            std::cout << "Назва: ";
            std::getline(std::cin, name);
            termManager.PrintChainFrom(name);
        }
        else if (choice == 13) ShowHelp();
        else if (choice == 14) termManager.PrintStats();
        else if (choice == 20 && currentUser.GetRole() == "admin") HandleAdminUserMenu(userManager);
        else std::cout << "Невідомий пункт." << std::endl;
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

    std::cout << "=== Система управління базою термінів ===" << std::endl;

    User currentUser;

    while (true) {
        std::string login, pass;

        std::cout << "Логін: ";
        std::getline(std::cin, login);
        std::cout << "Пароль: ";
        std::getline(std::cin, pass);

        if (userManager.Authenticate(login, pass, currentUser)) {
            std::cout << "Авторизація успішна." << std::endl;
            break;
        }
        else {
            std::string ans;
            std::cout << "Невірно. Спробувати ще? (y/n): ";
            std::getline(std::cin, ans);

            if (!ans.empty() && (ans[0] == 'n' || ans[0] == 'N')) {
                return 0;
            }
        }
    }

    ShowMainMenu(currentUser, termManager, userManager);

    std::cout << "Збереження даних..." << std::endl;
    userManager.Save();
    termManager.Save();
    std::cout << "Завершено." << std::endl;

    return 0;
}
