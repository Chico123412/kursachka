#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// -------------------------------------------------------------
//                     CONSTRUCTOR
// -------------------------------------------------------------
TermManager::TermManager(const std::string &filePath)
        : filePath(filePath) {}

// -------------------------------------------------------------
//                     LOAD
// -------------------------------------------------------------
void TermManager::Load() {
    terms.clear();
    std::ifstream in(filePath);
    if (!in.is_open()) {
        std::cout << "[INFO] Файл термінів не знайдено, буде створено новий." << std::endl;
        return;
    }

    try {
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;

            auto parts = Utils::Split(line, ';');
            if (parts.size() < 4) continue;

            std::string type = parts[0];
            std::string name = Utils::Unescape(parts[1]);
            std::string definition = Utils::Unescape(parts[2]);
            std::string refsStr = parts[3];

            if (type == "PRIM") {
                terms.push_back(std::make_shared<PrimitiveTerm>(name, definition));

            } else if (type == "TERM") {
                std::vector<std::string> refs;

                if (!refsStr.empty()) {
                    auto rawRefs = Utils::Split(refsStr, ',');
                    for (auto &r : rawRefs) {
                        auto trimmed = Utils::Trim(Utils::Unescape(r));
                        if (!trimmed.empty()) refs.push_back(trimmed);
                    }
                }

                terms.push_back(std::make_shared<Term>(name, definition, refs));
            }
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "[ERROR] Помилка читання файлу: " << ex.what() << std::endl;
    }
}

// -------------------------------------------------------------
//                     SAVE
// -------------------------------------------------------------
void TermManager::Save() const {
    std::ofstream out(filePath);
    if (!out.is_open()) {
        std::cerr << "[ERROR] Не вдалося зберегти файл термінів." << std::endl;
        return;
    }

    for (const auto &t : terms) {
        out << t->Serialize() << "\n";
    }
}

// -------------------------------------------------------------
//                     FIND (CASE-INSENSITIVE)
// -------------------------------------------------------------
std::shared_ptr<TermBase> TermManager::FindByName(const std::string &name) const {
    std::string target = Utils::ToLowerUTF8(name);

    for (const auto &t : terms) {
       if (Utils::ToLowerUTF8(t->GetName()) == target) {
            return t;
        }
    }
    return nullptr;
}

// -------------------------------------------------------------
//                     ADD TERM
// -------------------------------------------------------------
void TermManager::AddTerm(const std::shared_ptr<TermBase> &term) {
    terms.push_back(term);
}

// -------------------------------------------------------------
//                 CHECK IF TERM IS REFERENCED
// -------------------------------------------------------------
bool TermManager::IsReferenced(const std::string &name) const {
    std::string target = Utils::ToLowerUTF8(name);

    for (const auto &t : terms) {
        if (!t->IsPrimitive()) {
            auto termPtr = std::dynamic_pointer_cast<Term>(t);
            if (!termPtr) continue;

            for (const auto &ref : termPtr->GetReferences()) {
                if (Utils::ToLowerUTF8(ref) == target) {
                    return true;
                }
            }
        }
    }
    return false;
}

// -------------------------------------------------------------
//                REMOVE TERM (CASE-INSENSITIVE)
// -------------------------------------------------------------
bool TermManager::RemoveTerm(const std::string &name) {
    if (IsReferenced(name)) {
        std::cout << "[ПОМИЛКА] Неможливо видалити термін \"" << name
                  << "\", оскільки інші терміни містять на нього посилання." << std::endl;
        return false;
    }

    std::string target = Utils::ToLowerUTF8(name);

    auto it = std::remove_if(
        terms.begin(),
        terms.end(),
        [&](const std::shared_ptr<TermBase> &t) {
            return Utils::ToLowerUTF8(t->GetName()) == target
;
        }
    );

    bool removed = it != terms.end();
    terms.erase(it, terms.end());
    return removed;
}

// -------------------------------------------------------------
//                EDIT DEFINITION (CASE-INSENSITIVE)
// -------------------------------------------------------------
bool TermManager::EditDefinition(const std::string &name, const std::string &newDefinition) {
    auto term = FindByName(name);
    if (!term) return false;

    term->SetDefinition(newDefinition);
    return true;
}

// -------------------------------------------------------------
//                     SORTING
// -------------------------------------------------------------
void TermManager::SortByName() {
    std::sort(terms.begin(), terms.end(),
              [](const std::shared_ptr<TermBase> &a,
                 const std::shared_ptr<TermBase> &b)
              {
                  return Utils::ToLowerUTF8(a->GetName()) <
                         Utils::ToLowerUTF8(b->GetName());
              });
}


void TermManager::SortByDefinition() {
    std::sort(terms.begin(), terms.end(),
              [](const std::shared_ptr<TermBase> &a,
                 const std::shared_ptr<TermBase> &b)
              {
                  return Utils::ToLowerUTF8(a->GetDefinition()) <
                         Utils::ToLowerUTF8(b->GetDefinition());
              });
}


// -------------------------------------------------------------
//                  PRINT SHORT LIST
// -------------------------------------------------------------
void TermManager::PrintAllShort() const {
    if (terms.empty()) {
        std::cout << "База термінів порожня." << std::endl;
        return;
    }

    std::cout << "=== Список термінів ===" << std::endl;
    for (const auto &t : terms) {
        std::cout << "- " << t->GetName();
        if (t->IsPrimitive()) std::cout << " [первинний]";
        std::cout << std::endl;
    }
}

