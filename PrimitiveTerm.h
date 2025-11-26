/**
 * @file PrimitiveTerm.h
 * @brief Оголошення класу первинного терміна.
 */

#ifndef KURSOVA_PRIMITIVETERM_H
#define KURSOVA_PRIMITIVETERM_H

#include "TermBase.h"

/**
 * @class PrimitiveTerm
 * @brief Клас для представлення первинних термінів (PRIM).
 *
 * Первинний термін — це базове поняття предметної області, яке має лише назву
 * та визначення, але не посилається на інші терміни (не залежить від них).
 * Є "листовим" вузлом у дереві залежностей термінів.
 */
class PrimitiveTerm : public TermBase {
public:
    /**
     * @brief Конструктор за замовчуванням.
     */
    PrimitiveTerm();

    /**
     * @brief Параметризований конструктор.
     * @param name Назва терміна.
     * @param definition Текстове визначення.
     */
    PrimitiveTerm(const std::string &name, const std::string &definition);

    /**
     * @brief Конструктор копіювання.
     * @param other Об'єкт для копіювання.
     */
    PrimitiveTerm(const PrimitiveTerm &other);

    /**
     * @brief Конструктор переміщення.
     * @param other Об'єкт для переміщення (r-value).
     */
    PrimitiveTerm(PrimitiveTerm &&other) noexcept;

    /**
     * @brief Віртуальний деструктор.
     */
    ~PrimitiveTerm() override;

    /**
     * @brief Оператор присвоєння (копіювання).
     * @param other Об'єкт-джерело.
     * @return Посилання на поточний об'єкт.
     */
    PrimitiveTerm &operator=(const PrimitiveTerm &other);

    /**
     * @brief Оператор присвоєння (переміщення).
     * @param other Об'єкт-джерело.
     * @return Посилання на поточний об'єкт.
     */
    PrimitiveTerm &operator=(PrimitiveTerm &&other) noexcept;

    /**
     * @brief Повертає рядковий ідентифікатор типу.
     * @return "PRIM" — ідентифікатор первинного терміна.
     */
    std::string GetType() const override;

    /**
     * @brief Перевіряє, чи є термін первинним.
     * @return true, оскільки цей клас завжди представляє первинні поняття.
     */
    bool IsPrimitive() const override;

    /**
     * @brief Серіалізує об'єкт у рядок формату CSV.
     * @return Рядок вигляду: PRIM;Назва;Визначення;
     */
    std::string Serialize() const override;
};

#endif //KURSOVA_PRIMITIVETERM_H