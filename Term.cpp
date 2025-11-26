/**
 * @file Term.cpp
 * @brief Реалізація методів класу складного терміна.
 */

#include "Term.h"
#include "Utils.h"
#include <iostream>

/**
 * @brief Конструктор за замовчуванням.
 */
Term::Term() = default;

/**
 * @brief Параметризований конструктор.
 * @param name Назва терміна.
 * @param definition Текстове визначення.
 * @param references Список назв інших термінів, на які посилається цей термін.
 */
Term::Term(const std::string &name,
           const std::string &definition,
           const std::vector<std::string> &references)
        : TermBase(name, definition), references(references) {}

/**
 * @brief Конструктор копіювання.
 * @param other Об'єкт для копіювання.
 */
Term::Term(const Term &other)
        : TermBase(other), references(other.references) {}

/**
 * @brief Конструктор переміщення.
 * @param other Об'єкт для переміщення (r-value).
 */
Term::Term(Term &&other) noexcept
        : TermBase(std::move(other)), references(std::move(other.references)) {}

/**
 * @brief Деструктор.
 *
 * Виводить повідомлення в консоль для відстеження видалення об'єктів.
 */
Term::~Term() {
    std::cout << "[DEBUG] Знищення складного терміна: " << name << std::endl;
}

/**
 * @brief Оператор присвоєння (копіювання).
 * @param other Об'єкт-джерело.
 * @return Посилання на поточний об'єкт.
 */
Term &Term::operator=(const Term &other) {
    if (this != &other) {
        TermBase::operator=(other);
        references = other.references;
    }
    return *this;
}

/**
 * @brief Оператор присвоєння (переміщення).
 * @param other Об'єкт-джерело.
 * @return Посилання на поточний об'єкт.
 */
Term &Term::operator=(Term &&other) noexcept {
    if (this != &other) {
        TermBase::operator=(std::move(other));
        references = std::move(other.references);
    }
    return *this;
}

/**
 * @brief Отримує список посилань.
 * @return Константне посилання на вектор назв залежних термінів.
 */
const std::vector<std::string> &Term::GetReferences() const {
    return references;
}

/**
 * @brief Встановлює новий список посилань.
 * @param refs Вектор назв термінів.
 */
void Term::SetReferences(const std::vector<std::string> &refs) {
    references = refs;
}

/**
 * @brief Повертає рядковий ідентифікатор типу.
 * @return "TERM" — ідентифікатор складного терміна.
 */
std::string Term::GetType() const {
    return "TERM";
}

/**
 * @brief Перевіряє, чи є термін первинним.
 * @return false, оскільки цей клас описує складні терміни з посиланнями.
 */
bool Term::IsPrimitive() const {
    return false;
}

/**
 * @brief Серіалізує об'єкт у рядок формату CSV.
 *
 * Формує рядок вигляду: TERM;Назва;Визначення;Посил1,Посил2,Посил3
 * Використовує екранування для полів та об'єднання списку посилань через кому.
 *
 * @return std::string Серіалізовані дані.
 */
std::string Term::Serialize() const {
    // Формат: TYPE;name;definition;ref1,ref2,...
    std::string escapedName = Utils::Escape(name);
    std::string escapedDef = Utils::Escape(definition);
    
    std::vector<std::string> escapedRefs;
    escapedRefs.reserve(references.size());
    for (const auto &r : references) {
        escapedRefs.push_back(Utils::Escape(r));
    }
    
    // Об'єднуємо посилання через кому (вже екрановані)
    std::string refsJoined = Utils::Join(escapedRefs, ',');
    
    return GetType() + ";" + escapedName + ";" + escapedDef + ";" + refsJoined;
}