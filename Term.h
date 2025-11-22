#ifndef KURSOVA_TERM_H
#define KURSOVA_TERM_H

#include "TermBase.h"
#include <vector>

/**
 * Складний термін, який посилається на інші терміни.
 * Використовується для побудови ланцюжків до первинних понять.
 */
class Term : public TermBase {
private:
    std::vector<std::string> references; // імена термінів, на які є посилання

public:
    Term();
    Term(const std::string &name,
         const std::string &definition,
         const std::vector<std::string> &references);

    Term(const Term &other);
    Term(Term &&other) noexcept;
    ~Term() override;

    Term &operator=(const Term &other);
    Term &operator=(Term &&other) noexcept;

    const std::vector<std::string> &GetReferences() const;
    void SetReferences(const std::vector<std::string> &refs);

    std::string GetType() const override;
    bool IsPrimitive() const override;

    std::string Serialize() const override;
};

#endif //KURSOVA_TERM_H
