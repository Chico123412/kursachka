#ifndef KURSOVA_TERMMANAGER_H
#define KURSOVA_TERMMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_set>
#include "TermBase.h"

class TermManager {
private:
    std::vector<std::shared_ptr<TermBase>> terms;
    std::string filePath;

    void PrintChainRecursive(const std::string &name,
                             std::unordered_set<std::string> &visited,
                             int level) const;

public:
    explicit TermManager(const std::string &filePath);

    void Load();
    void Save() const;

    void AddTerm(const std::shared_ptr<TermBase> &term);
    bool RemoveTerm(const std::string &name);
    bool EditDefinition(const std::string &name, const std::string &newDefinition);

    std::shared_ptr<TermBase> FindByName(const std::string &name) const;

    void SortByName();
    void SortByDefinition();

    void PrintAllShort() const;
    void PrintAllFull() const;

    void SearchByDefinition(const std::string &substring) const;

    void PrintFilteredByPrimitive(bool primitiveOnly) const;

    void PrintChainFrom(const std::string &name) const;

    // додано для захисту від видалення
    bool IsReferenced(const std::string &name) const;

    // додано для автозаповнення
    void EnsureDefaultTerms();

    // додано для статистики
    void PrintStats() const;
};

#endif //KURSOVA_TERMMANAGER_H
