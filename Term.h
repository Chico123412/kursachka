/**
 * @file Term.h
 * @brief Оголошення класу складного терміна.
 */

#ifndef KURSOVA_TERM_H
#define KURSOVA_TERM_H

#include "TermBase.h"
#include <vector>

/**
 * @class Term
 * @brief Клас для представлення складних термінів.
 *
 * Складний термін — це поняття, яке визначається через інші терміни.
 * Він містить список посилань (назв інших термінів), що дозволяє будувати
 * ланцюжки залежностей аж до первинних понять (PrimitiveTerm).
 */
class Term : public TermBase {
private:
    /**
     * @brief Список назв термінів, на які посилається цей термін.
     *
     * Використовується для побудови графа залежностей.
     */
    std::vector<std::string> references;

public:
    /**
     * @brief Конструктор за замовчуванням.
     */
    Term();

    /**
     * @brief Параметризований конструктор.
     * @param name Назва терміна.
     * @param definition Текстове визначення.
     * @param references Вектор назв інших термінів, від яких залежить цей.
     */
    Term(const std::string &name,
         const std::string &definition,
         const std::vector<std::string> &references);

    /**
     * @brief Конструктор копіювання.
     * @param other Об'єкт для копіювання.
     */
    Term(const Term &other);

    /**
     * @brief Конструктор переміщення.
     * @param other Об'єкт для переміщення (r-value).
     */
    Term(Term &&other) noexcept;

    /**
     * @brief Віртуальний деструктор.
     */
    ~Term() override;

    /**
     * @brief Оператор присвоєння (копіювання).
     * @param other Об'єкт-джерело.
     * @return Посилання на поточний об'єкт.
     */
    Term &operator=(const Term &other);

    /**
     * @brief Оператор присвоєння (переміщення).
     * @param other Об'єкт-джерело.
     * @return Посилання на поточний об'єкт.
     */
    Term &operator=(Term &&other) noexcept;

    /**
     * @brief Отримує список посилань.
     * @return Константне посилання на вектор рядків.
     */
    const std::vector<std::string> &GetReferences() const;

    /**
     * @brief Встановлює новий список посилань.
     * @param refs Вектор назв термінів.
     */
    void SetReferences(const std::vector<std::string> &refs);

    /**
     * @brief Повертає тип терміна.
     * @return "TERM".
     */
    std::string GetType() const override;

    /**
     * @brief Перевіряє, чи є термін первинним.
     * @return false (для цього класу).
     */
    bool IsPrimitive() const override;

    /**
     * @brief Серіалізує об'єкт у рядок.
     * @return Рядок формату: TERM;Назва;Визначення;Посил1,Посил2...
     */
    std::string Serialize() const override;
};

#endif //KURSOVA_TERM_H