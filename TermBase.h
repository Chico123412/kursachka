/**
 * @file TermBase.h
 * @brief Оголошення абстрактного базового класу терміна.
 */

#ifndef KURSOVA_TERMBASE_H
#define KURSOVA_TERMBASE_H

#include <string>
#include "ITermSerializable.h"

/**
 * @class TermBase
 * @brief Абстрактний базовий клас для всіх видів термінів.
 *
 * Цей клас визначає спільну структуру для будь-якого терміна (назва та визначення)
 * і наслідує інтерфейс ITermSerializable, зобов'язуючи всіх нащадків
 * реалізувати механізм збереження у файл.
 *
 * Клас є абстрактним, оскільки містить чисті віртуальні методи (GetType, IsPrimitive).
 * Створення екземплярів цього класу неможливе, лише через класи-спадкоємці
 * (PrimitiveTerm або Term).
 */
class TermBase : public ITermSerializable {
protected:
    /**
     * @brief Назва терміна.
     * Доступна для класів-спадкоємців (protected).
     */
    std::string name;

    /**
     * @brief Текстове визначення терміна.
     * Доступна для класів-спадкоємців (protected).
     */
    std::string definition;

public:
    /**
     * @brief Конструктор за замовчуванням.
     */
    TermBase();

    /**
     * @brief Параметризований конструктор.
     * @param name Назва терміна.
     * @param definition Визначення терміна.
     */
    TermBase(const std::string &name, const std::string &definition);

    /**
     * @brief Конструктор копіювання.
     */
    TermBase(const TermBase &other);

    /**
     * @brief Конструктор переміщення.
     */
    TermBase(TermBase &&other) noexcept;

    /**
     * @brief Віртуальний деструктор.
     *
     * Гарантує правильне видалення об'єктів похідних класів при видаленні
     * через вказівник на базовий клас (TermBase*).
     */
    virtual ~TermBase();

    /**
     * @brief Оператор присвоєння (копіювання).
     */
    TermBase &operator=(const TermBase &other);

    /**
     * @brief Оператор присвоєння (переміщення).
     */
    TermBase &operator=(TermBase &&other) noexcept;

    /**
     * @brief Отримує назву терміна.
     * @return Константне посилання на рядок.
     */
    const std::string &GetName() const;

    /**
     * @brief Встановлює нову назву.
     * @param value Нова назва.
     */
    void SetName(const std::string &value);

    /**
     * @brief Отримує визначення терміна.
     * @return Константне посилання на рядок.
     */
    const std::string &GetDefinition() const;

    /**
     * @brief Встановлює нове визначення.
     * @param value Новий текст визначення.
     */
    void SetDefinition(const std::string &value);

    /**
     * @brief Повертає рядковий ідентифікатор типу терміна.
     * @return "PRIM" або "TERM" (реалізується у нащадках).
     */
    virtual std::string GetType() const = 0;

    /**
     * @brief Перевіряє, чи є термін первинним (не має залежностей).
     * @return true, якщо це PrimitiveTerm, false, якщо це Term.
     */
    virtual bool IsPrimitive() const = 0;

    // Метод Serialize() тут не оголошується повторно,
    // оскільки він успадкований від ITermSerializable
    // і повинен бути реалізований у конкретних класах.
};

#endif //KURSOVA_TERMBASE_H