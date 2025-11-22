#ifndef KURSOVA_TERMBASE_H
#define KURSOVA_TERMBASE_H

#include <string>

/**
 * Абстрактний базовий клас терміна.
 * Інкапсулює спільні поля: назва терміна та його визначення.
 */
class TermBase {
protected:
    std::string name;
    std::string definition;

public:
    TermBase();
    TermBase(const std::string &name, const std::string &definition);
    TermBase(const TermBase &other);
    TermBase(TermBase &&other) noexcept;
    virtual ~TermBase();

    TermBase &operator=(const TermBase &other);
    TermBase &operator=(TermBase &&other) noexcept;

    const std::string &GetName() const;
    void SetName(const std::string &value);

    const std::string &GetDefinition() const;
    void SetDefinition(const std::string &value);

    /// Тип терміна: "TERM" або "PRIM"
    virtual std::string GetType() const = 0;

    /// Чи є термін первинним поняттям
    virtual bool IsPrimitive() const = 0;

    /// Серіалізація у рядок CSV
    virtual std::string Serialize() const = 0;
};

#endif //KURSOVA_TERMBASE_H
