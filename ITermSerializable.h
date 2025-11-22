#ifndef ITERM_SERIALIZABLE_H
#define ITERM_SERIALIZABLE_H

#include <string>

/**
 * Інтерфейс серіалізації термінів.
 */
class ITermSerializable {
public:
    virtual ~ITermSerializable() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& line) = 0;
};

#endif // ITERM_SERIALIZABLE_H