// -------------------------------------------------------------
//                   PRINT FULL LIST
// -------------------------------------------------------------
void TermManager::PrintAllFull() const {
    if (terms.empty()) {
        std::cout << "База термінів порожня." << std::endl;
        return;
    }

    std::cout << "=== Повний список термінів ===\n";

    for (const auto &basePtr : terms) {
        std::cout << "Термін: " << basePtr->GetName() << std::endl;
        std::cout << "Визначення: " << basePtr->GetDefinition() << std::endl;

        if (!basePtr->IsPrimitive()) {
            auto termPtr = std::dynamic_pointer_cast<Term>(basePtr);
            std::cout << "Посилання: ";

            const auto &refs = termPtr->GetReferences();
            if (refs.empty()) std::cout << "немає";
            else {
                for (size_t i = 0; i < refs.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << refs[i];
                }
            }
        } else {
            std::cout << "Тип: первинне поняття.";
        }

        std::cout << "\n-----------------------------\n";
    }
}

// -------------------------------------------------------------
//               SEARCH IN DEFINITIONS (CASE-INSENSITIVE)
// -------------------------------------------------------------
void TermManager::SearchByDefinition(const std::string &substring) const {
    if (substring.empty()) {
        std::cout << "Порожній запит." << std::endl;
        return;
    }

    std::string needle = Utils::ToLowerUTF8(substring);
    bool found = false;

    std::cout << "Результати пошуку:\n";

    for (const auto &t : terms) {
        if (Utils::ToLowerUTF8(t->GetDefinition()).find(needle) != std::string::npos) {
            std::cout << "- " << t->GetName() << ": " << t->GetDefinition() << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Нічого не знайдено.\n";
    }
}

// -------------------------------------------------------------
//                   FILTER BY TYPE
// -------------------------------------------------------------
void TermManager::PrintFilteredByPrimitive(bool primitiveOnly) const {
    bool any = false;

    for (const auto &t : terms) {
        if (t->IsPrimitive() == primitiveOnly) {
            std::cout << "- " << t->GetName() << ": " << t->GetDefinition() << std::endl;
            any = true;
        }
    }

    if (!any) std::cout << "Нічого не знайдено.\n";
}

// -------------------------------------------------------------
//               CHAIN (RECURSIVE)
// -------------------------------------------------------------
void TermManager::PrintChainRecursive(const std::string &name,
                                      std::unordered_set<std::string> &visited,
                                      int level) const {

    for (int i = 0; i < level; i++) std::cout << "  ";
    std::cout << "→ " << name << std::endl;

    std::string key = Utils::ToLowerUTF8(name);

    if (visited.count(key)) {
        for (int i = 0; i < level; i++) std::cout << "  ";
        std::cout << "[ЦИКЛ У ПОСИЛАННЯХ]\n";
        return;
    }

    visited.insert(key);

    auto ptr = FindByName(name);
    if (!ptr) {
        std::cout << "[ПОМИЛКА] Термін не знайдено.\n";
        return;
    }

    if (ptr->IsPrimitive()) {
        for (int i = 0; i < level; i++) std::cout << "  ";
        std::cout << "(первинне поняття)\n";
        return;
    }

    auto t = std::dynamic_pointer_cast<Term>(ptr);
    for (const auto &r : t->GetReferences()) {
        PrintChainRecursive(r, visited, level + 1);
    }
}

// -------------------------------------------------------------
//                   CHAIN ENTRY
// -------------------------------------------------------------
void TermManager::PrintChainFrom(const std::string &name) const {
    if (!FindByName(name)) {
        std::cout << "Термін \"" << name << "\" не знайдено.\n";
        return;
    }

    std::unordered_set<std::string> visited;

    std::cout << "\n=== Ланцюжок терміна \"" << name << "\" ===\n";
    PrintChainRecursive(name, visited, 0);
}

// -------------------------------------------------------------
//               AUTO-FILL DEFAULT TERMS
// -------------------------------------------------------------
void TermManager::EnsureDefaultTerms() {
    if (!terms.empty()) return;

    std::cout << "[INFO] База порожня — створюються стандартні терміни." << std::endl;

    terms.push_back(std::make_shared<PrimitiveTerm>("Клас", "Основне поняття ООП."));
    terms.push_back(std::make_shared<PrimitiveTerm>("Об’єкт", "Екземпляр класу."));
    terms.push_back(std::make_shared<PrimitiveTerm>("Метод", "Опис поведінки."));
    terms.push_back(std::make_shared<PrimitiveTerm>("Алгоритм", "Послідовність дій."));

    terms.push_back(std::make_shared<Term>(
            "Компіляція",
            "Процес перетворення коду.",
            std::vector<std::string>{"Алгоритм"}
    ));

    terms.push_back(std::make_shared<Term>(
            "Інкапсуляція",
            "Приховування реалізації.",
            std::vector<std::string>{"Клас", "Об’єкт"}
    ));

    Save();
}

// -------------------------------------------------------------
//                     STATISTICS
// -------------------------------------------------------------
void TermManager::PrintStats() const {
    int total = terms.size();
    int prim = 0;
    int comp = 0;

    for (const auto &t : terms) {
        if (t->IsPrimitive()) prim++;
        else comp++;
    }

    std::cout << "\n===== Статистика бази =====\n";
    std::cout << "Загальна кількість: " << total << std::endl;
    std::cout << "Первинних:          " << prim << std::endl;
    std::cout << "Складних:           " << comp << std::endl;
}
