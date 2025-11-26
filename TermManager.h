/**
 * @file TermManager.h
 * @brief Оголошення класу менеджера термінів.
 */

#ifndef KURSOVA_TERMMANAGER_H
#define KURSOVA_TERMMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_set>
#include "TermBase.h"

/**
 * @class TermManager
 * @brief Клас-менеджер для роботи з базою термінів.
 *
 * Відповідає за:
 * - Завантаження та збереження даних у CSV-файл.
 * - Зберігання списку термінів у пам'яті (використовуючи std::shared_ptr).
 * - Виконання операцій CRUD (Create, Read, Update, Delete).
 * - Пошук, сортування та фільтрацію.
 * - Побудову ланцюжків залежностей термінів (ключовий функціонал).
 */
class TermManager {
private:
    /**
     * @brief Основний контейнер для зберігання термінів.
     *
     * Використовується поліморфізм: зберігаються вказівники на базовий клас TermBase,
     * що дозволяє тримати в одному списку і PrimitiveTerm, і Term.
     */
    std::vector<std::shared_ptr<TermBase>> terms;

    /**
     * @brief Шлях до файлу бази даних (CSV).
     */
    std::string filePath;

    /**
     * @brief Рекурсивний метод для виведення ланцюжка залежностей.
     *
     * Це допоміжний приватний метод, який викликається публічним PrintChainFrom.
     *
     * @param name Назва поточного терміна.
     * @param visited Множина відвіданих термінів (захист від зациклення).
     * @param level Рівень вкладеності (для відступів у консолі).
     */
    void PrintChainRecursive(const std::string &name,
                             std::unordered_set<std::string> &visited,
                             int level) const;

public:
    /**
     * @brief Конструктор.
     * @param filePath Шлях до файлу для завантаження/збереження даних.
     */
    explicit TermManager(const std::string &filePath);

    /**
     * @brief Завантажує дані з файлу у пам'ять.
     * Парсить CSV-формат, розпізнає типи термінів.
     */
    void Load();

    /**
     * @brief Зберігає поточний стан бази у файл.
     */
    void Save() const;

    /**
     * @brief Додає новий термін до списку.
     * @param term Розумний вказівник на об'єкт терміна.
     */
    void AddTerm(const std::shared_ptr<TermBase> &term);

    /**
     * @brief Видаляє термін за назвою.
     * @param name Назва терміна для видалення.
     * @return true, якщо видалення успішне; false, якщо термін не знайдено або він використовується іншими.
     */
    bool RemoveTerm(const std::string &name);

    /**
     * @brief Редагує визначення існуючого терміна.
     * @param name Назва терміна.
     * @param newDefinition Новий текст визначення.
     * @return true, якщо термін знайдено і оновлено.
     */
    bool EditDefinition(const std::string &name, const std::string &newDefinition);

    /**
     * @brief Шукає термін за повною назвою (без урахування регістру).
     * @param name Назва.
     * @return Вказівник на знайдений термін або nullptr.
     */
    std::shared_ptr<TermBase> FindByName(const std::string &name) const;

    /**
     * @brief Сортує список термінів за назвою (А-Я).
     */
    void SortByName();

    /**
     * @brief Сортує список термінів за текстом визначення.
     */
    void SortByDefinition();

    /**
     * @brief Виводить у консоль короткий список (тільки назви).
     */
    void PrintAllShort() const;

    /**
     * @brief Виводить у консоль повну інформацію про всі терміни.
     */
    void PrintAllFull() const;

    /**
     * @brief Шукає терміни, у визначенні яких зустрічається підрядок.
     * @param substring Фрагмент тексту для пошуку.
     */
    void SearchByDefinition(const std::string &substring) const;

    /**
     * @brief Виводить список, відфільтрований за типом (первинні або складні).
     * @param primitiveOnly Якщо true - виводить тільки PRIM, інакше - тільки TERM.
     */
    void PrintFilteredByPrimitive(bool primitiveOnly) const;

    /**
     * @brief Будує та виводить ланцюжок залежностей для заданого терміна.
     * @param name Назва початкового терміна.
     */
    void PrintChainFrom(const std::string &name) const;

    /**
     * @brief Перевіряє, чи є посилання на цей термін в інших термінах.
     * Використовується для заборони видалення важливих понять.
     * @param name Назва терміна.
     * @return true, якщо термін використовується.
     */
    bool IsReferenced(const std::string &name) const;

    /**
     * @brief Заповнює базу тестовими даними, якщо вона порожня.
     */
    void EnsureDefaultTerms();

    /**
     * @brief Виводить статистику (кількість термінів різних типів).
     */
    void PrintStats() const;
};

#endif //KURSOVA_TERMMANAGER_H