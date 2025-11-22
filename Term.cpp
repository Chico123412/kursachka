#include "Term.h"
#include "Utils.h"
#include <iostream>

Term::Term() = default;

Term::Term(const std::string &name,
           const std::string &definition,
           const std::vector<std::string> &references)
        : TermBase(name, definition), references(references) {}

Term::Term(const Term &other)
        : TermBase(other), references(other.references) {}

Term::Term(Term &&other) noexcept
        : TermBase(std::move(other)), references(std::move(other.references)) {}

Term::~Term() {
    std::cout << "[DEBUG] Знищення складного терміна: " << name << std::endl;
}

Term &Term::operator=(const Term &other) {
    if (this != &other) {
        TermBase::operator=(other);
        references = other.references;
    }
    return *this;
}

Term &Term::operator=(Term &&other) noexcept {
    if (this != &other) {
        TermBase::operator=(std::move(other));
        references = std::move(other.references);
    }
    return *this;
}

const std::vector<std::string> &Term::GetReferences() const {
    return references;
}

void Term::SetReferences(const std::vector<std::string> &refs) {
    references = refs;
}

std::string Term::GetType() const {
    return "TERM";
}

bool Term::IsPrimitive() const {
    return false;
}

std::string Term::Serialize() const {
    // Формат: TYPE;name;definition;ref1,ref2,...
    std::string escapedName = Utils::Escape(name);
    std::string escapedDef = Utils::Escape(definition);
    std::vector<std::string> escapedRefs;
    escapedRefs.reserve(references.size());
    for (const auto &r : references) {
        escapedRefs.push_back(Utils::Escape(r));
    }
    std::string refsJoined = Utils::Join(escapedRefs, ',');
    return GetType() + ";" + escapedName + ";" + escapedDef + ";" + refsJoined;
}
