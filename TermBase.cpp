/**
 * @file TermBase.cpp
 * @brief Реалізація методів базового абстрактного класу терміна.
 *
 * Містить реалізацію "Правила п'яти" (конструктори, деструктор, оператори присвоєння)
 * та методів доступу до спільних полів (назва, визначення).
 */

#include "TermBase.h"
#include <iostream>

/**
 * @brief Конструктор за замовчуванням.
 */
TermBase::TermBase() = default;

/**
 * @brief Параметризований конструктор.
 * @param name Назва терміна.
 * @param definition Текстове визначення терміна.
 */
TermBase::TermBase(const std::string &name, const std::string &definition)
        : name(name), definition(definition) {}

/**
 * @brief Конструктор копіювання.
 * @param other Об'єкт, з якого копіюються дані.
 */
TermBase::TermBase(const TermBase &other)
        : name(other.name), definition(other.definition) {}

/**
 * @brief Конструктор переміщення.
 * @param other Об'єкт, дані якого переміщуються (r-value).
 */
TermBase::TermBase(TermBase &&other) noexcept
        : name(std::move(other.name)), definition(std::move(other.definition)) {}

/**
 * @brief Віртуальний деструктор.
 *
 * Виводить налагоджувальне повідомлення. Оскільки деструктор віртуальний,
 * це повідомлення буде виводитись при знищенні будь-якого об'єкта-нащадка
 * (PrimitiveTerm або Term), що демонструє правильний порядок звільнення пам'яті.
 */
TermBase::~TermBase() {
    std::cout << "[DEBUG] Знищення терміна: " << name << std::endl;
}

/**
 * @brief Оператор присвоєння (копіювання).
 * @param other Об'єкт-джерело.
 * @return Посилання на поточний об'єкт.
 */
TermBase &TermBase::operator=(const TermBase &other) {
    if (this != &other) {
        name = other.name;
        definition = other.definition;
    }
    return *this;
}

/**
 * @brief Оператор присвоєння (переміщення).
 * @param other Об'єкт-джерело (r-value).
 * @return Посилання на поточний об'єкт.
 */
TermBase &TermBase::operator=(TermBase &&other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        definition = std::move(other.definition);
    }
    return *this;
}

/**
 * @brief Отримує назву терміна.
 * @return Константне посилання на рядок з назвою.
 */
const std::string &TermBase::GetName() const {
    return name;
}

/**
 * @brief Встановлює нову назву терміна.
 * @param value Нова назва.
 */
void TermBase::SetName(const std::string &value) {
    name = value;
}

/**
 * @brief Отримує визначення терміна.
 * @return Константне посилання на рядок з визначенням.
 */
const std::string &TermBase::GetDefinition() const {
    return definition;
}

/**
 * @brief Встановлює нове визначення терміна.
 * @param value Новий текст визначення.
 */
void TermBase::SetDefinition(const std::string &value) {
    definition = value;
}