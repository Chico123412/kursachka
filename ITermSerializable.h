#ifndef ITERM_SERIALIZABLE_H
#define ITERM_SERIALIZABLE_H

#include <string>

/**
 * Інтерфейс серіалізації термінів.
 */
class ITermSerializable {
public:
    virtual ~ITermSerializable() = default;

    //  Використовується всюди в проекті  PascalCase
    virtual std::string Serialize() const = 0;
};

#endif // ITERM_SERIALIZABLE_H
