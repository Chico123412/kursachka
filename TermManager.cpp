#include "TermManager.h"
#include "Term.h"
#include "PrimitiveTerm.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

TermManager::TermManager(const std::string &filePath)
        : filePath(filePath) {}

void TermManager::Load() {
    terms.clear();
    std::ifstream in(filePath);
    if (!in.is_open()) {
        // Файл може ще не існувати – це не критична помилка
        std::cout << "[INFO] Файл термінів не знайдено, буде створено новий." << std::endl;
        return;
    }

    try {
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) {
                continue;
            }
            auto parts = Utils::Split(line, ';');
            if (parts.size() < 4) {
                continue;
            }
            std::string type = parts[0];
            std::string name = Utils::Unescape(parts[1]);
            std::string definition = Utils::Unescape(parts[2]);
            std::string refsStr = parts[3];

            if (type == "PRIM") {
                auto t = std::make_shared<PrimitiveTerm>(name, definition);
                terms.push_back(t);
            } else if (type == "TERM") {
                std::vector<std::string> refs;
                if (!refsStr.empty()) {
                    auto rawRefs = Utils::Split(refsStr, ',');
                    for (auto &r : rawRefs) {
                        auto trimmed = Utils::Trim(Utils::Unescape(r));
                        if (!trimmed.empty()) {
                            refs.push_back(trimmed);
                        }
                    }
                }
                auto t = std::make_shared<Term>(name, definition, refs);
                terms.push_back(t);
            }
        }
    } catch (const std::exception &ex) {
        std::cerr << "[ERROR] Помилка читання файлу термінів: " << ex.what() << std::endl;
    }
}

void TermManager::Save() const {
    std::ofstream out(filePath);
    if (!out.is_open()) {
        std::cerr << "[ERROR] Не вдалося відкрити файл для запису термінів." << std::endl;
        return;
    }

    try {
        for (const auto &t : terms) {
            out << t->Serialize() << "\n";
        }
    } catch (const std::exception &ex) {
        std::cerr << "[ERROR] Помилка запису файлу термінів: " << ex.what() << std::endl;
    }
}

void TermManager::AddTerm(const std::shared_ptr<TermBase> &term) {
    terms.push_back(term);
}




bool TermManager::RemoveTerm(const std::string &name) {
    std::string target = Utils::ToLower(name);

    auto it = std::remove_if(terms.begin(), terms.end(),
        [&](const std::shared_ptr<TermBase> &t) {
            return Utils::ToLower(t->GetName()) == target;
        });

    bool removed = it != terms.end();
    terms.erase(it, terms.end());
    return removed;
}





bool TermManager::EditDefinition(const std::string &name, const std::string &newDefinition) {
    auto term = FindByName(name);
    if (!term) {
        return false;
    }
    term->SetDefinition(newDefinition);
    return true;
}

std::shared_ptr<TermBase> TermManager::FindByName(const std::string &name) const {
    std::string target = Utils::ToLower(name);

    for (const auto &t : terms) {
        if (Utils::ToLower(t->GetName()) == target) {
            return t;
        }
    }
    return nullptr;
}








void TermManager::SortByName() {
    std::sort(terms.begin(), terms.end(),
              [](const std::shared_ptr<TermBase> &a, const std::shared_ptr<TermBase> &b) {
                  return a->GetName() < b->GetName();
              });
}

void TermManager::SortByDefinition() {
    std::sort(terms.begin(), terms.end(),
              [](const std::shared_ptr<TermBase> &a, const std::shared_ptr<TermBase> &b) {
                  return a->GetDefinition() < b->GetDefinition();
              });
}

void TermManager::PrintAllShort() const {
    if (terms.empty()) {
        std::cout << "База термінів порожня." << std::endl;
        return;
    }
    std::cout << "=== Список термінів (тільки назви) ===" << std::endl;
    for (const auto &t : terms) {
        std::cout << "- " << t->GetName();
        if (t->IsPrimitive()) {
            std::cout << " [первинний]";
        }
        std::cout << std::endl;
    }
}

