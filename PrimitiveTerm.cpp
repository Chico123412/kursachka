/**
 * @file PrimitiveTerm.cpp
 * @brief Реалізація методів класу первинного терміна.
 */

#include "PrimitiveTerm.h"
#include "Utils.h"
#include <iostream>

/**
 * @brief Конструктор за замовчуванням.
 */
PrimitiveTerm::PrimitiveTerm() = default;

/**
 * @brief Параметризований конструктор.
 * @param name Назва терміна.
 * @param definition Текстове визначення терміна.
 */
PrimitiveTerm::PrimitiveTerm(const std::string &name, const std::string &definition)
        : TermBase(name, definition) {}

/**
 * @brief Конструктор копіювання.
 * @param other Об'єкт, з якого копіюються дані.
 */
PrimitiveTerm::PrimitiveTerm(const PrimitiveTerm &other)
        : TermBase(other) {}

/**
 * @brief Конструктор переміщення.
 * @param other Об'єкт, дані якого переміщуються (r-value reference).
 */
PrimitiveTerm::PrimitiveTerm(PrimitiveTerm &&other) noexcept
        : TermBase(std::move(other)) {}

/**
 * @brief Деструктор.
 *
 * Виводить налагоджувальне повідомлення для демонстрації керування пам'яттю.
 */
PrimitiveTerm::~PrimitiveTerm() {
    // [DEBUG] Повідомлення корисне для демонстрації роботи віртуальних деструкторів на захисті
    std::cout << "[DEBUG] Знищення первинного терміна: " << name << std::endl;
}

/**
 * @brief Оператор присвоєння (копіювання).
 * @param other Об'єкт-джерело.
 * @return Посилання на поточний об'єкт.
 */
PrimitiveTerm &PrimitiveTerm::operator=(const PrimitiveTerm &other) {
    if (this != &other) {
        TermBase::operator=(other);
    }
    return *this;
}

/**
 * @brief Оператор присвоєння (переміщення).
 * @param other Об'єкт-джерело (r-value).
 * @return Посилання на поточний об'єкт.
 */
PrimitiveTerm &PrimitiveTerm::operator=(PrimitiveTerm &&other) noexcept {
    if (this != &other) {
        TermBase::operator=(std::move(other));
    }
    return *this;
}

/**
 * @brief Повертає рядковий ідентифікатор типу терміна.
 * @return "PRIM" для первинних термінів.
 */
std::string PrimitiveTerm::GetType() const {
    return "PRIM";
}

/**
 * @brief Перевіряє, чи є термін первинним (базовим поняттям).
 * @return true, оскільки цей клас представляє саме первинні терміни.
 */
bool PrimitiveTerm::IsPrimitive() const {
    return true;
}

/**
 * @brief Серіалізує об'єкт у рядок для збереження у CSV.
 *
 * Формує рядок у форматі: PRIM;Назва;Визначення;
 * Використовує екранування спецсимволів (Utils::Escape).
 *
 * @return std::string Серіалізовані дані.
 */
std::string PrimitiveTerm::Serialize() const {
    std::string escapedName = Utils::Escape(name);
    std::string escapedDef = Utils::Escape(definition);
    // Формат: TYPE;name;definition;
    // Останню крапку з комою залишаємо для сумісності з форматом TERM (де далі йдуть посилання)
    return GetType() + ";" + escapedName + ";" + escapedDef + ";";
}