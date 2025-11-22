#include "PrimitiveTerm.h"
#include "Utils.h"
#include <iostream>

PrimitiveTerm::PrimitiveTerm() = default;

PrimitiveTerm::PrimitiveTerm(const std::string &name, const std::string &definition)
        : TermBase(name, definition) {}

PrimitiveTerm::PrimitiveTerm(const PrimitiveTerm &other)
        : TermBase(other) {}

PrimitiveTerm::PrimitiveTerm(PrimitiveTerm &&other) noexcept
        : TermBase(std::move(other)) {}

PrimitiveTerm::~PrimitiveTerm() {
    std::cout << "[DEBUG] Знищення первинного терміна: " << name << std::endl;
}

PrimitiveTerm &PrimitiveTerm::operator=(const PrimitiveTerm &other) {
    if (this != &other) {
        TermBase::operator=(other);
    }
    return *this;
}

PrimitiveTerm &PrimitiveTerm::operator=(PrimitiveTerm &&other) noexcept {
    if (this != &other) {
        TermBase::operator=(std::move(other));
    }
    return *this;
}

std::string PrimitiveTerm::GetType() const {
    return "PRIM";
}

bool PrimitiveTerm::IsPrimitive() const {
    return true;
}

std::string PrimitiveTerm::Serialize() const {
    std::string escapedName = Utils::Escape(name);
    std::string escapedDef = Utils::Escape(definition);
    // Формат: TYPE;name;definition;
    return GetType() + ";" + escapedName + ";" + escapedDef + ";";
}