void TermManager::PrintAllFull() const {
    if (terms.empty()) {
        std::cout << "База термінів порожня." << std::endl;
        return;
    }
    std::cout << "=== Повний список термінів ===" << std::endl;
    for (const auto &basePtr : terms) {
        std::cout << "Термін: " << basePtr->GetName() << std::endl;
        std::cout << "Визначення: " << basePtr->GetDefinition() << std::endl;
        if (!basePtr->IsPrimitive()) {
            auto termPtr = std::dynamic_pointer_cast<Term>(basePtr);
            if (termPtr) {
                std::cout << "Посилання на терміни: ";
                const auto &refs = termPtr->GetReferences();
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
        } else {
            std::cout << "Це первинне поняття (без посилань)." << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }
}

void TermManager::SearchByDefinition(const std::string &substring) const {
    std::string needle = substring;
    if (needle.empty()) {
        std::cout << "Порожній рядок пошуку." << std::endl;
        return;
    }

    std::cout << "Результати пошуку у визначеннях за підрядком \"" << needle << "\":" << std::endl;
    bool found = false;
    for (const auto &t : terms) {
        if (t->GetDefinition().find(needle) != std::string::npos) {
            std::cout << "- " << t->GetName() << ": " << t->GetDefinition() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Терміни з таким фрагментом у визначенні не знайдено." << std::endl;
    }
}

void TermManager::PrintFilteredByPrimitive(bool primitiveOnly) const {
    bool any = false;
    if (primitiveOnly) {
        std::cout << "=== Первинні терміни ===" << std::endl;
    } else {
        std::cout << "=== Складні терміни ===" << std::endl;
    }

    for (const auto &t : terms) {
        if (t->IsPrimitive() == primitiveOnly) {
            std::cout << "- " << t->GetName() << ": " << t->GetDefinition() << std::endl;
            any = true;
        }
    }

    if (!any) {
        std::cout << "Нічого не знайдено." << std::endl;
    }
}

void TermManager::PrintChainRecursive(const std::string &name,
                                      std::unordered_set<std::string> &visited,
                                      int level) const {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
    std::cout << "-> " << name << std::endl;

    if (visited.count(name) > 0) {
        for (int i = 0; i <= level; ++i) {
            std::cout << "  ";
        }
        std::cout << "[УВАГА] Виявлено цикл у посиланнях." << std::endl;
        return;
    }
    visited.insert(name);

    auto basePtr = FindByName(name);
    if (!basePtr) {
        for (int i = 0; i <= level; ++i) {
            std::cout << "  ";
        }
        std::cout << "[ПОПЕРЕДЖЕННЯ] Термін не знайдено у базі." << std::endl;
        return;
    }

    if (basePtr->IsPrimitive()) {
        for (int i = 0; i <= level; ++i) {
            std::cout << "  ";
        }
        std::cout << "(досягнуто первинного поняття)" << std::endl;
        return;
    }

    auto termPtr = std::dynamic_pointer_cast<Term>(basePtr);
    if (!termPtr) {
        return;
    }

    const auto &refs = termPtr->GetReferences();
    if (refs.empty()) {
        for (int i = 0; i <= level; ++i) {
            std::cout << "  ";
        }
        std::cout << "(посилань немає)" << std::endl;
        return;
    }

    for (const auto &childName : refs) {
        PrintChainRecursive(childName, visited, level + 1);
    }
}

void TermManager::PrintChainFrom(const std::string &name) const {
    auto basePtr = FindByName(name);
    if (!basePtr) {
        std::cout << "Термін \"" << name << "\" не знайдено у базі." << std::endl;
        return;
    }
    std::unordered_set<std::string> visited;
    std::cout << "=== Ланцюжок від терміна до первинних понять ===" << std::endl;
    PrintChainRecursive(name, visited, 0);
}
