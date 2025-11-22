#include "TermBase.h"
#include <iostream>

TermBase::TermBase() = default;

TermBase::TermBase(const std::string &name, const std::string &definition)
        : name(name), definition(definition) {}

TermBase::TermBase(const TermBase &other)
        : name(other.name), definition(other.definition) {}

TermBase::TermBase(TermBase &&other) noexcept
        : name(std::move(other.name)), definition(std::move(other.definition)) {}

TermBase::~TermBase() {
    // Невелике повідомлення для демонстрації виклику деструктора
    std::cout << "[DEBUG] Знищення терміна: " << name << std::endl;
}

TermBase &TermBase::operator=(const TermBase &other) {
    if (this != &other) {
        name = other.name;
        definition = other.definition;
    }
    return *this;
}

TermBase &TermBase::operator=(TermBase &&other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        definition = std::move(other.definition);
    }
    return *this;
}

const std::string &TermBase::GetName() const {
    return name;
}

void TermBase::SetName(const std::string &value) {
    name = value;
}

const std::string &TermBase::GetDefinition() const {
    return definition;
}

void TermBase::SetDefinition(const std::string &value) {
    definition = value;
}
